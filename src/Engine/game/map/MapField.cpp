/*
 * ===================== MapField.cpp =======================
 *                          -- tpr --
 *                                        CREATE -- 2019.02.27
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *   4*4mapent 构成一个 field   [第二版]
 * ----------------------------
 */
#include "MapField.h"


//-------------------- C --------------------//
#include <cassert>

//-------------------- Engine --------------------//
#include "random.h"
#include "IntVec.h"
#include "srcs_engine.h" //- 所有资源
#include "EcoSysInMap.h"
#include "FieldBorderSet.h"


namespace{//----------- namespace ---------------//

    std::default_random_engine  randEngine; //-通用 随机数引擎实例
    std::uniform_int_distribution<int> uDistribution_color(-2,2);
    bool  is_rand_init {false}; //- tmp

    std::map<occupyWeight_t,EcoSysInMap*> nearFour_ecoSysInMapPtrs {}; 
                        // 按照 ecoSysInMap.occupyWeight 倒叙排列（值大的在前面）


}//-------------- namespace : end ---------------//



/* ===========================================================
 *                    init
 * -----------------------------------------------------------
 *  仅 初始化 一阶数据
 * param: _mpos      -- 此 field 中的任意 mapent.mpos 
 * param: _chunkMPos -- 此 field 所属的 chunk mpos
 */
void MapField::init( const IntVec2 &_anyMPos ){

    
    if( is_rand_init == false ){
        is_rand_init = true;
        randEngine.seed( get_new_seed() );
    }
    

    //--- field.mcpos ---
    this->mcpos.set_by_mpos( anyMPos_2_fieldMPos(_anyMPos) );

    //--- field.fieldKey ---
    this->fieldKey = fieldMPos_2_fieldKey( this->mcpos.get_mpos() );


    //--- fieldFPos ----
    this->FDPos.x = (float)(this->get_mpos().x) / (float)ENTS_PER_FIELD;
    this->FDPos.y = (float)(this->get_mpos().y) / (float)ENTS_PER_FIELD;

    //--- field.nodeMPos ---
    this->init_nodeMPos();

    //--- assign_field_to_4_ecoSysInMaps ---
    this->assign_field_to_4_ecoSysInMaps();

    //--- lColorOff ---
    this->init_lColorOff();

    //--- originPerlin ---
    // 4*4 个 field 组成一个 perlinEnt
    float freq = 1.0 / 4.0; //- tmp
    this->originPerlin = esrc::gameSeed.pn_field.noise(this->FDPos.x * freq, 
                                                       this->FDPos.y * freq, 
                                                       0.1);  //- [0.0, 1.0]

    //--- weight ---
    this->weight = this->originPerlin * 200.0 - 100.0; //- [-100.0, 100.0]

    //--- fieldBorderSetId ---
    size_t randIdx = (size_t)(this->originPerlin*997); //- 素数 [0,977]
    this->fieldBorderSetId = apply_a_fieldBorderSetId( randIdx );


    //--- occupyWeight ---
    this->init_occupyWeight();
}


/* ===========================================================
 *                init_nodeMPos
 * -----------------------------------------------------------
 */
void MapField::init_nodeMPos(){

    float    freq = 1.0;
    float    pnX;
    float    pnY;
    size_t   idxX;
    size_t   idxY;

    pnX = esrc::gameSeed.pn_field.noise(this->FDPos.x * freq, 
                                        this->FDPos.y * freq, 
                                        0.3); //- [0.0, 1.0]
    pnY = esrc::gameSeed.pn_field.noise(this->FDPos.x * freq, 
                                        this->FDPos.y * freq, 
                                        0.7); //- [0.0, 1.0]

    pnX = pnX * 50 + 100; //- [50.0, 150.0]
    pnY = pnY * 50 + 100; //- [50.0, 150.0]
        assert( (pnX>0) && (pnY>0) );

    idxX = (size_t)pnX % ENTS_PER_FIELD; //- mod
    idxY = (size_t)pnY % ENTS_PER_FIELD; //- mod

    this->nodeMPos = this->get_mpos() + IntVec2{ (int)idxX, (int)idxY };
}

/* ===========================================================
 *                init_lColorOff
 * -----------------------------------------------------------
 */
void MapField::init_lColorOff(){

    
    float vx = this->FDPos.x * 3.20;
    float vy = this->FDPos.y * 3.20;

    float r = esrc::gameSeed.pn_field.noise( vx, vy, 50.25 ); //- [0.0, 1.0]
    float g = esrc::gameSeed.pn_field.noise( vx, vy, 150.55 );
    float b = esrc::gameSeed.pn_field.noise( vx, vy, 250.85 );
    
    this->lColorOff_r = (int)floor(r*20-10); //- [-5, 5]
    this->lColorOff_g = (int)floor(g*20-10); 
    this->lColorOff_b = (int)floor(b*20-10); 
    

    /*
    this->lColorOff_r = uDistribution_color(randEngine);
    this->lColorOff_g = uDistribution_color(randEngine);
    this->lColorOff_b = uDistribution_color(randEngine);
    */


}


/* ===========================================================
 *                init_occupyWeight
 * -----------------------------------------------------------
 */
void MapField::init_occupyWeight(){

    //-- 本 field 在 世界坐标中的 奇偶性 --
    // 得到的值将会是 {0,0}; {1,0}; {0,1}; {1,1} 中的一种
    IntVec2 v = floorDiv( this->get_mpos(), ENTS_PER_FIELD );
    IntVec2 oddEven = floorMod( v, 2 );

    //-- 相邻 field 间的 occupyWeight 没有关联性，就是 白噪音 --
    float Fidx = esrc::gameSeed.pn_field.noise( this->FDPos.x, 
                                                this->FDPos.y, 
                                                0.2) * 30.0 + 60.0; //- [30.0, 90.0]
    assert( Fidx > 0 );
    size_t randIdx = (size_t)floor(Fidx); //- [30, 90]

    this->occupyWeight = get_occupyWeight( oddEven, randIdx );
}



/* ===========================================================
 *          assign_field_to_4_ecoSysInMaps
 * -----------------------------------------------------------
 */
void MapField::assign_field_to_4_ecoSysInMaps(){

    //--- reset nearFour_ecoSysInMapPtrs ---//
    sectionKey_t sectionKey = anyMPos_2_sectionKey( this->get_mpos() );
        assert( esrc::sections.find(sectionKey) != esrc::sections.end() ); //- must exist
    nearFour_ecoSysInMapPtrs.clear();
    for( const auto &ecoPtr : esrc::sections.at(sectionKey).ecoSysInMapPtrs ){
        nearFour_ecoSysInMapPtrs.insert({ -(ecoPtr->occupyWeight), ecoPtr });
                            //- 通过负数，来实现 倒叙排列，occupyWeight 值大的排前面
    }

    float vx;
    float vy;

    IntVec2  mposOff;
    float    freqBig = 1.0;
    float    freqSml = 3.8;
    float    pnVal; //- 围绕 0 波动的 随机值
    float    off;
    int      count;
    EcoSysInMap*  tmpEcoPtr;

    float targetDistance = 0.48 * 1.4 * ENTS_PER_SECTION; //- 每个field 最终的 距离比较值。

    vx = (float)(this->get_mpos().x) / (float)ENTS_PER_CHUNK;
    vy = (float)(this->get_mpos().y) / (float)ENTS_PER_CHUNK;

    float pnValBig = esrc::gameSeed.pn_field_in_ecoSysInMap.noise(  vx * freqBig,
                                                                    vy * freqBig,
                                                                    0.15) * 2.0 - 1.0; // [-1.0, 1.0]
    float pnValSml = esrc::gameSeed.pn_field_in_ecoSysInMap.noise(  vx * freqSml,
                                                                    vy * freqSml,
                                                                    0.45) * 2.0 - 1.0; // [-1.0, 1.0]
    pnVal = 0.5 * (pnValBig + pnValSml);    // [-1.0, 1.0]
    pnVal = 0.5 * pnVal * ENTS_PER_SECTION; //- [-64.0, 64.0]
    //-----

    count = 0;
    for( auto &ecoPair : nearFour_ecoSysInMapPtrs ){
        count++;
        tmpEcoPtr = ecoPair.second;

        if( count != nearFour_ecoSysInMapPtrs.size()){ //- 前3个 eco

            mposOff = this->get_mpos() - tmpEcoPtr->get_mpos();
            off = sqrt( mposOff.x*mposOff.x + mposOff.y*mposOff.y );
            off += pnVal;

            if( off < targetDistance ){ //- tmp
                this->ecoSysInMapKey = tmpEcoPtr->sectionKey;
                break;
            }
        }else{ //- 第四个 eco
            this->ecoSysInMapKey = tmpEcoPtr->sectionKey;
        }
    }
}




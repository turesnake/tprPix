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
#include <cmath>

//-------------------- CPP --------------------//
#include <map>

//-------------------- Engine --------------------//
#include "random.h"
#include "IntVec.h"
#include "EcoSysInMap.h"
#include "FieldBorderSet.h"
#include "simplexNoise.h"
#include "esrc_gameSeed.h"
#include "esrc_ecoSysInMap.h"

//#include "debug.h"


namespace{//----------- namespace ---------------//


    //- section 四个端点 坐标偏移（以 ENTS_PER_SECTION 为单位）[left-bottom]
    const std::vector<IntVec2> quadSectionKeyOffs {
        IntVec2{ 0, 0 },
        IntVec2{ ENTS_PER_SECTION, 0 },
        IntVec2{ 0, ENTS_PER_SECTION },
        IntVec2{ ENTS_PER_SECTION, ENTS_PER_SECTION }
    };

}//-------------- namespace : end ---------------//



/* ===========================================================
 *                    init
 * -----------------------------------------------------------
 *  仅 初始化 一阶数据
 * param: _mpos      -- 此 field 中的任意 mapent.mpos 
 * param: _chunkMPos -- 此 field 所属的 chunk mpos
 */
void MapField::init( const IntVec2 &_anyMPos ){

    //--- field.mcpos ---
    this->mcpos.set_by_mpos( anyMPos_2_fieldMPos(_anyMPos) );

    //--- field.fieldKey ---
    this->fieldKey = fieldMPos_2_fieldKey( this->mcpos.get_mpos() );


    //--- fieldFPos ----
    this->FDPos = this->mcpos.get_fpos();
    this->FDPos /= ENTS_PER_FIELD;
    this->FDPos += esrc::gameSeed.field_pposOff;

    //--- field.nodeMPos ---
    this->init_nodeMPos();

    //--- assign_field_to_4_ecoSysInMaps ---
    //  顺带把 this->density 也初始化了
    this->assign_field_to_4_ecoSysInMaps();

    //--- originPerlin ---
    // 3*3 个 field 组成一个 pn晶格
    float freq = 1.0 / 3.0; //- tmp
    this->originPerlin = simplex_noise2(    this->FDPos.x * freq, 
                                            this->FDPos.y * freq );  //- [-1.0, 1.0]

    //--- weight ---
    this->weight = this->originPerlin * 100.0; //- [-100.0, 100.0]

    //--- fieldBorderSetId ---
    size_t randIdx = static_cast<size_t>(floor((this->originPerlin+3.1)*997));
    this->fieldBorderSetId = apply_a_fieldBorderSetId( randIdx );

    //--- occupyWeight ---
    this->init_occupyWeight();

    //--- density ---
    // 已在 this->assign_field_to_4_ecoSysInMaps(); 中被初始化
}


/* ===========================================================
 *                init_nodeMPos
 * -----------------------------------------------------------
 */
void MapField::init_nodeMPos(){

    float    freq = 13.0;
    float    pnX;
    float    pnY;
    size_t   idxX;
    size_t   idxY;

    pnX = simplex_noise2(   this->FDPos.x * freq, 
                            this->FDPos.y * freq ); //- [-1.0, 1.0]

    pnY = simplex_noise2(   (this->FDPos.x + 70.7) * freq, 
                            (this->FDPos.y + 70.7) * freq ); //- [-1.0, 1.0]


    pnX = pnX * 71 + 100; //- [71.0, 171.0]
    pnY = pnY * 71 + 100; //- [71.0, 171.0]
        assert( (pnX>0) && (pnY>0) );

    idxX = static_cast<size_t>(pnX) % ENTS_PER_FIELD; //- mod
    idxY = static_cast<size_t>(pnY) % ENTS_PER_FIELD; //- mod
    this->nodeMPos = this->get_mpos() + IntVec2{ (int)idxX, (int)idxY };
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
    float Fidx = simplex_noise2(    (this->FDPos.x + 17.1), 
                                    (this->FDPos.y + 17.1) ) * 30.0 + 60.0; //- [30.0, 90.0]


    assert( Fidx > 0 );
    size_t randIdx = (size_t)floor(Fidx); //- [30, 90]

    this->occupyWeight = calc_occupyWeight( oddEven, randIdx );
}



/* ===========================================================
 *          assign_field_to_4_ecoSysInMaps
 * -----------------------------------------------------------
 */
void MapField::assign_field_to_4_ecoSysInMaps(){

    //--- reset nearFour_ecoSysInMapPtrs ---//
    sectionKey_t sectionKey = anyMPos_2_sectionKey( this->get_mpos() );
    IntVec2      sectionMPos = sectionKey_2_mpos( sectionKey );
    
    // 按照 ecoSysInMap.occupyWeight 倒叙排列（值大的在前面）
    std::map<occupyWeight_t,EcoSysInMap_ReadOnly> nearFour_ecoSysInMapDatas {}; 

    EcoSysInMap   *tmpEcoSysInMapPtr;
    sectionKey_t   tmpKey;
    for( const auto &whOff : quadSectionKeyOffs ){
        tmpKey = sectionMPos_2_sectionKey( sectionMPos + whOff );
        nearFour_ecoSysInMapDatas.insert( esrc::atom_get_ecoSysInMap_readOnly( tmpKey ) );
    }

    float         vx;
    float         vy;
    IntVec2       mposOff;
    float         freqBig = 0.9;
    float         freqSml = 2.3;
    float         pnVal; //- 围绕 0 波动的 随机值
    float         off;
    int           count;
    EcoSysInMap*  tmpEcoInMapPtr;

    float targetDistance = 1.4 * (0.5 * ENTS_PER_SECTION) * 1.04; //- 每个field 最终的 距离比较值。

    vx = (float)(this->get_mpos().x) / (float)ENTS_PER_CHUNK;
    vy = (float)(this->get_mpos().y) / (float)ENTS_PER_CHUNK;

    vx += esrc::gameSeed.field_pposOff.x;
    vy += esrc::gameSeed.field_pposOff.y;
    float pnValBig = simplex_noise2(    (vx + 51.15) * freqBig,
                                        (vy + 151.15) * freqBig ) * 17; // [-x.0, x.0]
    float pnValSml = simplex_noise2(    (vx + 244.41) * freqSml,
                                        (vy + 144.41) * freqSml ) * 5; // [-x.0, x.0]
    pnVal = pnValBig + pnValSml;
    if( pnVal > 20.0 ){
        pnVal = 20.0;
    }
    if( pnVal < -20.0 ){
        pnVal = -20.0;
    }
    // now, pnVal: [-20.0, 20.0]

    count = 0;
    for( auto &ecoDataPair : nearFour_ecoSysInMapDatas ){
        count++;
        const auto &ecoReadOnly = ecoDataPair.second;

        if( count != nearFour_ecoSysInMapDatas.size()){ //- 前3个 eco

            mposOff = this->get_mpos() - sectionKey_2_mpos( ecoReadOnly.sectionKey );
            off = sqrt( mposOff.x*mposOff.x + mposOff.y*mposOff.y ); // [ ~ 90.0 ~ ]
            off += pnVal * 0.7; // [-x.0, x.0] + 90.0

            if( off < targetDistance ){ //- tmp
                this->ecoSysInMapKey = ecoReadOnly.sectionKey;
                this->density.set( this->get_mpos(), 
                                    ecoReadOnly.densitySeaLvlOff,
                                    ecoReadOnly.densityDivideValsPtr );
                break;
            }
        }else{ //- 第四个 eco
            this->ecoSysInMapKey = ecoReadOnly.sectionKey;
            this->density.set( this->get_mpos(), 
                                    ecoReadOnly.densitySeaLvlOff,
                                    ecoReadOnly.densityDivideValsPtr );
        }
    }
}




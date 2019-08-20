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
#include <cmath>

//-------------------- CPP --------------------//
#include <map>

//-------------------- Engine --------------------//
#include "tprAssert.h"
#include "tprCast.h"
#include "random.h"
#include "IntVec.h"
#include "EcoObj.h"
#include "FieldBorderSet.h"
#include "simplexNoise.h"
#include "MapEnt.h"

#include "esrc_gameSeed.h"
#include "esrc_ecoObj.h"
#include "tprCast.h"

#include "tprDebug.h"


namespace mapField_inn {//----------- namespace: mapField_inn ---------------//


    //- section 四个端点 坐标偏移（以 ENTS_PER_SECTION 为单位）[left-bottom]
    const std::vector<IntVec2> quadSectionKeyOffs {
        IntVec2{ 0, 0 },
        IntVec2{ ENTS_PER_SECTION, 0 },
        IntVec2{ 0, ENTS_PER_SECTION },
        IntVec2{ ENTS_PER_SECTION, ENTS_PER_SECTION }
    };

}//-------------- namespace: mapField_inn end ---------------//



/* ===========================================================
 *                    init
 * -----------------------------------------------------------
 *  仅 初始化 一阶数据
 * param: _mpos      -- 此 field 中的任意 mapent.mpos 
 * param: _chunkMPos -- 此 field 所属的 chunk mpos
 */
void MapField::init( const IntVec2 &anyMPos_ ){

    //--- field.mcpos ---
    this->mcpos.set_by_mpos( anyMPos_2_fieldMPos(anyMPos_) );

    //--- field.fieldKey ---
    this->fieldKey = fieldMPos_2_fieldKey( this->mcpos.get_mpos() );


    //--- fieldFPos ----
    this->FDPos = this->mcpos.get_dpos();
    this->FDPos /= static_cast<double>(ENTS_PER_FIELD);
    this->FDPos += esrc::get_gameSeed().get_field_pposOff();

    //--- field.nodeMPos ---
    this->init_nodeMPos_and_nodePPosOff();

    //--- assign_field_to_4_ecoObjs ---
    //  顺带把 this->density 也初始化了
    this->assign_field_to_4_ecoObjs();

    //--- originPerlin ---
    // 3*3 个 field 组成一个 pn晶格
    double freq = 1.0 / 3.0; //- tmp
    this->originPerlin = simplex_noise2(    this->FDPos.x * freq, 
                                            this->FDPos.y * freq );  //- [-1.0, 1.0]

    //--- weight ---
    this->weight = this->originPerlin * 100.0; //- [-100.0, 100.0]

    //--- fieldBorderSetId ---
    size_t randIdx = cast_2_size_t(floor((this->originPerlin+3.1)*997.0));
    this->fieldBorderSetId = apply_a_fieldBorderSetId( randIdx );

    //--- occupyWeight ---
    this->init_occupyWeight();

    //--- density ---
    // 已在 this->assign_field_to_4_ecoObjs(); 中被初始化
}



/* ===========================================================
 *                   set_nodeAlti_2
 * -----------------------------------------------------------
 */
void MapField::set_nodeAlti_2( const std::vector<std::unique_ptr<MemMapEnt>> &chunkMapEnts_ ){

    tprAssert( this->isNodeMapAltiSet == false );
    this->isNodeMapAltiSet = true;

    IntVec2 off = this->nodeMPos - anyMPos_2_chunkMPos( this->get_mpos() );
    size_t  idx = cast_2_size_t( off.y * ENTS_PER_CHUNK + off.x );

    tprAssert( idx < chunkMapEnts_.size() );
    this->nodeMapAlti = chunkMapEnts_.at(idx)->mapAlti;
}


/* ===========================================================
 *                init_nodeMPos_and_nodePPosOff
 * -----------------------------------------------------------
 */
void MapField::init_nodeMPos_and_nodePPosOff(){

    double    freq  { 13.0 };
    double    pnX   {};
    double    pnY   {};

    pnX = simplex_noise2(   this->FDPos.x * freq, 
                            this->FDPos.y * freq ); //- [-1.0, 1.0]

    pnY = simplex_noise2(   (this->FDPos.x + 70.7) * freq, 
                            (this->FDPos.y + 70.7) * freq ); //- [-1.0, 1.0]


    pnX = pnX * 71.0 + 100.0; //- [71.0, 171.0]
    pnY = pnY * 71.0 + 100.0; //- [71.0, 171.0]
        tprAssert( (pnX>0) && (pnY>0) );

    //-- nodeMPos --//
    //-- [0,3] 
    size_t idxX = cast_2_size_t(floor(pnX)) % (ENTS_PER_FIELD-1);
    size_t idxY = cast_2_size_t(floor(pnY)) % (ENTS_PER_FIELD-1);

    this->nodeMPos = this->get_mpos() + IntVec2{ static_cast<int>(idxX), 
                                                static_cast<int>(idxY) };

    //-- nodePPosOff --//
    size_t pposOffRange = 16;  // [-8,8]
    size_t halfPposOffRange = 8;
    size_t pX = cast_2_size_t(floor(pnX)) % pposOffRange;
    size_t pY = cast_2_size_t(floor(pnY)) % pposOffRange;
    this->nodePPosOff.set(  static_cast<int>( pX - halfPposOffRange ),
                            static_cast<int>( pY - halfPposOffRange ) ); // [-8,8]
    
}


/* ===========================================================
 *                init_occupyWeight
 * -----------------------------------------------------------
 */
void MapField::init_occupyWeight(){

    //-- 本 field 在 世界坐标中的 奇偶性 --
    // 得到的值将会是 {0,0}; {1,0}; {0,1}; {1,1} 中的一种
    IntVec2 v = floorDiv( this->get_mpos(), static_cast<double>(ENTS_PER_FIELD) );
    IntVec2 oddEven = floorMod( v, 2.0 );

    //-- 相邻 field 间的 occupyWeight 没有关联性，就是 白噪音 --
    double Fidx = simplex_noise2(   (this->FDPos.x + 17.1), 
                                    (this->FDPos.y + 17.1) ) * 30.0 + 60.0; //- [30.0, 90.0]


    tprAssert( Fidx > 0 );
    size_t randIdx = (size_t)floor(Fidx); //- [30, 90]

    this->occupyWeight = calc_occupyWeight( oddEven, randIdx );
}



/* ===========================================================
 *          assign_field_to_4_ecoObjs
 * -----------------------------------------------------------
 */
void MapField::assign_field_to_4_ecoObjs(){

    //--- reset nearFour_ecoObjPtrs ---//
    sectionKey_t sectionKey = anyMPos_2_sectionKey( this->get_mpos() );
    IntVec2      sectionMPos = sectionKey_2_mpos( sectionKey );
    
    // 按照 ecoObj.occupyWeight 倒叙排列（值大的在前面）
    std::map<occupyWeight_t,EcoObj_ReadOnly> nearFour_ecoObjDatas {}; 

    sectionKey_t   tmpKey {};
    for( const auto &whOff : mapField_inn::quadSectionKeyOffs ){
        tmpKey = sectionMPos_2_sectionKey( sectionMPos + whOff );
        nearFour_ecoObjDatas.insert( esrc::atom_get_ecoObj_readOnly( tmpKey ) );
    }

    double         vx        {};
    double         vy        {};
    IntVec2       mposOff   {};
    double         freqBig   { 0.9 };
    double         freqSml   { 2.3 };
    double         pnVal     {}; //- 围绕 0 波动的 随机值
    double         off       {};
    size_t        count     {};

    double targetDistance = 1.4 * (0.5 * ENTS_PER_SECTION) * 1.04; //- 每个field 最终的 距离比较值。

    vx = static_cast<double>(this->get_mpos().x) / static_cast<double>(ENTS_PER_CHUNK);
    vy = static_cast<double>(this->get_mpos().y) / static_cast<double>(ENTS_PER_CHUNK);

    const glm::dvec2 &field_pposOff = esrc::get_gameSeed().get_field_pposOff();
    vx += field_pposOff.x;
    vy += field_pposOff.y;
    double pnValBig = simplex_noise2(    (vx + 51.15) * freqBig,
                                        (vy + 151.15) * freqBig ) * 17; // [-x.0, x.0]
    double pnValSml = simplex_noise2(    (vx + 244.41) * freqSml,
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
    for( auto &ecoDataPair : nearFour_ecoObjDatas ){
        count++;
        const auto &ecoReadOnly = ecoDataPair.second;

        if( count != nearFour_ecoObjDatas.size()){ //- 前3个 eco

            mposOff = this->get_mpos() - sectionKey_2_mpos( ecoReadOnly.sectionKey );
            off = static_cast<double>(sqrt( mposOff.x*mposOff.x + mposOff.y*mposOff.y )); // [ ~ 90.0 ~ ]
            off += pnVal * 0.7; // [-x.0, x.0] + 90.0

            if( off < targetDistance ){ //- tmp
                this->ecoObjKey = ecoReadOnly.sectionKey;
                this->density.set( this->get_mpos(), 
                                    ecoReadOnly.densitySeaLvlOff,
                                    ecoReadOnly.densityDivideValsPtr );
                break;
            }
        }else{ //- 第四个 eco
            this->ecoObjKey = ecoReadOnly.sectionKey;
            this->density.set( this->get_mpos(), 
                                    ecoReadOnly.densitySeaLvlOff,
                                    ecoReadOnly.densityDivideValsPtr );
        }
    }
}




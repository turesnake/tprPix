/*
 * ========================== EcoSysInMap.cpp =======================
 *                          -- tpr --
 *                                        CREATE -- 2019.03.02
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 * ----------------------------
 */
#include "EcoSysInMap.h"

//--- glm - 0.9.8 ---
#include <glm/glm.hpp>
            //-- glm::vec3
            //-- glm::vec4
            //-- glm::mat4

//-------------------- C --------------------//
#include <math.h>

//-------------------- Engine --------------------//
#include "config.h"
#include "random.h"
#include "PerlinNoise3D.h" //- out 
#include "Density.h"
#include "simplexNoise.h"
#include "esrc_gameSeed.h"
#include "esrc_ecoSys.h"
#include "esrc_ecoSysInMap.h"

#include "debug.h"


namespace{//-------- namespace: --------------//

    std::default_random_engine  randEngine; //-通用 随机数引擎实例
    inline std::uniform_int_distribution<float> uDistribution_f(-100.0,100.0);
    inline std::uniform_int_distribution<int> uDistribution_2(0,1);
    inline std::uniform_int_distribution<int> uDistribution_4(0,3);


    //-- 周边4个 node 实例 mpos off 值 --
    const std::vector<IntVec2> nearFour_node_ecoSysInMap_mposOffs {
        IntVec2{ 0, 0 },
        IntVec2{ ENTS_PER_SECTION*2, 0 },
        IntVec2{ 0, ENTS_PER_SECTION*2 },
        IntVec2{ ENTS_PER_SECTION*2, ENTS_PER_SECTION*2 }
    };

}//------------- namespace: end --------------//


/* ===========================================================
 *          calc_nearFour_node_ecoSysInMapKey   [static] 
 * -----------------------------------------------------------
 * 生成 目标ecoSysInMapKey 周边 4个 node key 的值，写入 参数容器中
 * -- 将被 atom_try_to_inert_and_init_a_ecoSysInMap() 调用
 */
void EcoSysInMap::calc_nearFour_node_ecoSysInMapKey(sectionKey_t _targetKey,
                                                    std::vector<sectionKey_t> &_container ){
    //-- 获得 (2*2 section) 单元 左下角 mpos --
    float sideLen = 2*ENTS_PER_SECTION;
    IntVec2 baseMPos = floorDiv( sectionKey_2_mpos(_targetKey), sideLen ) * sideLen;

    _container.clear();
    for( const auto &off :  nearFour_node_ecoSysInMap_mposOffs ){ //- each off mpos
        _container.push_back( sectionMPos_2_sectionKey(baseMPos + off) );
    }
}




/* ===========================================================
 *                    init_for_node
 * -----------------------------------------------------------
 */
void EcoSysInMap::init_for_node( sectionKey_t _sectionKey ){

    this->init_fstOrder( _sectionKey );

    assert( (this->oddEven.x==0) && (this->oddEven.y==0) ); //- must be node 
    EcoSys *ecoSysPtr = esrc::get_ecoSysPtr( esrc::apply_a_rand_ecoSysId(this->weight) );
    //------------------------//
    //  确定 targetEcoPtr 后, 正式 分配数据
    //------------------------//
    this->copy_datas_from_ecosys( ecoSysPtr );
}



/* ===========================================================
 *                   init_for_regular
 * -----------------------------------------------------------
 */
void EcoSysInMap::init_for_regular( sectionKey_t _sectionKey,
                                    const std::vector<sectionKey_t> &_nearby_four_ecoSysIds ){

    this->init_fstOrder( _sectionKey );
    this->init_for_no_node_ecoSysInMap( _nearby_four_ecoSysIds );
}



/* ===========================================================
 *                      init_fstOrder
 * -----------------------------------------------------------
 * -- 仅初始化几个 最简单的数据
 */
void EcoSysInMap::init_fstOrder( sectionKey_t _sectionKey ){

    //------------------//
    //    key / mpos
    //------------------//
    this->sectionKey = _sectionKey;
    this->mcpos.set_by_mpos( sectionKey_2_mpos(this->sectionKey) );
    //------------------//
    //     oddEven
    //------------------//
    IntVec2 SPos = floorDiv( this->get_mpos(), ENTS_PER_SECTION );
    this->oddEven = floorMod( SPos, 2 );
    //------------------//
    //     weight
    //------------------//
    // 3*3 个 ecosysinmap 组成一个 pn晶格
    float freq = 1.0 / 3.0; 
    glm::vec2 fv = this->mcpos.get_fpos();
    fv /= ENTS_PER_SECTION;
    fv += esrc::gameSeed.ecoSysInMapWeight_pposOff;

    this->weight = simplex_noise2(  fv.x * freq,
                                    fv.y * freq ) * 100.0; //- [-100.0, 100.0]
    //------------------//
    //   occupyWeight
    //------------------//
    size_t randV = static_cast<size_t>(floor( this->weight * 3 + 757 ));
    this->occupyWeight = calc_occupyWeight( this->oddEven, randV );
    //------------------------------//
    //  landColors / goSpecIdPools
    //------------------------------//
    this->goSpecIdPools.clear();
    this->goSpecIdPools.resize( Density::get_idxNum(), std::vector<goSpecId_t> {} );
}


/* ===========================================================
 *                init_for_no_node_ecoSysInMap
 * -----------------------------------------------------------
 * -- 完成后半段初始化。 仅用于 非 node 实例
 */
void EcoSysInMap::init_for_no_node_ecoSysInMap( const std::vector<sectionKey_t> &_nearby_four_ecoSysIds ){

    EcoSys *node_1_Ptr;
    EcoSys *node_2_Ptr;
    EcoSys *node_3_Ptr;
    EcoSys *node_4_Ptr;
    EcoSys *targetEcoPtr;

    EcoSysType   ecoType;

    randEngine.seed( static_cast<size_t>(this->weight) ); //- 实现了伪随机
    goSpecId_t  tmpGoSpecId;

    //------------------------//
    //          右下
    //------------------------//
    if( (oddEven.x==1) && (oddEven.y==0) ){
        node_1_Ptr = esrc::get_ecoSysPtr( _nearby_four_ecoSysIds.at(0) );
        node_2_Ptr = esrc::get_ecoSysPtr( _nearby_four_ecoSysIds.at(1) );

        (uDistribution_2(randEngine)==0) ?
                ecoType = node_1_Ptr->get_type() :
                ecoType = node_2_Ptr->get_type();
        targetEcoPtr = esrc::get_ecoSysPtr( esrc::apply_a_ecoSysId_by_type(ecoType, this->weight) );
    }
    //------------------------//
    //          左上
    //------------------------//
    else if( (oddEven.x==0) && (oddEven.y==1) ){
        node_1_Ptr = esrc::get_ecoSysPtr( _nearby_four_ecoSysIds.at(0) );
        node_2_Ptr = esrc::get_ecoSysPtr( _nearby_four_ecoSysIds.at(2) );

        (uDistribution_2(randEngine)==0) ?
                ecoType = node_1_Ptr->get_type() :
                ecoType = node_2_Ptr->get_type();
        targetEcoPtr = esrc::get_ecoSysPtr( esrc::apply_a_ecoSysId_by_type(ecoType, this->weight) );
    }
    //------------------------//
    //          右上
    //------------------------//
    else{
        node_1_Ptr = esrc::get_ecoSysPtr( _nearby_four_ecoSysIds.at(0) );
        node_2_Ptr = esrc::get_ecoSysPtr( _nearby_four_ecoSysIds.at(1) );
        node_3_Ptr = esrc::get_ecoSysPtr( _nearby_four_ecoSysIds.at(2) );
        node_4_Ptr = esrc::get_ecoSysPtr( _nearby_four_ecoSysIds.at(3) );

        switch( uDistribution_4(randEngine) ){
            case 0: ecoType = node_1_Ptr->get_type(); break;
            case 1: ecoType = node_2_Ptr->get_type(); break;
            case 2: ecoType = node_3_Ptr->get_type(); break;
            case 3: ecoType = node_4_Ptr->get_type(); break;
            default:
                assert(0);
        }
        targetEcoPtr = esrc::get_ecoSysPtr( esrc::apply_a_ecoSysId_by_type(ecoType, this->weight) );
    }

    //------------------------//
    //  确定 targetEcoPtr 后, 正式 分配数据
    //------------------------//
    this->copy_datas_from_ecosys( targetEcoPtr );
}


/* ===========================================================
 *               copy_datas_from_ecosys
 * -----------------------------------------------------------
 * -- 
 */
void EcoSysInMap::copy_datas_from_ecosys( EcoSys *_targetEcoPtr ){

    randEngine.seed( static_cast<size_t>(this->weight) ); //- 实现了伪随机

    this->ecoSysId = _targetEcoPtr->get_id();
    this->ecoSysType = _targetEcoPtr->get_type();
    this->densitySeaLvlOff = _targetEcoPtr->get_densitySeaLvlOff();

    //--- 仅 获得 只读指针 ---
    this->landColorsPtr = _targetEcoPtr->get_landColorsPtr();
    this->applyPercentsPtr = _targetEcoPtr->get_applyPercentsPtr();
    this->densityDivideValsPtr = _targetEcoPtr->get_densityDivideValsPtr();

    //---- goSpecIdPools 数据 ----
    goSpecId_t  tmpGoSpecId;
    for( size_t i=0; i<Density::get_idxNum(); i++ ){ //- each pool in goSpecIdPools
        //--- 取 8 个元素 ---
        for( int ci=0; ci<8; ci++ ){ 
            tmpGoSpecId = _targetEcoPtr->apply_a_rand_goSpecId( i, uDistribution_f(randEngine) );
            this->goSpecIdPools.at(i).push_back( tmpGoSpecId );
        }
    }
}


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
    std::vector<IntVec2> nearFour_node_ecoSysInMap_mposOffs {
        IntVec2{ 0, 0 },
        IntVec2{ ENTS_PER_SECTION*2, 0 },
        IntVec2{ 0, ENTS_PER_SECTION*2 },
        IntVec2{ ENTS_PER_SECTION*2, ENTS_PER_SECTION*2 }
    };

    //-- 放置实际的 section mpos 
    std::vector<IntVec2> nearFour_node_ecoSysInMap_sectionMPos {};
    std::vector<EcoSysInMap*> nearFour_node_ecoSysInMapPtrs {};

    //===== funcs =====//
    void reset_nearFour_node_ecoSysInMap_sectionMPos( const IntVec2 &_ecosysInMapMPos );



}//------------- namespace: end --------------//

/* ===========================================================
 *                      init_fstOrder
 * -----------------------------------------------------------
 * -- 仅初始化几个 最简单的数据
 */
void EcoSysInMap::init_fstOrder(){

    //------------------//
    //     oddEven
    //------------------//
    this->oddEven = EcoSysInMap::calc_oddEven( this->get_mpos() );

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
    this->landColors.clear();
    this->applyPercents.clear();
    this->densityDivideVals.clear();
    this->goSpecIdPools.clear();
    this->goSpecIdPools.resize( Density::get_idxNum(), std::vector<goSpecId_t> {} );

}


/* ===========================================================
 *        find_or_create_the_ecoSysInMap    [static] 
 * -----------------------------------------------------------
 * -- 外部代码 通过此接口，一站式 获得 目标实例
 * -- 优先寻找，没有时 才创建 
 */
EcoSysInMap *EcoSysInMap::find_or_create_the_ecoSysInMap( sectionKey_t _sectionKey ){

    //------------------------------------//
    //    若目标实例 已存在，直接返回其指针
    //------------------------------------//
    if( esrc::find_from_ecoSysesInMap(_sectionKey) ){
        return esrc::get_ecoSysInMapPtr(_sectionKey);
    }

        //   若不存在，就新建一个 
    IntVec2 ecosysInMapMPos = sectionKey_2_mpos(_sectionKey);

    //----------------------------//
    //  一股脑，生成周边 4个 node ecoSysInMap 实例
    //----------------------------//
    reset_nearFour_node_ecoSysInMap_sectionMPos( ecosysInMapMPos );
    nearFour_node_ecoSysInMapPtrs.clear();
    EcoSysInMap *tmpEcoSysInMapPtr; 
    for( const auto &nodeMPos : nearFour_node_ecoSysInMap_sectionMPos ){ //- each node mpos
        tmpEcoSysInMapPtr = EcoSysInMap::find_or_create_target_node_ecoSysInMap( nodeMPos );
        nearFour_node_ecoSysInMapPtrs.push_back( tmpEcoSysInMapPtr );
    }

    //----------------------------//
    //  此时还不确定 本实例是否存在，
    //  先生成临时的 oddeven 数据
    IntVec2 tmpOddEven = EcoSysInMap::calc_oddEven( ecosysInMapMPos );
    //-- 若确认自己就是 node 实例，直接返回其指针 
    if( (tmpOddEven.x==0) && (tmpOddEven.y==0) ){
        return nearFour_node_ecoSysInMapPtrs.at(0);
    }

    //--- 若不是，说明自己是 “杂合实例”，开始创建自己 ---
    EcoSysInMap *selfPtr = esrc::insert_new_ecoSysInMap( ecosysInMapMPos );
    selfPtr->init_fstOrder();
    selfPtr->init_for_no_node_ecoSysInMap();

    return selfPtr;
}



/* ===========================================================
 *           find_or_create_target_node_ecoSysInMap    [static] 
 * -----------------------------------------------------------
 * -- 优先从 esrc::ecoSysesInMap 中寻找 目标 ecoSysInMap 实例
 *    如果不存在，则创建值
 * -- 目标必须是  “纯”／“节点” ecoSysInMap 实例
 * -- 还负责把它 塞进全局容器中
 */
EcoSysInMap *EcoSysInMap::find_or_create_target_node_ecoSysInMap( const IntVec2 &_ecosysInMapMPos ){

    //------------------------------------//
    //    若目标实例 已存在，直接返回其指针
    //------------------------------------//
    sectionKey_t sectionKey = sectionMPos_2_sectionKey( _ecosysInMapMPos );
    if( esrc::find_from_ecoSysesInMap(sectionKey) ){
        return esrc::get_ecoSysInMapPtr(sectionKey);
    }

    //------------------------------------//
    //         若不存在，就新建一个 
    //------------------------------------//
    EcoSysInMap *ecoSysInMapPtr = esrc::insert_new_ecoSysInMap( _ecosysInMapMPos );
    ecoSysInMapPtr->init_fstOrder();
    ecoSysInMapPtr->init_for_node_ecoSysInMap();

    return ecoSysInMapPtr;
}



/* ===========================================================
 *         init_for_node_ecoSysInMap   
 * -----------------------------------------------------------
 * -- 完成后半段初始化 仅用于 node 实例
 */
void EcoSysInMap::init_for_node_ecoSysInMap(){

    assert( (this->oddEven.x==0) && (this->oddEven.y==0) ); //- must be node 
    EcoSys *ecoSysPtr = esrc::get_ecoSysPtr( esrc::apply_a_rand_ecoSysId(this->weight) );

    //------------------------//
    //  确定 targetEcoPtr 后, 正式 分配数据
    //------------------------//
    this->copy_datas_from_ecosys( ecoSysPtr );
}



/* ===========================================================
 *                init_for_no_node_ecoSysInMap
 * -----------------------------------------------------------
 * -- 完成后半段初始化。 仅用于 非 node 实例
 */
void EcoSysInMap::init_for_no_node_ecoSysInMap(){

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
        node_1_Ptr = esrc::get_ecoSysPtr( nearFour_node_ecoSysInMapPtrs.at(0)->get_ecoSysId() );
        node_2_Ptr = esrc::get_ecoSysPtr( nearFour_node_ecoSysInMapPtrs.at(1)->get_ecoSysId() );

        (uDistribution_2(randEngine)==0) ?
                ecoType = node_1_Ptr->get_type() :
                ecoType = node_2_Ptr->get_type();
        targetEcoPtr = esrc::get_ecoSysPtr( esrc::apply_a_ecoSysId_by_type(ecoType, this->weight) );
    }
    //------------------------//
    //          左上
    //------------------------//
    else if( (oddEven.x==0) && (oddEven.y==1) ){
        node_1_Ptr = esrc::get_ecoSysPtr( nearFour_node_ecoSysInMapPtrs.at(0)->get_ecoSysId() );
        node_2_Ptr = esrc::get_ecoSysPtr( nearFour_node_ecoSysInMapPtrs.at(2)->get_ecoSysId() );

        (uDistribution_2(randEngine)==0) ?
                ecoType = node_1_Ptr->get_type() :
                ecoType = node_2_Ptr->get_type();
        targetEcoPtr = esrc::get_ecoSysPtr( esrc::apply_a_ecoSysId_by_type(ecoType, this->weight) );
    }
    //------------------------//
    //          右上
    //------------------------//
    else{
        node_1_Ptr = esrc::get_ecoSysPtr( nearFour_node_ecoSysInMapPtrs.at(0)->get_ecoSysId() );
        node_2_Ptr = esrc::get_ecoSysPtr( nearFour_node_ecoSysInMapPtrs.at(1)->get_ecoSysId() );
        node_3_Ptr = esrc::get_ecoSysPtr( nearFour_node_ecoSysInMapPtrs.at(2)->get_ecoSysId() );
        node_4_Ptr = esrc::get_ecoSysPtr( nearFour_node_ecoSysInMapPtrs.at(3)->get_ecoSysId() );

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
    //--- 完整的复制 landColors 数据 ---
    this->landColors.insert(this->landColors.end(),
                            _targetEcoPtr->get_landColors().begin(),
                            _targetEcoPtr->get_landColors().end() );

    this->applyPercents.insert( this->applyPercents.end(),
                                _targetEcoPtr->get_applyPercents().begin(),
                                _targetEcoPtr->get_applyPercents().end() );

    this->densityDivideVals.insert( this->densityDivideVals.end(),
                                    _targetEcoPtr->get_densityDivideVals().begin(),
                                    _targetEcoPtr->get_densityDivideVals().end() );

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



namespace{//-------- namespace: --------------//


/* ===========================================================
 *       reset_nearFour_node_ecoSysInMap_sectionMPos
 * -----------------------------------------------------------
 */
void reset_nearFour_node_ecoSysInMap_sectionMPos( const IntVec2 &_ecosysInMapMPos ){
    //-- 获得 (2*2 section) 单元 左下角 mpos --
    float sideLen = 2*ENTS_PER_SECTION;
    IntVec2 baseMPos = floorDiv( _ecosysInMapMPos, sideLen ) * sideLen;
    //-----
    nearFour_node_ecoSysInMap_sectionMPos.clear();
    for( const auto &off :  nearFour_node_ecoSysInMap_mposOffs ){ //- each off mpos
        nearFour_node_ecoSysInMap_sectionMPos.push_back( off+baseMPos );
    }
}


}//------------- namespace: end --------------//


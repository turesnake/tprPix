/*
 * ========================== EcoObj.cpp =======================
 *                          -- tpr --
 *                                        CREATE -- 2019.03.02
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#include "EcoObj.h"

//--- glm - 0.9.9.5 ---
#include <glm/glm.hpp>
            //-- glm::vec3
            //-- glm::vec4
            //-- glm::mat4

//-------------------- C --------------------//
#include <cmath>

//-------------------- Engine --------------------//
#include "tprAssert.h"
#include "config.h"
#include "random.h"
#include "PerlinNoise3D.h" //- out 
#include "Density.h"
#include "simplexNoise.h"
#include "esrc_gameSeed.h"
#include "esrc_ecoSysPlan.h"
#include "esrc_ecoObj.h"

#include "tprDebug.h"


namespace{//-------- namespace: --------------//

    std::default_random_engine  randEngine; //-通用 随机数引擎实例
    inline std::uniform_real_distribution<float> uDistribution_f(-100.0f,100.0f);
    inline std::uniform_int_distribution<int> uDistribution_2(0,1);
    inline std::uniform_int_distribution<int> uDistribution_4(0,3);


    //-- 周边4个 node 实例 mpos off 值 --
    const std::vector<IntVec2> nearFour_node_ecoObj_mposOffs {
        IntVec2{ 0, 0 },
        IntVec2{ ENTS_PER_SECTION*2, 0 },
        IntVec2{ 0, ENTS_PER_SECTION*2 },
        IntVec2{ ENTS_PER_SECTION*2, ENTS_PER_SECTION*2 }
    };

}//------------- namespace: end --------------//


/* ===========================================================
 *          calc_nearFour_node_ecoObjKey   [static] 
 * -----------------------------------------------------------
 * 生成 目标ecoObjKey 周边 4个 node key 的值，写入 参数容器中
 * -- 将被 atom_try_to_inert_and_init_a_ecoObj() 调用
 */
void EcoObj::calc_nearFour_node_ecoObjKey(sectionKey_t _targetKey,
                                                    std::vector<sectionKey_t> &_container ){
    //-- 获得 (2*2 section) 单元 左下角 mpos --
    int sideLen = 2*ENTS_PER_SECTION;
    IntVec2 baseMPos = floorDiv( sectionKey_2_mpos(_targetKey), static_cast<float>(sideLen) ) * sideLen;

    _container.clear();
    for( const auto &off :  nearFour_node_ecoObj_mposOffs ){ //- each off mpos
        _container.push_back( sectionMPos_2_sectionKey(baseMPos + off) );
    }
}




/* ===========================================================
 *                    init_for_node
 * -----------------------------------------------------------
 */
void EcoObj::init_for_node( sectionKey_t _sectionKey ){

    this->init_fstOrder( _sectionKey );

    tprAssert( (this->oddEven.x==0) && (this->oddEven.y==0) ); //- must be node 
    EcoSysPlan *ecoSysPlanPtr = esrc::get_ecoSysPlanPtr( esrc::apply_a_rand_ecoSysPlanId(this->weight) );
    //------------------------//
    //  确定 targetEcoPlanPtr 后, 正式 分配数据
    //------------------------//
    this->copy_datas_from_ecoSysPlan( ecoSysPlanPtr );
}



/* ===========================================================
 *                   init_for_regular
 * -----------------------------------------------------------
 */
void EcoObj::init_for_regular( sectionKey_t _sectionKey,
                                    const std::vector<sectionKey_t> &_nearby_four_ecoSysPlanIds ){

    this->init_fstOrder( _sectionKey );
    this->init_for_no_node_ecoObj( _nearby_four_ecoSysPlanIds );
}



/* ===========================================================
 *                      init_fstOrder
 * -----------------------------------------------------------
 * -- 仅初始化几个 最简单的数据
 */
void EcoObj::init_fstOrder( sectionKey_t _sectionKey ){

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
    // 3*3 个 ecoObj 组成一个 pn晶格
    float freq = 1.0f / 3.0f; 
    glm::vec2 fv = this->mcpos.get_fpos();
    fv /= ENTS_PER_SECTION;
    fv += esrc::get_gameSeed().get_ecoObjWeight_pposOff();

    this->weight = simplex_noise2(  fv.x * freq,
                                    fv.y * freq ) * 100.0f; //- [-100.0, 100.0]
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
 *                init_for_no_node_ecoObj
 * -----------------------------------------------------------
 * -- 完成后半段初始化。 仅用于 非 node 实例
 */
void EcoObj::init_for_no_node_ecoObj( const std::vector<sectionKey_t> &_nearby_four_ecoSysPlanIds ){

    EcoSysPlan *node_1_Ptr;
    EcoSysPlan *node_2_Ptr;
    EcoSysPlan *node_3_Ptr;
    EcoSysPlan *node_4_Ptr;
    EcoSysPlan *targetEcoPlanPtr;

    EcoSysPlanType   ecoPlanType;

    randEngine.seed( static_cast<size_t>(this->weight) ); //- 实现了伪随机

    //------------------------//
    //          右下
    //------------------------//
    if( (oddEven.x==1) && (oddEven.y==0) ){
        node_1_Ptr = esrc::get_ecoSysPlanPtr( static_cast<ecoSysPlanId_t>(_nearby_four_ecoSysPlanIds.at(0)) );
        node_2_Ptr = esrc::get_ecoSysPlanPtr( static_cast<ecoSysPlanId_t>(_nearby_four_ecoSysPlanIds.at(1)) );

        (uDistribution_2(randEngine)==0) ?
                ecoPlanType = node_1_Ptr->get_type() :
                ecoPlanType = node_2_Ptr->get_type();
        targetEcoPlanPtr = esrc::get_ecoSysPlanPtr( static_cast<ecoSysPlanId_t>(esrc::apply_a_ecoSysPlanId_by_type(ecoPlanType, this->weight)) );
    }
    //------------------------//
    //          左上
    //------------------------//
    else if( (oddEven.x==0) && (oddEven.y==1) ){
        node_1_Ptr = esrc::get_ecoSysPlanPtr( static_cast<ecoSysPlanId_t>(_nearby_four_ecoSysPlanIds.at(0)) );
        node_2_Ptr = esrc::get_ecoSysPlanPtr( static_cast<ecoSysPlanId_t>(_nearby_four_ecoSysPlanIds.at(2)) );

        (uDistribution_2(randEngine)==0) ?
                ecoPlanType = node_1_Ptr->get_type() :
                ecoPlanType = node_2_Ptr->get_type();
        targetEcoPlanPtr = esrc::get_ecoSysPlanPtr( static_cast<ecoSysPlanId_t>(esrc::apply_a_ecoSysPlanId_by_type(ecoPlanType, this->weight)) );
    }
    //------------------------//
    //          右上
    //------------------------//
    else{
        node_1_Ptr = esrc::get_ecoSysPlanPtr( static_cast<ecoSysPlanId_t>(_nearby_four_ecoSysPlanIds.at(0)) );
        node_2_Ptr = esrc::get_ecoSysPlanPtr( static_cast<ecoSysPlanId_t>(_nearby_four_ecoSysPlanIds.at(1)) );
        node_3_Ptr = esrc::get_ecoSysPlanPtr( static_cast<ecoSysPlanId_t>(_nearby_four_ecoSysPlanIds.at(2)) );
        node_4_Ptr = esrc::get_ecoSysPlanPtr( static_cast<ecoSysPlanId_t>(_nearby_four_ecoSysPlanIds.at(3)) );

        switch( uDistribution_4(randEngine) ){
            case 0: ecoPlanType = node_1_Ptr->get_type(); break;
            case 1: ecoPlanType = node_2_Ptr->get_type(); break;
            case 2: ecoPlanType = node_3_Ptr->get_type(); break;
            case 3: ecoPlanType = node_4_Ptr->get_type(); break;
            default:
                tprAssert(0);
                ecoPlanType = node_1_Ptr->get_type(); // never reach
        }
        targetEcoPlanPtr = esrc::get_ecoSysPlanPtr( static_cast<ecoSysPlanId_t>(esrc::apply_a_ecoSysPlanId_by_type(ecoPlanType, this->weight)) );
    }

    //------------------------//
    //  确定 targetEcoPlanPtr 后, 正式 分配数据
    //------------------------//
    this->copy_datas_from_ecoSysPlan( targetEcoPlanPtr );
}


/* ===========================================================
 *               copy_datas_from_ecoSysPlan
 * -----------------------------------------------------------
 * -- 
 */
void EcoObj::copy_datas_from_ecoSysPlan( EcoSysPlan *_targetEcoPlanPtr ){

    randEngine.seed( static_cast<size_t>(this->weight) ); //- 实现了伪随机

    this->ecoSysPlanId = _targetEcoPlanPtr->get_id();
    this->ecoSysPlanType = _targetEcoPlanPtr->get_type();
    this->densitySeaLvlOff = _targetEcoPlanPtr->get_densitySeaLvlOff();

    //--- 仅 获得 只读指针 ---
    this->landColorsPtr = _targetEcoPlanPtr->get_landColorsPtr();
    this->applyPercentsPtr = _targetEcoPlanPtr->get_applyPercentsPtr();
    this->densityDivideValsPtr = _targetEcoPlanPtr->get_densityDivideValsPtr();

    //---- goSpecIdPools 数据 ----
    goSpecId_t  tmpGoSpecId;
    for( size_t i=0; i<Density::get_idxNum(); i++ ){ //- each pool in goSpecIdPools
        //--- 取 8 个元素 ---
        for( int ci=0; ci<8; ci++ ){ 
            tmpGoSpecId = _targetEcoPlanPtr->apply_a_rand_goSpecId( i, uDistribution_f(randEngine) );
            this->goSpecIdPools.at(i).push_back( tmpGoSpecId );
        }
    }
}


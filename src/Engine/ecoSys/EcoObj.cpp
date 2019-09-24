/*
 * ========================== EcoObj.cpp =======================
 *                          -- tpr --
 *                                        CREATE -- 2019.03.02
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#include "EcoObj.h"

//--- glm - 0.9.9.5 ---
#include "glm_no_warnings.h"

//-------------------- C --------------------//
#include <cmath>

//-------------------- Engine --------------------//
#include "tprAssert.h"
#include "tprCast.h"
#include "config.h"
#include "random.h"
#include "Density.h"
#include "simplexNoise.h"
#include "esrc_gameSeed.h"
#include "esrc_ecoSysPlan.h"
#include "esrc_ecoObj.h"

#include "tprDebug.h"


namespace ecoObj_inn {//-------- namespace: ecoObj_inn --------------//

    std::default_random_engine  rEngine; //-通用 随机数引擎实例
    inline std::uniform_real_distribution<double> uDistribution_f(0.0,100.0);
    inline std::uniform_int_distribution<int> uDistribution_2(0,1);
    inline std::uniform_int_distribution<int> uDistribution_4(0,3);


    //-- 周边4个 node 实例 mpos off 值 --
    const std::vector<IntVec2> nearFour_node_ecoObj_mposOffs {
        IntVec2{ 0, 0 },
        IntVec2{ ENTS_PER_SECTION*2, 0 },
        IntVec2{ 0, ENTS_PER_SECTION*2 },
        IntVec2{ ENTS_PER_SECTION*2, ENTS_PER_SECTION*2 }
    };

}//------------- namespace: ecoObj_inn end --------------//


/* ===========================================================
 *          calc_nearFour_node_ecoObjKey   [static] 
 * -----------------------------------------------------------
 * 生成 目标ecoObjKey 周边 4个 node key 的值，写入 参数容器中
 * -- 将被 atom_try_to_inert_and_init_a_ecoObj() 调用
 */
void EcoObj::calc_nearFour_node_ecoObjKey(sectionKey_t targetKey_,
                                                    std::vector<sectionKey_t> &container_ ){
    //-- 获得 (2*2 section) 单元 左下角 mpos --
    int sideLen { 2*ENTS_PER_SECTION };
    IntVec2 baseMPos = floorDiv( sectionKey_2_mpos(targetKey_), static_cast<double>(sideLen) ) * sideLen;

    container_.clear();
    for( const auto &off :  ecoObj_inn::nearFour_node_ecoObj_mposOffs ){ //- each off mpos
        container_.push_back( sectionMPos_2_sectionKey(baseMPos + off) );
    }
}




/* ===========================================================
 *                    init_for_node
 * -----------------------------------------------------------
 */
void EcoObj::init_for_node( sectionKey_t sectionKey_ ){

    this->init_fstOrder( sectionKey_ );

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
void EcoObj::init_for_regular( sectionKey_t sectionKey_,
                                    const std::vector<sectionKey_t> &nearby_four_ecoSysPlanIds_ ){

    this->init_fstOrder( sectionKey_ );
    this->init_for_no_node_ecoObj( nearby_four_ecoSysPlanIds_ );
}



/* ===========================================================
 *                      init_fstOrder
 * -----------------------------------------------------------
 * -- 仅初始化几个 最简单的数据
 */
void EcoObj::init_fstOrder( sectionKey_t sectionKey_ ){

    //------------------//
    //    key / mpos
    //------------------//
    this->sectionKey = sectionKey_;
    this->mcpos.set_by_mpos( sectionKey_2_mpos(this->sectionKey) );
    //------------------//
    //     oddEven
    //------------------//
    IntVec2 SPos = floorDiv( this->get_mpos(), static_cast<double>(ENTS_PER_SECTION) );
    this->oddEven = floorMod( SPos, 2.0 );
    //------------------//
    //     weight
    //------------------//
    // 3*3 个 ecoObj 组成一个 pn晶格
    double freq = 1.0 / 3.0; 
    glm::dvec2 fv = this->mcpos.get_dpos();
    fv /= static_cast<double>(ENTS_PER_SECTION);
    fv += esrc::get_gameSeed().get_ecoObjWeight_dposOff();

    this->weight = simplex_noise2(  fv.x * freq,
                                    fv.y * freq ) * 100.0; //- [-100.0, 100.0]
    //------------------//
    //   occupyWeight
    //------------------//
    size_t randV = cast_2_size_t(floor( this->weight * 3.1 + 757.3 ));
    this->occupyWeight = calc_occupyWeight( this->oddEven, randV );
    //------------------------------//
    //  landColors / goSpecIdPools
    //------------------------------//
    this->goSpecDataPools.clear();
    this->goSpecDataPools.resize( Density::get_idxNum(), std::vector<GoSpecData>{} );
}


/* ===========================================================
 *                init_for_no_node_ecoObj
 * -----------------------------------------------------------
 * -- 完成后半段初始化。 仅用于 非 node 实例
 */
void EcoObj::init_for_no_node_ecoObj( const std::vector<sectionKey_t> &nearby_four_ecoSysPlanIds_ ){

    EcoSysPlan *node_1_Ptr       {nullptr};
    EcoSysPlan *node_2_Ptr       {nullptr};
    EcoSysPlan *node_3_Ptr       {nullptr};
    EcoSysPlan *node_4_Ptr       {nullptr};
    EcoSysPlan *targetEcoPlanPtr {nullptr};

    EcoSysPlanType   ecoPlanType {};

    ecoObj_inn::rEngine.seed( static_cast<u32_t>(this->weight) ); //- 实现了伪随机

    //------------------------//
    //          右下
    //------------------------//
    if( (oddEven.x==1) && (oddEven.y==0) ){
        node_1_Ptr = esrc::get_ecoSysPlanPtr( static_cast<ecoSysPlanId_t>(nearby_four_ecoSysPlanIds_.at(0)) );
        node_2_Ptr = esrc::get_ecoSysPlanPtr( static_cast<ecoSysPlanId_t>(nearby_four_ecoSysPlanIds_.at(1)) );

        (ecoObj_inn::uDistribution_2(ecoObj_inn::rEngine)==0) ?
                ecoPlanType = node_1_Ptr->get_type() :
                ecoPlanType = node_2_Ptr->get_type();
        targetEcoPlanPtr = esrc::get_ecoSysPlanPtr( static_cast<ecoSysPlanId_t>(esrc::apply_a_ecoSysPlanId_by_type(ecoPlanType, this->weight)) );
    }
    //------------------------//
    //          左上
    //------------------------//
    else if( (oddEven.x==0) && (oddEven.y==1) ){
        node_1_Ptr = esrc::get_ecoSysPlanPtr( static_cast<ecoSysPlanId_t>(nearby_four_ecoSysPlanIds_.at(0)) );
        node_2_Ptr = esrc::get_ecoSysPlanPtr( static_cast<ecoSysPlanId_t>(nearby_four_ecoSysPlanIds_.at(2)) );

        (ecoObj_inn::uDistribution_2(ecoObj_inn::rEngine)==0) ?
                ecoPlanType = node_1_Ptr->get_type() :
                ecoPlanType = node_2_Ptr->get_type();
        targetEcoPlanPtr = esrc::get_ecoSysPlanPtr( static_cast<ecoSysPlanId_t>(esrc::apply_a_ecoSysPlanId_by_type(ecoPlanType, this->weight)) );
    }
    //------------------------//
    //          右上
    //------------------------//
    else{
        node_1_Ptr = esrc::get_ecoSysPlanPtr( static_cast<ecoSysPlanId_t>(nearby_four_ecoSysPlanIds_.at(0)) );
        node_2_Ptr = esrc::get_ecoSysPlanPtr( static_cast<ecoSysPlanId_t>(nearby_four_ecoSysPlanIds_.at(1)) );
        node_3_Ptr = esrc::get_ecoSysPlanPtr( static_cast<ecoSysPlanId_t>(nearby_four_ecoSysPlanIds_.at(2)) );
        node_4_Ptr = esrc::get_ecoSysPlanPtr( static_cast<ecoSysPlanId_t>(nearby_four_ecoSysPlanIds_.at(3)) );

        switch( ecoObj_inn::uDistribution_4(ecoObj_inn::rEngine) ){
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
 */
void EcoObj::copy_datas_from_ecoSysPlan( EcoSysPlan *targetEcoPlanPtr_ ){

    ecoObj_inn::rEngine.seed( static_cast<u32_t>(this->weight) ); //- 实现了伪随机

    this->ecoSysPlanId = targetEcoPlanPtr_->get_id();
    this->ecoSysPlanType = targetEcoPlanPtr_->get_type();
    this->densitySeaLvlOff = targetEcoPlanPtr_->get_densitySeaLvlOff();
    this->colorTableId = targetEcoPlanPtr_->get_colorTableId();

    //--- 仅 获得 只读指针 ---
    this->applyPercentsPtr = targetEcoPlanPtr_->get_applyPercentsPtr();
    this->densityDivideValsPtr = targetEcoPlanPtr_->get_densityDivideValsPtr();

    //---- goSpecIdPools 数据 ----
    //goSpecId_t  tmpGoSpecId {};
    for( size_t i=0; i<Density::get_idxNum(); i++ ){ //- each pool in goSpecIdPools
        //--- 取 8 个元素 ---
        for( int ci=0; ci<8; ci++ ){ 
            const auto &goSpecData = targetEcoPlanPtr_->apply_a_rand_goSpecData( i, ecoObj_inn::uDistribution_f(ecoObj_inn::rEngine) );
            this->goSpecDataPools.at(i).push_back( goSpecData ); // copy
        }
    }
}


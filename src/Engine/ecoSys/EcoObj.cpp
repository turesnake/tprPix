/*
 * ========================== EcoObj.cpp =======================
 *                          -- tpr --
 *                                        CREATE -- 2019.03.02
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#include "pch.h"
#include "EcoObj.h"

//-------------------- Engine --------------------//
#include "random.h"
#include "Density.h"
#include "simplexNoise.h"
#include "esrc_gameSeed.h"
#include "esrc_ecoSysPlan.h"
#include "esrc_ecoObj.h"



namespace ecoObj_inn {//-------- namespace: ecoObj_inn --------------//

    //-- 周边4个 node 实例 mpos off 值 --
    const std::vector<IntVec2> nearFour_node_ecoObj_mposOffs {
        IntVec2{ 0, 0 },
        IntVec2{ ENTS_PER_SECTION<>*2, 0 },
        IntVec2{ 0, ENTS_PER_SECTION<>*2 },
        IntVec2{ ENTS_PER_SECTION<>*2, ENTS_PER_SECTION<>*2 }
    };

}//------------- namespace: ecoObj_inn end --------------//


/* [static] 
 * 生成 目标ecoObjKey 周边 4个 node key 的值，写入 参数容器中
 * -- 将被 atom_try_to_inert_and_init_a_ecoObj() 调用
 */
void EcoObj::calc_nearFour_node_ecoObjKey(sectionKey_t targetKey_,
                                            std::vector<sectionKey_t> &container_ ){
    //-- 获得 (2*2 section) 单元 左下角 mpos --
    int sideLen { 2*ENTS_PER_SECTION<> };
    IntVec2 baseMPos = floorDiv( sectionKey_2_mpos(targetKey_), static_cast<double>(sideLen) ) * sideLen;

    container_.clear();
    for( const auto &off :  ecoObj_inn::nearFour_node_ecoObj_mposOffs ){ //- each off mpos
        container_.push_back( sectionMPos_2_sectionKey(baseMPos + off) );
    }
}



void EcoObj::init( sectionKey_t sectionKey_ ){

    this->init_fstOrder( sectionKey_ );

    EcoSysPlan *ecoSysPlanPtr = esrc::get_ecoSysPlanPtr( esrc::apply_a_rand_ecoSysPlanId(this->uWeight) );
    //------------------------//
    //  确定 targetEcoPlanPtr 后, 正式 分配数据
    //------------------------//
    this->copy_datas_from_ecoSysPlan( ecoSysPlanPtr );

}


/* 仅初始化几个 最简单的数据
 */
void EcoObj::init_fstOrder( sectionKey_t sectionKey_ ){

    //------------------//
    //    key / mpos
    //------------------//
    this->sectionKey = sectionKey_;
    //this->mcpos.set_by_mpos( sectionKey_2_mpos(this->sectionKey) );
    this->mpos = sectionKey_2_mpos(this->sectionKey);
    //------------------//
    //     oddEven
    //------------------//
    IntVec2 SPos = floorDiv( this->mpos, ENTS_PER_SECTION_D );
    this->oddEven = floorMod( SPos, 2.0 );

    //------------------//
    //    self uWeight
    //------------------//
    // 3*3 个 ecoObj 组成一个 pn晶格
    //double freq2 = 1.0 / 3.0; 
    //glm::dvec2 fv = this->mcpos.get_dpos();
    //fv /= static_cast<double>(ENTS_PER_SECTION<>);
    //fv += esrc::get_gameSeed().get_ecoObjWeight_dposOff();

    //double originPerlin = simplex_noise2( fv.x * freq2, fv.y * freq2 ); //- [-1.0, 1.0]
    //this->uWeight = blender_the_perlinNoise(originPerlin, 279771, 10000); // [0,9999]

    this->uWeight = calc_simple_mapent_uWeight( this->mpos );

    //------------------//
    //   occupyWeight
    //------------------//
    this->occupyWeight = calc_occupyWeight( this->oddEven, this->uWeight );
    //------------------------------//
    //       densityPools
    //------------------------------//
    //...

    //------------------------------//
    //     ecoObjBorderPtr
    //------------------------------//
    this->ecoObjBorderPtr = EcoObjBorder::apply_rand_ecoObjBorderPtr( this->uWeight );

}


void EcoObj::copy_datas_from_ecoSysPlan( EcoSysPlan *targetEcoPlanPtr_ ){

    this->ecoSysPlanId = targetEcoPlanPtr_->get_id();
    this->ecoSysPlanType = targetEcoPlanPtr_->get_type();
    this->densitySeaLvlOff = targetEcoPlanPtr_->get_densitySeaLvlOff();
    this->colorTableId = targetEcoPlanPtr_->get_colorTableId();

    //--- 仅 获得 只读指针 ---
    this->densityDivideValsPtr = targetEcoPlanPtr_->get_densityDivideValsPtr();

    //---- goSpeciesIdPools 数据 ----

    //-- 最直接的方案，全盘复制 densityPools 数据。  超级临时 版本 ！！！！！！

    this->densityPoolsPtr = &(targetEcoPlanPtr_->get_densityPools());
                
                //-- 在没确定 ecoobj densitypool 分配方式之前
                //   先用指针，临时借用 ecoplan 的数据
                // ...

    //--- blueprint ---//                        
    this->villageBlueprintId = targetEcoPlanPtr_->apply_rand_villageBlueprintId( this->uWeight );

    this->natureFloorYardId = targetEcoPlanPtr_->apply_rand_natureFlooryardId( this->uWeight );
    this->natureFloorDensitysPtr = targetEcoPlanPtr_->get_natureFloorDensitysPtr();


    //=======================//
    //   goDatasForCreate
    //=======================//
    // 一次性生成 所有 人造物 godata 
    blueprint::build_ecoObj_goDatasForCreate(   this->villageBlueprintId,
                                                this->mpos,
                                                this->uWeight,
                                                this->majorGoDatasForCreate,
                                                this->floorGoDatasForCreate,
                                                this->artifactFieldKeys );
    

}


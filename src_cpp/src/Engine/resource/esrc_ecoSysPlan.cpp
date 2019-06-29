/*
 * ===================== esrc_ecoSysPlan.cpp =====================
 *                          -- tpr --
 *                                        CREATE -- 2019.02.22
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *  资源管理:  ecoSysPlanes
 * ----------------------------
 */
//-------------------- Engine --------------------//
#include "tprAssert.h"
#include "random.h"
#include "esrc_ecoSysPlan.h"

//-------------------- Script --------------------//
#include "Script/resource/ssrc.h" 

//#include "tprDebug.h" //- tmp


namespace esrc {//------------------ namespace: esrc -------------------------//


namespace ecoPlan_inn {//-------- namespace: ecoPlan_inn --------------//
                                            
    Density  density_m3 {-3};
    Density  density_m2 {-2};
    Density  density_m1 {-1};
    Density  density_0  {0};
    Density  density_1  {1};
    Density  density_2  {2};
    Density  density_3  {3};

    //--- landColor ---
    RGBA  color_Forest     { 130, 150, 109, 255 };
    RGBA  color_DarkForest { 106, 130, 113, 255 };
    RGBA  color_Plain      { 155, 159, 134, 255 };
    RGBA  color_Swamp      { 138, 139, 124, 255 };
    RGBA  color_Desert     { 210, 195, 142, 255 };

    //--- densityDivideVals ---
    std::vector<float> densityDivideVals_default 
        { -65.0f, -40.0f, -15.0f, 15.0f, 40.0f, 65.0f }; //- 较为均匀的分布

    std::vector<float> densityDivideVals_50_20_50
        { -50.0f, -30.0f, -10.0f, 10.0f, 30.0f, 50.0f }; //- 两极各占:50，中间lvl各占:20

    
    std::unordered_map<ecoSysPlanId_t, EcoSysPlan> ecoSysPlanes {};

    //-- 一种type，一个子容器，容纳此type 的所有变种 --
    std::vector<std::vector<ecoSysPlanId_t>> ecoSysPlanIds_in_type {};


    //-- 仅记录所有 ecoId 值 --
    std::vector<ecoSysPlanId_t> ecoSysPlanIds {};
    

    //===== funcs =====//
    void init_Forest_1();
    void init_Forest_2();
    void init_DarkForest_1();
    void init_Plain_1();
    void init_Swamp_1();
    void init_Desert_1();
    void init_Desert_2();


}//------------- namespace: ecoPlan_inn end --------------//


/* ===========================================================
 *                  get_ecoSysPlanPtr
 * -----------------------------------------------------------
 */
EcoSysPlan *get_ecoSysPlanPtr( ecoSysPlanId_t ecoId_ ){
        tprAssert( ecoPlan_inn::ecoSysPlanes.find(ecoId_) != ecoPlan_inn::ecoSysPlanes.end() ); //- tmp
    return &(ecoPlan_inn::ecoSysPlanes.at(ecoId_));
}


/* ===========================================================
 *                  insert_new_ecoSysPlan
 * -----------------------------------------------------------
 * -- 仅用于本文件内部
 */
EcoSysPlan *insert_new_ecoSysPlan( EcoSysPlanType type_ ){

    // ***| INSERT FIRST, INIT LATER  |***
    EcoSysPlan  ecoPlan {};
    ecoSysPlanId_t ecoPlanId = EcoSysPlan::id_manager.apply_a_u32_id();
    ecoPlan.set_id( ecoPlanId );
    ecoPlan.set_type( type_ );
        tprAssert( ecoPlan_inn::ecoSysPlanes.find(ecoPlanId) == ecoPlan_inn::ecoSysPlanes.end() );//- must not exist
    ecoPlan_inn::ecoSysPlanes.insert({ ecoPlanId, ecoPlan }); //- copy
    ecoPlan_inn::ecoSysPlanIds_in_type.at(ecoSysPlanType_2_idx(type_)).push_back(ecoPlanId);
    ecoPlan_inn::ecoSysPlanIds.push_back(ecoPlanId);
    return &(ecoPlan_inn::ecoSysPlanes.at(ecoPlanId)); 
}


/* ===========================================================
 *             apply_a_ecoSysPlanId_by_type
 * -----------------------------------------------------------
 * -- 指定了 type，在此基础上，分配一个 变种id
 */
ecoSysPlanId_t apply_a_ecoSysPlanId_by_type( EcoSysPlanType type_, float ecoObjWeight_ ){
    size_t randV = static_cast<size_t>(floor( ecoObjWeight_ * 2.7f + 907.9f ));
    auto &container = ecoPlan_inn::ecoSysPlanIds_in_type.at(ecoSysPlanType_2_idx(type_));    
    return container.at( randV % container.size() );
}


/* ===========================================================
 *              apply_a_rand_ecoSysPlanId
 * -----------------------------------------------------------
 */
ecoSysPlanId_t apply_a_rand_ecoSysPlanId( float ecoObjWeight_ ){
    size_t randV = static_cast<size_t>(floor( ecoObjWeight_ * 7.1f + 977.3f ));
    size_t idx = randV % ecoPlan_inn::ecoSysPlanIds.size();
    return ecoPlan_inn::ecoSysPlanIds.at(idx);
}


/* ===========================================================
 *                      init_ecoSysPlanes
 * -----------------------------------------------------------
 * -- 在游戏初始化阶段，被调用。
 * -- 统一初始化 所有 EcoSysPlan 资源
 *    不追求性能
 */
void init_ecoSysPlanes(){

    ecoPlan_inn::ecoSysPlanes.clear();
    ecoPlan_inn::ecoSysPlanIds_in_type.clear();
    ecoPlan_inn::ecoSysPlanIds_in_type.resize( EcoSysPlanType_Num );
    ecoPlan_inn::ecoSysPlanIds.clear();
    //---------------------//
    
    //ecoPlan_inn::init_Forest_1();
    //ecoPlan_inn::init_Forest_2();
    ecoPlan_inn::init_DarkForest_1();
    //ecoPlan_inn::init_Plain_1();
    //ecoPlan_inn::init_Swamp_1();
    //ecoPlan_inn::init_Desert_1();
    ecoPlan_inn::init_Desert_2();
    
    //---------------------//
    //   shuffle ecoSysPlanIds
    //---------------------//
    std::default_random_engine randEngine; 
    randEngine.seed(431); //- 提供固定seed
    std::shuffle(   ecoPlan_inn::ecoSysPlanIds.begin(), 
                    ecoPlan_inn::ecoSysPlanIds.end(),
                    randEngine );
}

namespace ecoPlan_inn {//-------- namespace: ecoPlan_inn --------------//

    
/* ===========================================================
 *                  init_Forest_1
 * -----------------------------------------------------------
 */
void init_Forest_1(){
    EcoSysPlan *ecoPlanPtr = insert_new_ecoSysPlan( EcoSysPlanType::Forest );
        ecoPlanPtr->init_landColor_onlyHighLand( ecoPlan_inn::color_Forest );

        ecoPlanPtr->init_densityDatas( 15.0f, ecoPlan_inn::densityDivideVals_default );

        ecoPlanPtr->init_goSpecIdPools_and_applyPercents();
        // -3
        ecoPlanPtr->insert(  ecoPlan_inn::density_m3, 0.0f, 
                            std::vector<EcoEnt>{
                                EcoEnt{ "lichen_Forest", 5 } 
                            });
        // -2
        ecoPlanPtr->insert(  ecoPlan_inn::density_m2, 0.1f, 
                            std::vector<EcoEnt>{
                                EcoEnt{ "lichen_Forest", 5 } 
                            });
        // -1
        ecoPlanPtr->insert(  ecoPlan_inn::density_m1, 0.1f, 
                            std::vector<EcoEnt>{
                                EcoEnt{ "lichen_Forest", 5 } 
                            });
        //  0
        ecoPlanPtr->insert(  ecoPlan_inn::density_0, 0.2f, 
                            std::vector<EcoEnt>{
                                EcoEnt{ "lichen_Forest", 5 } 
                            });
        //  1
        ecoPlanPtr->insert(  ecoPlan_inn::density_1, 0.4f, 
                            std::vector<EcoEnt>{
                                EcoEnt{ "lichen_Forest", 5 },
                                EcoEnt{ "oakTree",       3 }
                            });
        //  2
        ecoPlanPtr->insert(  ecoPlan_inn::density_2, 0.6f, 
                            std::vector<EcoEnt>{
                                EcoEnt{ "lichen_Forest", 3 },
                                EcoEnt{ "oakTree",       5 }
                            });
        //  3
        ecoPlanPtr->insert(  ecoPlan_inn::density_3, 0.95f, 
                            std::vector<EcoEnt>{
                                EcoEnt{ "oakTree",   9 },
                                EcoEnt{ "pineTree",  1 }
                            });
    // shuffle
    ecoPlanPtr->shuffle_goSpecIdPools( 43 ); //- 提供固定seed
    //------ end -------
    ecoPlanPtr->chueck_end();
}


/* ===========================================================
 *                  init_Forest_2   密林
 * -----------------------------------------------------------
 * 目前还存在小小的缺陷。 density freq 看起来有点大
 * 树木还不够密集
 */
void init_Forest_2(){
    EcoSysPlan *ecoPlanPtr = insert_new_ecoSysPlan( EcoSysPlanType::Forest );
        ecoPlanPtr->init_landColor_doubleDeep( ecoPlan_inn::color_Forest );

        ecoPlanPtr->init_densityDatas( -15.0f, ecoPlan_inn::densityDivideVals_50_20_50 );

        ecoPlanPtr->init_goSpecIdPools_and_applyPercents();
        // -3
        ecoPlanPtr->insert(  ecoPlan_inn::density_m3, 0.98f, 
                            std::vector<EcoEnt>{
                                EcoEnt{ "oakTree",   8 },
                                EcoEnt{ "pineTree",  2 }
                            });
        // -2
        ecoPlanPtr->insert(  ecoPlan_inn::density_m2, 0.95f, 
                            std::vector<EcoEnt>{
                                EcoEnt{ "lichen_Forest", 2 },
                                EcoEnt{ "oakTree",       8 }
                            });
        // -1
        ecoPlanPtr->insert(  ecoPlan_inn::density_m1, 0.6f, 
                            std::vector<EcoEnt>{
                                EcoEnt{ "lichen_Forest", 5 } 
                            });
        //  0
        ecoPlanPtr->insert(  ecoPlan_inn::density_0, 0.0f, 
                            std::vector<EcoEnt>{
                                EcoEnt{ "lichen_Forest", 5 } 
                            });
        //  1
        ecoPlanPtr->insert(  ecoPlan_inn::density_1, 0.6f, 
                            std::vector<EcoEnt>{
                                EcoEnt{ "lichen_Forest", 5 },
                                EcoEnt{ "oakTree",       3 }
                            });
        //  2
        ecoPlanPtr->insert(  ecoPlan_inn::density_2, 0.95f, 
                            std::vector<EcoEnt>{
                                EcoEnt{ "lichen_Forest", 3 },
                                EcoEnt{ "oakTree",       7 }
                            });
        //  3
        ecoPlanPtr->insert(  ecoPlan_inn::density_3, 0.98f, 
                            std::vector<EcoEnt>{
                                EcoEnt{ "oakTree",   9 },
                                EcoEnt{ "pineTree",  1 }
                            });
    // shuffle
    ecoPlanPtr->shuffle_goSpecIdPools( 79 ); //- 提供固定seed
    //------ end -------
    ecoPlanPtr->chueck_end();
}


/* ===========================================================
 *                  init_DarkForest_1
 * -----------------------------------------------------------
 */
void init_DarkForest_1(){
    EcoSysPlan *ecoPlanPtr = insert_new_ecoSysPlan( EcoSysPlanType::DarkForest );
        ecoPlanPtr->init_landColor_onlyHighLand( ecoPlan_inn::color_DarkForest );

        ecoPlanPtr->init_densityDatas( -5.0f, ecoPlan_inn::densityDivideVals_default );

        ecoPlanPtr->init_goSpecIdPools_and_applyPercents();
        // -3
        ecoPlanPtr->insert(  ecoPlan_inn::density_m3, 0.0f, 
                            std::vector<EcoEnt>{
                                EcoEnt{ "lichen_Forest", 5 } 
                            });
        // -2
        ecoPlanPtr->insert(  ecoPlan_inn::density_m2, 0.1f, 
                            std::vector<EcoEnt>{
                                EcoEnt{ "lichen_Forest", 5 } 
                            });
        // -1
        ecoPlanPtr->insert(  ecoPlan_inn::density_m1, 0.2f, 
                            std::vector<EcoEnt>{
                                EcoEnt{ "lichen_Forest", 5 } 
                            });
        //  0
        ecoPlanPtr->insert(  ecoPlan_inn::density_0, 0.3f, 
                            std::vector<EcoEnt>{
                                EcoEnt{ "lichen_Forest", 5 } 
                            });
        //  1
        ecoPlanPtr->insert(  ecoPlan_inn::density_1, 0.5f, 
                            std::vector<EcoEnt>{
                                EcoEnt{ "lichen_Forest", 5 },
                                EcoEnt{ "pineTree",      3 }
                            });
        //  2
        ecoPlanPtr->insert(  ecoPlan_inn::density_2, 0.7f, 
                            std::vector<EcoEnt>{
                                EcoEnt{ "lichen_Forest", 3 },
                                EcoEnt{ "pineTree",      5 }
                            });
        //  3
        ecoPlanPtr->insert(  ecoPlan_inn::density_3, 0.95f, 
                            std::vector<EcoEnt>{
                                EcoEnt{ "pineTree", 9 }
                            });
    // shuffle
    ecoPlanPtr->shuffle_goSpecIdPools( 241 ); //- 提供固定seed
    //------ end -------
    ecoPlanPtr->chueck_end();
}


/* ===========================================================
 *                  init_Plain_1
 * -----------------------------------------------------------
 */
void init_Plain_1(){
    EcoSysPlan *ecoPlanPtr = insert_new_ecoSysPlan( EcoSysPlanType::Plain );
        ecoPlanPtr->init_landColor_onlyHighLand( ecoPlan_inn::color_Plain );

        ecoPlanPtr->init_densityDatas( -5.0f, ecoPlan_inn::densityDivideVals_default );

        ecoPlanPtr->init_goSpecIdPools_and_applyPercents();
        // -3
        ecoPlanPtr->insert(  ecoPlan_inn::density_m3, 0.5f, 
                            std::vector<EcoEnt>{
                                EcoEnt{ "lichen_Forest", 5 } 
                            });
        // -2
        ecoPlanPtr->insert(  ecoPlan_inn::density_m2, 0.3f, 
                            std::vector<EcoEnt>{
                                EcoEnt{ "lichen_Forest", 5 } 
                            });
        // -1
        ecoPlanPtr->insert(  ecoPlan_inn::density_m1, 0.1f, 
                            std::vector<EcoEnt>{
                                EcoEnt{ "lichen_Forest", 5 } 
                            });
        //  0
        ecoPlanPtr->insert(  ecoPlan_inn::density_0, 0.2f, 
                            std::vector<EcoEnt>{
                                EcoEnt{ "lichen_Forest", 5 } 
                            });
        //  1
        ecoPlanPtr->insert(  ecoPlan_inn::density_1, 0.3f, 
                            std::vector<EcoEnt>{
                                EcoEnt{ "lichen_Forest", 5 },
                                EcoEnt{ "oakTree",       3 } 
                            });
        //  2
        ecoPlanPtr->insert(  ecoPlan_inn::density_2, 0.4f, 
                            std::vector<EcoEnt>{
                                EcoEnt{ "lichen_Forest", 3 },
                                EcoEnt{ "oakTree",       5 } 
                            });
        //  3
        ecoPlanPtr->insert(  ecoPlan_inn::density_3, 0.6f, 
                            std::vector<EcoEnt>{
                                EcoEnt{ "oakTree",   9 },
                                EcoEnt{ "pineTree",  1 } 
                            });
    // shuffle
    ecoPlanPtr->shuffle_goSpecIdPools( 349 ); //- 提供固定seed
    //------ end -------
    ecoPlanPtr->chueck_end();
}

/* ===========================================================
 *                  init_Swamp_1
 * -----------------------------------------------------------
 */
void init_Swamp_1(){
    EcoSysPlan *ecoPlanPtr = insert_new_ecoSysPlan( EcoSysPlanType::Swamp );
        ecoPlanPtr->init_landColor_onlyHighLand( ecoPlan_inn::color_Swamp );

        ecoPlanPtr->init_densityDatas( -5.0f, ecoPlan_inn::densityDivideVals_default );

        ecoPlanPtr->init_goSpecIdPools_and_applyPercents();
        // -3
        ecoPlanPtr->insert(  ecoPlan_inn::density_m3, 0.5f, 
                            std::vector<EcoEnt>{
                                EcoEnt{ "lichen_Forest", 5 } 
                            });
        // -2
        ecoPlanPtr->insert(  ecoPlan_inn::density_m2, 0.3f, 
                            std::vector<EcoEnt>{
                                EcoEnt{ "lichen_Forest", 5 } 
                            });
        // -1
        ecoPlanPtr->insert(  ecoPlan_inn::density_m1, 0.1f, 
                            std::vector<EcoEnt>{
                                EcoEnt{ "lichen_Forest", 5 } 
                            });
        //  0
        ecoPlanPtr->insert(  ecoPlan_inn::density_0, 0.2f, 
                            std::vector<EcoEnt>{
                                EcoEnt{ "lichen_Forest", 5 } 
                            });
        //  1
        ecoPlanPtr->insert(  ecoPlan_inn::density_1, 0.4f, 
                            std::vector<EcoEnt>{
                                EcoEnt{ "lichen_Forest", 5 }, 
                                EcoEnt{ "oakTree",       3 }
                            });
        //  2
        ecoPlanPtr->insert(  ecoPlan_inn::density_2, 0.6f, 
                            std::vector<EcoEnt>{
                                EcoEnt{ "lichen_Forest", 3 }, 
                                EcoEnt{ "oakTree",       5 }
                            });
        //  3
        ecoPlanPtr->insert(  ecoPlan_inn::density_3, 0.85f, 
                            std::vector<EcoEnt>{
                                EcoEnt{ "oakTree",   9 }, 
                                EcoEnt{ "pineTree",  1 }
                            });
    // shuffle
    ecoPlanPtr->shuffle_goSpecIdPools( 31 ); //- 提供固定seed
    //------ end -------
    ecoPlanPtr->chueck_end();
}


/* ===========================================================
 *                  init_Desert_1
 * -----------------------------------------------------------
 */
void init_Desert_1(){
    EcoSysPlan *ecoPlanPtr = insert_new_ecoSysPlan( EcoSysPlanType::Desert );
        ecoPlanPtr->init_landColor_onlyHighLand( ecoPlan_inn::color_Desert );

        ecoPlanPtr->init_densityDatas( -5.0f, ecoPlan_inn::densityDivideVals_default );

        ecoPlanPtr->init_goSpecIdPools_and_applyPercents();
        // -3
        ecoPlanPtr->insert(  ecoPlan_inn::density_m3, 0.0f, 
                            std::vector<EcoEnt>{
                                EcoEnt{ "wheat", 5 } 
                            });
        // -2
        ecoPlanPtr->insert(  ecoPlan_inn::density_m2, 0.1f, 
                            std::vector<EcoEnt>{
                                EcoEnt{ "wheat",              5 }, 
                                EcoEnt{ "singleStone_Desert", 3 } 
                            });
        // -1
        ecoPlanPtr->insert(  ecoPlan_inn::density_m1, 0.2f, 
                            std::vector<EcoEnt>{
                                EcoEnt{ "wheat",              6 }, 
                                EcoEnt{ "singleStone_Desert", 2 } 
                            });
        //  0
        ecoPlanPtr->insert(  ecoPlan_inn::density_0, 0.3f, 
                            std::vector<EcoEnt>{
                                EcoEnt{ "wheat",              6 }, 
                                EcoEnt{ "singleStone_Desert", 2 } 
                            });
        //  1
        ecoPlanPtr->insert(  ecoPlan_inn::density_1, 0.5f, 
                            std::vector<EcoEnt>{
                                EcoEnt{ "wheat",              5 }, 
                                EcoEnt{ "singleStone_Desert", 3 },
                                EcoEnt{ "oakTree",            3 } 
                            });
        //  2
        ecoPlanPtr->insert(  ecoPlan_inn::density_2, 0.7f, 
                            std::vector<EcoEnt>{
                                EcoEnt{ "wheat",              3 }, 
                                EcoEnt{ "oakTree",            5 } 
                            });
        //  3
        ecoPlanPtr->insert(  ecoPlan_inn::density_3, 0.85f, 
                            std::vector<EcoEnt>{
                                EcoEnt{ "oakTree",      9 } 
                            });
    // shuffle
    ecoPlanPtr->shuffle_goSpecIdPools( 37 ); //- 提供固定seed
    //------ end -------
    ecoPlanPtr->chueck_end();
}


/* ===========================================================
 *                  init_Desert_2
 * -----------------------------------------------------------
 * 高地(>=1) 变成 绿洲 的沙漠
 */
void init_Desert_2(){
    EcoSysPlan *ecoPlanPtr = insert_new_ecoSysPlan( EcoSysPlanType::Desert );
        ecoPlanPtr->init_landColor_twoPattern(   ecoPlan_inn::density_2,
                                                ecoPlan_inn::color_Forest,
                                                ecoPlan_inn::color_Desert,
                                                true,
                                                true );

        ecoPlanPtr->init_densityDatas( 5.0f, ecoPlan_inn::densityDivideVals_default );

        ecoPlanPtr->init_goSpecIdPools_and_applyPercents();
        // -3
        ecoPlanPtr->insert(  ecoPlan_inn::density_m3, 0.5f, 
                            std::vector<EcoEnt>{
                                EcoEnt{ "wheat", 5 } 
                            });
        // -2
        ecoPlanPtr->insert(  ecoPlan_inn::density_m2, 0.1f, 
                            std::vector<EcoEnt>{
                                EcoEnt{ "wheat",              7 },
                                EcoEnt{ "singleStone_Desert", 3 } 
                            });
        // -1
        ecoPlanPtr->insert(  ecoPlan_inn::density_m1, 0.0f, 
                            std::vector<EcoEnt>{
                                EcoEnt{ "wheat",              6 }, 
                                EcoEnt{ "singleStone_Desert", 2 } 
                            });
        //  0
        ecoPlanPtr->insert(  ecoPlan_inn::density_0, 0.1f, 
                            std::vector<EcoEnt>{
                                EcoEnt{ "wheat",              6 }, 
                                EcoEnt{ "singleStone_Desert", 2 } 
                            });
        //  1
        ecoPlanPtr->insert(  ecoPlan_inn::density_1, 0.5f, 
                            std::vector<EcoEnt>{
                                EcoEnt{ "wheat",              6 }, 
                                EcoEnt{ "singleStone_Desert", 2 }
                            });

        //  2 ------ forest: 
        ecoPlanPtr->insert(  ecoPlan_inn::density_2, 1.0f, 
                            std::vector<EcoEnt>{
                                EcoEnt{ "lichen_Forest",      5 }, 
                                EcoEnt{ "singleStone_Desert", 1 },
                                EcoEnt{ "oakTree",            8 }
                            });
        //  3
        ecoPlanPtr->insert(  ecoPlan_inn::density_3, 0.9f, 
                            std::vector<EcoEnt>{
                                EcoEnt{ "oakTree",      9 } 
                            });
    // shuffle
    ecoPlanPtr->shuffle_goSpecIdPools( 83 ); //- 提供固定seed
    //------ end -------
    ecoPlanPtr->chueck_end();
}


}//------------- namespace: ecoPlan_inn end --------------//
}//---------------------- namespace: esrc -------------------------//

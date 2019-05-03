/*
 * ================== ecoSyses_mem.cpp =====================
 *                          -- tpr --
 *                                        CREATE -- 2019.02.22
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *  资源管理:  ecoSyses
 * ----------------------------
 */

//-------------------- Engine --------------------//
#include "random.h"
#include "esrc_ecoSys.h"

//-------------------- Script --------------------//
#include "Script/resource/ssrc.h" 

//#include "debug.h" //- tmp


namespace esrc{ //------------------ namespace: esrc -------------------------//


namespace{//-------- namespace: --------------//
                                            
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
        { -65.0, -40.0, -15.0, 15.0, 40.0, 65.0 }; //- 较为均匀的分布

    std::vector<float> densityDivideVals_50_20_50
        { -50.0, -30.0, -10.0, 10.0, 30.0, 50.0 }; //- 两极各占:50，中间lvl各占:20
    

    //===== funcs =====//
    void init_Forest_1();
    void init_Forest_2();
    void init_DarkForest_1();
    void init_Plain_1();
    void init_Swamp_1();
    void init_Desert_1();
    void init_Desert_2();




}//------------- namespace: end --------------//


/* ===========================================================
 *                  insert_new_ecoSys
 * -----------------------------------------------------------
 * -- 仅用于本文件内部
 */
EcoSys *insert_new_ecoSys( EcoSysType _type ){

    // ***| INSERT FIRST, INIT LATER  |***
    EcoSys  ecoSys {};
    ecoSysId_t ecoId = EcoSys::id_manager.apply_a_u32_id();
    ecoSys.set_id( ecoId );
    ecoSys.set_type( _type );
        assert( esrc::ecoSyses.find(ecoId) == esrc::ecoSyses.end() );//- must not exist
    esrc::ecoSyses.insert({ ecoId, ecoSys }); //- copy
    esrc::ecoSysIds_in_type.at(ecoSysType_2_idx(_type)).push_back(ecoId);
    esrc::ecoSysIds.push_back(ecoId);
    return static_cast<EcoSys*>( &(ecoSyses.at(ecoId)) ); 
}


/* ===========================================================
 *             apply_a_ecoSysId_by_type
 * -----------------------------------------------------------
 * -- 指定了 type，在此基础上，分配一个 变种id
 */
ecoSysId_t apply_a_ecoSysId_by_type( EcoSysType _type, float _ecoSysInMapWeight ){
    size_t randV = static_cast<size_t>(floor( _ecoSysInMapWeight * 2.7 + 907.9 ));
    auto &container = esrc::ecoSysIds_in_type.at(ecoSysType_2_idx(_type));    
    return container.at( randV % container.size() );
}


/* ===========================================================
 *              apply_a_rand_ecoSysId
 * -----------------------------------------------------------
 */
ecoSysId_t apply_a_rand_ecoSysId( float _ecoSysInMapWeight ){
    size_t randV = static_cast<size_t>(floor( _ecoSysInMapWeight * 7.1 + 977.3 ));
    size_t idx = randV % esrc::ecoSysIds.size();
    return esrc::ecoSysIds.at(idx);
}


/* ===========================================================
 *                      init_ecoSyses
 * -----------------------------------------------------------
 * -- 在游戏初始化阶段，被调用。
 * -- 统一初始化 所有 EcoSys 资源
 *    不追求性能
 */
void init_ecoSyses(){

    esrc::ecoSyses.clear();
    esrc::ecoSysIds_in_type.clear();
    esrc::ecoSysIds_in_type.resize( EcoSysType_Num );
    esrc::ecoSysIds.clear();
    //---------------------//

    //init_Forest_1();
    //init_Forest_2();
    /*
    init_DarkForest_1();
    init_Plain_1();
    init_Swamp_1();
    init_Desert_1();
    */

    init_Desert_2();
    
    
    //---------------------//
    //   shuffle ecoSysIds
    //---------------------//
    std::default_random_engine randEngine; 
    randEngine.seed(431); //- 提供固定seed
    std::shuffle(   esrc::ecoSysIds.begin(), 
                    esrc::ecoSysIds.end(),
                    randEngine );
}

namespace{//-------- namespace: --------------//

    
/* ===========================================================
 *                  init_Forest_1
 * -----------------------------------------------------------
 */
void init_Forest_1(){
    EcoSys *ecoSysPtr = insert_new_ecoSys( EcoSysType::Forest );
        ecoSysPtr->init_landColor_onlyHighLand( color_Forest );

        ecoSysPtr->init_densityDatas( 15.0, densityDivideVals_default );

        ecoSysPtr->init_goSpecIdPools_and_applyPercents();
        // -3
        ecoSysPtr->insert(  density_m3, 0.0, 
                            std::vector<EcoEnt>{
                                EcoEnt{ "lichen_Forest", 5 } 
                            });
        // -2
        ecoSysPtr->insert(  density_m2, 0.1, 
                            std::vector<EcoEnt>{
                                EcoEnt{ "lichen_Forest", 5 } 
                            });
        // -1
        ecoSysPtr->insert(  density_m1, 0.1, 
                            std::vector<EcoEnt>{
                                EcoEnt{ "lichen_Forest", 5 } 
                            });
        //  0
        ecoSysPtr->insert(  density_0, 0.2, 
                            std::vector<EcoEnt>{
                                EcoEnt{ "lichen_Forest", 5 } 
                            });
        //  1
        ecoSysPtr->insert(  density_1, 0.4, 
                            std::vector<EcoEnt>{
                                EcoEnt{ "lichen_Forest", 5 },
                                EcoEnt{ "oakTree",       3 }
                            });
        //  2
        ecoSysPtr->insert(  density_2, 0.6, 
                            std::vector<EcoEnt>{
                                EcoEnt{ "lichen_Forest", 3 },
                                EcoEnt{ "oakTree",       5 }
                            });
        //  3
        ecoSysPtr->insert(  density_3, 0.95, 
                            std::vector<EcoEnt>{
                                EcoEnt{ "oakTree",   9 },
                                EcoEnt{ "pineTree",  1 }
                            });
    // shuffle
    ecoSysPtr->shuffle_goSpecIdPools( 43 ); //- 提供固定seed
    //------ end -------
    ecoSysPtr->chueck_end();
}


/* ===========================================================
 *                  init_Forest_2   密林
 * -----------------------------------------------------------
 * 目前还存在小小的缺陷。 density freq 看起来有点大
 * 树木还不够密集
 */
void init_Forest_2(){
    EcoSys *ecoSysPtr = insert_new_ecoSys( EcoSysType::Forest );
        ecoSysPtr->init_landColor_doubleDeep( color_Forest );

        ecoSysPtr->init_densityDatas( -15.0, densityDivideVals_50_20_50 );

        ecoSysPtr->init_goSpecIdPools_and_applyPercents();
        // -3
        ecoSysPtr->insert(  density_m3, 0.98, 
                            std::vector<EcoEnt>{
                                EcoEnt{ "oakTree",   8 },
                                EcoEnt{ "pineTree",  2 }
                            });
        // -2
        ecoSysPtr->insert(  density_m2, 0.95, 
                            std::vector<EcoEnt>{
                                EcoEnt{ "lichen_Forest", 2 },
                                EcoEnt{ "oakTree",       8 }
                            });
        // -1
        ecoSysPtr->insert(  density_m1, 0.6, 
                            std::vector<EcoEnt>{
                                EcoEnt{ "lichen_Forest", 5 } 
                            });
        //  0
        ecoSysPtr->insert(  density_0, 0.0, 
                            std::vector<EcoEnt>{
                                EcoEnt{ "lichen_Forest", 5 } 
                            });
        //  1
        ecoSysPtr->insert(  density_1, 0.6, 
                            std::vector<EcoEnt>{
                                EcoEnt{ "lichen_Forest", 5 },
                                EcoEnt{ "oakTree",       3 }
                            });
        //  2
        ecoSysPtr->insert(  density_2, 0.95, 
                            std::vector<EcoEnt>{
                                EcoEnt{ "lichen_Forest", 3 },
                                EcoEnt{ "oakTree",       7 }
                            });
        //  3
        ecoSysPtr->insert(  density_3, 0.98, 
                            std::vector<EcoEnt>{
                                EcoEnt{ "oakTree",   9 },
                                EcoEnt{ "pineTree",  1 }
                            });
    // shuffle
    ecoSysPtr->shuffle_goSpecIdPools( 79 ); //- 提供固定seed
    //------ end -------
    ecoSysPtr->chueck_end();
}


/* ===========================================================
 *                  init_DarkForest_1
 * -----------------------------------------------------------
 */
void init_DarkForest_1(){
    EcoSys *ecoSysPtr = insert_new_ecoSys( EcoSysType::DarkForest );
        ecoSysPtr->init_landColor_onlyHighLand( color_DarkForest );

        ecoSysPtr->init_densityDatas( -5.0, densityDivideVals_default );

        ecoSysPtr->init_goSpecIdPools_and_applyPercents();
        // -3
        ecoSysPtr->insert(  density_m3, 0.0, 
                            std::vector<EcoEnt>{
                                EcoEnt{ "lichen_Forest", 5 } 
                            });
        // -2
        ecoSysPtr->insert(  density_m2, 0.1, 
                            std::vector<EcoEnt>{
                                EcoEnt{ "lichen_Forest", 5 } 
                            });
        // -1
        ecoSysPtr->insert(  density_m1, 0.2, 
                            std::vector<EcoEnt>{
                                EcoEnt{ "lichen_Forest", 5 } 
                            });
        //  0
        ecoSysPtr->insert(  density_0, 0.3, 
                            std::vector<EcoEnt>{
                                EcoEnt{ "lichen_Forest", 5 } 
                            });
        //  1
        ecoSysPtr->insert(  density_1, 0.5, 
                            std::vector<EcoEnt>{
                                EcoEnt{ "lichen_Forest", 5 },
                                EcoEnt{ "pineTree",      3 }
                            });
        //  2
        ecoSysPtr->insert(  density_2, 0.7, 
                            std::vector<EcoEnt>{
                                EcoEnt{ "lichen_Forest", 3 },
                                EcoEnt{ "pineTree",      5 }
                            });
        //  3
        ecoSysPtr->insert(  density_3, 0.95, 
                            std::vector<EcoEnt>{
                                EcoEnt{ "pineTree", 9 }
                            });
    // shuffle
    ecoSysPtr->shuffle_goSpecIdPools( 241 ); //- 提供固定seed
    //------ end -------
    ecoSysPtr->chueck_end();
}


/* ===========================================================
 *                  init_Plain_1
 * -----------------------------------------------------------
 */
void init_Plain_1(){
    EcoSys *ecoSysPtr = insert_new_ecoSys( EcoSysType::Plain );
        ecoSysPtr->init_landColor_onlyHighLand( color_Plain );

        ecoSysPtr->init_densityDatas( -5.0, densityDivideVals_default );

        ecoSysPtr->init_goSpecIdPools_and_applyPercents();
        // -3
        ecoSysPtr->insert(  density_m3, 0.5, 
                            std::vector<EcoEnt>{
                                EcoEnt{ "lichen_Forest", 5 } 
                            });
        // -2
        ecoSysPtr->insert(  density_m2, 0.3, 
                            std::vector<EcoEnt>{
                                EcoEnt{ "lichen_Forest", 5 } 
                            });
        // -1
        ecoSysPtr->insert(  density_m1, 0.1, 
                            std::vector<EcoEnt>{
                                EcoEnt{ "lichen_Forest", 5 } 
                            });
        //  0
        ecoSysPtr->insert(  density_0, 0.2, 
                            std::vector<EcoEnt>{
                                EcoEnt{ "lichen_Forest", 5 } 
                            });
        //  1
        ecoSysPtr->insert(  density_1, 0.3, 
                            std::vector<EcoEnt>{
                                EcoEnt{ "lichen_Forest", 5 },
                                EcoEnt{ "oakTree",       3 } 
                            });
        //  2
        ecoSysPtr->insert(  density_2, 0.4, 
                            std::vector<EcoEnt>{
                                EcoEnt{ "lichen_Forest", 3 },
                                EcoEnt{ "oakTree",       5 } 
                            });
        //  3
        ecoSysPtr->insert(  density_3, 0.6, 
                            std::vector<EcoEnt>{
                                EcoEnt{ "oakTree",   9 },
                                EcoEnt{ "pineTree",  1 } 
                            });
    // shuffle
    ecoSysPtr->shuffle_goSpecIdPools( 349 ); //- 提供固定seed
    //------ end -------
    ecoSysPtr->chueck_end();
}

/* ===========================================================
 *                  init_Swamp_1
 * -----------------------------------------------------------
 */
void init_Swamp_1(){
    EcoSys *ecoSysPtr = insert_new_ecoSys( EcoSysType::Swamp );
        ecoSysPtr->init_landColor_onlyHighLand( color_Swamp );

        ecoSysPtr->init_densityDatas( -5.0, densityDivideVals_default );

        ecoSysPtr->init_goSpecIdPools_and_applyPercents();
        // -3
        ecoSysPtr->insert(  density_m3, 0.5, 
                            std::vector<EcoEnt>{
                                EcoEnt{ "lichen_Forest", 5 } 
                            });
        // -2
        ecoSysPtr->insert(  density_m2, 0.3, 
                            std::vector<EcoEnt>{
                                EcoEnt{ "lichen_Forest", 5 } 
                            });
        // -1
        ecoSysPtr->insert(  density_m1, 0.1, 
                            std::vector<EcoEnt>{
                                EcoEnt{ "lichen_Forest", 5 } 
                            });
        //  0
        ecoSysPtr->insert(  density_0, 0.2, 
                            std::vector<EcoEnt>{
                                EcoEnt{ "lichen_Forest", 5 } 
                            });
        //  1
        ecoSysPtr->insert(  density_1, 0.4, 
                            std::vector<EcoEnt>{
                                EcoEnt{ "lichen_Forest", 5 }, 
                                EcoEnt{ "oakTree",       3 }
                            });
        //  2
        ecoSysPtr->insert(  density_2, 0.6, 
                            std::vector<EcoEnt>{
                                EcoEnt{ "lichen_Forest", 3 }, 
                                EcoEnt{ "oakTree",       5 }
                            });
        //  3
        ecoSysPtr->insert(  density_3, 0.85, 
                            std::vector<EcoEnt>{
                                EcoEnt{ "oakTree",   9 }, 
                                EcoEnt{ "pineTree",  1 }
                            });
    // shuffle
    ecoSysPtr->shuffle_goSpecIdPools( 31 ); //- 提供固定seed
    //------ end -------
    ecoSysPtr->chueck_end();
}


/* ===========================================================
 *                  init_Desert_1
 * -----------------------------------------------------------
 */
void init_Desert_1(){
    EcoSys *ecoSysPtr = insert_new_ecoSys( EcoSysType::Desert );
        ecoSysPtr->init_landColor_onlyHighLand( color_Desert );

        ecoSysPtr->init_densityDatas( -5.0, densityDivideVals_default );

        ecoSysPtr->init_goSpecIdPools_and_applyPercents();
        // -3
        ecoSysPtr->insert(  density_m3, 0.0, 
                            std::vector<EcoEnt>{
                                EcoEnt{ "wheat", 5 } 
                            });
        // -2
        ecoSysPtr->insert(  density_m2, 0.1, 
                            std::vector<EcoEnt>{
                                EcoEnt{ "wheat",              5 }, 
                                EcoEnt{ "singleStone_Desert", 3 } 
                            });
        // -1
        ecoSysPtr->insert(  density_m1, 0.2, 
                            std::vector<EcoEnt>{
                                EcoEnt{ "wheat",              6 }, 
                                EcoEnt{ "singleStone_Desert", 2 } 
                            });
        //  0
        ecoSysPtr->insert(  density_0, 0.3, 
                            std::vector<EcoEnt>{
                                EcoEnt{ "wheat",              6 }, 
                                EcoEnt{ "singleStone_Desert", 2 } 
                            });
        //  1
        ecoSysPtr->insert(  density_1, 0.5, 
                            std::vector<EcoEnt>{
                                EcoEnt{ "wheat",              5 }, 
                                EcoEnt{ "singleStone_Desert", 3 },
                                EcoEnt{ "oakTree",            3 } 
                            });
        //  2
        ecoSysPtr->insert(  density_2, 0.7, 
                            std::vector<EcoEnt>{
                                EcoEnt{ "wheat",              3 }, 
                                EcoEnt{ "oakTree",            5 } 
                            });
        //  3
        ecoSysPtr->insert(  density_3, 0.85, 
                            std::vector<EcoEnt>{
                                EcoEnt{ "oakTree",      9 } 
                            });
    // shuffle
    ecoSysPtr->shuffle_goSpecIdPools( 37 ); //- 提供固定seed
    //------ end -------
    ecoSysPtr->chueck_end();
}


/* ===========================================================
 *                  init_Desert_2
 * -----------------------------------------------------------
 * 高地(>=1) 变成 绿洲 的沙漠
 */
void init_Desert_2(){
    EcoSys *ecoSysPtr = insert_new_ecoSys( EcoSysType::Desert );
        ecoSysPtr->init_landColor_twoPattern(   density_2,
                                                color_Forest,
                                                color_Desert,
                                                true,
                                                true );

        ecoSysPtr->init_densityDatas( 5.0, densityDivideVals_default );

        ecoSysPtr->init_goSpecIdPools_and_applyPercents();
        // -3
        ecoSysPtr->insert(  density_m3, 0.5, 
                            std::vector<EcoEnt>{
                                EcoEnt{ "wheat", 5 } 
                            });
        // -2
        ecoSysPtr->insert(  density_m2, 0.1, 
                            std::vector<EcoEnt>{
                                EcoEnt{ "wheat",              7 },
                                EcoEnt{ "singleStone_Desert", 3 } 
                            });
        // -1
        ecoSysPtr->insert(  density_m1, 0.0, 
                            std::vector<EcoEnt>{
                                EcoEnt{ "wheat",              6 }, 
                                EcoEnt{ "singleStone_Desert", 2 } 
                            });
        //  0
        ecoSysPtr->insert(  density_0, 0.1, 
                            std::vector<EcoEnt>{
                                EcoEnt{ "wheat",              6 }, 
                                EcoEnt{ "singleStone_Desert", 2 } 
                            });
        //  1
        ecoSysPtr->insert(  density_1, 0.5, 
                            std::vector<EcoEnt>{
                                EcoEnt{ "wheat",              6 }, 
                                EcoEnt{ "singleStone_Desert", 2 }
                            });

        //  2 ------ forest: 
        ecoSysPtr->insert(  density_2, 1.0, 
                            std::vector<EcoEnt>{
                                EcoEnt{ "lichen_Forest",      5 }, 
                                EcoEnt{ "singleStone_Desert", 1 },
                                EcoEnt{ "oakTree",            8 }
                            });
        //  3
        ecoSysPtr->insert(  density_3, 0.9, 
                            std::vector<EcoEnt>{
                                EcoEnt{ "oakTree",      9 } 
                            });
    // shuffle
    ecoSysPtr->shuffle_goSpecIdPools( 83 ); //- 提供固定seed
    //------ end -------
    ecoSysPtr->chueck_end();
}


}//------------- namespace: end --------------//
}//---------------------- namespace: esrc -------------------------//

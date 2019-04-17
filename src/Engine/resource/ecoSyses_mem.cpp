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
#include "srcs_engine.h" //- 所有资源

//-------------------- Script --------------------//
#include "Script/resource/srcs_script.h" 

//#include "debug.h" //- tmp


namespace esrc{ //------------------ namespace: esrc -------------------------//



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

    Density  density_m3 {-3};
    Density  density_m2 {-2};
    Density  density_m1 {-1};
    Density  density_0  {0};
    Density  density_1  {1};
    Density  density_2  {2};
    Density  density_3  {3};


    esrc::ecoSyses.clear();
    esrc::ecoSysIds_in_type.clear();
    esrc::ecoSysIds_in_type.resize( EcoSysType_Num );
    esrc::ecoSysIds.clear();

    //----
    EcoSys *ecoSysPtr;

    //---------------------//
    //      Forest.1
    //---------------------//
    ecoSysPtr = insert_new_ecoSys( EcoSysType::Forest );
        ecoSysPtr->init_landColor( RGBA{ 130, 150, 109, 255 } );

        ecoSysPtr->init_goSpecIdPools();
        ecoSysPtr->init_applyPercents();
        // -3
        ecoSysPtr->insert_applyPercent( density_m3, 0.0 );
        ecoSysPtr->insert_goSpecIdPool( density_m3, ssrc::get_goSpecId("lichen_Forest"), 5);
        // -2
        ecoSysPtr->insert_applyPercent( density_m2, 0.1 );
        ecoSysPtr->insert_goSpecIdPool( density_m2, ssrc::get_goSpecId("lichen_Forest"), 5);
        // -1
        ecoSysPtr->insert_applyPercent( density_m1, 0.1 );
        ecoSysPtr->insert_goSpecIdPool( density_m1, ssrc::get_goSpecId("lichen_Forest"), 5);
        //  0
        ecoSysPtr->insert_applyPercent( density_0, 0.2 );
        ecoSysPtr->insert_goSpecIdPool( density_0, ssrc::get_goSpecId("lichen_Forest"), 5);
        //  1
        ecoSysPtr->insert_applyPercent( density_1, 0.4 );
        ecoSysPtr->insert_goSpecIdPool( density_1, ssrc::get_goSpecId("lichen_Forest"), 5);
        ecoSysPtr->insert_goSpecIdPool( density_1, ssrc::get_goSpecId("oakTree"), 3);
        //  2
        ecoSysPtr->insert_applyPercent( density_2, 0.6 );
        ecoSysPtr->insert_goSpecIdPool( density_2, ssrc::get_goSpecId("lichen_Forest"), 3);
        ecoSysPtr->insert_goSpecIdPool( density_2, ssrc::get_goSpecId("oakTree"), 5);
        //  3
        ecoSysPtr->insert_applyPercent( density_3, 0.95 );
        ecoSysPtr->insert_goSpecIdPool( density_3, ssrc::get_goSpecId("oakTree"), 9);
        ecoSysPtr->insert_goSpecIdPool( density_3, ssrc::get_goSpecId("pineTree"), 1);
        // shuffle
        ecoSysPtr->shuffle_goSpecIdPools();

    //---------------------//
    //      Forest.2
    //---------------------//
    /*
    ecoSysPtr = insert_new_ecoSys( EcoSysType::Forest );
        ecoSysPtr->init_landColor( RGBA{ 130, 150, 109, 255 } );

        ecoSysPtr->init_goSpecIdPools();
        ecoSysPtr->init_applyPercents();
        // -3
        ecoSysPtr->insert_applyPercent( density_m3, 0.9 );
        ecoSysPtr->insert_goSpecIdPool( density_m3, ssrc::get_goSpecId("oakTree"), 9);
        // -2
        ecoSysPtr->insert_applyPercent( density_m2, 0.7 );
        ecoSysPtr->insert_goSpecIdPool( density_m2, ssrc::get_goSpecId("oakTree"), 5);
        ecoSysPtr->insert_goSpecIdPool( density_m2, ssrc::get_goSpecId("lichen_Forest"), 3);
        // -1
        ecoSysPtr->insert_applyPercent( density_m1, 0.1 );
        ecoSysPtr->insert_goSpecIdPool( density_m1, ssrc::get_goSpecId("lichen_Forest"), 5);
        //  0
        ecoSysPtr->insert_applyPercent( density_0, 0.0 );
        ecoSysPtr->insert_goSpecIdPool( density_0, ssrc::get_goSpecId("lichen_Forest"), 5);
        //  1
        ecoSysPtr->insert_applyPercent( density_1, 0.2 );
        ecoSysPtr->insert_goSpecIdPool( density_1, ssrc::get_goSpecId("lichen_Forest"), 5);
        ecoSysPtr->insert_goSpecIdPool( density_1, ssrc::get_goSpecId("oakTree"), 3);
        //  2
        ecoSysPtr->insert_applyPercent( density_2, 0.6 );
        ecoSysPtr->insert_goSpecIdPool( density_2, ssrc::get_goSpecId("lichen_Forest"), 3);
        ecoSysPtr->insert_goSpecIdPool( density_2, ssrc::get_goSpecId("oakTree"), 5);
        //  3
        ecoSysPtr->insert_applyPercent( density_3, 0.95 );
        ecoSysPtr->insert_goSpecIdPool( density_3, ssrc::get_goSpecId("oakTree"), 9);
        ecoSysPtr->insert_goSpecIdPool( density_3, ssrc::get_goSpecId("pineTree"), 1);
        // shuffle
        ecoSysPtr->shuffle_goSpecIdPools();
    */

                //-- 目前这一块存在缺陷：应当让 -3，-2，-1 地面，也涂上深色 


    //---------------------//
    //      DarkForest
    //---------------------//
    ecoSysPtr = insert_new_ecoSys( EcoSysType::DarkForest );
        ecoSysPtr->init_landColor( RGBA{ 106, 130, 113, 255 } );

        ecoSysPtr->init_goSpecIdPools();
        ecoSysPtr->init_applyPercents();
        // -3
        ecoSysPtr->insert_applyPercent( density_m3, 0.0 );
        ecoSysPtr->insert_goSpecIdPool( density_m3, ssrc::get_goSpecId("lichen_Forest"), 5);
        // -2
        ecoSysPtr->insert_applyPercent( density_m2, 0.1 );
        ecoSysPtr->insert_goSpecIdPool( density_m2, ssrc::get_goSpecId("lichen_Forest"), 5);
        // -1
        ecoSysPtr->insert_applyPercent( density_m1, 0.2 );
        ecoSysPtr->insert_goSpecIdPool( density_m1, ssrc::get_goSpecId("lichen_Forest"), 5);
        //  0
        ecoSysPtr->insert_applyPercent( density_0, 0.3 );
        ecoSysPtr->insert_goSpecIdPool( density_0, ssrc::get_goSpecId("lichen_Forest"), 5);
        //  1
        ecoSysPtr->insert_applyPercent( density_1, 0.5 );
        ecoSysPtr->insert_goSpecIdPool( density_1, ssrc::get_goSpecId("lichen_Forest"), 5);
        ecoSysPtr->insert_goSpecIdPool( density_1, ssrc::get_goSpecId("pineTree"), 3);
        //  2
        ecoSysPtr->insert_applyPercent( density_2, 0.7 );
        ecoSysPtr->insert_goSpecIdPool( density_2, ssrc::get_goSpecId("lichen_Forest"), 3);
        ecoSysPtr->insert_goSpecIdPool( density_2, ssrc::get_goSpecId("pineTree"), 5);
        //  3
        ecoSysPtr->insert_applyPercent( density_3, 0.95 );
        ecoSysPtr->insert_goSpecIdPool( density_3, ssrc::get_goSpecId("pineTree"), 9);
        ecoSysPtr->insert_goSpecIdPool( density_3, ssrc::get_goSpecId("oakTree"),  1);
        // shuffle
        ecoSysPtr->shuffle_goSpecIdPools();


    //---------------------//
    //       Plain
    //---------------------//
    ecoSysPtr = insert_new_ecoSys( EcoSysType::Plain );
        ecoSysPtr->init_landColor( RGBA{ 155, 159, 134, 255 } );

        ecoSysPtr->init_goSpecIdPools();
        ecoSysPtr->init_applyPercents();
        // -3
        ecoSysPtr->insert_applyPercent( density_m3, 0.5 );
        ecoSysPtr->insert_goSpecIdPool( density_m3, ssrc::get_goSpecId("lichen_Forest"), 5);
        // -2
        ecoSysPtr->insert_applyPercent( density_m2, 0.3 );
        ecoSysPtr->insert_goSpecIdPool( density_m2, ssrc::get_goSpecId("lichen_Forest"), 5);
        // -1
        ecoSysPtr->insert_applyPercent( density_m1, 0.1 );
        ecoSysPtr->insert_goSpecIdPool( density_m1, ssrc::get_goSpecId("lichen_Forest"), 5);
        //  0
        ecoSysPtr->insert_applyPercent( density_0, 0.2 );
        ecoSysPtr->insert_goSpecIdPool( density_0, ssrc::get_goSpecId("lichen_Forest"), 5);
        //  1
        ecoSysPtr->insert_applyPercent( density_1, 0.3 );
        ecoSysPtr->insert_goSpecIdPool( density_1, ssrc::get_goSpecId("lichen_Forest"), 5);
        ecoSysPtr->insert_goSpecIdPool( density_1, ssrc::get_goSpecId("oakTree"), 3);
        //  2
        ecoSysPtr->insert_applyPercent( density_2, 0.4 );
        ecoSysPtr->insert_goSpecIdPool( density_2, ssrc::get_goSpecId("lichen_Forest"), 3);
        ecoSysPtr->insert_goSpecIdPool( density_2, ssrc::get_goSpecId("oakTree"), 5);
        //  3
        ecoSysPtr->insert_applyPercent( density_3, 0.6 );
        ecoSysPtr->insert_goSpecIdPool( density_3, ssrc::get_goSpecId("oakTree"), 9);
        ecoSysPtr->insert_goSpecIdPool( density_3, ssrc::get_goSpecId("pineTree"), 1);
        // shuffle
        ecoSysPtr->shuffle_goSpecIdPools();



    //---------------------//
    //       Swamp
    //---------------------//
    ecoSysPtr = insert_new_ecoSys( EcoSysType::Swamp );
        ecoSysPtr->init_landColor( RGBA{ 138, 139, 124, 255 } );

        ecoSysPtr->init_goSpecIdPools();
        ecoSysPtr->init_applyPercents();
        // -3
        ecoSysPtr->insert_applyPercent( density_m3, 0.5 );
        ecoSysPtr->insert_goSpecIdPool( density_m3, ssrc::get_goSpecId("lichen_Forest"), 5);
        // -2
        ecoSysPtr->insert_applyPercent( density_m2, 0.3 );
        ecoSysPtr->insert_goSpecIdPool( density_m2, ssrc::get_goSpecId("lichen_Forest"), 5);
        // -1
        ecoSysPtr->insert_applyPercent( density_m1, 0.1 );
        ecoSysPtr->insert_goSpecIdPool( density_m1, ssrc::get_goSpecId("lichen_Forest"), 5);
        //  0
        ecoSysPtr->insert_applyPercent( density_0, 0.2 );
        ecoSysPtr->insert_goSpecIdPool( density_0, ssrc::get_goSpecId("lichen_Forest"), 5);
        //  1
        ecoSysPtr->insert_applyPercent( density_1, 0.4 );
        ecoSysPtr->insert_goSpecIdPool( density_1, ssrc::get_goSpecId("lichen_Forest"), 5);
        ecoSysPtr->insert_goSpecIdPool( density_1, ssrc::get_goSpecId("oakTree"), 3);
        //  2
        ecoSysPtr->insert_applyPercent( density_2, 0.6 );
        ecoSysPtr->insert_goSpecIdPool( density_2, ssrc::get_goSpecId("lichen_Forest"), 3);
        ecoSysPtr->insert_goSpecIdPool( density_2, ssrc::get_goSpecId("oakTree"), 5);
        //  3
        ecoSysPtr->insert_applyPercent( density_3, 0.85 );
        ecoSysPtr->insert_goSpecIdPool( density_3, ssrc::get_goSpecId("oakTree"), 9);
        ecoSysPtr->insert_goSpecIdPool( density_3, ssrc::get_goSpecId("pineTree"), 1);
        // shuffle
        ecoSysPtr->shuffle_goSpecIdPools();



    //---------------------//
    //       Desert
    //---------------------//
    ecoSysPtr = insert_new_ecoSys( EcoSysType::Desert );
        ecoSysPtr->init_landColor( RGBA{ 210, 195, 142, 255 } );

        ecoSysPtr->init_goSpecIdPools();
        ecoSysPtr->init_applyPercents();
        // -3
        ecoSysPtr->insert_applyPercent( density_m3, 0.0 );
        ecoSysPtr->insert_goSpecIdPool( density_m3, ssrc::get_goSpecId("wheat"), 5);
        // -2
        ecoSysPtr->insert_applyPercent( density_m2, 0.1 );
        ecoSysPtr->insert_goSpecIdPool( density_m2, ssrc::get_goSpecId("wheat"), 5);
        ecoSysPtr->insert_goSpecIdPool( density_m2, ssrc::get_goSpecId("singleStone_Desert"), 3);
        // -1
        ecoSysPtr->insert_applyPercent( density_m1, 0.2 );
        ecoSysPtr->insert_goSpecIdPool( density_m1, ssrc::get_goSpecId("wheat"), 5);
        ecoSysPtr->insert_goSpecIdPool( density_m1, ssrc::get_goSpecId("singleStone_Desert"), 3);
        //  0
        ecoSysPtr->insert_applyPercent( density_0, 0.3 );
        ecoSysPtr->insert_goSpecIdPool( density_0, ssrc::get_goSpecId("wheat"), 5);
        ecoSysPtr->insert_goSpecIdPool( density_0, ssrc::get_goSpecId("singleStone_Desert"), 3);
        //  1
        ecoSysPtr->insert_applyPercent( density_1, 0.5 );
        ecoSysPtr->insert_goSpecIdPool( density_1, ssrc::get_goSpecId("wheat"), 5);
        ecoSysPtr->insert_goSpecIdPool( density_1, ssrc::get_goSpecId("singleStone_Desert"), 3);
        ecoSysPtr->insert_goSpecIdPool( density_1, ssrc::get_goSpecId("oakTree"), 3);
        //  2
        ecoSysPtr->insert_applyPercent( density_2, 0.7 );
        ecoSysPtr->insert_goSpecIdPool( density_2, ssrc::get_goSpecId("wheat"), 3);
        ecoSysPtr->insert_goSpecIdPool( density_2, ssrc::get_goSpecId("oakTree"), 5);
        //  3
        ecoSysPtr->insert_applyPercent( density_3, 0.85 );
        ecoSysPtr->insert_goSpecIdPool( density_3, ssrc::get_goSpecId("oakTree"), 9);
        ecoSysPtr->insert_goSpecIdPool( density_3, ssrc::get_goSpecId("pineTree"), 1);
        // shuffle
        ecoSysPtr->shuffle_goSpecIdPools();


    //---------------------//
    //   shuffle ecoSysIds
    //---------------------//
    std::shuffle(   esrc::ecoSysIds.begin(), 
                    esrc::ecoSysIds.end(),
                    esrc::gameSeed.randEngine );
        //-- 目前这么实现还不够 “伪随机”...


}

}//---------------------- namespace: esrc -------------------------//

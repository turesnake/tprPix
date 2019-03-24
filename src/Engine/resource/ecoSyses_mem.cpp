/*
 * ================== ecoSyses_mem.cpp =====================
 *                          -- tpr --
 *                                        CREATE -- 2019.02.22
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *  资源管理:  ecoSyses
 * -----
 *   这部分代码，应该放到 script层 中去
 * ----------------------------
 */
//-------------------- CPP --------------------//

//-------------------- Engine --------------------//
#include "srcs_engine.h" //- 所有资源

//#include "debug.h" //- tmp


namespace esrc{ //------------------ namespace: esrc -------------------------//



/* ===========================================================
 *                  insert_new_ecoSys
 * -----------------------------------------------------------
 */
EcoSys *insert_new_ecoSys( EcoSysType _type ){

    // ***| INSERT FIRST, INIT LATER  |***
    EcoSys  ecoSys {};
    esrc::ecoSyses.insert({ _type, ecoSys }); //- copy
    return (EcoSys*)&(ecoSyses.at(_type)); 
}


/* ===========================================================
 *                      init_ecoSyses
 * -----------------------------------------------------------
 * -- 在游戏初始化阶段，被调用。
 * -- 统一初始化 所有 EcoSys 资源
 */
void init_ecoSyses(){

    ecoSyses.clear();
    //----
    EcoSys *ecoSysPtr;

    //---------------------//
    //      Forest
    //---------------------//
    ecoSysPtr = insert_new_ecoSys( EcoSysType::Forst );
        ecoSysPtr->color_low  = RGBA{ 130, 150,  109, 255 };
        ecoSysPtr->color_underWater  = RGBA{ 180, 160, 120, 255 };

        ecoSysPtr->altiPerlin_freqBig = 0.5;
        ecoSysPtr->altiPerlin_freqBig = 4.0;


    //---------------------//
    //      DarkForst
    //---------------------//
    ecoSysPtr = insert_new_ecoSys( EcoSysType::DarkForst );
        ecoSysPtr->color_low  = RGBA{ 106, 130,  113, 255 };
        ecoSysPtr->color_underWater  = RGBA{ 180, 160, 120, 255 };

        ecoSysPtr->altiPerlin_freqBig = 0.5;
        ecoSysPtr->altiPerlin_freqBig = 4.0;


    //---------------------//
    //       Plain
    //---------------------//
    ecoSysPtr = insert_new_ecoSys( EcoSysType::Plain );
        ecoSysPtr->color_low  = RGBA{ 155, 159, 134, 255 };
        ecoSysPtr->color_underWater  = RGBA{ 180, 160, 120, 255 };

        ecoSysPtr->altiPerlin_freqBig = 0.5;
        ecoSysPtr->altiPerlin_freqBig = 4.0;


    //---------------------//
    //       Swamp
    //---------------------//
    ecoSysPtr = insert_new_ecoSys( EcoSysType::Swamp );
        ecoSysPtr->color_low  = RGBA{ 138, 139,  124, 255 };
        ecoSysPtr->color_underWater  = RGBA{ 180, 160, 120, 255 };

        ecoSysPtr->altiPerlin_freqBig = 0.5;
        ecoSysPtr->altiPerlin_freqBig = 4.0;


    //---------------------//
    //       Desert
    //---------------------//
    ecoSysPtr = insert_new_ecoSys( EcoSysType::Desert );
        ecoSysPtr->color_low  = RGBA{ 176, 167, 140, 255 };
        ecoSysPtr->color_underWater  = RGBA{ 180, 160, 120, 255 };

        ecoSysPtr->altiPerlin_freqBig = 0.5;
        ecoSysPtr->altiPerlin_freqBig = 4.0;


}

}//---------------------- namespace: esrc -------------------------//

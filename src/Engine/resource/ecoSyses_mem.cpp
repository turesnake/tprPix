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
#include <string>

//-------------------- Engine --------------------//
#include "srcs_engine.h" //- 所有资源

//#include "debug.h" //- tmp


namespace esrc{ //------------------ namespace: esrc -------------------------//



/* ===========================================================
 *                  insert_new_ecoSys
 * -----------------------------------------------------------
 */
EcoSys *insert_new_ecoSys(const std::string &_name){

    // ***| INSERT FIRST, INIT LATER  |***
    EcoSys  ecoSys {};
    esrc::ecoSyses.insert({ _name, ecoSys }); //- copy
    return (EcoSys*)&(ecoSyses.at(_name)); 
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

    //-- Forest --//
    ecoSysPtr = insert_new_ecoSys("Forest");
        ecoSysPtr->color_low  = RGBA{ 166, 149,  90, 255 };
        ecoSysPtr->color_mid  = RGBA{ 153, 126,  73, 255 };
        ecoSysPtr->color_high = RGBA{ 107, 192,  54, 255 };



    //-- Plain --//
    ecoSysPtr = insert_new_ecoSys("Plain");
        ecoSysPtr->color_low  = RGBA{ 166, 249,  90, 255 };
        ecoSysPtr->color_mid  = RGBA{ 153, 226,  73, 255 };
        ecoSysPtr->color_high = RGBA{ 107, 250,  54, 255 };


    //-- Swamp --//
    ecoSysPtr = insert_new_ecoSys("Swamp");
        ecoSysPtr->color_low  = RGBA{ 136, 149,  90, 255 };
        ecoSysPtr->color_mid  = RGBA{ 123, 126,  73, 255 };
        ecoSysPtr->color_high = RGBA{  77, 192,  54, 255 };



}

}//---------------------- namespace: esrc -------------------------//

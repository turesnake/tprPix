/*
 * ========================= goSpecIds.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2018.12.23
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *   将 具象go species 注册到 
 *    ssrc::go_specId_names 
 *    ssrc::go_name_specIds 中去
 * ----------------------------
 */
//-------------------- CPP --------------------//
#include <string>

//-------------------- Engine --------------------//
#include "GameObjType.h" //- goSpecId_t

//-------------------- Script --------------------//
#include "Script/resource/ssrc.h" 
#include "Script/gameObjs/allGoes.h"
#include "Script/uiGos/allUIGoes.h"

#include "Script/json/GoJsonData.h"
#include "Script/json/UIGoJsonData.h"



using namespace std::placeholders;

//#include "tprDebug.h" //- tmp

//namespace goSpecIds_inn {//-------------- namespace: goSpecIds_inn ---------------------
//}//------------------------- namespace: goSpecIds_inn end -------------------


/* ===========================================================
 *                   onGoSpecIds_SignUp   
 * -----------------------------------------------------------
 * 
 */
void onGoSpecIds_SignUp(){

    goSpecId_t   id {};

    ssrc::clear_goInit_funcs();
    ssrc::clear_go_specId_names();
    ssrc::clear_go_name_specIds();

    //==================================//
    parse_from_goJsonFile();
    parse_from_uiGoJsonFile();

    //================ Goes ==================//
    ssrc::insert_2_goInit_funcs( "norman",         std::bind( &gameObjs::Norman::init_in_autoMod, _1, _2 ) );
    ssrc::insert_2_goInit_funcs( "bigMan",         std::bind( &gameObjs::BigMan::init_in_autoMod, _1, _2 ) );
    ssrc::insert_2_goInit_funcs( "oneEyeBoy",      std::bind( &gameObjs::OneEyeBoy::init_in_autoMod, _1, _2 ) );

    //-------------------//
    //        tree
    //-------------------//
    ssrc::insert_2_goInit_funcs( "oakTree",  std::bind( &gameObjs::OakTree::init_in_autoMod, _1, _2 ) );
    ssrc::insert_2_goInit_funcs( "pineTree", std::bind( &gameObjs::PineTree::init_in_autoMod, _1, _2 ) );

    //-------------------//
    //        bush
    //-------------------//
    ssrc::insert_2_goInit_funcs( "wheat",              std::bind( &gameObjs::Wheat::init_in_autoMod, _1, _2 ) );

    //-------------------//
    //    mapSurface
    //-------------------//
    ssrc::insert_2_goInit_funcs( "mapSurfaceLower",      std::bind( &gameObjs::MapSurfaceLower::init_in_autoMod, _1, _2 ) );


    //-------------------//
    //        oth
    //-------------------//
    ssrc::insert_2_goInit_funcs( "playerGoCircle", std::bind( &gameObjs::PlayerGoCircle::init_in_autoMod, _1, _2 ) );

     //...

    //================ UIGoes ==================//

    ssrc::insert_2_uiGoInit_funcs( "button_sceneBegin_archive_2", std::bind( &uiGos::Button_SceneBegin_Archive::init_in_autoMod, _1, _2 ) );
    ssrc::insert_2_uiGoInit_funcs( "button_sceneBegin_pointer_2", std::bind( &uiGos::Button_SceneBegin_Pointer::init_in_autoMod, _1, _2 ) );



    //cout << "--- onGoSpecIds_SignUp(). DONE. ---" << endl; 
}



//namespace goSpecIds_inn {//------------------- namespace: goSpecIds_inn ---------------------
//}//------------------------- namespace: goSpecIds_inn end -------------------


//-- 仅用于流程测试 
void onStart_test(){
    //cout << "--- onStart_test(). DONE. ---" << endl;
}




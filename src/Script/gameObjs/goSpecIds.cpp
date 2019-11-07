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

#include "esrc_state.h"

//-------------------- Script --------------------//
#include "Script/resource/ssrc_all.h" 
#include "Script/gameObjs/allGoes.h"
#include "Script/uiGos/allUIGoes.h"

#include "Script/json/json_all.h"



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

    //==================================//
    tprAssert( esrc::is_setState("json_gameObj") );
    tprAssert( esrc::is_setState("json_uiGo") );


    //================ Goes ==================//
    //ssrc::insert_2_goInit_funcs( "norman",         std::bind( &gameObjs::Norman::init, _1, _2 ) );
    //ssrc::insert_2_goInit_funcs( "bigMan",         std::bind( &gameObjs::BigMan::init, _1, _2 ) );
    ssrc::insert_2_goInit_funcs( "oneEyeBoy",      std::bind( &gameObjs::OneEyeBoy::init, _1, _2 ) );

    ssrc::insert_2_goInit_funcs( "wallA",          std::bind( &gameObjs::WallA::init, _1, _2 ) );


    //-------------------//
    //        tree
    //-------------------//
    //ssrc::insert_2_goInit_funcs( "oakTree",  std::bind( &gameObjs::OakTree::init, _1, _2 ) );
    ssrc::insert_2_goInit_funcs( "pineTree",        std::bind( &gameObjs::PineTree::init, _1, _2 ) );
    ssrc::insert_2_goInit_funcs( "poplarTree",      std::bind( &gameObjs::PoplarTree::init, _1, _2 ) );

    //-------------------//
    //        bush
    //-------------------//
    //ssrc::insert_2_goInit_funcs( "wheat",              std::bind( &gameObjs::Wheat::init, _1, _2 ) );
    ssrc::insert_2_goInit_funcs( "thorn",           std::bind( &gameObjs::Thorn::init, _1, _2 ) );
    ssrc::insert_2_goInit_funcs( "mushroom",        std::bind( &gameObjs::Mushroom::init, _1, _2 ) );

    
    ssrc::insert_2_goInit_funcs( "bushOth",           std::bind( &gameObjs::BushOth::init, _1, _2 ) );
    
  


    ssrc::insert_2_goInit_funcs( "grass",     std::bind( &gameObjs::Grass::init, _1, _2 ) );

    //-------------------//
    //        rock
    //-------------------//
    ssrc::insert_2_goInit_funcs( "rock",       std::bind( &gameObjs::Rock::init, _1, _2 ) );

    //-------------------//
    //    mapSurface
    //-------------------//
    ssrc::insert_2_goInit_funcs( "mapSurfaceLower",      std::bind( &gameObjs::MapSurfaceLower::init, _1, _2 ) );


    //-------------------//
    //        oth
    //-------------------//
    ssrc::insert_2_goInit_funcs( "playerGoCircle",  std::bind( &gameObjs::PlayerGoCircle::init, _1, _2 ) );
    ssrc::insert_2_goInit_funcs( "groundGo",        std::bind( &gameObjs::GroundGo::init, _1, _2 ) );
    ssrc::insert_2_goInit_funcs( "fieldRim",        std::bind( &gameObjs::FieldRim::init, _1, _2 ) );

     //...

    //================ UIGoes ==================//
    ssrc::insert_2_uiGoInit_funcs( "button_sceneBegin_archive_2", std::bind( &uiGos::Button_SceneBegin_Archive::init, _1, _2 ) );
    ssrc::insert_2_uiGoInit_funcs( "button_sceneBegin_pointer_2", std::bind( &uiGos::Button_SceneBegin_Pointer::init, _1, _2 ) );



    //cout << "--- onGoSpecIds_SignUp(). DONE. ---" << endl; 
}



//namespace goSpecIds_inn {//------------------- namespace: goSpecIds_inn ---------------------
//}//------------------------- namespace: goSpecIds_inn end -------------------


//-- 仅用于流程测试 
void onStart_test(){
    //cout << "--- onStart_test(). DONE. ---" << endl;
}




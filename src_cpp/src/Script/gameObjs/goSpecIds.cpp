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
#include "Script/json/GoJsonData.h"

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

    //==================================//
    ssrc::insert_2_goInit_funcs( "norman", std::bind( &gameObjs::Norman::init_in_autoMod, _1, _2, _3, _4, _5, _6 ) );
    ssrc::insert_2_goInit_funcs( "bigMan", std::bind( &gameObjs::BigMan::init_in_autoMod, _1, _2, _3, _4, _5, _6 ) );
    ssrc::insert_2_goInit_funcs( "crab",   std::bind( &gameObjs::Crab::init_in_autoMod, _1, _2, _3, _4, _5, _6 ) );

    //-------------------//
    //        tree
    //-------------------//
    ssrc::insert_2_goInit_funcs( "oakTree",  std::bind( &gameObjs::OakTree::init_in_autoMod, _1, _2, _3, _4, _5, _6 ) );
    ssrc::insert_2_goInit_funcs( "pineTree", std::bind( &gameObjs::PineTree::init_in_autoMod, _1, _2, _3, _4, _5, _6 ) );

    //-------------------//
    //        bush
    //-------------------//
    ssrc::insert_2_goInit_funcs( "wheat",              std::bind( &gameObjs::Wheat::init_in_autoMod, _1, _2, _3, _4, _5, _6 ) );
    ssrc::insert_2_goInit_funcs( "singleStone_Desert", std::bind( &gameObjs::SingleStone_Desert::init_in_autoMod, _1, _2, _3, _4, _5, _6 ) );
    ssrc::insert_2_goInit_funcs( "leaf_DForest",       std::bind( &gameObjs::Leaf_DForest::init_in_autoMod, _1, _2, _3, _4, _5, _6 ) );

    //-------------------//
    //    mapSurface
    //-------------------//
    ssrc::insert_2_goInit_funcs( "lichen_Forest",      std::bind( &gameObjs::Lichen_Forest::init_in_autoMod, _1, _2, _3, _4, _5, _6 ) );
    ssrc::insert_2_goInit_funcs( "lichen_DForest",     std::bind( &gameObjs::Lichen_DForest::init_in_autoMod, _1, _2, _3, _4, _5, _6 ) );

     //...

    //cout << "--- onGoSpecIds_SignUp(). DONE. ---" << endl; 
}



//namespace goSpecIds_inn {//------------------- namespace: goSpecIds_inn ---------------------
//}//------------------------- namespace: goSpecIds_inn end -------------------


//-- 仅用于流程测试 
void onStart_test(){
    //cout << "--- onStart_test(). DONE. ---" << endl;
}




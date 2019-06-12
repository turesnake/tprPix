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

using namespace std::placeholders;

//#include "tprDebug.h" //- tmp


namespace goSpecIds_inn {//-------------- namespace: goSpecIds_inn ---------------------
    void goSpec_alloc( goSpecId_t _id, const std::string &_name );
}//------------------------- namespace: goSpecIds_inn end -------------------


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

    //goSpecIds.insert(0); //--- 0号id 保留。
    //==================================//
    id = 1; 
        goSpecIds_inn::goSpec_alloc( id, "norman" );  
        gameObjs::Norman::specId = id; 
        ssrc::insert_2_goInit_funcs( id, std::bind( &gameObjs::Norman::init_in_autoMod, &gameObjs::norman, _1, _2, _3, _4, _5 ) );

    id = 2; 
        goSpecIds_inn::goSpec_alloc( id, "bigMan" );  
        gameObjs::BigMan::specId = id;
        ssrc::insert_2_goInit_funcs( id, std::bind( &gameObjs::BigMan::init_in_autoMod, &gameObjs::big_man, _1, _2, _3, _4, _5 ) );

    id = 3; 
        goSpecIds_inn::goSpec_alloc( id, "crab" );  
        gameObjs::Crab::specId = id;
        ssrc::insert_2_goInit_funcs( id, std::bind( &gameObjs::Crab::init_in_autoMod, &gameObjs::crab, _1, _2, _3, _4, _5 ) );

    //-------------------//
    //        tree
    //-------------------//
    id = 1001;   //- 橡树
        goSpecIds_inn::goSpec_alloc( id, "oakTree" );  
        gameObjs::OakTree::specId = id;
        ssrc::insert_2_goInit_funcs( id, std::bind( &gameObjs::OakTree::init_in_autoMod, &gameObjs::oakTree, _1, _2, _3, _4, _5 ) );

    id = 1002;  //- 松树
        goSpecIds_inn::goSpec_alloc( id, "pineTree" ); 
        gameObjs::PineTree::specId = id;
        ssrc::insert_2_goInit_funcs( id, std::bind( &gameObjs::PineTree::init_in_autoMod, &gameObjs::pineTree, _1, _2, _3, _4, _5 ) );


    //-------------------//
    //        bush
    //-------------------//
    id = 2001;  //- 小麦
        goSpecIds_inn::goSpec_alloc( id, "wheat" );              
        gameObjs::Wheat::specId = id;
        ssrc::insert_2_goInit_funcs( id, std::bind( &gameObjs::Wheat::init_in_autoMod, &gameObjs::wheat, _1, _2, _3, _4, _5 ) );

    id = 2002;  //- 沙漠中的 单块石头
        goSpecIds_inn::goSpec_alloc( id, "singleStone_Desert" ); 
        gameObjs::SingleStone_Desert::specId = id;
        ssrc::insert_2_goInit_funcs( id, std::bind( &gameObjs::SingleStone_Desert::init_in_autoMod, &gameObjs::singleStone_Desert, _1, _2, _3, _4, _5 ) );

    id = 2003;  //- 黑森林中的 竖状叶片
        goSpecIds_inn::goSpec_alloc( id, "leaf_DForest" );       
        gameObjs::Leaf_DForest::specId = id;
        ssrc::insert_2_goInit_funcs( id, std::bind( &gameObjs::Leaf_DForest::init_in_autoMod, &gameObjs::leaf_DForest, _1, _2, _3, _4, _5 ) );


    //-------------------//
    //    mapSurface
    //-------------------//
    id = 3001;   //- 地衣
        goSpecIds_inn::goSpec_alloc( id, "lichen_Forest" ); 
        gameObjs::Lichen_Forest::specId = id;
        ssrc::insert_2_goInit_funcs( id, std::bind( &gameObjs::Lichen_Forest::init_in_autoMod, &gameObjs::lichen_Forest, _1, _2, _3, _4, _5 ) );


    id = 3002;   //- 地衣
        goSpecIds_inn::goSpec_alloc( id, "lichen_DForest" ); 
        gameObjs::Lichen_DForest::specId = id;
        ssrc::insert_2_goInit_funcs( id, std::bind( &gameObjs::Lichen_DForest::init_in_autoMod, &gameObjs::lichen_DForest, _1, _2, _3, _4, _5 ) );


    //cout << "--- onGoSpecIds_SignUp(). DONE. ---" << endl; 
}



namespace goSpecIds_inn {//------------------- namespace: goSpecIds_inn ---------------------



/* ===========================================================
 *                   goSpec_alloc      
 * -----------------------------------------------------------
 * -- 拼装 正反表
 */
void goSpec_alloc( goSpecId_t _id, const std::string &_name ){
    ssrc::insert_2_go_specId_names_containers( _id, _name );
}

}//------------------------- namespace: goSpecIds_inn end -------------------


//-- 仅用于流程测试 
void onStart_test(){
    //cout << "--- onStart_test(). DONE. ---" << endl;
}




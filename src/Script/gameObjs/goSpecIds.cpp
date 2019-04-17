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
#include "GameObj.h" //- goSpecId_t

//-------------------- Script --------------------//
#include "Script/resource/srcs_script.h" 
#include "Script/gameObjs/allGoes.h"

using namespace std::placeholders;

using std::string;

//#include "debug.h" //- tmp


namespace{//------------------- namespace ---------------------
    void goSpec_alloc( goSpecId_t _id, const string &_name );
}//------------------------- namespace: end -------------------


/* ===========================================================
 *                   onGoSpecIds_SignUp   
 * -----------------------------------------------------------
 * 
 */
void onGoSpecIds_SignUp(){

    goSpecId_t   id;

    ssrc::goInit_funcs.clear();
    ssrc::go_specId_names.clear();
    ssrc::go_name_specIds.clear();
    

    //goSpecIds.insert(0); //--- 0号id 保留。
    //==================================//
    id = 1; 
        goSpec_alloc( id, "norman" );  
        gameObjs::Norman::specId = id; 
        ssrc::goInit_funcs.insert({ id, ssrc::F_GO_INIT {} });
        ssrc::goInit_funcs.at(id) = std::bind( &gameObjs::Norman::init_in_autoMod, &gameObjs::norman, _1, _2, _3, _4, _5 );

    id = 2; 
        goSpec_alloc( id, "bigMan" );  
        gameObjs::BigMan::specId = id;
        ssrc::goInit_funcs.insert({ id, ssrc::F_GO_INIT {} });
        ssrc::goInit_funcs.at(id) = std::bind( &gameObjs::BigMan::init_in_autoMod, &gameObjs::big_man, _1, _2, _3, _4, _5 );

    //-------------------//
    //        tree
    //-------------------//
    id = 1001;   //- 橡树
        goSpec_alloc( id, "oakTree" );  
        gameObjs::OakTree::specId = id;
        ssrc::goInit_funcs.insert({ id, ssrc::F_GO_INIT {} });
        ssrc::goInit_funcs.at(id) = std::bind( &gameObjs::OakTree::init_in_autoMod, &gameObjs::oakTree, _1, _2, _3, _4, _5 );

    id = 1002;  //- 松树
        goSpec_alloc( id, "pineTree" ); 
        gameObjs::PineTree::specId = id;
        ssrc::goInit_funcs.insert({ id, ssrc::F_GO_INIT {} });
        ssrc::goInit_funcs.at(id) = std::bind( &gameObjs::PineTree::init_in_autoMod, &gameObjs::pineTree, _1, _2, _3, _4, _5 );


    //-------------------//
    //        bush
    //-------------------//
    id = 2001;  //- 小麦
        goSpec_alloc( id, "wheat" );              
        gameObjs::Wheat::specId = id;
        ssrc::goInit_funcs.insert({ id, ssrc::F_GO_INIT {} });
        ssrc::goInit_funcs.at(id) = std::bind( &gameObjs::Wheat::init_in_autoMod, &gameObjs::wheat, _1, _2, _3, _4, _5 );

    id = 2002;  //- 沙漠中的 单块石头
        goSpec_alloc( id, "singleStone_Desert" ); 
        gameObjs::SingleStone_Desert::specId = id;
        ssrc::goInit_funcs.insert({ id, ssrc::F_GO_INIT {} });
        ssrc::goInit_funcs.at(id) = std::bind( &gameObjs::SingleStone_Desert::init_in_autoMod, &gameObjs::singleStone_Desert, _1, _2, _3, _4, _5 );

    id = 2003;  //- 黑森林中的 竖状叶片
        goSpec_alloc( id, "leaf_DForest" );       
        gameObjs::Leaf_DForest::specId = id;
        ssrc::goInit_funcs.insert({ id, ssrc::F_GO_INIT {} });
        ssrc::goInit_funcs.at(id) = std::bind( &gameObjs::Leaf_DForest::init_in_autoMod, &gameObjs::leaf_DForest, _1, _2, _3, _4, _5 );


    //-------------------//
    //    mapSurface
    //-------------------//
    id = 3001;   //- 地衣
        goSpec_alloc( id, "lichen_Forest" ); 
        gameObjs::Lichen_Forest::specId = id;
        ssrc::goInit_funcs.insert({ id, ssrc::F_GO_INIT {} });
        ssrc::goInit_funcs.at(id) = std::bind( &gameObjs::Lichen_Forest::init_in_autoMod, &gameObjs::lichen_Forest, _1, _2, _3, _4, _5 );


    id = 3002;   //- 地衣
        goSpec_alloc( id, "lichen_DForest" ); 
        gameObjs::Lichen_DForest::specId = id;
        ssrc::goInit_funcs.insert({ id, ssrc::F_GO_INIT {} });
        ssrc::goInit_funcs.at(id) = std::bind( &gameObjs::Lichen_DForest::init_in_autoMod, &gameObjs::lichen_DForest, _1, _2, _3, _4, _5 );


    //cout << "--- onGoSpecIds_SignUp(). DONE. ---" << endl; 
}



namespace{//------------------- namespace ---------------------



/* ===========================================================
 *                   goSpec_alloc      
 * -----------------------------------------------------------
 * -- 拼装 正反表
 */
void goSpec_alloc( goSpecId_t _id, const string &_name ){

    ssrc::go_specId_names.insert({ _id, _name });
    ssrc::go_name_specIds.insert({ _name, _id });
}

}//------------------------- namespace: end -------------------


//-- 仅用于流程测试 
void onStart_test(){
    //cout << "--- onStart_test(). DONE. ---" << endl;
}




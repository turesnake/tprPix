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

#include "Script/gameObjs/Norman.h" 
#include "Script/gameObjs/BigMan.h" 
#include "Script/gameObjs/OakTree.h" 
#include "Script/gameObjs/mapSurfaces/Lichen.h"
#include "Script/gameObjs/bush/Wheat.h"
#include "Script/gameObjs/bush/SingleStone_Desert.h"


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

    u32_t   id;

    //goSpecIds.insert(0); //--- 0号id 保留。
    //==================================//
    id = 1; goSpec_alloc( id, "norman" );  gameObjs::Norman::specId = id;
    id = 2; goSpec_alloc( id, "bigMan" );  gameObjs::BigMan::specId = id;

    //-------------------//
    //        tree
    //-------------------//
    id = 1001; goSpec_alloc( id, "oakTree" ); gameObjs::OakTree::specId = id; //- 橡树



    //-------------------//
    //        bush
    //-------------------//
    id = 2001; goSpec_alloc( id, "wheat" );              gameObjs::Wheat::specId = id; //- 小麦
    id = 2002; goSpec_alloc( id, "singleStone_Desert" ); gameObjs::SingleStone_Desert::specId = id; //- 沙漠中的 单块石头


    //-------------------//
    //    mapSurface
    //-------------------//
    id = 3001; goSpec_alloc( id, "lichen" ); gameObjs::Lichen::specId = id; //- 地衣


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


void onStart_test(){
    //cout << "--- onStart_test(). DONE. ---" << endl;
}




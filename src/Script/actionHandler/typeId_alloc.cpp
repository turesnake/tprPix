/*
 * ========================= typeId_alloc.cpp ==========================
 *                          -- tpr --
 *                                        创建 -- 2018.12.31
 *                                        修改 -- 2018.12.31
 * ----------------------------------------------------------
 *   
 * ----------------------------
 */
#include "Engine/game/ID_Manager.h"
#include "Script/resource/src_script.h" 

//-------------------- CPP --------------------//
#include <string>

//------ need to include all ActionHandler .h files --------
#include "Cycle.h" 


using std::string;


namespace actionHdlr{//------------- namespace ActionHdlr ----------------

namespace{//------------------- namespace ---------------------
    ID_Manager idm { ID_TYPE::U32, 1 }; 
    u64 alloc( const string &_name );
}//------------------------- namespace: end -------------------

/* ===========================================================
 *                 typeId_alloc
 * -----------------------------------------------------------
 * -- 为每一个 actionHandler 类 分配 typeId
 */
void typeId_alloc(){

    Cycle::typeId = alloc( "Cycle" );


    //...



}


namespace{//------------------- namespace ---------------------

/* ===========================================================
 *                    alloc
 * -----------------------------------------------------------
 * -- 
 */
u64 alloc( const string &_name ){

    u64 id = idm.apply_a_u32_id();

    //-- 存入一个 script区的 公共容器中 --
    ssrc::acionHdlr_typeId_names.insert({ id, _name });
    ssrc::acionHdlr_name_typeIds.insert({ _name, id });

    return id;
}









}//------------------------- namespace: end -------------------
}//----------------- namespace ActionHdlr: end -------------------


/*
 * ========================= typeId_alloc.cpp ==========================
 *                          -- tpr --
 *                                        创建 -- 2018.12.31
 *                                        修改 -- 2018.12.31
 * ----------------------------------------------------------
 *   
 * ----------------------------
 */
//-------------------- Engine --------------------//
#include "ID_Manager.h"

//-------------------- Script --------------------//
#include "Script/resource/srcs_script.h" 

//-------------------- CPP --------------------//
#include <string>

//------ need to include all ActionHandle .h files --------
#include "Cycle.h" 


using std::string;


namespace actionHdle{//------------- namespace ActionHdle ----------------

namespace{//------------------- namespace ---------------------
    ID_Manager idm { ID_TYPE::U32, 1 }; 
    u64 alloc( const string &_name );
}//------------------------- namespace: end -------------------

/* ===========================================================
 *                 typeId_alloc
 * -----------------------------------------------------------
 * -- 为每一个 actionHandle 类 分配 typeId
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
    ssrc::acionHdle_typeId_names.insert({ id, _name });
    ssrc::acionHdle_name_typeIds.insert({ _name, id });

    return id;
}









}//------------------------- namespace: end -------------------
}//----------------- namespace ActionHdle: end -------------------


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

//------ need to include all ActionHandler .h files --------
#include "Cycle.h" 



namespace actionHdlr{//------------- namespace ActionHdlr ----------------

namespace{
    ID_Manager idm { ID_TYPE::U32, 1 }; 
    u64 alloc(  );
}

/* ===========================================================
 *                 typeId_alloc
 * -----------------------------------------------------------
 * -- 
 */
void typeId_alloc(){

    Cycle::typeId = alloc();


    //...



}


namespace{

/* ===========================================================
 *                    alloc
 * -----------------------------------------------------------
 * -- 
 */
u64 alloc(  ){

    u64 id = idm.apply_a_u32_id();
    //...

    return id;
}









}
}//----------------- namespace ActionHdlr: end -------------------


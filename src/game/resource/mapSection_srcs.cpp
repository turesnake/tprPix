/*
 * ========================= mapSection_srcs.cpp ==========================
 *                          -- tpr --
 *                                        创建 -- 2018.12.11
 *                                        修改 -- 2018.12.11
 * ----------------------------------------------------------
 *  资源管理: mapSection 
 * ----------------------------
 */
#include "srcs_manager.h"

//-------------------- C ----------------------//
//#include <string.h> //- memset, memcpy
#include <cassert> //-- assert

//-------------------- CPP --------------------//
#include <iostream> //-- cout
#include <string>
#include <vector>

//------------------- Libs --------------------//
#include "tprDB.h"

//------------------- SELF --------------------//
#include "global.h"
#include "GameObj.h" 
#include "MapSection.h"

using std::string;
using std::cout;
using std::endl;

//------------------- 提供给外部的函数 ----------------
void mapSection_srcs_init();
//void mapSection_srcs_save();


namespace{//------------ namespace --------------//

    //-- 一个 pure_var 数据库实例。存储 唯一一个 dbent --
    tpr::tprDB mapSection_fst_db{ tpr::DB::DB_TYPE::Pure_Var, false };

    
}//---------------- namespace end --------------//


/* ===========================================================
 *               mapSection_srcs_init  
 * -----------------------------------------------------------
 * -- 
 */
void mapSection_srcs_init(){

    mapSection_fst_db.init_huge( path_data,
                                "mapSection_fst",
                                sizeof(diskMapSection_fst)
                                );

}








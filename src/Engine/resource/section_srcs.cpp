/*
 * ========================= section_srcs.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2018.12.11
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *  资源管理: Section 
 * ----------------------------
 */
//-------------------- C ----------------------//
#include <cassert> //-- assert

//-------------------- CPP --------------------//
#include <string>
#include <vector>

//------------------- Libs --------------------//
#include "tprDB.h"

//-------------------- Engine --------------------//
#include "srcs_engine.h" //- 所有资源
#include "global.h"
#include "GameObj.h" 
#include "Section.h"

using std::string;

//#include "debug.h" //- tmp

namespace esrc{ //------------------ namespace: esrc -------------------------//

//------------------- 提供给外部的函数 ----------------
void section_srcs_init();
//void section_srcs_save();


namespace{//------------ namespace --------------//

    //-- 一个 pure_var 数据库实例。存储 唯一一个 dbent --
    tpr::tprDB  fstSection_db  { tpr::DB::DB_TYPE::Pure_Var, false };

    
}//---------------- namespace end --------------//


/* ===========================================================
 *               section_srcs_init  
 * -----------------------------------------------------------
 * -- 
 */
void mapSection_srcs_init(){

    fstSection_db.init_huge( path_data,
                                "fstSection",
                                sizeof(Fst_diskMapSection)
                                );
    //----------------------------------
    // 临时方案：
    //     若有存档，读取之
    //     若无存档，创建之
    //----------------------------------
    size_t fstSection_db_ents = fstSection_db.ent_nums();
    if( fstSection_db_ents == 0 ){
        //----- 没有存档，新建之 --------//
        



    }else{
        //----- 有现成存档，读取之 --------//
        //...



    }









}






}//---------------------- namespace: esrc -------------------------//


/*
 * ===================== table_goes.cpp ======================
 *                          -- tpr --
 *                                        CREATE -- 2019.05.02
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#include "dataBase.h"
#include "dataBase_inn.h"


namespace db{//---------------- namespace: db ----------------------//


/* ===========================================================
 *            atom_select_one_from_table_goes
 * -----------------------------------------------------------
 *  param:diskGo_ [out]
 */
void atom_select_one_from_table_goes( goid_t goid_, DiskGameObj &diskGo_ ){

    //--- atom ---//
    std::lock_guard<std::mutex> lg( dbMutex );

    //-- reset --
    w_sqlite3_reset( dbConnect, stmt_select_one_from_table_goes );

    //-- bind goid_ --
    w_sqlite3_bind_int64( dbConnect, stmt_select_one_from_table_goes, 1, static_cast<i64_t>(goid_) );

    if( sqlite3_step(stmt_select_one_from_table_goes) == SQLITE_ROW ){
        diskGo_.goid = goid_;
        diskGo_.goSpeciesId = static_cast<goSpeciesId_t>( sqlite3_column_int(stmt_select_one_from_table_goes, 0) );
        diskGo_.dpos.x = sqlite3_column_double(stmt_select_one_from_table_goes, 1);
        diskGo_.dpos.y = sqlite3_column_double(stmt_select_one_from_table_goes, 2);
    }
}


/* ===========================================================
 *          atom_insert_or_replace_to_table_goes
 * -----------------------------------------------------------
 */
void atom_insert_or_replace_to_table_goes( const DiskGameObj &diskGo_ ){

    //--- atom ---//
    std::lock_guard<std::mutex> lg( dbMutex );

    //-- reset --
    w_sqlite3_reset( dbConnect, stmt_insert_or_replace_to_table_goes );

    //-- bind --      
    //  注意：下面这组操作，必须在一个 atom 函数内被调用 --          
    reset_stmt_for_bindFuncs_inn_( stmt_insert_or_replace_to_table_goes );
    sqlite3_bind_int64_inn_(  ":goid",  
                            static_cast<i64_t>(diskGo_.goid) );

    sqlite3_bind_int_inn_(  ":goSpeciesId", 
                            static_cast<int>(diskGo_.goSpeciesId) );

    sqlite3_bind_double_inn_(  ":dposX", 
                            diskGo_.dpos.x );

    sqlite3_bind_double_inn_(  ":dposY", 
                            diskGo_.dpos.y );

    //-- step --
    w_sqlite3_step( dbConnect, stmt_insert_or_replace_to_table_goes, SQLITE_DONE );
}




}//----------------------- namespace: db end ----------------------//


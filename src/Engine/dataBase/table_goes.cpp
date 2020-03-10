/*
 * ===================== table_goes.cpp ======================
 *                          -- tpr --
 *                                        CREATE -- 2019.05.02
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#include "pch.h"
#include "dataBase.h"
#include "dataBase_inn.h"


namespace db{//---------------- namespace: db ----------------------//


// param:diskGo_ [out]
void atom_select_one_from_table_goes( goid_t goid_, DiskGameObj &diskGo_ ){

    //--- atom ---//
    std::lock_guard<std::mutex> lg( dbMutex );

    //-- reset --
    w_sqlite3_reset( dbConnect, stmt_select_one_from_table_goes );

    //-- bind goid_ --
    w_sqlite3_bind_int64( dbConnect, stmt_select_one_from_table_goes, 1, static_cast<int64_t>(goid_) );

    if( sqlite3_step(stmt_select_one_from_table_goes) == SQLITE_ROW ){
        diskGo_.goid = goid_;
        diskGo_.goSpeciesId = static_cast<goSpeciesId_t>( sqlite3_column_int64(stmt_select_one_from_table_goes, 0) );
        diskGo_.goLabelId = static_cast<goLabelId_t>( sqlite3_column_int64(stmt_select_one_from_table_goes, 1) );
        diskGo_.dpos.x = sqlite3_column_double(stmt_select_one_from_table_goes, 2);
        diskGo_.dpos.y = sqlite3_column_double(stmt_select_one_from_table_goes, 3);
        diskGo_.goUWeight = static_cast<size_t>( sqlite3_column_int64(stmt_select_one_from_table_goes, 4) );
        diskGo_.dir = idx_2_nineDirection(static_cast<size_t>( sqlite3_column_double(stmt_select_one_from_table_goes, 5) ));
        diskGo_.brokenLvl = int_2_brokenLvl(static_cast<int>( sqlite3_column_double(stmt_select_one_from_table_goes, 6) ));
    }
}



void atom_insert_or_replace_to_table_goes( const DiskGameObj &diskGo_ ){

    //--- atom ---//
    std::lock_guard<std::mutex> lg( dbMutex );

    //-- reset --
    w_sqlite3_reset( dbConnect, stmt_insert_or_replace_to_table_goes );

    //-- bind --      
    //  注意：下面这组操作，必须在一个 atom 函数内被调用 --          
    reset_stmt_for_bindFuncs_inn_( stmt_insert_or_replace_to_table_goes );
    sqlite3_bind_int64_inn_(  ":goid",  
                            static_cast<int64_t>(diskGo_.goid) );

    sqlite3_bind_int64_inn_(  ":goSpeciesId", 
                            static_cast<int64_t>(diskGo_.goSpeciesId) );

    sqlite3_bind_int64_inn_(  ":goLabelId", 
                            static_cast<int64_t>(diskGo_.goLabelId) );

    sqlite3_bind_double_inn_(  ":dposX", 
                            diskGo_.dpos.x );

    sqlite3_bind_double_inn_(  ":dposY", 
                            diskGo_.dpos.y );

    sqlite3_bind_int64_inn_(  ":goUWeight", 
                            static_cast<int64_t>(diskGo_.goUWeight) );

    sqlite3_bind_int64_inn_(  ":dir", 
                            static_cast<int64_t>(diskGo_.dir) );

    sqlite3_bind_int64_inn_(  ":brokenLvl", 
                            static_cast<int64_t>(diskGo_.brokenLvl) );

    //-- step --
    w_sqlite3_step( dbConnect, stmt_insert_or_replace_to_table_goes, SQLITE_DONE );
}




}//----------------------- namespace: db end ----------------------//


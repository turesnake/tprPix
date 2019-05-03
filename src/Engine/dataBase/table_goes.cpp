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
 *  param:_diskGo [out]
 */
void atom_select_one_from_table_goes( goid_t _goid, DiskGameObj &_diskGo ){

    //--- atom ---//
    std::lock_guard<std::mutex> lg( dbMutex );

    //-- reset --
    w_sqlite3_reset( dbConnect, stmt_select_one_from_table_goes );

    //-- bind _goid --
    w_sqlite3_bind_int64( dbConnect, stmt_select_one_from_table_goes, 1, static_cast<i64_t>(_goid) );

    if( sqlite3_step(stmt_select_one_from_table_goes) == SQLITE_ROW ){
        _diskGo.goid = _goid;
        _diskGo.goSpecId = static_cast<goSpecId_t>( sqlite3_column_int(stmt_select_one_from_table_goes, 0) );
        _diskGo.mpos.x = sqlite3_column_int(stmt_select_one_from_table_goes, 1);
        _diskGo.mpos.y = sqlite3_column_int(stmt_select_one_from_table_goes, 2);
    }
}



/* ===========================================================
 *          atom_insert_or_replace_to_table_goes
 * -----------------------------------------------------------
 */
void atom_insert_or_replace_to_table_goes( const DiskGameObj &_diskGo ){

    //--- atom ---//
    std::lock_guard<std::mutex> lg( dbMutex );

    //-- reset --
    w_sqlite3_reset( dbConnect, stmt_insert_or_replace_to_table_goes );

    //-- bind --      
    //  注意：下面这组操作，必须在一个 atom 函数内被调用 --          
    reset_stmt_for_bindFuncs_inn_( stmt_insert_or_replace_to_table_goes );
    sqlite3_bind_int64_inn_(  ":goid",  
                            static_cast<i64_t>(_diskGo.goid) );

    sqlite3_bind_int_inn_(  ":goSpecId", 
                            static_cast<int>(_diskGo.goSpecId) );

    sqlite3_bind_int_inn_(  ":mposX", 
                            _diskGo.mpos.x );

    sqlite3_bind_int_inn_(  ":mposY", 
                            _diskGo.mpos.y );
    

    //-- step --
    w_sqlite3_step( dbConnect, stmt_insert_or_replace_to_table_goes, SQLITE_DONE );
}












}//----------------------- namespace: db end ----------------------//


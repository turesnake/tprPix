/*
 * =================== table_gameArchive.cpp ====================
 *                          -- tpr --
 *                                        CREATE -- 2019.04.29
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#include "pch.h"
#include "dataBase.h"
#include "dataBase_inn.h"

namespace db{//---------------- namespace: db ----------------------//

/* ===========================================================
 *           atom_select_all_from_table_gameArchive
 * -----------------------------------------------------------
 * 将 table_gameArchive 中数据 全部读取，写入 参数传入的 _container 中
 */
void atom_select_all_from_table_gameArchive( std::unordered_map<gameArchiveId_t, GameArchive> &container_ ){

    container_.clear();
    GameArchive archive {};

    //--- atom ---//
    std::lock_guard<std::mutex> lg( dbMutex );

    //-- reset --
    w_sqlite3_reset( dbConnect, stmt_select_all_from_table_gameArchive );
    //-- steps --
    while( sqlite3_step(stmt_select_all_from_table_gameArchive) == SQLITE_ROW ){
        archive.id = static_cast<gameArchiveId_t>( sqlite3_column_int(stmt_select_all_from_table_gameArchive, 0) );
        archive.baseSeed = static_cast<uint32_t>( sqlite3_column_int(stmt_select_all_from_table_gameArchive, 1) );
        archive.playerGoId = static_cast<uint64_t>( sqlite3_column_int64(stmt_select_all_from_table_gameArchive, 2) );
        archive.playerGoDPos.x =  sqlite3_column_double(stmt_select_all_from_table_gameArchive, 3);
        archive.playerGoDPos.y =  sqlite3_column_double(stmt_select_all_from_table_gameArchive, 4);
        archive.maxGoId        =  static_cast<uint64_t>( sqlite3_column_int64(stmt_select_all_from_table_gameArchive, 5) );
        archive.gameTime       =  sqlite3_column_double(stmt_select_all_from_table_gameArchive, 6);
        //---
        auto [insertIt, insertBool] = container_.emplace( archive.id, archive ); //- copy
        tprAssert( insertBool );
    }
                //-- 这样写 很可能是 不够安全的。暂时先不管....
}



/* ===========================================================
 *            atom_insert_or_replace_to_table_gameArchive
 * -----------------------------------------------------------
 */
void atom_insert_or_replace_to_table_gameArchive( const GameArchive &archive_ ){

    //--- atom ---//
    std::lock_guard<std::mutex> lg( dbMutex );

    //-- reset --
    w_sqlite3_reset( dbConnect, stmt_insert_or_replace_to_table_gameArchive );
    //-- bind --
    //-- 注意：下面这组操作，必须在一个 atom 函数内被调用 --
    reset_stmt_for_bindFuncs_inn_( stmt_insert_or_replace_to_table_gameArchive );
    sqlite3_bind_int_inn_( ":id", 
                            static_cast<int>(archive_.id) );

    sqlite3_bind_int_inn_( ":baseSeed", 
                            static_cast<int>(archive_.baseSeed) );

    sqlite3_bind_int64_inn_( ":playerGoId", 
                            static_cast<int64_t>(archive_.playerGoId) );

    sqlite3_bind_double_inn_( ":playerGoDPosX", 
                            archive_.playerGoDPos.x );

    sqlite3_bind_double_inn_( ":playerGoDPosY", 
                            archive_.playerGoDPos.y );

    sqlite3_bind_int64_inn_( ":maxGoId", 
                            static_cast<int64_t>(archive_.maxGoId) );
    
    sqlite3_bind_double_inn_( ":gameTime", 
                            archive_.gameTime );

    //-- step --
    w_sqlite3_step( dbConnect, stmt_insert_or_replace_to_table_gameArchive, SQLITE_DONE );
}


}//----------------------- namespace: db end ----------------------//


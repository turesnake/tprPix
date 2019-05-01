/*
 * =================== table_gameArchive.cpp ====================
 *                          -- tpr --
 *                                        CREATE -- 2019.04.29
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#include "dataBase.h"
#include "dataBase_inn.h"



namespace db{//---------------- namespace: db ----------------------//



/* ===========================================================
 *            insert_or_replace_to_table_gameArchive
 * -----------------------------------------------------------
 */
void insert_or_replace_to_table_gameArchive( const GameArchive &_archive ){
    //-- reset --
    w_sqlite3_reset( db, stmt_insert_or_replace_to_table_gameArchive );
    //-- bind --
    w_sqlite3_bind_int( db, stmt_insert_or_replace_to_table_gameArchive, 
                        w_sqlite3_bind_parameter_index( stmt_insert_or_replace_to_table_gameArchive, ":id" ),  
                        static_cast<int>(_archive.id) );
    
    w_sqlite3_bind_int( db, stmt_insert_or_replace_to_table_gameArchive, 
                        w_sqlite3_bind_parameter_index( stmt_insert_or_replace_to_table_gameArchive, ":baseSeed" ),  
                        static_cast<int>(_archive.baseSeed) );
    //-- step --
    w_sqlite3_step( db, stmt_insert_or_replace_to_table_gameArchive, SQLITE_DONE );
}




/* ===========================================================
 *           select_all_from_table_gameArchive
 * -----------------------------------------------------------
 * 将 table_gameArchive 中数据 全部读取，写入 参数传入的 _container 中
 */
void select_all_from_table_gameArchive( std::unordered_map<gameArchiveId_t, GameArchive> &_container ){

    _container.clear();

    GameArchive archive {};
    //-- reset --
    w_sqlite3_reset( db, stmt_select_all_from_table_gameArchive );

    while( sqlite3_step(stmt_select_all_from_table_gameArchive) == SQLITE_ROW ){
        archive.id = static_cast<gameArchiveId_t>( sqlite3_column_int(stmt_select_all_from_table_gameArchive, 0) );
        archive.baseSeed = static_cast<u32_t>( sqlite3_column_int(stmt_select_all_from_table_gameArchive, 1) );
        //_container.push_back(archive); //- copy
        _container.insert({ archive.id, archive }); //- copy
    }
                //-- 这样写 很可能是 不够安全的。暂时先不管....

}






}//----------------------- namespace: db end ----------------------//


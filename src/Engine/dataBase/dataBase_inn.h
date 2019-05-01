/*
 * ========================= dataBase_inn.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.04.29
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 * only in dataBase inner
 */
#ifndef _TPR_DATA_BASE_INN_H_
#define _TPR_DATA_BASE_INN_H_

//-------------------- CPP --------------------//
#include <string>
#include <vector>
#include <unordered_map>

//-------------------- Engine --------------------//
#include "wrapSqlite3.h"
#include "global.h"
#include "GameArchive.h"



namespace db{//---------------- namespace: db ----------------------//


//-- 全游戏唯一的 db connect 实例 --
inline sqlite3 *db;
inline char    *zErrMsg {nullptr};
inline int      rc;


//===== funcs =====//
int callback_1(void *_data, int _argc, char **_argv, char **_azColNames);


//===== table_gameArchive =====//
// 主table，存储一切 全局性的数据
inline const std::string sql_create_table_gameArchive  {
    "CREATE TABLE IF NOT EXISTS table_gameArchive("  \
    "id             INT     PRIMARY KEY     NOT NULL," \
    "baseSeed       INT     NOT NULL" \
    ");" \

    "PRAGMA journal_mode=WAL;" \
    "SELECT * FROM table_gameArchive" 
    };


//-- 游戏启动初期，将 table_gameArchive 中数据全部读取。
//   由玩家选择，进入哪个存档
inline const std::string sql_select_all_from_table_gameArchive  {
    "SELECT id, baseSeed FROM table_gameArchive;" 
    };
inline sqlite3_stmt *stmt_select_all_from_table_gameArchive {nullptr};



inline const std::string sql_insert_or_replace_to_table_gameArchive  {
    "INSERT OR REPLACE INTO table_gameArchive (id, baseSeed) " \
    "VALUES ( :id, :baseSeed );" 
    };
inline sqlite3_stmt *stmt_insert_or_replace_to_table_gameArchive {nullptr};


//--- old test --

inline const std::string sql_select_all  {
    "SELECT * FROM table_test" 
    };


inline const std::string sql_insert_or_replace  {
    "INSERT OR REPLACE INTO table_test (ID, AGE, SALARY, NAME, BINARY) " \
    "VALUES ( :ID, :AGE, :SALARY, :NAME, :BINARY );" 
    };


inline const std::string sql_select_to_read  {
    "SELECT AGE, SALARY, NAME, BINARY FROM table_test WHERE ID = ?;" 
    };





}//----------------------- namespace: db end ----------------------//
#endif 


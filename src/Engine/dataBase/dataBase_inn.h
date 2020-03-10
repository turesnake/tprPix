/*
 * ========================= dataBase_inn.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.04.29
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 * only in dataBase inner
 */
#ifndef TPR_DATA_BASE_INN_H
#define TPR_DATA_BASE_INN_H

#include "pch.h"

//-------------------- CPP --------------------//
#include <mutex>

//-------------------- Engine --------------------//
#include "wrapSqlite3.h"
#include "global.h"
#include "GameArchive.h"



namespace db{//---------------- namespace: db ----------------------//


//-- 全游戏唯一的 db connect 实例 --
inline sqlite3 *dbConnect {nullptr};
inline char    *zErrMsg   {nullptr};
inline int      rc        {};

inline std::mutex dbMutex;


//===== funcs =====//
int callback_1(void *data_, int argc_, char **argv_, char **azColNames_);


//-- prepare_v2 --
//-- 注意，参数 ppStmt_ 必须为 pp，
inline void sqlite3_prepare_v2_inn_( const std::string &sql_str_, sqlite3_stmt **ppStmt_ ){
    w_sqlite3_prepare_v2(   dbConnect, 
                            sql_str_.c_str(), 
                            static_cast<int>(sql_str_.size()+1),
                            ppStmt_,
                            nullptr );
}


//---- bind vals ----
//  注意：下面这组操作，必须在一个 atom 函数内被调用 --
inline sqlite3_stmt *stmt_for_bindFuncs {nullptr};

inline void reset_stmt_for_bindFuncs_inn_( sqlite3_stmt *newStmt_ ){
    stmt_for_bindFuncs = newStmt_;
}

inline void sqlite3_bind_int_inn_( const std::string &paramStr_, const int &val_ ){
    w_sqlite3_bind_int( dbConnect, stmt_for_bindFuncs, 
                        w_sqlite3_bind_parameter_index( stmt_for_bindFuncs, paramStr_.c_str() ),  
                        val_ );
}
inline void sqlite3_bind_int64_inn_( const std::string &paramStr_, const int64_t &val_ ){
    w_sqlite3_bind_int64( dbConnect, stmt_for_bindFuncs, 
                        w_sqlite3_bind_parameter_index( stmt_for_bindFuncs, paramStr_.c_str() ),  
                        val_ );
}
inline void sqlite3_bind_double_inn_( const std::string &paramStr_, const double &val_ ){
    w_sqlite3_bind_double( dbConnect, stmt_for_bindFuncs, 
                        w_sqlite3_bind_parameter_index( stmt_for_bindFuncs, paramStr_.c_str() ),  
                        val_ );
}



//=============================//
//      table_gameArchive 
//=============================//
// 主table，存储一切 全局性的数据
inline const std::string sql_create_table_gameArchive  {
    "CREATE TABLE IF NOT EXISTS table_gameArchive("  \
    "id               INT     PRIMARY KEY     NOT NULL," \
    "baseSeed         INT     NOT NULL, " \
    /* player */
    "playerGoId       INTEGER    NOT NULL, " \
    "playerGoDPosX    DOUBLE     NOT NULL,  " \
    "playerGoDPosY    DOUBLE     NOT NULL,  " \
    /* GameObj */
    "maxGoId          INTEGER    NOT NULL,  " \
    /* time */
    "gameTime         DOUBLE     NOT NULL  " \
    ");" \

    "PRAGMA journal_mode=WAL;" 
    "SELECT * FROM table_gameArchive" 
    };


//-- 游戏启动初期，将 table_gameArchive 中数据全部读取。
//   由玩家选择，进入哪个存档
inline const std::string sql_select_all_from_table_gameArchive  {
    "SELECT "\
        "id, "\
        "baseSeed,   "\
        "playerGoId,   "\
        "playerGoDPosX, "\
        "playerGoDPosY,  "\
        "maxGoId,  "\
        "gameTime  "\
        "FROM table_gameArchive;" 
    };
inline sqlite3_stmt *stmt_select_all_from_table_gameArchive {nullptr};


inline const std::string sql_insert_or_replace_to_table_gameArchive  {
    "INSERT OR REPLACE INTO table_gameArchive (id, baseSeed, playerGoId, playerGoDPosX, playerGoDPosY, maxGoId, gameTime ) " \
    "VALUES ( :id, :baseSeed, :playerGoId, :playerGoDPosX, :playerGoDPosY, :maxGoId, :gameTime );" 
    };
inline sqlite3_stmt *stmt_insert_or_replace_to_table_gameArchive {nullptr};




//=============================//
//      table_chunks
//=============================//
inline const std::string sql_create_table_chunks  {
    "CREATE TABLE IF NOT EXISTS table_chunks("  \
    "chunkKey       INTEGER     PRIMARY KEY     NOT NULL," \
    "padding        INT         NOT NULL " \
    ");" 
    };


//=============================//
//        table_goes
//=============================//
inline const std::string sql_create_table_goes  {
    "CREATE TABLE IF NOT EXISTS table_goes("  \
    "goid           INTEGER     PRIMARY KEY     NOT NULL," \
    "goSpeciesId    INTEGER        NOT NULL,  " \
    "goLabelId      INTEGER        NOT NULL,  " \
    "dposX          DOUBLE         NOT NULL,  " \
    "dposY          DOUBLE         NOT NULL,  " \
    "goUWeight      INTEGER        NOT NULL,  " \
    "dir            INTEGER         NOT NULL,  " \
    "brokenLvl      INTEGER         NOT NULL  " \
    ");" 
    };

inline const std::string sql_select_one_from_table_goes  {
    "SELECT  "\
        "goSpeciesId, "\
        "goLabelId, "\
        "dposX,  "\
        "dposY,  "\
        "goUWeight,  "\
        "dir,  "\
        "brokenLvl   "\
        "FROM table_goes WHERE goid = ?;" 
    };
inline sqlite3_stmt *stmt_select_one_from_table_goes {nullptr};


inline const std::string sql_insert_or_replace_to_table_goes  {
    "INSERT OR REPLACE INTO table_goes ( goid, goSpeciesId, goLabelId, dposX, dposY, goUWeight, dir, brokenLvl ) " \
    "VALUES ( :goid, :goSpeciesId, :goLabelId, :dposX, :dposY, :goUWeight, :dir, :brokenLvl );" 
    };
inline sqlite3_stmt *stmt_insert_or_replace_to_table_goes {nullptr};





}//----------------------- namespace: db end ----------------------//
#endif 


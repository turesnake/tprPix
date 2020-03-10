/*
 * ====================== wrapSqlite3.h =======================
 *                          -- tpr --
 *                                        CREATE -- 2019.04.22
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *    wrap funcs of sqlite3
 * ----------------------------
 */
#ifndef TPR_WRAP_SQLITE3_H
#define TPR_WRAP_SQLITE3_H

//-------------------- CPP --------------------//
#include <string>

//-------------------- Libs --------------------//
#include "tprAssert.h"
#include "sqlite3.h"

#include "tprDebug.h"


/* ===========================================================
 *                   handle_sqlite_err_inn
 * -----------------------------------------------------------
 * -- 版本1，更加精细的出错信息
 * -- 版本2，仅仅翻译 ERROR CODE 名称
 */
inline void handle_sqlite_err_inn( sqlite3 *db_, int rc_, const std::string &funcName_ ){
    tprAssert( rc_ != SQLITE_OK );

    tprDebug::console(
        "ERROR: {0}: \n{1}",
        funcName_,
        sqlite3_errmsg( db_ )
    );

    sqlite3_close( db_ ); //- better
    tprAssert(0);
}
inline void handle_sqlite_err_without_db_inn( int rc_, const std::string &funcName_ ){
    tprAssert( rc_ != SQLITE_OK );

    tprDebug::console(
        "ERROR_CODE: {0}: \n{1}",
        funcName_,
        sqlite3_errstr( rc_ )
    );
    tprAssert(0);
}



/* ===============================================
 *            config : singleThread
 * -----------------------------------------------
 * -- 废弃，改为从 编译器设置 
 */
inline void w_sqlite3_config_singleThread(){
    int rc = sqlite3_config( SQLITE_CONFIG_SINGLETHREAD );
    if( rc != SQLITE_OK ){
        handle_sqlite_err_without_db_inn( rc, "sqlite3_config():single_thread" );
    }
}


/* ===============================================
 *                 open
 * -----------------------------------------------
 */
inline void w_sqlite3_open( const char *filename_, /* Database filename (UTF-8) */
                            sqlite3 **ppDb_        /* OUT: SQLite db handle */
                            ){
    int rc = sqlite3_open(filename_, ppDb_);
    if( rc != SQLITE_OK ){
        handle_sqlite_err_inn( *ppDb_, rc, "sqlite3_open()" );
    }
}

/* ===============================================
 *                 exec
 * -----------------------------------------------
 */
inline void w_sqlite3_exec(  sqlite3* db_,                               /* An open database */
                            const char *sql_,                           /* SQL to be evaluated */
                            int (*callback_)(void*,int,char**,char**),  /* Callback function */
                            void *fstArg_,                              /* 1st argument to callback */
                            char **errmsg_                              /* Error msg written here */
                            ){
    int rc = sqlite3_exec( db_, sql_, callback_, fstArg_, errmsg_ );
    if( rc != SQLITE_OK ){
        handle_sqlite_err_inn( db_, rc, "sqlite3_exec()" );
    }
}


/* ===============================================
 *                 prepare
 * -----------------------------------------------
 */
inline void w_sqlite3_prepare_v2( sqlite3 *db_,           /* Database handle */
                                const char *zSql_,      /* SQL statement, UTF-8 encoded */
                                int nByte_,             /* Maximum length of zSql in bytes. */
                                sqlite3_stmt **ppStmt_, /* OUT: Statement handle */
                                const char **pzTail_    /* OUT: Pointer to unused portion of zSql */
                                ){
    int rc = sqlite3_prepare_v2( db_, zSql_, nByte_, ppStmt_, pzTail_ );
    if( rc != SQLITE_OK ){
        handle_sqlite_err_inn( db_, rc, "sqlite3_prepare_v2()" );
    }
}

/* ===============================================
 *                 reset
 * -----------------------------------------------
 */
inline void w_sqlite3_reset( sqlite3 *db_, sqlite3_stmt *pStmt_ ){
    int rc = sqlite3_reset( pStmt_ );
    if( rc != SQLITE_OK ){
        handle_sqlite_err_inn( db_, rc, "sqlite3_reset()" );
    }
}


/* ===============================================
 *                 step
 * -----------------------------------------------
 */
//-- 一个不全面的 wrap，仅适用于 部分场合 --
inline void w_sqlite3_step( sqlite3 *db_, sqlite3_stmt *pStmt_, int resultCord_){
    int rc = sqlite3_step( pStmt_ );
    if( rc != resultCord_ ){
        handle_sqlite_err_inn( db_, rc, "sqlite3_step()" );
    }
}

/* ===============================================
 *                 finalize
 * -----------------------------------------------
 */
inline void w_sqlite3_finalize( sqlite3 *db_, sqlite3_stmt *pStmt_ ){
    int rc = sqlite3_finalize( pStmt_ );
    if( rc != SQLITE_OK ){
        handle_sqlite_err_inn( db_, rc, "sqlite3_finalize()" );
    }
}


/* ===============================================
 *                     bind 
 * -----------------------------------------------
 */
inline void w_sqlite3_bind_blob( sqlite3 *db_, sqlite3_stmt *pStmt_, int idx_, const void *val_, int valSize_, void(*callback_)(void*)){
    int rc = sqlite3_bind_blob( pStmt_, idx_, val_, valSize_, callback_ );
    if( rc != SQLITE_OK ){
        handle_sqlite_err_inn( db_, rc, "sqlite3_bind_blob()" );
    }
}
//-- 暂不使用 blob64 --
//int sqlite3_bind_blob64(sqlite3_stmt *pStmt_, int idx_, const void *val_, sqlite3_uint64 valSize_, void(*callback_)(void*));

inline void w_sqlite3_bind_double( sqlite3 *db_, sqlite3_stmt *pStmt_, int idx_, double val_){
    int rc = sqlite3_bind_double( pStmt_, idx_, val_);
    if( rc != SQLITE_OK ){
        handle_sqlite_err_inn( db_, rc, "sqlite3_bind_double()" );
    }
}
inline void w_sqlite3_bind_int( sqlite3 *db_, sqlite3_stmt *pStmt_, int idx_, int val_){
    int rc = sqlite3_bind_int( pStmt_, idx_, val_);
    if( rc != SQLITE_OK ){
        handle_sqlite_err_inn( db_, rc, "sqlite3_bind_int()" );
    }
}
inline void w_sqlite3_bind_int64( sqlite3 *db_, sqlite3_stmt *pStmt_, int idx_, sqlite3_int64 val_){
    int rc = sqlite3_bind_int64( pStmt_, idx_, val_);
    if( rc != SQLITE_OK ){
        handle_sqlite_err_inn( db_, rc, "sqlite3_bind_int64()" );
    }
}
inline void w_sqlite3_bind_null( sqlite3 *db_, sqlite3_stmt *pStmt_, int idx_){
    int rc = sqlite3_bind_null( pStmt_, idx_);
    if( rc != SQLITE_OK ){
        handle_sqlite_err_inn( db_, rc, "sqlite3_bind_null()" );
    }
}
inline void w_sqlite3_bind_text( sqlite3 *db_, sqlite3_stmt *pStmt_, int idx_, const char *val_, int valSize_, void(*callback_)(void*)){
    int rc = sqlite3_bind_text( pStmt_, idx_, val_, valSize_, callback_ );
    if( rc != SQLITE_OK ){
        handle_sqlite_err_inn( db_, rc, "sqlite3_bind_text()" );
    }
}

//-- 暂不使用 text16 --
//int sqlite3_bind_text16(sqlite3_stmt *pStmt_, int idx_, const void *val_, int valSize_, void(*callback_)(void*));

//-- 暂不使用 text64 --
//int sqlite3_bind_text64(sqlite3_stmt *pStmt_, int idx_, const char *val_, sqlite3_uint64 valSize_, void(*callback_)(void*), unsigned char _encoding);

//-- 暂不使用 sqlite3_value --
//int sqlite3_bind_value(sqlite3_stmt *pStmt_, int idx_, const sqlite3_value *val_);

//-- 暂不使用 pointer --
//int sqlite3_bind_pointer( sqlite3_stmt *pStmt_, int idx_, void *val_, const char *type_, void(*callback_)(void*) );

inline void w_sqlite3_bind_zeroblob( sqlite3 *db_, sqlite3_stmt *pStmt_, int idx_, int valSize_){
    int rc = sqlite3_bind_zeroblob( pStmt_, idx_, valSize_);
    if( rc != SQLITE_OK ){
        handle_sqlite_err_inn( db_, rc, "sqlite3_bind_zeroblob()" );
    }
}

//-- 暂不使用 zeroblob64 --
//int sqlite3_bind_zeroblob64(sqlite3_stmt *pStmt_, int idx_, sqlite3_uint64 valSize_);

/* ===============================================
 *          paramName -> paramIdx
 * -----------------------------------------------
 */
inline int w_sqlite3_bind_parameter_index( sqlite3_stmt *pStmt_, const char *zName_){
    int retIdx = sqlite3_bind_parameter_index( pStmt_, zName_);
    if( retIdx == 0 ){
        tprDebug::console( "cant find parameter: {}", zName_ );
        tprAssert(0);
    }
    return retIdx;
}


/* ===============================================
 *                column
 * -----------------------------------------------
 */
//-- 暂不需要 wrap --


#endif 


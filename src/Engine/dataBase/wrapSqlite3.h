/*
 * ====================== wrapSqlite3.h =======================
 *                          -- tpr --
 *                                        CREATE -- 2019.04.22
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *    wrap funcs of sqlite3
 * ----------------------------
 */
#ifndef _TPR_WRAP_SQLITE3_H_
#define _TPR_WRAP_SQLITE3_H_

//-------------------- C --------------------//
#include <cassert>

//-------------------- CPP --------------------//
#include <iostream>
#include <string>

//-------------------- Libs --------------------//
#include "sqlite3.h"


/* ===========================================================
 *                   _handle_sqlite_err
 * -----------------------------------------------------------
 * -- 版本1，更加精细的出错信息
 * -- 版本2，仅仅翻译 ERROR CODE 名称
 */
inline void _handle_sqlite_err( sqlite3 *_db, int _rc, const std::string &_funcName ){
    assert( _rc != SQLITE_OK );
    std::cout << "ERROR: " << _funcName << ": \n"
        << sqlite3_errmsg( _db )
        << std::endl;
    sqlite3_close( _db ); //- better
    assert(0);
}
inline void _handle_sqlite_err_without_db( int _rc, const std::string &_funcName ){
    assert( _rc != SQLITE_OK );
    std::cout << "ERROR_CODE: " << _funcName << ": \n"
        << sqlite3_errstr( _rc )
        << std::endl;
    assert(0);
}



/* ===============================================
 *            config : singleThread
 * -----------------------------------------------
 * -- 废弃，改为从 编译器设置 
 */
inline void w_sqlite3_config_singleThread(){
    int rc = sqlite3_config( SQLITE_CONFIG_SINGLETHREAD );
    if( rc != SQLITE_OK ){
        _handle_sqlite_err_without_db( rc, "sqlite3_config():single_thread" );
    }
}


/* ===============================================
 *                 open
 * -----------------------------------------------
 */
inline void w_sqlite3_open( const char *_filename, /* Database filename (UTF-8) */
                            sqlite3 **_ppDb        /* OUT: SQLite db handle */
                            ){
    int rc = sqlite3_open(_filename, _ppDb);
    if( rc != SQLITE_OK ){
        _handle_sqlite_err( *_ppDb, rc, "sqlite3_open()" );
    }
}

/* ===============================================
 *                 exec
 * -----------------------------------------------
 */
inline void w_sqlite3_exec(  sqlite3* _db,                               /* An open database */
                            const char *_sql,                           /* SQL to be evaluated */
                            int (*_callback)(void*,int,char**,char**),  /* Callback function */
                            void *_fstArg,                              /* 1st argument to callback */
                            char **_errmsg                              /* Error msg written here */
                            ){
    int rc = sqlite3_exec( _db, _sql, _callback, _fstArg, _errmsg );
    if( rc != SQLITE_OK ){
        _handle_sqlite_err( _db, rc, "sqlite3_exec()" );
    }
}


/* ===============================================
 *                 prepare
 * -----------------------------------------------
 */
inline void w_sqlite3_prepare_v2( sqlite3 *_db,           /* Database handle */
                                const char *_zSql,      /* SQL statement, UTF-8 encoded */
                                int _nByte,             /* Maximum length of zSql in bytes. */
                                sqlite3_stmt **_ppStmt, /* OUT: Statement handle */
                                const char **_pzTail    /* OUT: Pointer to unused portion of zSql */
                                ){
    int rc = sqlite3_prepare_v2( _db, _zSql, _nByte, _ppStmt, _pzTail );
    if( rc != SQLITE_OK ){
        _handle_sqlite_err( _db, rc, "sqlite3_prepare_v2()" );
    }
}

/* ===============================================
 *                 reset
 * -----------------------------------------------
 */
inline void w_sqlite3_reset( sqlite3 *_db, sqlite3_stmt *_pStmt ){
    int rc = sqlite3_reset( _pStmt );
    if( rc != SQLITE_OK ){
        _handle_sqlite_err( _db, rc, "sqlite3_reset()" );
    }
}


/* ===============================================
 *                 step
 * -----------------------------------------------
 */
//-- 一个不全面的 wrap，仅适用于 部分场合 --
inline void w_sqlite3_step( sqlite3 *_db, sqlite3_stmt *_pStmt, int _resultCord){
    int rc = sqlite3_step( _pStmt );
    if( rc != _resultCord ){
        _handle_sqlite_err( _db, rc, "sqlite3_step()" );
    }
}

/* ===============================================
 *                 finalize
 * -----------------------------------------------
 */
inline void w_sqlite3_finalize( sqlite3 *_db, sqlite3_stmt *_pStmt ){
    int rc = sqlite3_finalize( _pStmt );
    if( rc != SQLITE_OK ){
        _handle_sqlite_err( _db, rc, "sqlite3_finalize()" );
    }
}


/* ===============================================
 *                     bind 
 * -----------------------------------------------
 */
inline void w_sqlite3_bind_blob( sqlite3 *_db, sqlite3_stmt *_pStmt, int _idx, const void *_val, int _valSize, void(*_callback)(void*)){
    int rc = sqlite3_bind_blob( _pStmt, _idx, _val, _valSize, _callback );
    if( rc != SQLITE_OK ){
        _handle_sqlite_err( _db, rc, "sqlite3_bind_blob()" );
    }
}
//-- 暂不使用 blob64 --
//int sqlite3_bind_blob64(sqlite3_stmt *_pStmt, int _idx, const void *_val, sqlite3_uint64 _valSize, void(*_callback)(void*));

inline void w_sqlite3_bind_double( sqlite3 *_db, sqlite3_stmt *_pStmt, int _idx, double _val){
    int rc = sqlite3_bind_double( _pStmt, _idx, _val);
    if( rc != SQLITE_OK ){
        _handle_sqlite_err( _db, rc, "sqlite3_bind_double()" );
    }
}
inline void w_sqlite3_bind_int( sqlite3 *_db, sqlite3_stmt *_pStmt, int _idx, int _val){
    int rc = sqlite3_bind_int( _pStmt, _idx, _val);
    if( rc != SQLITE_OK ){
        _handle_sqlite_err( _db, rc, "sqlite3_bind_int()" );
    }
}
inline void w_sqlite3_bind_int64( sqlite3 *_db, sqlite3_stmt *_pStmt, int _idx, sqlite3_int64 _val){
    int rc = sqlite3_bind_int64( _pStmt, _idx, _val);
    if( rc != SQLITE_OK ){
        _handle_sqlite_err( _db, rc, "sqlite3_bind_int64()" );
    }
}
inline void w_sqlite3_bind_null( sqlite3 *_db, sqlite3_stmt *_pStmt, int _idx){
    int rc = sqlite3_bind_null( _pStmt, _idx);
    if( rc != SQLITE_OK ){
        _handle_sqlite_err( _db, rc, "sqlite3_bind_null()" );
    }
}
inline void w_sqlite3_bind_text( sqlite3 *_db, sqlite3_stmt *_pStmt, int _idx, const char *_val, int _valSize, void(*_callback)(void*)){
    int rc = sqlite3_bind_text( _pStmt, _idx, _val, _valSize, _callback );
    if( rc != SQLITE_OK ){
        _handle_sqlite_err( _db, rc, "sqlite3_bind_text()" );
    }
}

//-- 暂不使用 text16 --
//int sqlite3_bind_text16(sqlite3_stmt *_pStmt, int _idx, const void *_val, int _valSize, void(*_callback)(void*));

//-- 暂不使用 text64 --
//int sqlite3_bind_text64(sqlite3_stmt *_pStmt, int _idx, const char *_val, sqlite3_uint64 _valSize, void(*_callback)(void*), unsigned char _encoding);

//-- 暂不使用 sqlite3_value --
//int sqlite3_bind_value(sqlite3_stmt *_pStmt, int _idx, const sqlite3_value *_val);

//-- 暂不使用 pointer --
//int sqlite3_bind_pointer( sqlite3_stmt *_pStmt, int _idx, void *_val, const char *_type, void(*_callback)(void*) );

inline void w_sqlite3_bind_zeroblob( sqlite3 *_db, sqlite3_stmt *_pStmt, int _idx, int _valSize){
    int rc = sqlite3_bind_zeroblob( _pStmt, _idx, _valSize);
    if( rc != SQLITE_OK ){
        _handle_sqlite_err( _db, rc, "sqlite3_bind_zeroblob()" );
    }
}

//-- 暂不使用 zeroblob64 --
//int sqlite3_bind_zeroblob64(sqlite3_stmt *_pStmt, int _idx, sqlite3_uint64 _valSize);

/* ===============================================
 *          paramName -> paramIdx
 * -----------------------------------------------
 */
inline int w_sqlite3_bind_parameter_index( sqlite3_stmt *_pStmt, const char *_zName){
    int retIdx = sqlite3_bind_parameter_index( _pStmt, _zName);
    //assert( retIdx != 0 );
    if( retIdx == 0 ){
        std::cout << "cant find parameter: " << _zName
            << std::endl;
        assert(0);
    }
    return retIdx;
}


/* ===============================================
 *                column
 * -----------------------------------------------
 */
//-- 暂不需要 wrap --


#endif 


/*
 * ===================== dataBase.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.04.29
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#include "dataBase.h"
#include "dataBase_inn.h"

//-------------------- CPP --------------------//
#include <string>
#include <vector>

//------------------- Libs --------------------//
#include "tprFileSys.h" 

//-------------------- Engine --------------------//


#include <iostream>
using std::cout;
using std::endl;


namespace db{//---------------- namespace: db ----------------------//


//namespace{//-------- namespace ----------//
//}//-------------- namespace: end --------//


/* ===========================================================
 *                    init_dataBase
 * -----------------------------------------------------------
 */
void init_dataBase(){


    //-- 返回 0，说明为 单线程 模式 --
    //   已在 编译阶段设置
    cout << "sqlite3_threadsafe(): " << sqlite3_threadsafe() << endl;


    //------------------//
    //       open
    //------------------//
    //w_sqlite3_open( "db/koko_1.db", &db );
    w_sqlite3_open( tpr::path_combine( path_dataBase, "tpr" ).c_str(), 
                    &db );


    //---------------//
    //     exec
    //---------------//
    //- 传入 sqlite3_exec() callback 的参数。
    //  可用来 提示本次 callback 的功能 （仅 debug 时有效）
    std::string data { "Callback function called:" };
    w_sqlite3_exec( db, 
                    sql_create_table_gameArchive.c_str(), 
                    callback_1, 
                    (void*)(data.c_str()), 
                    &zErrMsg);

    //---------------//
    //  prepare all stmt
    //---------------//
    w_sqlite3_prepare_v2(   db, 
                            sql_select_all_from_table_gameArchive.c_str(), 
                            sql_select_all_from_table_gameArchive.size()+1,
                            &stmt_select_all_from_table_gameArchive,
                            NULL );

    w_sqlite3_prepare_v2(   db, 
                            sql_insert_or_replace_to_table_gameArchive.c_str(), 
                            sql_insert_or_replace_to_table_gameArchive.size()+1,
                            &stmt_insert_or_replace_to_table_gameArchive,
                            NULL );


    
}


/* ===========================================================
 *                    close_dataBase
 * -----------------------------------------------------------
 */
void close_dataBase(){

    //w_sqlite3_finalize( db, stmt_insert_or_replace );
    //w_sqlite3_finalize( db, stmt_select_to_read );

    sqlite3_close( db );
    cout << "== DATABASE ALL SUCCESS; ==" << endl;
}














/* ===========================================================
 *                         callback_1
 * -----------------------------------------------------------
 */
int callback_1(void *_data, int _argc, char **_argv, char **_azColNames){
    cout << (const char*)_data << endl;
    for(int i=0; i<_argc; i++){
      cout << _azColNames[i] << " = "
            << (_argv[i] ? _argv[i] : "NULL")
            << endl;
    }
    printf("\n");
    return 0;
}


}//----------------------- namespace: db end ----------------------//


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
//#include "tprFileSys.h" 
#include "tprGeneral.h"

//-------------------- Engine --------------------//
#include "GameObj.h"
#include "esrc_gameArchive.h"
#include "esrc_player.h"
#include "esrc_time.h"

#include <iostream>
using std::cout;
using std::endl;


namespace db{//---------------- namespace: db ----------------------//


/* ===========================================================
 *                    atom_init_dataBase
 * -----------------------------------------------------------
 */
void atom_init_dataBase(){

    //--- atom ---//
    std::lock_guard<std::mutex> lg( dbMutex );

    //-- 返回 0，说明为 单线程 模式 --
    //   已在 编译阶段设置
    cout << "sqlite3_threadsafe(): " << sqlite3_threadsafe() << endl;

    //------------------//
    //       open
    //------------------//
    w_sqlite3_open( tprGeneral::path_combine( path_dataBase, "tpr" ).c_str(), 
                    &dbConnect );

    //---------------//
    //     exec
    //---------------//
    //- 传入 sqlite3_exec() callback 的参数。
    //  可用来 提示本次 callback 的功能 （仅 tprDebug 时有效）
    std::string data { "Callback function called:" };
    w_sqlite3_exec( dbConnect, 
                    sql_create_table_gameArchive.c_str(), 
                    callback_1, 
                    (void*)(data.c_str()), 
                    &zErrMsg);

    w_sqlite3_exec( dbConnect, 
                    sql_create_table_chunks.c_str(), 
                    callback_1, 
                    (void*)(data.c_str()), 
                    &zErrMsg);

    w_sqlite3_exec( dbConnect, 
                    sql_create_table_goes.c_str(), 
                    callback_1, 
                    (void*)(data.c_str()), 
                    &zErrMsg);

    //--------------------//
    //  prepare all stmt
    //--------------------//
    //-- table_gameArchive --    
    sqlite3_prepare_v2_inn_( sql_select_all_from_table_gameArchive,
                            &stmt_select_all_from_table_gameArchive );
    
    sqlite3_prepare_v2_inn_( sql_insert_or_replace_to_table_gameArchive,
                            &stmt_insert_or_replace_to_table_gameArchive );

    //-- table_goes --    
    sqlite3_prepare_v2_inn_( sql_select_one_from_table_goes,
                            &stmt_select_one_from_table_goes );

    sqlite3_prepare_v2_inn_( sql_insert_or_replace_to_table_goes,
                            &stmt_insert_or_replace_to_table_goes );
    
}



/* ===========================================================
 *                 atom_writeBack_to_table_gameArchive
 * -----------------------------------------------------------
 * -- 潦草测试版
 */
void atom_writeBack_to_table_gameArchive(){

    //--- atom ---//
    //此处不该上锁，下面调用了 atom 函数，会引发 递归锁

    goid_t goid = esrc::player.goPtr->id;
    IntVec2 mpos = esrc::player.goPtr->goPos.get_currentMPos();


    //-- 将新数据 写回 db --
    esrc::gameArchive.playerGoId = goid;
    esrc::gameArchive.playerGoMPos = mpos;
    esrc::gameArchive.maxGoId = GameObj::id_manager.get_max_id();
    esrc::gameArchive.gameTime = esrc::timer.get_gameTime();
    //...

    db::atom_insert_or_replace_to_table_gameArchive( esrc::gameArchive );

    DiskGameObj diskGo {};
    diskGo.goid = goid;
    diskGo.goSpecId = esrc::player.goPtr->species;
    diskGo.mpos = mpos;
    db::atom_insert_or_replace_to_table_goes( diskGo );

}


/* ===========================================================
 *                    atom_close_dataBase
 * -----------------------------------------------------------
 */
void atom_close_dataBase(){

    //--- atom ---//
    std::lock_guard<std::mutex> lg( dbMutex );

    //--------------------//
    //    finalize stmts
    //--------------------//
    //-- table_gameArchive --
    w_sqlite3_finalize( dbConnect, stmt_select_all_from_table_gameArchive );
    w_sqlite3_finalize( dbConnect, stmt_insert_or_replace_to_table_gameArchive );

    //-- table_goes --
    w_sqlite3_finalize( dbConnect, stmt_select_one_from_table_goes );
    w_sqlite3_finalize( dbConnect, stmt_insert_or_replace_to_table_goes );

    //-- close --
    sqlite3_close( dbConnect );
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


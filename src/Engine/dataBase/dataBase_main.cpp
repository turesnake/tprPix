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
    Player &playerRef = esrc::get_player();

    goid_t goid = playerRef.goid;
    GameObj &playerGoRef = playerRef.get_goRef();
    const glm::dvec2 &dposRef = playerGoRef.get_dpos();
    //-- 将新数据 写回 db --

    GameArchive &gameArchive = esrc::get_gameArchive();
    gameArchive.playerGoId = goid;
    gameArchive.playerGoDPos = dposRef;
    gameArchive.maxGoId = GameObj::id_manager.get_max_id();
    gameArchive.gameTime = esrc::get_timer().get_gameTime();
    //...

    db::atom_insert_or_replace_to_table_gameArchive( gameArchive );


    DiskGameObj diskGo {};
    diskGo.goid = goid;
    diskGo.goSpeciesId = playerGoRef.speciesId;
    diskGo.goLabelId = playerGoRef.goLabelId;
    diskGo.dpos = dposRef;
    diskGo.dir = playerGoRef.actionDirection.get_newVal();
    diskGo.brokenLvl = playerGoRef.brokenLvl.get_newVal();

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
int callback_1(void *data_, int argc_, char **argv_, char **azColNames_){
    cout << (const char*)data_ << endl;
    for(int i=0; i<argc_; i++){
      cout << azColNames_[i] << " = "
            << (argv_[i] ? argv_[i] : "NULL")
            << endl;
    }
    printf("\n");
    return 0;
}


}//----------------------- namespace: db end ----------------------//


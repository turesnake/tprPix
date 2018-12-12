/*
 * ========================= player_srcs.cpp ==========================
 *                          -- tpr --
 *                                        创建 -- 2018.12.10
 *                                        修改 -- 2018.12.10
 * ----------------------------------------------------------
 *  资源管理: player
 * ----------------------------
 */
#include "srcs_manager.h"

//-------------------- C ----------------------//
//#include <string.h> //- memset, memcpy
#include <cassert> //-- assert

//-------------------- CPP --------------------//
#include <iostream> //-- cout
#include <string>
#include <vector>


//------------------- Libs --------------------//
#include "tprDB.h"

//------------------- SELF --------------------//
#include "global.h"
#include "GameObj.h" 


using std::string;
using std::cout;
using std::endl;


//------------------- 提供给外部的函数 ----------------
void player_srcs_init();
void player_srcs_save();


namespace{//------------ namespace --------------//

    //-- 一个 pure_fix 数据库实例。存储 唯一一个 dbent --
    tpr::tprDB player_db { tpr::DB::DB_TYPE::Pure_Fix, true };
    
}//---------------- namespace end --------------//


/* ===========================================================
 *                       player_srcs_init
 * -----------------------------------------------------------
 */
void player_srcs_init(){

    player_db.init( path_data,
                    "player",
                    { 
                        tpr::DB::DATA_T::U64,
                        tpr::DB::DATA_T::I32,
                        tpr::DB::DATA_T::I32
                    },
                    sizeof(diskPlayer)
                    );
    //----------------------------------
    // 临时方案：
    //     若有存档，读取之
    //     若无存档，创建之
    //----------------------------------
    size_t player_db_ents = player_db.ent_nums();
    assert( player_db_ents <= 1 );
    if( player_db_ents == 1 ){
        //----- 有现成存档，读取之 --------//
        diskPlayer dp;

        tpr::DB::eid_t id = player_db.get_fst_id();

        ssize_t rlen = player_db.read( true, id, &dp, sizeof(dp) );
        assert( rlen == sizeof(dp) );

        //--- 将读取到的 dp, 写入 player ---
        player.d2m( &dp );


    }else{
        //----- 没有存档，新建之 --------//
        diskPlayer dp;
        dp.go_id = GameObj::id_manager.apply_a_id();
        //dp.pos = PixVec2{ 0,0 };
        dp.posx = 0;
        dp.posy = 0;

        //---- 将dp, 写入 player -----
        player.d2m( &dp );
        
        //---- 添加到 数据库 ----
        //-- 由于 数据库为空，所以此处使用 insert
        player_db.insert( true, &dp, sizeof(dp) );


    }

    //cout << "player_db.ent_nums() = " << player_db.ent_nums()
        //<< endl;
}


/* ===========================================================
 *                       player_srcs_save
 * -----------------------------------------------------------
 */
void player_srcs_save(){

    diskPlayer dp = player.m2d();

    //-- 获得 dbent id --
    tpr::DB::eid_t id = player_db.get_fst_id();

    //---- 正式存入 数据库 ----
    player_db.write(true, id, &dp, sizeof(dp) );

}











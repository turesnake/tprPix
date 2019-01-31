/*
 * ========================= globState_srcs.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2018.12.10
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *  资源管理: 游戏全局状态
 *  GlobState 本质是一个 杂物箱，存储游戏中的 杂乱数据
 *  它没有 全局实例，而是 
 *  - 在load时 立即设置到每个变量身上
 *  - 在save时 临时从各个变量上抽取
 * ----------------------------
 */
//-------------------- C ----------------------//
//#include <string.h> //- memset, memcpy
#include <cassert> //-- assert

//-------------------- CPP --------------------//
#include <string>
#include <vector>

//------------------- Libs --------------------//
#include "tprDB.h"

//-------------------- Engine --------------------//
#include "srcs_engine.h" //- 所有资源
#include "global.h"
#include "GameObj.h" 

using std::string;


//#include "debug.h" //- tmp

namespace esrc{ //------------------ namespace: esrc -------------------------//

//------------------- 提供给外部的函数 ----------------
void init_globState_srcs();
void save_globState_srcs();


namespace{//------------ namespace --------------//

    //-- 一个 pure_var 数据库实例。存储 唯一一个 dbent --
    tpr::tprDB globState_db{ tpr::DB::DB_TYPE::Pure_Var, true };

    //--- 全局状态 资源 ---
    struct diskGlobState{
        
        u64  GameObj_max_id {}; //- GameObj 类静态成员 id_manager.max_id
        //...

    };

    
}//---------------- namespace end --------------//


/* ===========================================================
 *                    init_globState_srcs 
 * -----------------------------------------------------------
 */
void init_globState_srcs(){

    globState_db.init( path_data,
                    "globState",
                    { tpr::DB::DATA_T::STRID },
                    sizeof( tpr::DB::T_STRID )
                    );
    //----------------------------------
    // 临时方案：
    //     若有存档，读取之
    //     若无存档，创建之
    //----------------------------------
    size_t globState_db_ents = globState_db.ent_nums();
    assert( globState_db_ents <= 1 );
    if( globState_db_ents == 1 ){
        //----- 有现成存档，读取之 --------//
        diskGlobState dgs;

        tpr::DB::eid_t id = globState_db.get_fst_id();
        ssize_t rlen = globState_db.read( false, id, &dgs, sizeof(dgs) );
        assert( rlen == sizeof(dgs) );

        //--- 将读取到的 全局状态，写入各模块中 --
        //GameObj::id_manager.max_id = dgs.GameObj_max_id;
        GameObj::id_manager.set_max_id( dgs.GameObj_max_id );
        //...

    }else{
        //----- 没有存档，新建之 --------//

        //--- 初始化各 全局状态 ------
        //GameObj::id_manager.max_id = 0;
        GameObj::id_manager.set_max_id( 0 );
        //...

        //---- 制作 diskGlobState 实例 ----
        diskGlobState  dgs;
        dgs.GameObj_max_id = GameObj::id_manager.get_max_id();
        //....

        //---- 存入 数据库 ----
        //-- 由于 数据库为空，所以此处使用 insert
        globState_db.insert( false, &dgs, sizeof(dgs) );
    }

        //cout << "globState_db.ent_nums() = " << globState_db.ent_nums()
            //<< endl;

}



/* ===========================================================
 *                    save_globState_srcs 
 * -----------------------------------------------------------
 */
void save_globState_srcs(){

    //---- 制作 diskGlobState 实例 ----
    diskGlobState  dgs;
    dgs.GameObj_max_id = GameObj::id_manager.get_max_id();
    //....

    //-- 获得 dbent id --
    tpr::DB::eid_t id = globState_db.get_fst_id();

    //---- 正式存入 数据库 ----
    globState_db.write( false, id, &dgs, sizeof(dgs) );
}



}//---------------------- namespace: esrc -------------------------//


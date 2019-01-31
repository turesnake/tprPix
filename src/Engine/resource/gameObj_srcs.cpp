/*
 * ========================= gameObj_srcs.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2018.12.10
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *  资源管理: GameObj
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


namespace{//------------ namespace --------------//

    //-- 一个 regular 数据库实例。存储 所有 go --
    tpr::tprDB gameObj_db { tpr::DB::DB_TYPE::Regular, false };
    
}//---------------- namespace end --------------//


/* ===========================================================
 *                  gameObj_srcs_init
 * -----------------------------------------------------------
 */
void gameObj_srcs_init(){

    gameObj_db.init( path_data,
                    "gameObj",
                    { 
                        tpr::DB::DATA_T::U64
                        
                    },
                    sizeof(diskPlayer)
                    );



}




}//---------------------- namespace: esrc -------------------------//


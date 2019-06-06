/*
 * ===================== gameArchive_mem.cpp ====================
 *                          -- tpr --
 *                                        CREATE -- 2019.06.03
 *                                        MODIFY --
 * ----------------------------------------------------------
 */
#include "esrc_gameArchive.h"

namespace esrc {//------------------ namespace: esrc -------------------------//


namespace {//-------- namespace: --------------//

    //-- 在游戏开始阶段，从 db中读取。 
    //   之后一直存放与此，但并不被改写，也不参与游戏运行
    //   在游戏存储点（或游戏结束时）会被拿出来做数据同步，然后在被存入 db
    GameArchive gameArchive {};

}//------------- namespace: end --------------//


GameArchive &get_gameArchive(){
    return gameArchive;
}





}//---------------------- namespace: esrc -------------------------//






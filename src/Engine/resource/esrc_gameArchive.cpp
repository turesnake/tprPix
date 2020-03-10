/*
 * ===================== esrc_gameArchive.cpp ====================
 *                          -- tpr --
 *                                        CREATE -- 2019.06.03
 *                                        MODIFY --
 * ----------------------------------------------------------
 */
#include "pch.h"
#include "esrc_gameArchive.h"
#include "esrc_state.h"

namespace esrc {//------------------ namespace: esrc -------------------------//


namespace gameArchive_inn {//-------- namespace: gameArchive_inn --------------//

    //-- 在游戏开始阶段，从 db中读取。 
    //   之后一直存放与此，但并不被改写，也不参与游戏运行
    //   在游戏存储点（或游戏结束时）会被拿出来做数据同步，然后在被存入 db
    std::unique_ptr<GameArchive> gameArchiveUPtr;

                    //- 若此数据 会被 多线程读写，则需要加锁...

}//------------- namespace: gameArchive_inn end --------------//

void init_gameArchive(){
    gameArchive_inn::gameArchiveUPtr = std::make_unique<GameArchive>();
    esrc::insertState("gameArchive");
}


GameArchive &get_gameArchive(){
    return *(gameArchive_inn::gameArchiveUPtr);
}





}//---------------------- namespace: esrc -------------------------//






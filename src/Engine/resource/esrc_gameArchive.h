/*
 * ===================== esrc_gameArchive.h ====================
 *                          -- tpr --
 *                                        CREATE -- 2019.05.02
 *                                        MODIFY --
 * ----------------------------------------------------------
 */
#ifndef _TPR_ESRC_GAME_ARCHIVE_H_
#define _TPR_ESRC_GAME_ARCHIVE_H_

//-------------------- Engine --------------------//
#include "GameArchive.h"

namespace esrc{ //------------------ namespace: esrc -------------------------//


//-- 在游戏开始阶段，从 db中读取。 
//   之后一直存放与此，但并不被改写，也不参与游戏运行
//   在游戏存储点（或游戏结束时）会被拿出来做数据同步，然后在被存入 db
inline GameArchive gameArchive {};

        //- 若此数据 会被 多线程读写，则需要加锁...


}//---------------------- namespace: esrc -------------------------//
#endif


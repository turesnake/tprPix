/*
 * ====================== GameArchive.h =======================
 *                          -- tpr --
 *                                        CREATE -- 2019.04.29
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *   游戏存档
 * ----------------------------
 */
#ifndef _TPR_GAME_ARCHIVE_H_
#define _TPR_GAME_ARCHIVE_H_


//-------------------- Engine --------------------//
#include "IntVec.h"
#include "ID_Manager.h" 
#include "gameArchiveId.h"
#include "GameObjType.h"


//-- 与 数据库 table_gameArchive 的结构 一致
class GameArchive{
public:

    gameArchiveId_t  id       {}; //- u32 存档id.目前版本中，只能是 1,2,3 
    u32_t            baseSeed {};

    //- player.go -
    goid_t         playerGoId     {}; //- u64   
    IntVec2        playerGoMPos   {}; //- int, int

    goid_t         maxGoId        {}; //- u64


    //======== static ========//
    static ID_Manager  id_manager; //- 负责生产 go_id ( 在.cpp文件中初始化 )

private:
};

//============== static ===============//
inline ID_Manager GameArchive::id_manager { ID_TYPE::U32, 1};


#endif 


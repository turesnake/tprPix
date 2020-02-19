/*
 * ====================== GameArchive.h =======================
 *                          -- tpr --
 *                                        CREATE -- 2019.04.29
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#ifndef TPR_GAME_ARCHIVE_H
#define TPR_GAME_ARCHIVE_H

//--- glm - 0.9.9.5 ---
#include "glm_no_warnings.h"

//-------------------- Engine --------------------//
#include "IntVec.h"
#include "ID_Manager.h" 
#include "gameArchiveId.h"
#include "GameObjType.h"

//-- 与 数据库 table_gameArchive 的结构 一致
class GameArchive{
public:

    gameArchiveId_t  id       {}; //- u32 存档id.目前版本中，只能是 1,2,3 
    uint32_t         baseSeed {};

    //- player.go -
    goid_t         playerGoId     {}; //- u64   
    glm::dvec2     playerGoDPos   {}; //- double, double
    //- GameObj -
    goid_t         maxGoId        {}; //- u64


    double         gameTime       {}; //- double


    //======== static ========//
    static ID_Manager  id_manager; //- 负责生产 go_id ( 在.cpp文件中初始化 )

private:
};

//============== static ===============//
inline ID_Manager GameArchive::id_manager { ID_TYPE::U32, 1};


#endif 


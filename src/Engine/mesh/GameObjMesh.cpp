/*
 * ========================= GameObjMesh.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2018.11.24
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *   GameObjMesh 类, 可以看作 图元类。
 *   一个 GameObjMesh，负责管理 一张 矩形图元
 * ----------------------------
 */
#include "GameObjMesh.h" 

//-------------------- Engine --------------------//
#include "srcs_engine.h"


/* ===========================================================
 *                 bind_animFrameSet
 * -----------------------------------------------------------
 */
void GameObjMesh::bind_animFrameSet( const std::string &_name ){
    animFrameSetName = _name;
    animFrameSetPtr  = &(esrc::animFrameSets.at(_name));
}


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
 * -- 目前版本中，此函数 在 go.creat_new_goMesh() 中就被调用了
 *    所以，其执行时间，要早于 this->init()
 */
void GameObjMesh::bind_animFrameSet( const std::string &_name ){
    this->animFrameSetName = _name;
    this->animFrameSetPtr  = &(esrc::animFrameSets.at(_name));
    this->animFrameIdxHandle.init( (GameObjMesh*)this );
    this->isHaveShadow = this->animFrameSetPtr->isHaveShadow;
}


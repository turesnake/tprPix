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

//-------------------- C --------------------//
#include <cassert>

//-------------------- Engine --------------------//
#include "srcs_engine.h"

//#include "debug.h"


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

/* ===========================================================
 *                  RenderUpdate
 * -----------------------------------------------------------
 * -- 针对本实例包含的 pic/shadow mesh, 执行必要的 update
 * -- 然后把它们 压入 对应的 renderpool 中
 */
void GameObjMesh::RenderUpdate(){

    if( this->isVisible == false ){
        return;
    }

    this->animFrameIdxHandle.update();
    //---------------//
    //      pic
    //---------------//
    this->picMesh.refresh_translate();
    this->picMesh.refresh_scale_auto();
    switch (this->picRenderLayerType){
        case RenderLayerType::MajorGoes:
            esrc::renderPool_goMeshs_pic.insert({ this->picMesh.get_render_z(), this->picMesh.get_ChildMeshPtr() });
            break;
        case RenderLayerType::MapSurfaces:
            esrc::renderPool_mapSurfaces.push_back( this->picMesh.get_ChildMeshPtr() );
            break;
        default:
            assert(0);
    }

    //---------------//
    //   shadow
    //---------------//
    if( this->isHaveShadow ){
        this->shadowMesh.refresh_translate();
        this->shadowMesh.refresh_scale_auto();
        esrc::renderPool_goMeshs_shadow.push_back( this->shadowMesh.get_ChildMeshPtr() );
    }
}


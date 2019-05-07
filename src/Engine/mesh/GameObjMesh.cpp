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

//-------------------- CPP --------------------//
#include <functional>

//-------------------- Engine --------------------//
#include "esrc_animFrameSet.h"
#include "esrc_renderPool.h"

using namespace std::placeholders;

//#include "debug.h"


/* ===========================================================
 *                 bind_animAction
 * -----------------------------------------------------------
 */
void GameObjMesh::bind_animAction(   const std::string &_animFrameSetName,
                        const std::string &_actionName  ){

    this->animActionPtr = esrc::getnc_animActionPtr( _animFrameSetName, _actionName );
    this->animActionPtr->reset_pvtData( this->animActionPvtData );

    this->isHaveShadow = this->animActionPtr->get_isHaveShadow();
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

    this->animActionPtr->update( this->animActionPvtData );

    //---------------//
    //      pic
    //---------------//
    this->picMesh.refresh_translate();
    this->picMesh.refresh_scale_auto();
    switch (this->picRenderLayerType){
        case RenderLayerType::MajorGoes:
            esrc::renderPool_goMeshs_pic.insert({   this->picMesh.get_render_z() + this->off_z , 
                                                    this->picMesh.getnc_ChildMeshPtr() });
            break;
        case RenderLayerType::MapSurfaces:
            esrc::renderPool_mapSurfaces.push_back( this->picMesh.getnc_ChildMeshPtr() );
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
        esrc::renderPool_goMeshs_shadow.push_back( this->shadowMesh.getnc_ChildMeshPtr() );
    }
}


/*
 * ========================= GameObjMesh.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2018.11.24
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#include "GameObjMesh.h" 

//-------------------- CPP --------------------//
#include <functional>

//-------------------- Engine --------------------//
#include "tprAssert.h"
#include "esrc_animFrameSet.h"
#include "esrc_renderPool.h"

using namespace std::placeholders;

//#include "tprDebug.h"


/* ===========================================================
 *                 bind_animAction
 * -----------------------------------------------------------
 * -- 切换动作时的 核心函数
 */
void GameObjMesh::bind_animAction(  const std::string &animFrameSetName_,
                                    const std::string &actionName_  ){

    this->animActionPtr = esrc::getnc_animActionPtr( animFrameSetName_, actionName_ );
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

    //---------------//
    //  animAction
    //---------------//
    this->animActionPtr->update( this->animActionPvtData );

    //---------------//
    //      pic
    //---------------//
    if( this->isVisible == false ){
        return;
    }

    this->picMesh.refresh_translate();
    this->picMesh.refresh_scale_auto();

            //  需要一次判断，是否为 不透明／半透明
            //  未实现...
            //  目前假设，所有 pic 图元都是 不透明的

    esrc::insert_2_renderPool_goMeshs_opaque(   this->picMesh.get_render_z(), 
                                                this->picMesh.getnc_ChildMeshPtr() );

    //---------------//
    //   shadow
    //---------------//
    if( this->isHaveShadow ){
        this->shadowMesh.refresh_translate();
        this->shadowMesh.refresh_scale_auto();
        // shadow 一律是 半透明的 
        esrc::insert_2_renderPool_goMeshs_translucent(  this->shadowMesh.get_render_z(), 
                                                        this->shadowMesh.getnc_ChildMeshPtr() );
    }
}


/* ===========================================================
 *           playerGoIndication_RenderUpdateImm
 * -----------------------------------------------------------
 * -- 暂未被使用 ......
 */
void GameObjMesh::playerGoIndication_RenderUpdateImm(){


    //---------------//
    //      pic
    //---------------//
    if( this->isVisible == false ){
        return;
    }

    //---------------//
    //   只有可以代表 playerGo 的图元，才会被渲染
    //---------------//
    //... 未实现...

    //this->picMesh.refresh_translate();
    //this->picMesh.refresh_scale_auto();

    tprAssert( this->picRenderLayerType == RenderLayerType::MajorGoes );

    this->picMesh.playerGoIndication_draw();

}








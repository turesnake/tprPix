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
void GameObjMesh::bind_animAction(  animSubspeciesId_t subspeciesId_,
                                    NineDirection      dir_,
                                    const std::string &actionName_  ){

    this->animActionPtr = esrc::get_animActionPtr( subspeciesId_, dir_, actionName_ );
    this->animActionPtr->reset_pvtData( this->animActionPvtData );

    this->isHaveShadow = this->animActionPtr->get_isHaveShadow();

    //-- childMeshes --//
    // -- 需要但尚未存在的 chileMesh 会被及时生成
    // -- 不需要但已经存在的 chileMesh 会被及时销毁
    if( this->picMeshUPtr == nullptr ){
        this->picMeshUPtr = std::make_unique<ChildMesh>(true, *this);
    }
    if( this->isHaveShadow ){
        if( this->shadowMeshUPtr == nullptr ){
            this->shadowMeshUPtr = std::make_unique<ChildMesh>(false, *this);
        }
    }else{
        if( this->shadowMeshUPtr != nullptr ){
            this->shadowMeshUPtr = nullptr;
        }
    }
}

/* ===========================================================
 *                RenderUpdate_auto
 * -----------------------------------------------------------
 * -- 针对本实例包含的 pic/shadow mesh, 执行必要的 update
 * -- 然后把它们 压入 对应的 renderpool 中
 */
void GameObjMesh::RenderUpdate_auto(){

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

    this->picMeshUPtr->refresh_scale_auto();
    this->picMeshUPtr->refresh_translate();

    //  不透明／半透明    
    if( this->animActionPtr->get_isOpaque() ){
        esrc::insert_2_renderPool_goMeshs_opaque(   this->picMeshUPtr->get_render_z(), 
                                                    this->picMeshUPtr->getnc_ChildMeshPtr() );
    }else{
        esrc::insert_2_renderPool_goMeshs_translucent(  this->picMeshUPtr->get_render_z(), 
                                                        this->picMeshUPtr->getnc_ChildMeshPtr() );
    }

    //---------------//
    //   shadow
    //---------------//
    if( this->isHaveShadow ){
        this->shadowMeshUPtr->refresh_scale_auto();
        this->shadowMeshUPtr->refresh_translate();
        // shadow 一律是 半透明的 
        esrc::insert_2_renderPool_goMeshs_translucent(  this->shadowMeshUPtr->get_render_z(), 
                                                        this->shadowMeshUPtr->getnc_ChildMeshPtr() );
    }
}









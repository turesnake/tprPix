/*
 * ========================= UIMesh.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.05.04
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#include "UIMesh.h" 

//-------------------- CPP --------------------//
#include <functional>

//-------------------- Engine --------------------//
#include "esrc_animFrameSet.h"
#include "esrc_renderPool.h"

using namespace std::placeholders;

//#include "tprDebug.h"


/* ===========================================================
 *                 bind_animAction
 * -----------------------------------------------------------
 */
void UIMesh::bind_animAction(   const std::string &animFrameSetName_,
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
void UIMesh::RenderUpdate(){

    
    //---------------//
    //   animAction
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

    esrc::insert_2_renderPool_uiMeshs_pic(  this->picMesh.get_render_z() + this->off_z, 
                                            this->picMesh.getnc_UIChildMeshPtr() );

    //---------------//
    //   shadow
    //---------------//
    // 在未来实现 shadow
    /*
    if( this->isHaveShadow ){
        this->shadowMesh.refresh_translate();
        this->shadowMesh.refresh_scale_auto();
        esrc::renderPool_goMeshs_shadow.push_back( this->shadowMesh.getnc_UIChildMeshPtr() );
    }
    */
}


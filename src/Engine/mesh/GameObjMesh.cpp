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
#include "GameObj.h"
#include "esrc_animFrameSet.h"
#include "esrc_renderPool.h"
#include "esrc_camera.h"

using namespace std::placeholders;

//#include "tprDebug.h"


/* 切换动作时的 核心函数 [-old-]
 *
 *        这个函数应该被 停止使用 ...
 * 
 */
/*
void GameObjMesh::bind_animAction(  animSubspeciesId_t    subspeciesId_,
                                    NineDirection      dir_,
                                    BrokenLvl          brokenLvl_,
                                    const std::string &actionName_,
                                    int                timeStepOff_ ){
    
    // 依赖 参数
    this->animActionPtr = esrc::get_animActionPtr( subspeciesId_, dir_, brokenLvl_, actionName_ );
    this->animActionPtr->reset_pvtData( this->animActionPvtData );
    this->animActionPvtData.timeStepOff = timeStepOff_;

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
*/


/* 切换动作时的 核心函数 [-new-]
 */
void GameObjMesh::bind_animAction( int timeStepOff_ ){

    // 依赖自身存储的数据 
    this->animActionPtr = esrc::get_animActionPtr( 
                                this->animSubspeciesId,
                                this->goRef.actionDirection.get_newVal(),
                                this->goRef.brokenLvl.get_newVal(),
                                this->animActionEName );

    this->animActionPtr->reset_pvtData( this->animActionPvtData );
    this->animActionPvtData.timeStepOff = timeStepOff_;

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
    //  callBack (only in PlayType::Once )
    //---------------//
    // 尚未实现 

    //---------------//
    //  camera renderScope
    //---------------//
    glm::dvec2 goMeshDPos = this->goRef.get_dpos() + this->pposOff;
    if( !esrc::get_camera().is_in_renderScope(goMeshDPos) ){
        return;
    }

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
        esrc::get_renderPool(RenderPoolType::Opaque).insert( 
                this->picMeshUPtr->get_render_z(), 
                this->picMeshUPtr->getnc_ChildMeshPtr() );
    }else{
        esrc::get_renderPool(RenderPoolType::Translucent).insert( 
                this->picMeshUPtr->get_render_z(), 
                this->picMeshUPtr->getnc_ChildMeshPtr() );
    }

    //---------------//
    //   shadow
    //---------------//
    if( this->isHaveShadow ){
        this->shadowMeshUPtr->refresh_scale_auto();
        this->shadowMeshUPtr->refresh_translate();
        //---
        esrc::get_renderPool(RenderPoolType::Shadow).insert( 
                this->shadowMeshUPtr->get_render_z(), 
                this->shadowMeshUPtr->getnc_ChildMeshPtr() );
    }
}


/* ===========================================================
 *                RenderUpdate_ground
 * -----------------------------------------------------------
 * GroundGo 专用
 */
void GameObjMesh::RenderUpdate_ground(){

    tprAssert( (!this->isHaveShadow) && (this->isVisible) );
    tprAssert( this->colorTableId != NilColorTableId ); // tmp
    //---------------//
    //      pic
    //---------------//
    this->picMeshUPtr->refresh_scale_auto();
    this->picMeshUPtr->refresh_translate(); 
    //-- 暂不区分，是否半透明 --
    esrc::get_groundRenderPool().insert( 
                this->colorTableId, // 暂未检测其是否被设置 ...
                this->picMeshUPtr->get_render_z(), 
                this->picMeshUPtr->getnc_ChildMeshPtr() );
}




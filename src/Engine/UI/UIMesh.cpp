/*
 * ========================= UIMesh.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.05.04
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#include "UIMesh.h" 

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
 *                 bind_animFrameSet
 * -----------------------------------------------------------
 * -- 目前版本中，此函数 在 ui.creat_new_uiMesh() 中就被调用了
 *    所以，其执行时间，要早于 this->init()
 */
void UIMesh::bind_animFrameSet( const std::string &_name ){
    this->animFrameSetName = _name;
    this->animFrameSetPtr  = &(esrc::animFrameSets.at(_name));
    this->animFrameIdxHandle.bind_get_animFrameSet_currentTimeStep_func(
        std::bind( &UIMesh::get_animFrameSet_currentTimeStep, this, _1 ) );
    this->isHaveShadow = this->animFrameSetPtr->isHaveShadow;
}

/* ===========================================================
 *                  RenderUpdate
 * -----------------------------------------------------------
 * -- 针对本实例包含的 pic/shadow mesh, 执行必要的 update
 * -- 然后把它们 压入 对应的 renderpool 中
 */
void UIMesh::RenderUpdate(){

    if( this->isVisible == false ){
        return;
    }

    this->animFrameIdxHandle.update();
    //---------------//
    //      pic
    //---------------//
    this->picMesh.refresh_translate();
    this->picMesh.refresh_scale_auto();

    esrc::renderPool_uiMeshs_pic.insert({   this->picMesh.get_render_z() + this->off_z, 
                                            this->picMesh.getnc_UIChildMeshPtr() });

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


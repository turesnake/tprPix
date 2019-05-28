/*
 * ====================== UIObj.cpp ==========================
 *                          -- tpr --
 *                                        创建 -- 2019.05.04
 *                                        修改 -- 
 * ----------------------------------------------------------
 */
#include "UIObj.h"

#include <cmath>


/* ===========================================================
 *                         init
 * -----------------------------------------------------------
 */
void UIObj::init(){


    //this->actionSwitch.init( static_cast<GameObj*>(this) );
    //...
}


/* ===========================================================
 *                     creat_new_uiMesh
 * -----------------------------------------------------------
 * -- 通过一组参数来实现 gomesh 的初始化。
 * -- 在这个函数结束hou，仅剩下一件事要做： gomesh.animFrameIdxHandle.bind_xxx()
 */
UIMesh &UIObj::creat_new_uiMesh(   //const std::string &_animFrameSetName,
                                ShaderProgram     *_pixShaderPtr,
                                ShaderProgram     *_shadowShaderPtr,
                                float             _off_z,
                                bool              _isVisible,
                                bool              _isFlipOver ){

    //----- init -----//
    //this->uiMesh.bind_animFrameSet( _animFrameSetName );
    this->uiMesh.init( const_cast<UIObj*>(this) );
    this->uiMesh.set_pic_shader_program( _pixShaderPtr );
    this->uiMesh.set_shadow_shader_program( _shadowShaderPtr );

    //-- goMesh pos in go --
    this->uiMesh.set_off_z(_off_z);

    //-- flags --//
    this->uiMesh.isVisible = _isVisible;
    this->uiMesh.isFlipOver = _isFlipOver;

    //----
    return this->uiMesh;
}



/* ===========================================================
 *                   renderUpdate
 * -----------------------------------------------------------
 * -- 暂时不向 具象UIObj 类 提供 自定义的 renderUpdate
 */
void UIObj::renderUpdate(){

    this->renderUpdate_move();
    this->uiMesh.RenderUpdate();
}



/* ===========================================================
 *                   renderUpdate_move
 * -----------------------------------------------------------
 */
void UIObj::renderUpdate_move(){

    if( this->isMoving == false ){
        return;
    }

    float criticalVal = 1.0;  //- 临界值
    float  approachPercent = 0.28; //- go运动的 “接近比率”
                                   // 在未来，会根据 move.speedLvl 来修改
    glm::vec2 off = this->targetFPos - this->currentFPos;
    //-- 若非常接近，直接同步 --
    if( (std::abs(off.x)<=criticalVal) && (std::abs(off.y)<=criticalVal) ){
        this->currentFPos = this->targetFPos;
        this->isMoving = false;
    }else{
        this->currentFPos += off * approachPercent;
        //- 并不设置 z 轴值
    }
}


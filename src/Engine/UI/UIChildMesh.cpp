/*
 * ===================== UIChildMesh.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.05.04
 *                                        MODIFY --
 * ----------------------------------------------------------
 */
#include "UIChildMesh.h"

//-------------------- C --------------------//
//#include <cassert> //-- asserts

//-------------------- Engine --------------------//
#include "tprAssert.h"
#include "UIObj.h"
#include "UIMesh.h"
#include "VAOVBO.h" 
#include "vector_matrix.h"
#include "esrc_camera.h"


namespace{//------------------ namespace ---------------------//

    //-- 3个常量，分别代表 xyz 三个轴，用在 glm::rotate 函数中。
    //const glm::vec3 axle_x( 1.0f, 0.0f, 0.0f );
    //const glm::vec3 axle_y( 0.0f, 1.0f, 0.0f );
    const glm::vec3 axis_z( 0.0f, 0.0f, 1.0f );

}//--------------------- namespace end ------------------------//


/* ===========================================================
 *                refresh_scale_auto
 * -----------------------------------------------------------
 * 大部分 具象go实例 的 GameObjMesh图元 长宽值 与 AnimFrameSet 数据 强关联 --
 * 目前每个mesh，每帧都被调用，计算量不大。
 */
void UIChildMesh::refresh_scale_auto(){
    const IntVec2 &p = this->uiMeshPtr->get_animAction_pixNum_per_frame();

    this->scale_val.x = (float)p.x;
    this->scale_val.y = (float)p.y;
    this->scale_val.z = 1.0f;

    //---- 亦或时 才左右翻转 ----//
    //  暂不实现 左右翻转...
    /*
    if( this->uiGoPtr->isFlipOver != this->uiMeshPtr->isFlipOver ){
        this->scale_val.x *= -1.0f;
    }
    */
}


/* ===========================================================
 *                    refresh_translate
 * -----------------------------------------------------------
 * -- 此函数 只能在 go.RenderUpdate 阶段被调用. 其余代码 不应随意调用 此函数!!!
 * -- *** 只适用于 picMesh ***
 */
void UIChildMesh::refresh_translate(){

    const glm::vec2 &goCurrentFPos = this->uiGoPtr->get_currentFPos();
    //- 图元帧 左下角 到 rootAnchor 的 off偏移 --
    const IntVec2 &vRef = this->uiMeshPtr->get_currentRootAnchorPPosOff();


    //--- set translate_val ---//
    this->translate_val.x = goCurrentFPos.x - (float)vRef.x;
    //---- 亦或时 才左右翻转 ----//
    //  暂不实现 左右翻转...
    /*
    if( this->uiGoPtr->isFlipOver != this->uiMeshPtr->isFlipOver ){
        this->translate_val.x += this->uiMeshPtr->get_animFrameSet_pixNum_per_frame().x;
    }
    */

    if( this->isPic == true ){
        this->translate_val.y = goCurrentFPos.y - (float)vRef.y;
                                    
        this->translate_val.z = esrc::get_camera().get_zFar() + ViewingBox::get_renderLayerZOff(RenderLayerType::UIs) +
                                this->uiMeshPtr->get_off_z();
        
    }else{
        this->translate_val.y = goCurrentFPos.y - (float)vRef.y;  
        this->translate_val.z = esrc::get_camera().get_zFar() + ViewingBox::goShadows_zOff;         
    }
}


/* ===========================================================
 *                         draw     [搬运后，未修改]
 * -----------------------------------------------------------
 */
void UIChildMesh::draw(){

    if( this->uiMeshPtr->isVisible == false ){
        return;
    }

    //---------- refresh texName -------------
    GLuint texName;
    if( this->isPic ){
        texName=this->uiMeshPtr->get_currentTexName_pic();
    }else{
        //--- 若没有 shadow，直接跳过本次 draw-call --- IMPORTANT !!!!!
        if( this->uiMeshPtr->isHaveShadow == false ){
            return;
        }
        //------
        texName=this->uiMeshPtr->get_currentTexName_shadow();
    }

    //---------- refresh mat4_model -------------
    update_mat4_model();

    //---------- 将 model矩阵的值传入 绑定的 着色器程序 ---------
    tprAssert( this->shaderPtr != nullptr );
    this->shaderPtr->send_mat4_model_2_shader( this->mat4_model );

    //----------- 绑定 本UIMesh对象 唯一的 texture ------------   
    //-- 单次 draw call 最多支持 32 个 texture。（完全够用）
    //   但是， gl本体可以存储 非常多个 tex实例
    glActiveTexture( GL_TEXTURE0 );  //- 激活纹理单元
    tprAssert( texName != 0 ); 
    glBindTexture(GL_TEXTURE_2D, texName ); //- 绑定纹理单元

    //----------- 绑定 本Model对象 的 VAO ----------
    glBindVertexArray( VAO );

    //----------- 正式绘制 ----------
    glDrawArrays( GL_TRIANGLES, 0, 6 ); //-- 绘制 图元(rect). 不依赖 EBO 的写法
}


/* ===========================================================
 *                 update_mat4_model      [搬运后，未修改]
 * -----------------------------------------------------------
 * -- 当 UIMesh对象的 位移／旋转／缩放 任一维发生变化，就要重新计算 model矩阵
 * -- 目前 优化粒度较粗。一有变化就全盘重算。
 */
void UIChildMesh::update_mat4_model(){

    //----- translate: regular ------
    // 请确保，输入函数的 translate 值，已经叠加了 go 的 pos。
    this->mat4_model = glm::translate( normal_mat4, this->translate_val );

    //----- rotate: only Z-axis ------
    //- pix游戏 只支持 z轴旋转
    this->mat4_model = glm::rotate( this->mat4_model, 
                            glm::radians(this->rotate_z),
                            axis_z );

    //----- scale ------
    this->mat4_model = glm::scale( this->mat4_model, this->scale_val );
}




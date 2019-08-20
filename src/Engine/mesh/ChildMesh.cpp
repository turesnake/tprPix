/*
 * ========================= ChildMesh.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.01.22
 *                                        MODIFY --
 * ----------------------------------------------------------
 */
#include "ChildMesh.h"

//-------------------- Engine --------------------//
#include "tprAssert.h"
#include "GameObj.h"
#include "GameObjMesh.h"
#include "VAOVBO.h" 
#include "esrc_camera.h"
#include "esrc_shader.h"


namespace childMesh_inn {//------------------ namespace: childMesh_inn ---------------------//

    //-- 3个常量，分别代表 xyz 三个轴，用在 glm::rotate 函数中。
    //const glm::vec3 axle_x( 1.0f, 0.0f, 0.0f );
    //const glm::vec3 axle_y( 0.0f, 1.0f, 0.0f );
    const glm::vec3 axis_z( 0.0f, 0.0f, 1.0f );

}//--------------------- namespace: childMesh_inn end ------------------------//


/* ===========================================================
 *                refresh_scale_auto
 * -----------------------------------------------------------
 * 大部分 具象go实例 的 GameObjMesh图元 长宽值 与 AnimFrameSet 数据 强关联 --
 * 目前每个mesh，每帧都被调用，计算量不大。
 */
void ChildMesh::refresh_scale_auto(){
    const IntVec2 &p = this->goMeshRef.get_animAction_pixNum_per_frame();

    this->scale_val.x = (float)p.x;
    this->scale_val.y = (float)p.y;
    this->scale_val.z = 1.0f;

    //---- 亦或时 才左右翻转 ----//
    /*
    const auto &goRef = this->goMeshRef.get_goCRef();
    if( goRef.isFlipOver != this->goMeshRef.isFlipOver ){
        this->scale_val.x *= -1.0f;
    }
    */
}


/* ===========================================================
 *                  refresh_translate
 * -----------------------------------------------------------
 * -- 此函数 只能在 go.RenderUpdate 阶段被调用. 其余代码 不应随意调用 此函数!!!
 * -- *** 只适用于 picMesh ***
 */
void ChildMesh::refresh_translate(){

    const auto &goRef = this->goMeshRef.get_goCRef();

    const glm::vec2 &goCurrentFPos = glm_dvec2_2_vec2( goRef.goPos.get_currentDPos() );

    //- 图元帧 左下角 到 rootAnchor 的 off偏移 --
    const IntVec2 &vRef = this->goMeshRef.get_currentRootAnchorPPosOff();
    const glm::vec2 &pposOff = this->goMeshRef.get_pposOff();

    //--- set translate_val ---//
    this->translate_val.x = goCurrentFPos.x + (float)pposOff.x - (float)vRef.x;
    //---- 亦或时 才左右翻转 ----//
    /*
    if( goRef.isFlipOver != this->goMeshRef.isFlipOver ){
        this->translate_val.x += this->goMeshRef.get_animAction_pixNum_per_frame().x;
    }
    */

    if( this->isPic == true ){
        this->translate_val.y = goCurrentFPos.y + 
                                static_cast<float>(pposOff.y) - 
                                static_cast<float>(vRef.y) + 
                                static_cast<float>(goRef.goPos.get_alti());
                                    //-- 累加 高度alti
        if( goMeshRef.isPicFixedZOff ){
            this->translate_val.z = static_cast<float>(esrc::get_camera().get_zFar()) + 
                                    goMeshRef.get_picFixedZOff();
        }else{
            this->translate_val.z = -(goCurrentFPos.y + (float)pposOff.y  + this->goMeshRef.get_off_z());
                                        //-- ** 注意！**  z值的计算有不同：
                                        // -1- 取负， 摄像机朝向 z轴 负方向
                                        // -2- 没有算入 vRef.y; 因为这个值只代表：
                                        //     图元 和 根锚点的 偏移
                                        //     而 z值 仅仅记录 GameObjMesh锚点 在 游戏世界中的位置
        }
        
    }else{
        this->translate_val.y = goCurrentFPos.y - static_cast<float>(vRef.y);
                                    //-- shadow 的 y值 并不随着 pposOff 而变化。
                                    //   这样才能实现： go跳起来腾空个了。而阴影没有跟着也“抬高”
        this->translate_val.z = static_cast<float>(esrc::get_camera().get_zFar() + ViewingBox::goShadows_zOff);
                                    //-- 对于 shadow 来说，z值 是跟随 camera 而变化的
                                    //   而且始终 “相对 camera.viewingBox 静止”
    }
    
            //-- 未来拓展：
            //  应当为 每个go 设置一个随机的 z深度 base值
            //  在此 z_base 基础上，再做 深度加减
            //  从而避免同一 z深度的 图元 在渲染时 碰撞
}




/* ===========================================================
 *                  draw
 * -----------------------------------------------------------
 */
void ChildMesh::draw(){

    if( this->goMeshRef.isVisible == false ){
        return;
    }

    //---------- refresh texName -------------
    GLuint texName {};
    if( this->isPic ){
        texName=this->goMeshRef.get_currentTexName_pic();
    }else{
        //--- 若没有 shadow，直接跳过本次 draw-call --- IMPORTANT !!!!!
        if( this->goMeshRef.isHaveShadow == false ){
            return;
        }
        //------
        texName=this->goMeshRef.get_currentTexName_shadow();
    }

    //---------- refresh mat4_model -------------
    update_mat4_model();

    //---------- 将 model矩阵的值传入 某个 着色器程序 ---------
    //esrc::get_rect_shader().send_mat4_model_2_shader( this->mat4_model );
    tprAssert( this->shaderPtr != nullptr );
    this->shaderPtr->send_mat4_model_2_shader( this->mat4_model );

    //----------- 绑定 本GameObjMesh对象 唯一的 texture ------------   
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
 *                playerGoIndication_draw
 * -----------------------------------------------------------
 */
void ChildMesh::playerGoIndication_draw(){

    if( this->goMeshRef.isVisible == false ){
        return;
    }

    //---------- refresh texName -------------
    tprAssert( this->isPic );

    GLuint texName = this->goMeshRef.get_currentTexName_pic();

    //---------- refresh mat4_model -------------
    // 重新指定 z值：playerGoIndication_zOff
    //this->translate_val.z = static_cast<float>(esrc::get_camera().get_zFar() + ViewingBox::playerGoIndication_zOff );   
    this->translate_val.z += 1.0;
    update_mat4_model();

    //---------- 将 model矩阵的值传入 某个 着色器程序 ---------
    esrc::get_playerGoIndication_shader().send_mat4_model_2_shader( this->mat4_model );
                    // tmp ...

    //----------- 绑定 本GameObjMesh对象 唯一的 texture ------------   
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
 *                 update_mat4_model
 * -----------------------------------------------------------
 * -- 当 GameObjMesh对象的 位移／旋转／缩放 任一维发生变化，就要重新计算 model矩阵
 * -- 目前 并未实现任何优化
 */
void ChildMesh::update_mat4_model(){

    //----- translate: regular ------
    // 请确保，输入函数的 translate 值，已经叠加了 go 的 pos。
    this->mat4_model = glm::translate( glm::mat4(1.0), this->translate_val );

    //----- rotate: only Z-axis ------
    //- pix游戏 只支持 z轴旋转
    this->mat4_model = glm::rotate( this->mat4_model, 
                            glm::radians(this->rotate_z),
                            childMesh_inn::axis_z );

    //----- scale ------
    this->mat4_model = glm::scale( this->mat4_model, this->scale_val );

    //----- translate: anchor／锚点 修正 ------
    //...
}




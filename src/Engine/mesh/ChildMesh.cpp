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
#include "esrc_VAOVBO.h" 
#include "esrc_camera.h"
#include "esrc_shader.h"

#include "tprDebug.h"


namespace childMesh_inn {//------------------ namespace: childMesh_inn ---------------------//

    //-- 3个常量，分别代表 xyz 三个轴，用在 glm::rotate 函数中。
    const glm::vec3 axis_x( 1.0f, 0.0f, 0.0f );
    const glm::vec3 axis_y( 0.0f, 1.0f, 0.0f );
    const glm::vec3 axis_z( 0.0f, 0.0f, 1.0f );

}//--------------------- namespace: childMesh_inn end ------------------------//


void ChildMesh::init()noexcept{
    this->VAO = esrc::get_VAO();                // copy and hold
    this->pointNums = esrc::get_pointNums();    // copy and hold 
}


/* ===========================================================
 *                refresh_scale_auto
 * -----------------------------------------------------------
 * 大部分 具象go实例 的 GameObjMesh图元 长宽值 与 AnimFrameSet 数据 强关联 --
 * 目前每个mesh，每帧都被调用，计算量不大。
 */
void ChildMesh::refresh_scale_auto(){
    const IntVec2 p = this->goMeshRef.get_animAction_pixNum_per_frame();

    this->scale_frameSZ.x = static_cast<float>(p.x);
    this->scale_frameSZ.y = static_cast<float>(p.y);

    const auto &scaleRef = this->goMeshRef.rotateScaleData.get_scaleRef();
    this->scale_total.x = this->scale_frameSZ.x * scaleRef.x;
    this->scale_total.y = this->scale_frameSZ.y * scaleRef.y;
}


/* ===========================================================
 *                  refresh_translate
 * -----------------------------------------------------------
 * -- 此函数 只能在 go.RenderUpdate 阶段被调用. 其余代码 不应随意调用 此函数!!!
 */
void ChildMesh::refresh_translate(){

    const auto &goRef = this->goMeshRef.get_goCRef();
    const glm::vec2 &currentFPos = glm_dvec2_2_vec2( goRef.get_dpos() );       
    
    //- 图元帧 左下角 到 rootAnchor 的 偏移
    glm::vec2 vf = glm_dvec2_2_vec2( this->goMeshRef.get_currentRootAnchorDPosOff() );

    const glm::vec2 &pposOff = this->goMeshRef.get_pposOff();

    float goAlti = static_cast<float>(goRef.get_pos_alti());


    //--- set translate_val ---//
    this->translate_val.x = currentFPos.x + pposOff.x - vf.x;

    const float &off_zRef = this->goMeshRef.get_off_z();

    if( this->isPic == true ){
        this->translate_val.y = currentFPos.y + pposOff.y - vf.y + goAlti;
                                    
        if( goMeshRef.isPicFixedZOff ){
            this->translate_val.z = static_cast<float>(esrc::get_camera().get_zFar()) + 
                                    goMeshRef.get_picFixedZOff() + off_zRef;
        }else{
            this->translate_val.z = -(currentFPos.y + pposOff.y) + off_zRef;
                                        //-- ** 注意！**  z值的计算有不同：
                                        // -1- 取负， 摄像机朝向 z轴 负方向
                                        // -2- 没有算入 vf.y; 因为这个值只代表：
                                        //     图元 和 根锚点的 偏移
                                        //     而 z值 仅仅记录 GameObjMesh锚点 在 游戏世界中的位置
        }
    }else{
        this->translate_val.y = currentFPos.y + pposOff.y - vf.y;
                                    //-- 若是类似 mushroom 这种 multiGoMeshs-go，
                                    //   其 阴影应当 添加 pposOff.y
                                    //-- 若是 go身上绑定的 子mesh，其 pposOff.y 不应被累加
                                    //   在未来，这个变化，应当通过一个 flags 来控制
                                    //   并允许外部选配 ...
        this->translate_val.z = static_cast<float>(esrc::get_camera().get_zFar() + ViewingBox::goShadows_zOff) + off_zRef;
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
    tprAssert( this->shaderPtr != nullptr );
    this->shaderPtr->send_mat4_model_2_shader( this->mat4_model );

    //----------- 绑定 本GameObjMesh对象 唯一的 texture ------------   
    //-- 单次 draw call 最多支持 32 个 texture。（完全够用）
    //   但是， gl本体可以存储 非常多个 tex实例
    glActiveTexture( GL_TEXTURE0 );  //- 激活纹理单元
    tprAssert( texName != 0 ); 
    glBindTexture(GL_TEXTURE_2D, texName ); //- 绑定纹理单元

    //----------- 绑定 本Model对象 的 VAO ----------
    glBindVertexArray( this->VAO );

    //----------- 正式绘制 ----------
    glDrawArrays( GL_TRIANGLES, 0, this->pointNums );
}


/* ===========================================================
 *                 update_mat4_model
 * -----------------------------------------------------------
 * -- 当 GameObjMesh对象的 位移／旋转／缩放 任一维发生变化，就要重新计算 model矩阵
 * -- 目前 并未实现任何优化
 */
void ChildMesh::update_mat4_model(){

    auto &rotateScaleRef = this->goMeshRef.rotateScaleData;
    //--------------------------------//
    //       translate: regular
    //--------------------------------//
    
    //- 修正： 图元帧 左下角 -> 中心
    this->translate_val.x += this->scale_frameSZ.x * 0.5f;
    this->translate_val.y += this->scale_frameSZ.y * 0.5f;

    this->mat4_model = glm::translate( glm::mat4(1.0), this->translate_val );
   
    //--------------------------------//
    //            rotate
    //--------------------------------//
    if( rotateScaleRef.isNeedToRotate() ){

        const auto &rotateDegreeRef = rotateScaleRef.get_rotateDegreeRef();
        for( const auto &i : rotateScaleRef.get_rotateOrder() ){
            switch (i){
                case RotateType::X:
                    this->mat4_model = glm::rotate( this->mat4_model, 
                                                    glm::radians(rotateDegreeRef.x),
                                                    childMesh_inn::axis_x );
                    break;
                case RotateType::Y:
                    this->mat4_model = glm::rotate( this->mat4_model, 
                                                    glm::radians(rotateDegreeRef.y),
                                                    childMesh_inn::axis_y );
                    break;
                case RotateType::Z:
                    this->mat4_model = glm::rotate( this->mat4_model, 
                                                    glm::radians(rotateDegreeRef.z),
                                                    childMesh_inn::axis_z );
                    break;
                default:
                    tprAssert(0);
                    break;
            }
        }
    }

    //--------------------------------//
    //             scale
    //--------------------------------//

    this->mat4_model = glm::scale( this->mat4_model, this->scale_total );


    //----- translate: anchor／锚点 修正 ------
    //...
}




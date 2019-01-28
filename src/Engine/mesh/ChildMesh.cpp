/*
 * ========================= ChildMesh.cpp ==========================
 *                          -- tpr --
 *                                        创建 -- 2019.01.22
 *                                        修改 -- 2019.01.22
 * ----------------------------------------------------------
 *  
 * ----------------------------
 */
#include "ChildMesh.h"

//-------------------- C --------------------//
#include <cassert> //-- asserts

//-------------------- Engine --------------------//
#include "GameObj.h"
#include "GameObjMesh.h"
#include "VAOVBO.h" 
#include "vector_matrix.h"
#include "srcs_engine.h"


namespace{//------------------ namespace ---------------------//

    //-- 3个常量，分别代表 xyz 三个轴，用在 glm::rotate 函数中。
    //const glm::vec3 axle_x( 1.0f, 0.0f, 0.0f );
    //const glm::vec3 axle_y( 0.0f, 1.0f, 0.0f );
    const glm::vec3 axis_z( 0.0f, 0.0f, 1.0f );

}//--------------------- namespace end ------------------------//


/* ===========================================================
 *                refresh_scale_auto
 * -----------------------------------------------------------
 * 大部分 具象go实例 的 GameObjMesh图元 长宽值 与 action数据 强关联 --
 *     所以可以直接从 action 中获取数据
 *     这个函数很常用
 *     但如果 action实例 并不更换，也没必要 每1视觉帧 都执行此函数
 */
void ChildMesh::refresh_scale_auto(){
    const IntVec2 &p = goMeshPtr->get_action_pixNum_per_frame();

    scale_val.x = (float)p.x;
    scale_val.y = (float)p.y;
    scale_val.z = 1.0f;

    //---- 亦或时 才左右翻转 ----//
    if( goPtr->isFlipOver != goMeshPtr->isFlipOver ){
        scale_val.x *= -1.0f;
    }
}


/* ===========================================================
 *                  refresh_translate
 * -----------------------------------------------------------
 * -- 此函数 只能在 go.RenderUpdate 阶段被调用. 其余代码 不应随意调用 此函数!!!
 * -- *** 只适用于 picMesh ***
 */
void ChildMesh::refresh_translate(){

    const glm::vec2 &goCurrentFPos = goPtr->goPos.get_currentFPos();
    //- 图元帧 左下角 到 rootAnchor 的 off偏移 --
    const IntVec2 &vRef = goMeshPtr->get_currentRootAnchorPPosOff();
    const glm::vec2 &pposOff = goMeshPtr->pposOff;

    //--- set translate_val ---//
    translate_val.x = goCurrentFPos.x + (float)pposOff.x - (float)vRef.x;
    //---- 亦或时 才左右翻转 ----//
    if( goPtr->isFlipOver != goMeshPtr->isFlipOver ){
        translate_val.x += goMeshPtr->get_action_pixNum_per_frame().x;
    }

    if( isPic == true ){
        translate_val.y = goCurrentFPos.y + (float)pposOff.y - (float)vRef.y;
        translate_val.z = -(goCurrentFPos.y + (float)pposOff.y  + goMeshPtr->off_z);
                                    //-- ** 注意！**  z值的计算有不同：
                                    // -1- 取负...
                                    // -2- 没有算入 vRef.y; 因为这个值只代表：
                                    //     图元 和 根锚点的 偏移
                                    //     而 z值 仅仅记录 GameObjMesh锚点 在 游戏世界中的位置
    }else{
        translate_val.y = goCurrentFPos.y - (float)vRef.y;
                                    //-- shadow 的 y值 并不随着 pposOff 而变化。
                                    //   这样才能实现： go跳起来腾空个了。而阴影没有跟着也“抬高”
        translate_val.z = esrc::camera.get_zFar() + ViewingBox::goShadows_zOff;
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

    if( goMeshPtr->isVisible == false ){
        return;
    }

    //---------- refresh texName -------------
    GLuint texName;
    (isPic) ? texName=goMeshPtr->get_currentTexName_pic() :
              texName=goMeshPtr->get_currentTexName_shadow();


    //---------- refresh mat4_model -------------
    update_mat4_model();

    //---------- 将 model矩阵的值传入 绑定的 着色器程序 ---------
    assert( shaderPtr != nullptr );
    shaderPtr->send_mat4_model_2_shader( mat4_model );

    //----------- 绑定 本GameObjMesh对象 唯一的 texture ------------   
    //-- 单次 draw call 最多支持 32 个 texture。（完全够用）
    //   但是， gl本体可以存储 非常多个 tex实例
    glActiveTexture( GL_TEXTURE0 );  //- 激活纹理单元
    assert( texName != 0 ); 
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
 * -- 目前 优化粒度较粗。一有变化就全盘重算。
 */
void ChildMesh::update_mat4_model(){

    //----- translate: regular ------
    // 请确保，输入函数的 translate 值，已经叠加了 go 的 pos。
    mat4_model = glm::translate( normal_mat4, translate_val );

    //----- rotate: only Z-axis ------
    //- pix游戏 只支持 z轴旋转
    mat4_model = glm::rotate( mat4_model, 
                            glm::radians(rotate_z),
                            axis_z );

    //----- scale ------
    mat4_model = glm::scale( mat4_model, scale_val );

    //----- translate: anchor／锚点 修正 ------
    //...
}




/*
 * ========================= ChildMesh.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.01.22
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#ifndef TPR_CHILD_MESH_H
#define TPR_CHILD_MESH_H

//--- glm - 0.9.9.5 ---
#include "glm_no_warnings.h"

//-------------------- Engine --------------------//
#include "ShaderProgram.h" //-- each GameObjMesh instance,will bind a shader
#include "ViewingBox.h"
#include "functorTypes.h"


//--- need ---//
class GameObjMesh;


//-- pic/shadow 共用一套结构 --//
//   这个 class 应尽可能地 轻量化，让公共数据，放到 GoMesh 中
class ChildMesh{
public:
    ChildMesh( bool isPic_, GameObjMesh &goMeshRef_ ):
        goMeshRef(goMeshRef_),
        isPic(isPic_)
        {
            this->init();
        }

    void init()noexcept;

    void draw();

    inline void set_shader_program( ShaderProgram *sp_ ) noexcept{ this->shaderPtr=sp_; }
    //inline void bind_before_drawCall( F_void f_ ){ this->before_drawCall = f_; }

    //-- 此函数 只能在 RenderUpdate 阶段被调用 --
    //-- 其余代码 不应随意调用 此函数!!! --
    void refresh_translate();

    //- 大部分 具象go实例 的 goMesh图元 长宽值 与 AnimFrameSet数据 强关联 --
    //  所以可以直接从 AnimFrameSet 中获取数据
    //  这个函数很常用
    //  但如果 AnimFrameSet实例 并不更换，也没必要 每1视觉帧 都执行此函数
    void refresh_scale_auto();

    inline ChildMesh *getnc_ChildMeshPtr()noexcept{
        return  const_cast<ChildMesh*>(this);
    }
    //- 通过 translate_val.z 值 来给 待渲染的 meshs 排序 --
    inline float get_render_z()const noexcept{ return this->translate_val.z; }

    //-- 外部 debug 用
    inline const glm::vec3 &get_translate_val()const noexcept{ return this->translate_val; }
    
private:
    void update_mat4_model(); //-- 重新计算 model矩阵

    //======== vals ========//
    GameObjMesh    &goMeshRef;
    ShaderProgram  *shaderPtr  {nullptr}; 
    GLuint          VAO  {};        // simple copy from esrc_VAOVBO
    GLsizei         pointNums {};   // glDrawArrays() param
    //+++++++++ 与 图元 矩阵计算 有关的 变量 ++++++++++++
    glm::mat4 mat4_model = glm::mat4(1.0); //-- 每个 物体obj 都私有一个 模型矩阵
                          //-- 自动初始化为 标准矩阵

    //-- 位移／旋转／缩放 变化向量。
    glm::vec3 translate_val  {};    
    glm::vec2 scale_frameSZ    {glm::vec2(1.0f, 1.0f)};       //- 仅仅表达 图元帧 的缩放比例
    glm::vec3 scale_total      {glm::vec3(1.0f, 1.0f, 1.0f)}; //- 最终传入 mat4 的值  

    //----- functor -----//
    // not used yet ...
    //F_void before_drawCall {nullptr}; // call back. e.g. do some shader uniform bind

    //======== flags ========//  
    bool    isPic {true}; //-- pic / shadow
};


#endif 


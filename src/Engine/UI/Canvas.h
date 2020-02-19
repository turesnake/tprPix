/*
 * ========================= Canvas.h ==========================
 *                          -- tpr --
 *                                        创建 -- 2019.03.31
 *                                        修改 -- 
 * ----------------------------------------------------------
 *  canvas 是一个 大型图元，其尺寸和位置 始终与 window 对齐。
 *  且不受 camera 影响。
 *  通过控制其唯一的 texture，来实现复杂的 动画效果
 * ----------------------------
 */
#ifndef TPR_CANVAS_H
#define TPR_CANVAS_H
/* -- 确保 glad GLFW 两个库 的引用顺序 ---
 * --    glad.h 包含了正确的OpenGL头文件（如GL/gl.h），
 * --    所以需要在其它依赖于OpenGL的头文件之前 包含 glad.h
 */
#include<glad/glad.h>  

//-------------------- CPP --------------------//
#include <vector>
#include <string>
#include <unordered_map>

//-------------------- Engine --------------------//
#include "tprAssert.h"
#include "IntVec.h"
#include "ShaderProgram.h"
#include "Mesh.h"


class Canvas{
public:
    Canvas() = default;

    void init(  IntVec2 *texSizePtr_, ShaderProgram *shaderPtr_ );

    void draw();

    inline void use_shaderProgram()noexcept{
        this->is_binded = true;
        this->shaderPtr->use_program();
    }
    inline void set_translate( float x_, float y_, float z_ )noexcept{
        tprAssert( this->is_binded );
        this->mesh.set_translate( glm::vec3{ x_, y_, z_ } );
    }
    inline GLint get_uniform_location( const std::string &name_ )noexcept{
        tprAssert( this->is_binded );
        return this->shaderPtr->get_uniform_location( name_ );
    }

private:
    //===== vals =====//
    IntVec2       *texSizePtr {}; //- = ViewingBox::screenSZ,
    Mesh           mesh {};
    ShaderProgram *shaderPtr {nullptr};

    //===== flags =====//
    bool  is_binded {false};    //- 统一 绑定／释放, 看起来有点笨拙
};


#endif 



//=======================================================//
//                   glCreateShader
//=======================================================//

#include "_gl_self.h"  //-- 抄写了 gl 的各种 类型定义


//-- creates an empty shader object 
//-- and returns a non-zero value by which it can be referenced
GLuint glCreateShader(	GLenum shaderType);

    //-- param: shaderType. -- 指定 要被创建的 shader 的类型：
        //-- GL_VERTEX_SHADER           -- 顶点着色器
        //-- GL_GEOMETRY_SHADER         -- 几何着色器
        //-- GL_FRAGMENT_SHADER         -- 片段着色器

        //-- GL_COMPUTE_SHADER *
                //-- run on the programmable compute processor
                //-- 只由 GL 4.3 或 更高版本 才提供。

        //-- GL_TESS_CONTROL_SHADER    
                //-- run on the programmable tessellation processor in the control stage

        //-- GL_TESS_EVALUATION_SHADER 
                //-- run on the programmable tessellation processor in the evaluation stage

    //-- 创建成功后，新的 shader obj 的 GL_SHADER_TYPE 也将被设置为 
    //-- 参数 shaderType 的值。

    //-- return:
    //-- 若成功，返回一个 非0值，referenced to the shader obj
    //-- 若出错，返回 0.
    //     若 参数shaderType 不为可接受值，将生成 error：GL_INVALID_ENUM







/*
 * ========================= Texture.h ==========================
 *                          -- tpr --
 *                                        创建 -- 2018.11.24
 *                                        修改 -- 2018.11.24
 * ----------------------------------------------------------
 *   png --> texture
 *   一个临时性的，png图片文件加载，texture 生成模块
 *   后期会被拆分 
 * 
 * ------------
 *   单纯地加载一张 png图片（非动画帧集）
 *   制作成一份 textel 实例
 * 
 * ----------------------------
 */

#ifndef _GLCPP_TEXTURE_H_
#define _GLCPP_TEXTURE_H_

/* -- 确保 glad GLFW 两个库 的引用顺序 ---
 * --    glad.h 包含了正确的OpenGL头文件（如GL/gl.h），
 * --    所以需要在其它依赖于OpenGL的头文件之前 包含 glad.h
 */
#include <glad/glad.h> 

//-------------------- CPP --------------------//
#include <string>

//------------------- Libs --------------------//
#include "nocopyable.h" 


class Texture {

public:
    //-- 构造函数 暂时只用来 初始化 成员变量 --
    Texture( const std::string &lpathImg )
            :
            lpath_img(lpathImg)
            {}

    //-- 真正的初始化函数 --
    void init();

    inline GLuint get_textel_name() const 
                { return textel_name; }

private:
    GLuint      textel_name {0};    //-- texture textel_name
    std::string lpath_img;          //-- 贴图的 相对路径名
    GLenum      format { GL_RGBA }; //-- jpg:GL_RGB. png:GL_RGBA.


};









#endif

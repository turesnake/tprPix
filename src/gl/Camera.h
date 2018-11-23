/*
 * ========================= Camera.h ===================
 *                          -- tpr --
 *                                        创建 -- 2018.11.22
 *                                        修改 -- 2018.11.22
 * ----------------------------------------------------------
 * 
 *   camera
 * 
 * ----------------------------
 */
#ifndef _TPR_CAMERA_H_
#define _TPR_CAMERA_H_

/* -- 确保 glad GLFW 两个库 的引用顺序 ---
 * --    glad.h 包含了正确的OpenGL头文件（如GL/gl.h），
 * --    所以需要在其它依赖于OpenGL的头文件之前 包含 glad.h
 */
#include<glad/glad.h>  
#include<GLFW/glfw3.h>

//--- glm - 0.9.8 ---
#include <glm/glm.hpp>
            //-- glm::vec3
            //-- glm::vec4
            //-- glm::mat4
#include <glm/gtc/matrix_transform.hpp>
            //-- glm::translate
            //-- glm::rotate
            //-- glm::scale
            //-- glm::perspective
#include <glm/gtc/type_ptr.hpp> 
            //-- glm::value_ptr



//------------------- Libs --------------------//
#include "nocopyable.h" 



class Camera : tpr::nocopyable {

public:
    explicit Camera()
        //:
        {}

    ~Camera() = default;



private:




};



#endif

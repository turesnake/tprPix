/*
 * ========================= input.cpp ==========================
 *                          -- tpr --
 *                                        创建 -- 2018.11.21
 *                                        修改 -- 2018.11.21
 * ----------------------------------------------------------
 *   键盘，鼠标，joystick 等输入设备 管理文件
 * ----------------------------
 */

/* -- 确保 glad GLFW 两个库 的引用顺序 ---
 * --    glad.h 包含了正确的OpenGL头文件（如GL/gl.h），
 * --    所以需要在其它依赖于OpenGL的头文件之前 包含 glad.h
 */
#include<glad/glad.h>  
#include<GLFW/glfw3.h>

//#include <iostream> // cout

#include "Camera.h" //- camera_current,

//using std::cout;
//using std::endl;


//------------------- 提供给全局的 函数 ----------------
void processInput( GLFWwindow *window );
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);


/* ==========================================================
 *                 processInput
 *-----------------------------------------------------------
 * input 处理函数。 看起来很简陋。
 */
void processInput( GLFWwindow *window ){

    //-- ESC -- 
	if( glfwGetKey( window, GLFW_KEY_ESCAPE ) == GLFW_PRESS ){
		glfwSetWindowShouldClose( window, GL_TRUE );
	}

    //-- W -- 
    if( glfwGetKey( window, GLFW_KEY_W ) == GLFW_PRESS ){
            //cout << "-W-" << endl;
        camera_current()->cameraPos_down();
    }

    //-- S -- 
    if( glfwGetKey( window, GLFW_KEY_S ) == GLFW_PRESS ){
            //cout << "-S-" << endl;
        camera_current()->cameraPos_up();
    }

    //-- A -- 
    if( glfwGetKey( window, GLFW_KEY_A ) == GLFW_PRESS ){
            //cout << "-A-" << endl;
        camera_current()->cameraPos_left();
    }

    //-- D -- 
    if( glfwGetKey( window, GLFW_KEY_D ) == GLFW_PRESS ){
            //cout << "-D-" << endl;
        camera_current()->cameraPos_right();
    }

    //-- SPACE -- 
    //if( glfwGetKey( window, GLFW_KEY_SPACE ) == GLFW_PRESS ){
    //}

    //-- TAB -- 
    //if( glfwGetKey( window, GLFW_KEY_TAB ) == GLFW_PRESS ){
    //}
}



/* ==========================================================
 *                 mouse_callback
 *-----------------------------------------------------------
 *  鼠标位移运动 回调函数，控制 摄像机 俯仰，偏航
 *    （double 类型是 回调函数 定死的，不能改。）
 */
void mouse_callback(GLFWwindow* window, double xpos, double ypos){

        //cout << "xpos = " << xpos << "ypos = " << ypos << endl; 
    camera_current()->mousePos_move( xpos, ypos );

}


/* ==========================================================
 *                 scroll_callback
 *-----------------------------------------------------------
 *  鼠标 滚轮 缩放。 控制摄像机 广角度
 *    （double 类型是定死的，不能改。）
 */
/*
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset){

        //cout << "xoffset = " << xoffset << "yoffset = " << yoffset << endl;
    //camera_current()->mouseFov_reset( xoffset, yoffset );
}
*/





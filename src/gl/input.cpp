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

#include "glob_gl.h" 
#include "PixVec.h" 

//using std::cout;
//using std::endl;


//------------------- 提供给全局的 函数 ----------------
void processInput( GLFWwindow *window );
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
PixVec2 get_mouse_pos();


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
        camera_current()->cameraPos_up();
    }

    //-- S -- 
    if( glfwGetKey( window, GLFW_KEY_S ) == GLFW_PRESS ){
            //cout << "-S-" << endl;
        camera_current()->cameraPos_down();
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
    //camera_current()->mousePos_move( xpos, ypos );

}


/* ==========================================================
 *                 mouse_callback
 *-----------------------------------------------------------
 * -- 获得鼠标 当前 坐标值
 * -- 当鼠标指针 未隐藏时：
 *       窗口左上角为 (0,0)
 *       光标初始值，就是 当前指针 与 窗口左上角的 offset
 *       光标初始值 是 “绝对值”模式
 *       当鼠标移动到 屏幕便捷后，鼠标坐标值不再增长
 *       当鼠标移动到 屏幕左下角时，会触发 屏保
 * -- 当鼠标制作 隐藏时：
 *       窗口左上角为 (0,0)
 *            景观无法看见 鼠标光标位置，但光标确实在向 右下 移动时，显示出 x,y 轴的正值。
 *       光标初始值 为 (0,0)
 *       这似乎是一种 类似 鼠标滚轮的 “相对值模式”，相对于 glfw 初始时光标坐标 的位移
 *       就算鼠标移出屏幕边界，坐标值仍会无限增长。
 *       这种模式时，并不适合 在 游戏初始化阶段，通过鼠标坐标值来 启动 随机数种子。 
 *       毕竟，此时的 鼠标坐标值，始终是 (0,0) 
 */
PixVec2 get_mouse_pos(){

    double x;
    double y;
    glfwGetCursorPos( window, &x, &y);
    return PixVec2{ (int)x, (int)y };
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





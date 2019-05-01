/*
 * ========================= input.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2018.11.21
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *   键盘，鼠标，joystick 等输入设备 管理文件
 * ----------------------------
 */
#include "input.h" 

//--- glm - 0.9.8 ---
#include <glm/glm.hpp>
            //-- glm::vec2
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

//-------------------- C --------------------//
#include <cassert>
#include <math.h>

//-------------------- CPP --------------------//
#include <unordered_map>

//-------------------- Engine --------------------//
#include "config.h"
#include "gameKeyTable.h"
#include "IntVec.h" 
#include "KeyBoard.h"
#include "GameKey.h"
#include "esrc_player.h"
#include "esrc_window.h"
#include "esrc_thread.h"

#include "debug.h" //- tmp


namespace input{//------------- namespace input --------------------


namespace{
    InputINS   inputINS {}; //- 记录玩家 鼠键输入。

    
    F_InputINS_Handle inputINS_handleFunc  {nullptr}; 

    glm::vec2 lastMousePos    {0.0f, 0.0f};    
    glm::vec2 currentMousePos {0.0f, 0.0f}; 

    //---------
    void mousePos_2_dir();
}


/* ==========================================================
 *                    init_input    [pure]
 *-----------------------------------------------------------
 * 
 */
void init_input(){

    //----
    assert( gameKeyTable.size() == GameKeyNum );
    assert( gameKeyTable.at(GameKey::LEFT)  != KeyBoard::NIL );
    assert( gameKeyTable.at(GameKey::RIGHT) != KeyBoard::NIL );
    assert( gameKeyTable.at(GameKey::UP)    != KeyBoard::NIL );
    assert( gameKeyTable.at(GameKey::DOWN)  != KeyBoard::NIL );
}



/* ==========================================================
 *                 bind_inputINS_handleFunc
 *-----------------------------------------------------------
 */
void bind_inputINS_handleFunc( const F_InputINS_Handle &_func ){
    inputINS_handleFunc = _func;
}



/* ==========================================================
 *                 processInput
 *-----------------------------------------------------------
 * input 处理函数。 每一视觉帧 都调用
 */
void processInput( GLFWwindow *_windowPtr ){

    //-------------------------------//
    //   快速退出（tmp，未来将被删除...）
    //-------------------------------//
	if( glfwGetKey( _windowPtr, GLFW_KEY_ESCAPE )==GLFW_PRESS ){ //- ESC -
        //-- 注意，请不要在此处 添加 thread.join() 系列指令 --
		glfwSetWindowShouldClose( _windowPtr, GL_TRUE );
                    //-- 并不是立即结束程序，而是退出 main.cpp -> while 大循环
                    //   while 大循环 之后的代码还将被调用
                    //   thread.join() 系列指令 应当添加在那个位置
	}

    //------------------------//
    //     将玩家鼠键输入，
    //     装填到 inputINS 中
    //------------------------//
    inputINS.clear_allKeys(); //- 0
    mousePos_2_dir(); //-- 未来拓展：鼠标／右侧摇杆 都可以控制 方向dir值
    for( const auto &ipair : gameKeyTable ){ //-- each gameKey
        //-- 跳过 1.未设置的; 2.没有被按下的 --
        if( (ipair.second==KeyBoard::NIL) ||
            (glfwGetKey(_windowPtr,(int)(ipair.second))!=GLFW_PRESS) ){
            continue;
        }            
        inputINS.set_key( ipair.first );
    }

    //------------------------//
    //  处理 inputINS 中的数据
    //------------------------//
    
    inputINS_handleFunc( inputINS );

    //-- player --
    //esrc::player.handle_inputINS( inputINS );

            //-- 不是直接 传给 player，而是传给一个 functor
}


/* ==========================================================
 *                 get_mouse_pos
 *-----------------------------------------------------------
 * -- 获得鼠标 当前 坐标值
 * -- 当鼠标指针 未隐藏时：
 *       窗口左上角为 (0,0)
 *       光标初始值，就是 当前指针 与 窗口左上角的 offset
 *       光标初始值 是 “绝对值”模式
 *       当鼠标移动到 屏幕边界后，鼠标坐标值不再增长
 *       当鼠标移动到 屏幕左下角时，会触发 屏保
 * -- 当鼠标指针 隐藏时：
 *       窗口左上角为 (0,0)
 *            尽管无法看见 鼠标光标位置，但光标确实在向 右下 移动时，显示出 x,y 轴的正值。
 *       光标初始值 为 (0,0)
 *       这似乎是一种 类似 鼠标滚轮的 “相对值模式”，相对于 glfw 初始时光标坐标 的位移
 *       就算鼠标移出屏幕边界，坐标值仍会无限增长。
 *       这种模式时，并不适合 在 游戏初始化阶段，通过鼠标坐标值来 启动 随机数种子。 
 *       毕竟，此时的 鼠标坐标值，始终是 (0,0) 
 * ----------
 * 这个函数 目前仅用于 randow 种子生成
 */
IntVec2 get_mouse_pos(){
    double x;
    double y;
    glfwGetCursorPos( esrc::windowPtr, &x, &y);
    return IntVec2{ (int)x, (int)y };
}



/* ==========================================================
 *                 scroll_callback
 *-----------------------------------------------------------
 *  鼠标 滚轮 缩放。 控制摄像机 广角度
 *    （double 类型是定死的，不能改。）
 */
/*
void scroll_callback(GLFWwindow* _window, double _xoffset, double _yoffset){
        //cout << "xoffset = " << xoffset << "yoffset = " << yoffset << endl;
    //camera_current()->mouseFov_reset( xoffset, yoffset );
}
*/


namespace{ //------------------- namespace ----------------------//

/* ==========================================================
 *                 mousePos_2_dir
 *-----------------------------------------------------------
 *  主动查询 mousePos值，通过它计算出 player_go 单位方向向量
 * ---
 *  临时性的写法...
 */
void mousePos_2_dir(){

    double x;
    double y;
    glfwGetCursorPos( esrc::windowPtr, &x, &y);

    currentMousePos.x = (float)x; 
    currentMousePos.y = (float)y;

    glm::vec2 off;
    off.x = lastMousePos.x - currentMousePos.x; //- 此处是反的，别问为什么...
    off.y = currentMousePos.y - lastMousePos.y;

    //-- 过滤掉 微小抖动, 和彻底静止值 --
    if( (abs(off.x)<0.001f) && (abs(off.y)<0.001f) ){
        return;
    }

    //- 计算出 新的 dir
    inputINS.dir = glm::normalize( inputINS.dir - 0.02f*off );

    lastMousePos = currentMousePos;  //-- MUST --//

            //-- 清空 debug: renderPool --
            debug::clear_pointPics();
            debug::insert_new_pointPic( inputINS.dir * 20.0f );
}


}//------------------------ namespace: end --------------------//



}//----------------- namespace input: end -------------------


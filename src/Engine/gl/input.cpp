/*
 * ========================= input.cpp ==========================
 *                          -- tpr --
 *                                        创建 -- 2018.11.21
 *                                        修改 -- 2018.11.21
 * ----------------------------------------------------------
 *   键盘，鼠标，joystick 等输入设备 管理文件
 * ----------------------------
 */
#include "input.h" 

//-------------------- C --------------------//
#include <cassert>

//-------------------- CPP --------------------//
//#include <string>

//-------------------- Engine --------------------//
#include "PixVec.h" 
#include "srcs_engine.h" //- 所有资源


//#include "debug.h" //- tmp


namespace input{//------------- namespace input --------------------


namespace{

    inline F_V_V   keyDown_W      {nullptr};
    inline F_V_V   keyDown_S      {nullptr};
    inline F_V_V   keyDown_A      {nullptr};
    inline F_V_V   keyDown_D      {nullptr};
    inline F_V_V   keyDown_SPACE  {nullptr};
    
    //--
    inline F_CROSS_STATE   gameCross  {nullptr};

    NineBox   nb {}; 
    //---------
    void onMoveLeft();
    void onMoveRight();
    void onMoveUp();
    void onMoveDown();
}


/* ==========================================================
 *                 processInput
 *-----------------------------------------------------------
 * input 处理函数。 每一视觉帧 都调用
 */
void processInput( GLFWwindow *_windowPtr ){

    //-- ESC -- 
	if( glfwGetKey( _windowPtr, GLFW_KEY_ESCAPE )==GLFW_PRESS ){
		glfwSetWindowShouldClose( _windowPtr, GL_TRUE );
	}

    //-- clear --
    nb.x = 0;
    nb.y = 0;

    //-------------------------------------------------------//
    //  each Key 
    //--- W ---
    if( (glfwGetKey( _windowPtr, GLFW_KEY_W )==GLFW_PRESS) && (keyDown_W!=nullptr) ){
        keyDown_W();
    }
    //--- S ---
    if( (glfwGetKey( _windowPtr, GLFW_KEY_S )==GLFW_PRESS) && (keyDown_S!=nullptr) ){
        keyDown_S();
    }
    //--- A --- 
    if( (glfwGetKey( _windowPtr, GLFW_KEY_A )==GLFW_PRESS) && (keyDown_A!=nullptr) ){
        keyDown_A();
    }
    //--- D --- 
    if( (glfwGetKey( _windowPtr, GLFW_KEY_D )==GLFW_PRESS) && (keyDown_D!=nullptr) ){
        keyDown_D();
    }
    //-- SPACE -- 
    if( glfwGetKey( _windowPtr, GLFW_KEY_SPACE ) == GLFW_PRESS ){
        keyDown_SPACE();
    }
    //-- TAB -- 
    //if( glfwGetKey( _window, GLFW_KEY_TAB ) == GLFW_PRESS ){
    //}
    //-------------------------------------------------------//

    //--- call gameCross() to handle the nineBox [-Each Render Frame-] --
    assert( gameCross!=nullptr );
    gameCross( nb );
}




/* ==========================================================
 *                 mouse_callback
 *-----------------------------------------------------------
 *  鼠标位移运动 回调函数，控制 摄像机 俯仰，偏航
 *    （double 类型是 回调函数 定死的，不能改。）
 */
void mouse_callback(GLFWwindow* _window, double _xpos, double _ypos){

        //cout << "xpos = " << xpos << "ypos = " << ypos << endl; 
    //camera_current()->mousePos_move( xpos, ypos );
}


/* ==========================================================
 *                 get_mouse_pos
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
    glfwGetCursorPos( esrc::windowPtr, &x, &y);
    return PixVec2{ (int)x, (int)y };
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

/* ==========================================================
 *                bind_gameCross_key
 * -----------------------------------------------------------
 * -- 设置 哪些key 来实现 游戏十字键
 * -- 这些被选中的 key，原本绑定的 callback 会被 替代。
 */
void bind_gameCross_key( DIRECTION _dir, input::KEY _key ){

    switch( _dir ){
        case DIRECTION::Left:  bind_key_callback( _key, std::bind( &onMoveLeft ) );
            break;
        case DIRECTION::Right: bind_key_callback( _key, std::bind( &onMoveRight ) );
            break;
        case DIRECTION::Up:    bind_key_callback( _key, std::bind( &onMoveUp ) );
            break;
        case DIRECTION::Down:  bind_key_callback( _key, std::bind( &onMoveDown ) );
            break;
        case DIRECTION::Idle:  //-- just do nothing...
            break;
        default:
            assert(0);
    }
}

/* ==========================================================
 *                 bind_key_callback
 * -----------------------------------------------------------
 * -- 外部代码通过 此函数 来将一个 callback函数 绑定到 某个key上
 */
void bind_key_callback( input::KEY _key, F_V_V _fp ){

    switch(_key){
        case KEY::W:  keyDown_W = _fp;
            break;
        case KEY::A:  keyDown_A = _fp;
            break;
        case KEY::S:  keyDown_S = _fp;
            break;
        case KEY::D:  keyDown_D = _fp;
            break;
        case KEY::SPACE:  keyDown_SPACE = _fp;
            break;
        default:
            assert(0);
    }
}

/* ==========================================================
 *                bind_gameCross_callback
 * -----------------------------------------------------------
 * -- 当发生 游戏十字键操作，最终调用的 callback
 * -- 会把 十字健的状态 回传出去。
 */
void bind_gameCross_callback( F_CROSS_STATE _fp ){
    gameCross = _fp;
}


namespace{ //------------------- namespace ----------------------//

void onMoveLeft(){
    nb.x = -1;
}
void onMoveRight(){
    nb.x = 1;
}
void onMoveUp(){
    nb.y = 1;
}
void onMoveDown(){
    nb.y = -1;
}

}//------------------------ namespace: end --------------------//



}//----------------- namespace input: end -------------------


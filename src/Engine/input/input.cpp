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

//--- glm - 0.9.9.5 ---
#include "glm_no_warnings.h"

//-------------------- C --------------------//
#include <cmath>

//-------------------- CPP --------------------//
#include <unordered_map>

//-------------------- Engine --------------------//
#include "tprAssert.h"
#include "config.h"
#include "gameKeyTable.h"
#include "IntVec.h" 
#include "KeyBoard.h"
#include "GameKey.h"
#include "JoyStickButton.h"
#include "esrc_player.h"
#include "esrc_window.h"
#include "esrc_thread.h"

#include "tprDebug.h" //- tmp


namespace input{//------------- namespace input --------------------


namespace _inn {

    InputINS   inputINS {}; //- 记录玩家 鼠键输入。
    
    F_InputINS_Handle inputINS_handleFunc  {nullptr}; 

    //--- mouse ---//
    //glm::vec2 lastMousePos    {0.0f, 0.0f};    
    //glm::vec2 currentMousePos {0.0f, 0.0f};

    //-- 清除 NIL元素。制作 运行时检测表 --
    //   减少每帧 不必要的检测
    std::unordered_map<GameKey, KeyBoard>        keyboardTable_runtime {};
    std::unordered_map<GameKey, JoyStickButton>  joyButtonTable_runtime {};


    //-- joystick --//
    bool isFindJoystick {false};
    int  joystickId     {};
    
    int  joystickAxesCount     {};
    int  joystickButtonsCount  {};

    const float* joystickAxesPtr             {nullptr};
    const unsigned char* joystickButtonsPtr  {nullptr};


    //---------
    //void mousePos_2_dir();
    void joystick_update();
}


/* ==========================================================
 *                    init_input    [pure]
 *-----------------------------------------------------------
 * 
 */
void init_input(){

    //----------------------//
    // keyboardTable_runtime
    //----------------------//
    tprAssert( keyboardTable.size() == GameKeyNum );
    tprAssert( keyboardTable.at(GameKey::LEFT)  != KeyBoard::NIL );
    tprAssert( keyboardTable.at(GameKey::RIGHT) != KeyBoard::NIL );
    tprAssert( keyboardTable.at(GameKey::UP)    != KeyBoard::NIL );
    tprAssert( keyboardTable.at(GameKey::DOWN)  != KeyBoard::NIL );

    _inn::keyboardTable_runtime.clear();
    for( const auto &pair : keyboardTable ){
        if( pair.second != KeyBoard::NIL ){
            _inn::keyboardTable_runtime.insert(pair); //- copy
        }
    }

    _inn::joyButtonTable_runtime.clear();
    for( const auto &pair : joyButtonTable ){
        if( pair.second != JoyStickButton::NIL ){
            _inn::joyButtonTable_runtime.insert(pair); //- copy
        }
    }

    //----------------------//
    //       joystick
    //----------------------//
    //-- 简单版，找到第一个 joystick 就停手 --
    _inn::isFindJoystick = false;
    for( int i=0; i<10; i++ ){
        if( glfwJoystickPresent(i)==GL_TRUE ){
            _inn::isFindJoystick = true;
            _inn::joystickId = i;
            break;
        }
    }
}


/* ==========================================================
 *                 bind_inputINS_handleFunc
 *-----------------------------------------------------------
 */
void bind_inputINS_handleFunc( const F_InputINS_Handle &func_ ){
    _inn::inputINS_handleFunc = func_;
}



/* ==========================================================
 *                 processInput
 *-----------------------------------------------------------
 */
void processInput( GLFWwindow *windowPtr_ ){

    //-------------------------------//
    //   快速退出（tmp，未来将被删除...）
    //-------------------------------//
	if( glfwGetKey( windowPtr_, GLFW_KEY_ESCAPE )==GLFW_PRESS ){ //- ESC -
        //-- 注意，请不要在此处 添加 thread.join() 系列指令 --
		glfwSetWindowShouldClose( windowPtr_, GL_TRUE );
                    //-- 并不是立即结束程序，而是退出 main.cpp -> while 大循环
                    //   while 大循环 之后的代码还将被调用
                    //   thread.join() 系列指令 应当添加在那个位置
	}
                //... 在未来，ESC 键 将被屏蔽。改为 游戏内置的 退出键 ...

    //------------------------//
    //     1. 读取鼠键输入
    //------------------------//
    _inn::inputINS.clear_allKeys(); //- 0

    //mousePos_2_dir(); //-- 目前暂时不识别 mouse 输入...

    for( const auto &ipair : _inn::keyboardTable_runtime ){ //-- each gameKey
        //-- 跳过 没有被按下的 --
        if( glfwGetKey(windowPtr_, static_cast<int>(ipair.second)) == GLFW_PRESS ){
            _inn::inputINS.set_key_from_keyboard( ipair.first );
        }            
    }

    //------------------------//
    //  2. 读取 joystick 输入
    // (可能会覆盖之前的 鼠键数据)
    //------------------------//
    _inn::joystick_update();

    //------------------------//
    //  3. 修正 inputINS.dirAxes 
    //------------------------//
    _inn::inputINS.limit_dirAxes();


    //------------------------//
    //  处理 inputINS 中的数据
    //------------------------//
    _inn::inputINS_handleFunc( _inn::inputINS );
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
    double x {};
    double y {};
    glfwGetCursorPos( esrc::get_windowPtr(), &x, &y);
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


namespace _inn {//------------------- namespace: _inn ----------------------//

/* ==========================================================
 *                 mousePos_2_dir
 *-----------------------------------------------------------
 *  主动查询 mousePos值，通过它计算出 player_go 单位方向向量
 * ---
 *  临时性的写法...
 */
/*
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
    if( (std::abs(off.x)<0.001f) && (std::abs(off.y)<0.001f) ){
        return;
    }

    //- 计算出 新的 dir
    inputINS.dir = glm::normalize( inputINS.dir - 0.02f*off );

    lastMousePos = currentMousePos;  //-- MUST --//

            //-- 清空 tprDebug: renderPool --
            tprDebug::clear_pointPics();
            tprDebug::insert_new_pointPic( inputINS.dir * 20.0f );
}
*/



/* ==========================================================
 *                 joystick_update
 *-----------------------------------------------------------
 */
void joystick_update(){
    if( isFindJoystick == false ){
        return;
    }

    //--------------//
    //     Axes
    //--------------//
    joystickAxesPtr = glfwGetJoystickAxes(joystickId, &joystickAxesCount);
    //-- 目前，只读取 joystick 头两个 axes 数据 --
    if( joystickAxesCount >= 2 ){
        float fx =  *(joystickAxesPtr+0);
        float fy = -*(joystickAxesPtr+1); //- 注意，要取反
        double x = static_cast<double>(fx);
        double y = static_cast<double>(fy);

        //-- 只有在 确认有效时，才改写 inputINS --
        //   这个行为会覆盖之前 输入的 鼠键数据
        if( DirAxes::is_effectVal(x,y) ){
            inputINS.set_dirAxes_from_joystick( x, y );
        }
    }

    //--------------//
    //    Buttons
    //--------------//
    //-- 暂时不处理 手柄 按键 ---
    
    joystickButtonsPtr = glfwGetJoystickButtons(joystickId, &joystickButtonsCount);    

    /*
    for( int i=0; i<joystickButtonsCount; i++ ){
        button = *(joystickButtonsPtr+i);
        if( button == GLFW_PRESS ){
        }
    }
    */

    for( const auto &pair : joyButtonTable_runtime ){
        if( *(joystickButtonsPtr + static_cast<int>(pair.second)) == GLFW_PRESS ){
            inputINS.set_key_from_joystick( pair.first );
        }
    }
}


}//------------------------ namespace: _inn end --------------------//
}//----------------- namespace input: end -------------------


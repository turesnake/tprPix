/*
 * ========================= input.h ==========================
 *                          -- tpr --
 *                                        创建 -- 2019.01.05
 *                                        修改 -- 2019.01.05
 * ----------------------------------------------------------
 *   键盘，鼠标，joystick 等输入设备 管理文件
 * ----------------------------
 */
#ifndef _TPR_INPUT_H_
#define _TPR_INPUT_H_
//=== *** glad FIRST, glfw SECEND *** ===
#include<glad/glad.h>  
#include<GLFW/glfw3.h>

//-------------------- CPP --------------------//
#include <functional>

//-------------------- Engine --------------------//
#include "IntVec.h" 
#include "NineBox.h" 

namespace input{//------------- namespace input --------------------


//-- 所有录入的 key --
enum class KEY{
    W = 1,
    S,
    A,
    D,
    J,
    K,
    SPACE
};

//-- 临时方这里 --
enum class DIRECTION{
    Idle, //- 无方向
    Left,
    Right,
    Up,
    Down
};


void processInput( GLFWwindow *_windowPtr );
void mouse_callback(GLFWwindow* _windowPtr, double _xpos, double _ypos);
void scroll_callback(GLFWwindow* _windowPtr, double _xoffset, double _yoffset);
IntVec2 get_mouse_pos();

using F_V_V = std::function<void()>;
void bind_key_callback( input::KEY _key, F_V_V _fp );

using F_CROSS_STATE = std::function<void(NineBox)>;
void bind_gameCross_callback( F_CROSS_STATE _fp );

//- 绑定 游戏十字键 对应的 key --
void bind_gameCross_key( DIRECTION _dir, input::KEY _key );



}//----------------- namespace input: end -------------------
#endif 


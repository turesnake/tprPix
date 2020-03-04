/*
 * ======================== Joystick.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2020.01.27
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#ifndef TPR_JOYSTICK_H
#define TPR_JOYSTICK_H
//=== *** glad FIRST, glfw SECEND *** ===
// Don't include glfw3.h ALONE!!!
#include<glad/glad.h>  
#include<GLFW/glfw3.h>

//--- glm - 0.9.9.5 ---
#include "glm_no_warnings.h"

//-------------------- CPP --------------------//
#include <string>
#include <vector>


//-------------------- Engine --------------------//
#include "tprAssert.h"


// support SDL_GameControllerDB
// 本游戏 暂时仅支持 这类 joystick
class Joystick{
public:

    enum class Button : size_t{
        A = GLFW_GAMEPAD_BUTTON_A,
        B = GLFW_GAMEPAD_BUTTON_B,
        X = GLFW_GAMEPAD_BUTTON_X,
        Y = GLFW_GAMEPAD_BUTTON_Y,
        LB = GLFW_GAMEPAD_BUTTON_LEFT_BUMPER, 
        RB = GLFW_GAMEPAD_BUTTON_RIGHT_BUMPER,
        Back = GLFW_GAMEPAD_BUTTON_BACK, 
        Start = GLFW_GAMEPAD_BUTTON_START, 
        Guide = GLFW_GAMEPAD_BUTTON_GUIDE,
        LeftThumb = GLFW_GAMEPAD_BUTTON_LEFT_THUMB, 
        RightThumb = GLFW_GAMEPAD_BUTTON_RIGHT_THUMB,
        Hat_Up = GLFW_GAMEPAD_BUTTON_DPAD_UP, 
        Hat_Right = GLFW_GAMEPAD_BUTTON_DPAD_RIGHT,
        Hat_Down = GLFW_GAMEPAD_BUTTON_DPAD_DOWN, 
        Hat_Left = GLFW_GAMEPAD_BUTTON_DPAD_LEFT, 
    };

    //========== Selfs ==========//
    explicit Joystick( int connectIdx_ ):
        connectIdx(connectIdx_)
        {
            this->name = glfwGetJoystickName( this->connectIdx );
            this->gamePadName = glfwGetGamepadName( this->connectIdx );
        }


    void refresh()noexcept;

    //------- get -------//
    inline bool get_isAnyAxisPress()const noexcept{ return this->isAnyAxisPress; }
    inline bool get_isAnyButtonPress()const noexcept{ return this->isAnyButtonPress; }

    inline bool get_button( Button b_ )const noexcept{
        return (this->state.buttons[ static_cast<size_t>(b_) ] == GLFW_PRESS);
    }

    inline glm::dvec2 get_LeftAxes()const noexcept{
        return glm::dvec2{
            static_cast<double>( state.axes[GLFW_GAMEPAD_AXIS_LEFT_X] ),
            static_cast<double>( state.axes[GLFW_GAMEPAD_AXIS_LEFT_Y] ) };
    }

    inline glm::dvec2 get_RightAxes()const noexcept{
        return glm::dvec2{
            static_cast<double>( state.axes[GLFW_GAMEPAD_AXIS_RIGHT_X] ),
            static_cast<double>( state.axes[GLFW_GAMEPAD_AXIS_RIGHT_Y] ) };
    }

    inline double get_LT()const noexcept{ return static_cast<double>( state.axes[GLFW_GAMEPAD_AXIS_LEFT_TRIGGER] ); }
    inline double get_RT()const noexcept{ return static_cast<double>( state.axes[GLFW_GAMEPAD_AXIS_RIGHT_TRIGGER] ); }


private:
    //--- idx ---//
    int         connectIdx;

    //--- strs ---//
    std::string name        {}; // glfw 读取的 name
    std::string gamePadName {}; // 在 SDL_GameControllerDB 中登记的 name


    GLFWgamepadstate state {}; // 完整的数据

    //===== flags =====//
    bool isAnyAxisPress   {false}; // 是否有任一轴 发生输入
    bool isAnyButtonPress {false}; // 是否有某任一按钮 发生输入
};



#endif 


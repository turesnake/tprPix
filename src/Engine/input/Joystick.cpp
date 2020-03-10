/*
 * ======================== Joystick.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2020.01.28
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#include "pch.h"
#include "Joystick.h"

//--------------- Libs ------------------//
#include "magic_enum.hpp"


namespace joystick_inn {//------------------- namespace: joystick_inn ----------------------//

    // 6 个轴 的默认值
    std::vector<float> axisDefaultVals {
        0.0f,  // GLFW_GAMEPAD_AXIS_LEFT_X   0
        0.0f,  // GLFW_GAMEPAD_AXIS_LEFT_Y   1
        0.0f,  // GLFW_GAMEPAD_AXIS_RIGHT_X   2
        0.0f,  // GLFW_GAMEPAD_AXIS_RIGHT_Y   3
        -1.0f, // GLFW_GAMEPAD_AXIS_LEFT_TRIGGER   4
        -1.0f, // GLFW_GAMEPAD_AXIS_RIGHT_TRIGGER   5
    };

}//------------------- namespace: joystick_inn ----------------------//


// call each renderFrame
// 刷新整个 手柄数据
// 设置相应 flags
void Joystick::refresh()noexcept{

    int ret = glfwGetGamepadState( this->connectIdx, &(this->state) );
    tprAssert( ret == GLFW_TRUE );

    //--- axes input check ---//
    this->isAnyAxisPress = false;
    for( size_t i=0; i<(GLFW_GAMEPAD_AXIS_LAST-GLFW_GAMEPAD_AXIS_LEFT_X+1); i++ ){
        if( !is_closeEnough<float>( this->state.axes[i], joystick_inn::axisDefaultVals.at(i), 0.001f ) ){
            this->isAnyAxisPress = true;
            break;
        }
    }

    //--- buttons input check ---//
    this->isAnyButtonPress = false;
    for( size_t i=0; i<(GLFW_GAMEPAD_BUTTON_LAST-GLFW_GAMEPAD_BUTTON_A+1); i++ ){
        if( this->state.buttons[i] == GLFW_PRESS ){
            this->isAnyButtonPress = true;
            break;
        }
    }
}




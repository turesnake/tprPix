/*
 * ================= JoystickButton_2_GameKey_map.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2020.01.29
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 * only include by input.cpp
 * ---
 */
#ifndef TPR_JOYSTICK_BUTTON_2_GAMEKEY_MAP_H
#define TPR_JOYSTICK_BUTTON_2_GAMEKEY_MAP_H

#include <unordered_map>

#include "Joystick.h"
#include "GameKey.h"


// 按键映射表
// 键值完全一致的表
inline std::unordered_map<Joystick::Button, GameKey> JoystickButton_2_GameKey_map{

    {Joystick::Button::A, GameKey::A},
    {Joystick::Button::B, GameKey::B},
    {Joystick::Button::X, GameKey::X},
    {Joystick::Button::Y, GameKey::Y},

    {Joystick::Button::LB, GameKey::LB},
    {Joystick::Button::RB, GameKey::RB},

    {Joystick::Button::Back, GameKey::Back},
    {Joystick::Button::Start, GameKey::Start},
    {Joystick::Button::Guide, GameKey::Guide},

    {Joystick::Button::LeftThumb, GameKey::LeftThumb},
    {Joystick::Button::RightThumb, GameKey::RightThumb},

    {Joystick::Button::Hat_Up, GameKey::Hat_Up},
    {Joystick::Button::Hat_Right, GameKey::Hat_Right},
    {Joystick::Button::Hat_Down, GameKey::Hat_Down},
    {Joystick::Button::Hat_Left, GameKey::Hat_Left}
};


#endif 


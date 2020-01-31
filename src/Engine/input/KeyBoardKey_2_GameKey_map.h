/*
 * ================= KeyBoardKey_2_GameKey_map ==========================
 *                          -- tpr --
 *                                        CREATE -- 2020.01.29
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 * only include by input.cpp
 * ---
 */
#ifndef TPR_KEYBOARD_KEY_2_GAMEKEY_MAP_H
#define TPR_KEYBOARD_KEY_2_GAMEKEY_MAP_H

#include <unordered_map>

#include "KeyBoard.h"
#include "GameKey.h"


// 按键映射表
// 未来，实现 按键修改界面后，这组值可被用户改写，存储进 json / db
inline std::unordered_map<KeyBoard::Key, GameKey> KeyBoardKey_2_GameKey_map{

    {KeyBoard::Key::ENTER, GameKey::A},
    {KeyBoard::Key::H, GameKey::A},

    {KeyBoard::Key::J, GameKey::B},
    {KeyBoard::Key::K, GameKey::X},
    {KeyBoard::Key::L, GameKey::Y},

    {KeyBoard::Key::Q, GameKey::LB},
    {KeyBoard::Key::E, GameKey::RB},

    {KeyBoard::Key::BACKSPACE, GameKey::Back},
    {KeyBoard::Key::DELETE_, GameKey::Back},
    {KeyBoard::Key::ESCAPE, GameKey::Back},

    {KeyBoard::Key::TAB,    GameKey::Start},
    {KeyBoard::Key::F1,     GameKey::Guide},

    {KeyBoard::Key::F2, GameKey::LeftThumb},
    {KeyBoard::Key::F3, GameKey::RightThumb},

    {KeyBoard::Key::NUM_1, GameKey::Hat_Up},
    {KeyBoard::Key::NUM_2, GameKey::Hat_Right},
    {KeyBoard::Key::NUM_3, GameKey::Hat_Down},
    {KeyBoard::Key::NUM_4, GameKey::Hat_Left}
};


#endif 


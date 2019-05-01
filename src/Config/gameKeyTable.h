/*
 * ========================= gameKeyTable.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2018.11.21
 *                                        MODIFY --
 * ----------------------------------------------------------
 *   玩家手动配置的 鼠标按键 映射
 *  ------
 *   此文件 仅被 input 模块 include
 * ----------------------------
 */
#ifndef _TPR_GAME_KEY_TABLE_H_
#define _TPR_GAME_KEY_TABLE_H_


//-------------------- CPP --------------------//
#include <unordered_map>

//-------------------- Engine --------------------//
#include "KeyBoard.h"
#include "GameKey.h"


inline std::unordered_map<GameKey, KeyBoard> gameKeyTable {

    { GameKey::KEY_0,   KeyBoard::SPACE }, //- Jump
    { GameKey::KEY_1,   KeyBoard::H },     //- JoyStick_A
    { GameKey::KEY_2,   KeyBoard::J },     //- JoyStick_B
    { GameKey::KEY_3,   KeyBoard::K },     //- JoyStick_X
    { GameKey::KEY_4,   KeyBoard::L },     //- JoyStick_Y
    { GameKey::KEY_5,   KeyBoard::NIL },
    { GameKey::KEY_6,   KeyBoard::NIL },
    { GameKey::KEY_7,   KeyBoard::NIL },
    { GameKey::KEY_8,   KeyBoard::NIL },
    { GameKey::KEY_9,   KeyBoard::NIL },
    { GameKey::KEY_10,  KeyBoard::ENTER }, //- enter 
    { GameKey::KEY_11,  KeyBoard::NIL },
    { GameKey::KEY_12,  KeyBoard::NIL },
    { GameKey::KEY_13,  KeyBoard::NIL },
    { GameKey::KEY_14,  KeyBoard::NIL },
    { GameKey::KEY_15,  KeyBoard::NIL },
    //-------
    { GameKey::LEFT,   KeyBoard::A },
    { GameKey::RIGHT,  KeyBoard::D },
    { GameKey::UP,     KeyBoard::W },
    { GameKey::DOWN,   KeyBoard::S },
};


#endif


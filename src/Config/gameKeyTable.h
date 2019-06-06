/*
 * ========================= gameKeyTable.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2018.11.21
 *                                        MODIFY --
 * ----------------------------------------------------------
 *   玩家手动配置的 鼠标按键 映射
 *  ------
 *   此文件 仅被 input 模块 include
 *  ------
 *   目前的实现很丑陋，在未来做改良...
 * ----------------------------
 */
#ifndef TPR_GAME_KEY_TABLE_H_
#define TPR_GAME_KEY_TABLE_H_

//-------------------- CPP --------------------//
#include <unordered_map>

//-------------------- Engine --------------------//
#include "KeyBoard.h"
#include "GameKey.h"
#include "JoyStickButton.h"


//-- KeyBoard --//
inline std::unordered_map<GameKey, KeyBoard> keyboardTable {
    //- 所有未设置的键（NIL），在 runtime 会被清出 检测队列 --//

    //-- 这部分顺序，安全按照 北通pc手柄 --
    { GameKey::KEY_BACK,         KeyBoard::NIL },   // joystick_Back

    { GameKey::KEY_AXES_L_MID,   KeyBoard::NIL },    // joystick_Axes_L_Mid
    { GameKey::KEY_AXES_R_MID,   KeyBoard::NIL },    // joystick_Axes_R_Mid

    { GameKey::KEY_START,       KeyBoard::NIL },    // joystick_Start

    { GameKey::KEY_HAT_UP,      KeyBoard::NIL },    // joystick_Hat_Up
    { GameKey::KEY_HAT_RIGHT,   KeyBoard::NIL },    // joystick_Hat_Right
    { GameKey::KEY_HAT_DOWN,    KeyBoard::NIL },    // joystick_Hat_Down
    { GameKey::KEY_HAT_LEFT,    KeyBoard::NIL },    // joystick_Hat_Left

    { GameKey::KEY_L2,   KeyBoard::NIL },    // joystick_L2
    { GameKey::KEY_R2,   KeyBoard::NIL },    // joystick_R2
    { GameKey::KEY_L1,   KeyBoard::NIL },    // joystick_L1
    { GameKey::KEY_R1,   KeyBoard::NIL },    // joystick_R1

    { GameKey::KEY_Y,  KeyBoard::L },      // joystick_Y
    { GameKey::KEY_B,  KeyBoard::J },      // joystick_B
    { GameKey::KEY_A,  KeyBoard::H },      // joystick_A
    { GameKey::KEY_X,  KeyBoard::K },      // joystick_X
    //-------  
    { GameKey::KEY_HOME,  KeyBoard::NIL },    // joystick_Home
    { GameKey::KEY_17,    KeyBoard::NIL },
    { GameKey::KEY_18,    KeyBoard::NIL },

    { GameKey::KEY_19,  KeyBoard::NIL },
    { GameKey::KEY_20,  KeyBoard::NIL },
    { GameKey::KEY_21,  KeyBoard::NIL },
    { GameKey::KEY_22,  KeyBoard::NIL },
    { GameKey::KEY_23,  KeyBoard::NIL },
    { GameKey::KEY_24,  KeyBoard::NIL },
    { GameKey::KEY_25,  KeyBoard::NIL },
    { GameKey::KEY_26,  KeyBoard::NIL },
    { GameKey::KEY_27,  KeyBoard::NIL },
    { GameKey::KEY_28,  KeyBoard::NIL },
    { GameKey::KEY_29,  KeyBoard::NIL },
    { GameKey::KEY_30,  KeyBoard::NIL },

    { GameKey::KEY_ENTER,  KeyBoard::ENTER },  //- enter
    //-------
    { GameKey::LEFT,   KeyBoard::A },
    { GameKey::RIGHT,  KeyBoard::D },
    { GameKey::UP,     KeyBoard::W },
    { GameKey::DOWN,   KeyBoard::S },
};



//-- JoyStick.Button --//
inline std::unordered_map<GameKey, JoyStickButton> joyButtonTable {
    //- 所有未设置的键（NIL），在 runtime 会被清出 检测队列 --//

    //-- 这部分顺序，安全按照 北通pc手柄 --
    { GameKey::KEY_BACK,         JoyStickButton::BACK }, 

    { GameKey::KEY_AXES_L_MID,   JoyStickButton::AXES_L_MID },  
    { GameKey::KEY_AXES_R_MID,   JoyStickButton::AXES_R_MID }, 

    { GameKey::KEY_START,       JoyStickButton::START }, 

    { GameKey::KEY_HAT_UP,      JoyStickButton::HAT_UP },   
    { GameKey::KEY_HAT_RIGHT,   JoyStickButton::HAT_RIGHT },   
    { GameKey::KEY_HAT_DOWN,    JoyStickButton::HAT_DOWN },   
    { GameKey::KEY_HAT_LEFT,    JoyStickButton::HAT_LEFT },  

    { GameKey::KEY_L2,   JoyStickButton::L2 },   
    { GameKey::KEY_R2,   JoyStickButton::R2 },   
    { GameKey::KEY_L1,   JoyStickButton::L1 },   
    { GameKey::KEY_R1,   JoyStickButton::R1 },  

    { GameKey::KEY_Y,  JoyStickButton::Y },    
    { GameKey::KEY_B,  JoyStickButton::B },     
    { GameKey::KEY_A,  JoyStickButton::A },   
    { GameKey::KEY_X,  JoyStickButton::X },    
    //-------  
    { GameKey::KEY_HOME,  JoyStickButton::HOME },  
    { GameKey::KEY_17,    JoyStickButton::NIL },
    { GameKey::KEY_18,    JoyStickButton::NIL },

    { GameKey::KEY_19,  JoyStickButton::NIL },
    { GameKey::KEY_20,  JoyStickButton::NIL },
    { GameKey::KEY_21,  JoyStickButton::NIL },
    { GameKey::KEY_22,  JoyStickButton::NIL },
    { GameKey::KEY_23,  JoyStickButton::NIL },
    { GameKey::KEY_24,  JoyStickButton::NIL },
    { GameKey::KEY_25,  JoyStickButton::NIL },
    { GameKey::KEY_26,  JoyStickButton::NIL },
    { GameKey::KEY_27,  JoyStickButton::NIL },
    { GameKey::KEY_28,  JoyStickButton::NIL },
    { GameKey::KEY_29,  JoyStickButton::NIL },
    { GameKey::KEY_30,  JoyStickButton::NIL },

    { GameKey::KEY_ENTER,  JoyStickButton::NIL },  //- enter
    //-------
    { GameKey::LEFT,   JoyStickButton::NIL },
    { GameKey::RIGHT,  JoyStickButton::NIL },
    { GameKey::UP,     JoyStickButton::NIL },
    { GameKey::DOWN,   JoyStickButton::NIL },
};













#endif


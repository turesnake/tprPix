/*
 * ========================= GameKey.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.02.13
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *   游戏按键 enum类型
 * ----------------------------
 */
#ifndef TPR_GAME_KEY_H
#define TPR_GAME_KEY_H

//-------------------- Libs --------------------//
#include "tprDataType.h"


//-- 游戏可识别的按键，数量有限，
//   对应存储在 InputINS 实例中 
enum class GameKey : u32_t {
    //-- 这部分顺序，安全按照 北通pc手柄 --
    KEY_BACK = 0,  // joystick_Back

    KEY_AXES_L_MID,      // joystick_Axes_L_Mid
    KEY_AXES_R_MID,      // joystick_Axes_R_Mid

    KEY_START,      // joystick_Start

    KEY_HAT_UP,      // joystick_Hat_Up
    KEY_HAT_RIGHT,      // joystick_Hat_Right
    KEY_HAT_DOWN,      // joystick_Hat_Down
    KEY_HAT_LEFT,      // joystick_Hat_Left

    KEY_L2,      // joystick_L2
    KEY_R2,      // joystick_R2
    KEY_L1,     // joystick_L1
    KEY_R1,     // joystick_R1

    KEY_Y,     // joystick_Y
    KEY_B,     // joystick_B
    KEY_A,     // joystick_A
    KEY_X,     // joystick_X

    //----
    KEY_HOME,     // joystick_Home
    KEY_17,
    KEY_18,
    KEY_19,
    KEY_20,
    KEY_21,
    KEY_22,
    KEY_23,

    KEY_24,
    KEY_25,
    KEY_26,
    KEY_27,
    KEY_28,
    KEY_29,
    KEY_30,

    KEY_ENTER,

    //-- 从键盘上映射的 4个方向键，通常为 WSAD,
    //   也应该同时支持 4个方向键 --
    LEFT,
    RIGHT,
    UP,
    DOWN
};
inline const int GameKeyNum {36}; //- 共 36个按键


inline size_t gameKey_2_size_t( GameKey key_ )noexcept{
    return static_cast<size_t>(key_);
}



#endif 


/*
 * ========================= GameKey.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.02.13
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *   游戏按键 enum类型
 * ----------------------------
 */
#ifndef _TPR_GAME_KEY_H_
#define _TPR_GAME_KEY_H_

//-------------------- Libs --------------------//
#include "tprDataType.h"


//-- 游戏可识别的按键，数量有限，
//   对应存储在 InputINS 实例中 
enum class GameKey : u32_t {
    //----
    KEY_0 = 0,  // Jump
    KEY_1,  // JoyStick_A
    KEY_2,  // JoyStick_B
    KEY_3,  // JoyStick_X
    KEY_4,  // JoyStick_Y
    KEY_5,
    KEY_6,
    KEY_7,
    KEY_8,
    KEY_9,
    KEY_10,
    KEY_11,
    KEY_12,
    KEY_13,
    KEY_14,
    KEY_15,
    //----
    LEFT,
    RIGHT,
    UP,
    DOWN
};
inline const int GameKeyNum {20}; //- 共 20个按键



#endif 


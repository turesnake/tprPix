/*
 * ========================= GameKey.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.02.13
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *   游戏按键
 * ----------------------------
 */
#ifndef TPR_GAME_KEY_H
#define TPR_GAME_KEY_H


// 游戏程序内部，对 input按键的 类型识别
// 数据存储在 InputINS 实例中 
// ---
// 完全参照 joystick 按键顺序排布
// 或者是，标准 joystick 支持哪些按键，本游戏就支持哪些按键
enum class GameKey : int {
    A = 0,
    B,
    X,
    Y,

    LB,
    RB,

    Back,
    Start,
    Guide,

    LeftThumb,
    RightThumb,

    Hat_Up,
    Hat_Right,
    Hat_Down,
    Hat_Left,
};


#endif 


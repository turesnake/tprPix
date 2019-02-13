/*
 * ========================= KeyBoard.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.02.13
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *   键盘按键 enum类型
 * ----------------------------
 */
#ifndef _TPR_KEYBOARD_H_
#define _TPR_KEYBOARD_H_

//-- 键盘上的所有 按键 --
// 排列次序，几乎参照 glfw3.h 中的顺序.
// 元素值 也与 glfw3 一致，可以直接在 glfw3 函数中使用
enum class KeyBoard{
    NIL = 0, //- null
    //--------
    SPACE      = 32,
    APOSTROPHE = 39,  /* ' */
    COMMA      = 44,  /* , */
    MINUS      = 45,  /* - */
    PERIOD     = 46,  /* . */
    SLASH      = 47,  /* / */
    //--------
    NUM_0      = 48,  //-- same in ASCII --
    NUM_1, NUM_2, NUM_3, NUM_4, NUM_5, NUM_6, NUM_7, NUM_8, NUM_9,
    //--------
    SEMICOLON = 59,   /* ; */
    EQUAL     = 61,   /* = */
    //--------
    A         = 65,   //-- same in ASCII --
    B,C,D,E,F,G, 
    H,I,J,K,L,M,N,
    O,P,Q,R,S,T,
    U,V,W,X,Y,Z,
    //--------
    LEFT_BRACKET  = 91,  /* [ */
    BACKSLASH     = 92,  /* \ */
    RIGHT_BRACKET = 93,  /* ] */
    GRAVE_ACCENT  = 96,  /* ` */
    //--------
    ESCAPE     = 256,
    ENTER,
    TAB,
    BACKSPACE,
    INSERT,
    DELETE,
    RIGHT,
    LEFT,
    DOWN,
    UP,
    PAGE_UP,
    PAGE_DOWN,
    HOME,
    END,
    CAPS_LOCK  = 280,
    SCROLL_LOCK,
    NUM_LOCK,
    PRINT_SCREEN,
    PAUSE,
    //--------
    F1       = 290, 
    F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12, 
    //--------
    LEFT_SHIFT  = 340,
    LEFT_CONTROL,
    LEFT_ALT,
    LEFT_SUPER,
    RIGHT_SHIFT,
    RIGHT_CONTROL,
    RIGHT_ALT,
    RIGHT_SUPER,
    MENU,
    //--------
    /*
    MOUSE_BUTTON_1,
    MOUSE_BUTTON_2,
    MOUSE_BUTTON_3,
    MOUSE_BUTTON_4,
    MOUSE_BUTTON_5,
    MOUSE_BUTTON_6,
    MOUSE_BUTTON_7,
    MOUSE_BUTTON_8,
    //--------
    JOYSTICK_1,
    JOYSTICK_2,
    JOYSTICK_3,
    JOYSTICK_4,
    JOYSTICK_5,
    JOYSTICK_6,
    JOYSTICK_7,
    JOYSTICK_8,
    JOYSTICK_9,
    JOYSTICK_10,
    JOYSTICK_11,
    JOYSTICK_12,
    JOYSTICK_13,
    JOYSTICK_14,
    JOYSTICK_15,
    JOYSTICK_16
    */
};


#endif 


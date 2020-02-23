/*
 * ========================= KeyBoard.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.02.13
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#ifndef TPR_KEYBOARD_H
#define TPR_KEYBOARD_H

//=== *** glad FIRST, glfw SECEND *** ===
// Don't include glfw3.h ALONE!!!
#include<glad/glad.h> 
#include<GLFW/glfw3.h>

//-------------------- CPP --------------------//
#include <string>
#include <set>

//-------------------- Engine --------------------//
#include "tprAssert.h"




class KeyBoard{
public:

    enum class Key : int{
        NOT_CARE = -2, // glfw 有效的键，但不在本游戏关心范围内，一律归为本类型
        //--
        UNKNOWN = GLFW_KEY_UNKNOWN, // -1
        //---
        NIL = 0, //- null
        //--- 
        SPACE      = GLFW_KEY_SPACE,
        APOSTROPHE = GLFW_KEY_APOSTROPHE,  /* ' */
        COMMA      = GLFW_KEY_COMMA,    /* , */
        MINUS      = GLFW_KEY_MINUS,    /* - */
        PERIOD     = GLFW_KEY_PERIOD,   /* . */
        SLASH      = GLFW_KEY_SLASH,    /* / */
        SEMICOLON = GLFW_KEY_SEMICOLON, /* ; */
        EQUAL     = GLFW_KEY_EQUAL,     /* = */
        // num
        NUM_0 = GLFW_KEY_0,
        NUM_1 = GLFW_KEY_1, 
        NUM_2 = GLFW_KEY_2, 
        NUM_3 = GLFW_KEY_3, 
        NUM_4 = GLFW_KEY_4, 
        NUM_5 = GLFW_KEY_5, 
        NUM_6 = GLFW_KEY_6, 
        NUM_7 = GLFW_KEY_7, 
        NUM_8 = GLFW_KEY_8, 
        NUM_9 = GLFW_KEY_9,

        // keypad 
        KP_0 = GLFW_KEY_KP_0,
        KP_1 = GLFW_KEY_KP_1,
        KP_2 = GLFW_KEY_KP_2,
        KP_3 = GLFW_KEY_KP_3,
        KP_4 = GLFW_KEY_KP_4,
        KP_5 = GLFW_KEY_KP_5,
        KP_6 = GLFW_KEY_KP_6,
        KP_7 = GLFW_KEY_KP_7,
        KP_8 = GLFW_KEY_KP_8,
        KP_9 = GLFW_KEY_KP_9,
        KP_DIVIDE   = GLFW_KEY_KP_DIVIDE,
        KP_MULTIPLY = GLFW_KEY_KP_MULTIPLY,
        KP_SUBTRACT = GLFW_KEY_KP_SUBTRACT,
        KP_ADD      = GLFW_KEY_KP_ADD,
        KP_DECIMAL  = GLFW_KEY_KP_DECIMAL,
        KP_EQUAL    = GLFW_KEY_KP_EQUAL,
        KP_ENTER    = GLFW_KEY_KP_ENTER,
        
        //--------
        A = GLFW_KEY_A,
        B = GLFW_KEY_B,
        C = GLFW_KEY_C,
        D = GLFW_KEY_D,
        E = GLFW_KEY_E,
        F = GLFW_KEY_F,
        G = GLFW_KEY_G, 
        H = GLFW_KEY_H,
        I = GLFW_KEY_I,
        J = GLFW_KEY_J,
        K = GLFW_KEY_K,
        L = GLFW_KEY_L,
        M = GLFW_KEY_M,
        N = GLFW_KEY_N,
        O = GLFW_KEY_O,
        P = GLFW_KEY_P,
        Q = GLFW_KEY_Q,
        R = GLFW_KEY_R,
        S = GLFW_KEY_S,
        T = GLFW_KEY_T,
        U = GLFW_KEY_U,
        V = GLFW_KEY_V,
        W = GLFW_KEY_W,
        X = GLFW_KEY_X,
        Y = GLFW_KEY_Y,
        Z = GLFW_KEY_Z,
        //--------
        LEFT_BRACKET  = GLFW_KEY_LEFT_BRACKET,  /* [ */
        BACKSLASH     = GLFW_KEY_BACKSLASH,     /* \ */
        RIGHT_BRACKET = GLFW_KEY_RIGHT_BRACKET, /* ] */
        GRAVE_ACCENT  = GLFW_KEY_GRAVE_ACCENT,  /* ` */
        //--------
        ESCAPE      = GLFW_KEY_ESCAPE,
        ENTER       = GLFW_KEY_ENTER,
        TAB         = GLFW_KEY_TAB,
        BACKSPACE   = GLFW_KEY_BACKSPACE,
        INSERT      = GLFW_KEY_INSERT,
        DELETE_     = GLFW_KEY_DELETE, //- DELETE 是关键词，有冲突
        RIGHT       = GLFW_KEY_RIGHT,
        LEFT        = GLFW_KEY_LEFT,
        DOWN        = GLFW_KEY_DOWN,
        UP          = GLFW_KEY_UP,
        PAGE_UP     = GLFW_KEY_PAGE_UP,
        PAGE_DOWN   = GLFW_KEY_PAGE_DOWN,
        HOME        = GLFW_KEY_HOME,
        END         = GLFW_KEY_END,
        CAPS_LOCK   = GLFW_KEY_CAPS_LOCK,
        SCROLL_LOCK = GLFW_KEY_SCROLL_LOCK,
        NUM_LOCK    = GLFW_KEY_NUM_LOCK,
        PRINT_SCREEN = GLFW_KEY_PRINT_SCREEN,
        PAUSE       = GLFW_KEY_PAUSE,
        //--------
        F1  = GLFW_KEY_F1, 
        F2  = GLFW_KEY_F2, 
        F3  = GLFW_KEY_F3, 
        F4  = GLFW_KEY_F4, 
        F5  = GLFW_KEY_F5, 
        F6  = GLFW_KEY_F6, 
        F7  = GLFW_KEY_F7, 
        F8  = GLFW_KEY_F8, 
        F9  = GLFW_KEY_F9, 
        F10 = GLFW_KEY_F10, 
        F11 = GLFW_KEY_F11, 
        F12 = GLFW_KEY_F12,

        //--------
        LEFT_SHIFT      = GLFW_KEY_LEFT_SHIFT,
        LEFT_CONTROL    = GLFW_KEY_LEFT_CONTROL,
        LEFT_ALT        = GLFW_KEY_LEFT_ALT,
        LEFT_SUPER      = GLFW_KEY_LEFT_SUPER,
        RIGHT_SHIFT     = GLFW_KEY_RIGHT_SHIFT,
        RIGHT_CONTROL   = GLFW_KEY_RIGHT_CONTROL,
        RIGHT_ALT       = GLFW_KEY_RIGHT_ALT,
        RIGHT_SUPER     = GLFW_KEY_RIGHT_SUPER,
        MENU            = GLFW_KEY_MENU,
        //--------
    };

    //========== Selfs ==========//
    KeyBoard()=default;

    inline bool get_isAnyKeyPress()const noexcept{
        return !this->pressedKeys.empty();
    }

    inline bool get_key( KeyBoard::Key key_ )const noexcept{
        return (this->pressedKeys.find(key_) != this->pressedKeys.end());
    }

    inline void insert_key( KeyBoard::Key key_ )noexcept{
        auto [insertIt, insertBool] = this->pressedKeys.insert(key_);
        tprAssert( insertBool ); // Must success
    }

    inline void erase_key( KeyBoard::Key key_ )noexcept{
        size_t eraseNum = this->pressedKeys.erase( key_ );
        tprAssert( eraseNum == 1 ); // Must have
    }   

    inline const std::set<KeyBoard::Key> &get_pressedKeysRef()const noexcept{
        return this->pressedKeys;
    }


private:
    //========== Selfs ==========//
    std::set<KeyBoard::Key> pressedKeys {}; // 当前正被按下的 键盘按键
};


std::string keyBoardKey_2_str( KeyBoard::Key kb_ )noexcept;


// 仅用于，从 glfw 函数中获得的 key 值
inline KeyBoard::Key glfwKey_2_KeyBoardKey( int glfwKey_ )noexcept{

    switch (glfwKey_){
        case GLFW_KEY_UNKNOWN: return KeyBoard::Key::UNKNOWN;

        case GLFW_KEY_SPACE: return KeyBoard::Key::SPACE;
        case GLFW_KEY_APOSTROPHE: return KeyBoard::Key::APOSTROPHE;
        case GLFW_KEY_COMMA: return KeyBoard::Key::COMMA;
        case GLFW_KEY_MINUS: return KeyBoard::Key::MINUS;
        case GLFW_KEY_PERIOD: return KeyBoard::Key::PERIOD;
        case GLFW_KEY_SLASH: return KeyBoard::Key::SLASH;
        case GLFW_KEY_SEMICOLON: return KeyBoard::Key::SEMICOLON;
        case GLFW_KEY_EQUAL: return KeyBoard::Key::EQUAL;

        case GLFW_KEY_0: return KeyBoard::Key::NUM_0;
        case GLFW_KEY_1: return KeyBoard::Key::NUM_1;
        case GLFW_KEY_2: return KeyBoard::Key::NUM_2;
        case GLFW_KEY_3: return KeyBoard::Key::NUM_3;
        case GLFW_KEY_4: return KeyBoard::Key::NUM_4;
        case GLFW_KEY_5: return KeyBoard::Key::NUM_5;
        case GLFW_KEY_6: return KeyBoard::Key::NUM_6;
        case GLFW_KEY_7: return KeyBoard::Key::NUM_7;
        case GLFW_KEY_8: return KeyBoard::Key::NUM_8;
        case GLFW_KEY_9: return KeyBoard::Key::NUM_9;

        case GLFW_KEY_KP_0: return KeyBoard::Key::KP_0;
        case GLFW_KEY_KP_1: return KeyBoard::Key::KP_1;
        case GLFW_KEY_KP_2: return KeyBoard::Key::KP_2;
        case GLFW_KEY_KP_3: return KeyBoard::Key::KP_3;
        case GLFW_KEY_KP_4: return KeyBoard::Key::KP_4;
        case GLFW_KEY_KP_5: return KeyBoard::Key::KP_5;
        case GLFW_KEY_KP_6: return KeyBoard::Key::KP_6;
        case GLFW_KEY_KP_7: return KeyBoard::Key::KP_7;
        case GLFW_KEY_KP_8: return KeyBoard::Key::KP_8;
        case GLFW_KEY_KP_9: return KeyBoard::Key::KP_9;

        case GLFW_KEY_KP_DIVIDE: return KeyBoard::Key::KP_DIVIDE;
        case GLFW_KEY_KP_MULTIPLY: return KeyBoard::Key::KP_MULTIPLY;
        case GLFW_KEY_KP_SUBTRACT: return KeyBoard::Key::KP_SUBTRACT;
        case GLFW_KEY_KP_ADD: return KeyBoard::Key::KP_ADD;
        case GLFW_KEY_KP_DECIMAL: return KeyBoard::Key::KP_DECIMAL;
        case GLFW_KEY_KP_EQUAL: return KeyBoard::Key::KP_EQUAL;
        case GLFW_KEY_KP_ENTER: return KeyBoard::Key::KP_ENTER;

        case GLFW_KEY_A: return KeyBoard::Key::A;
        case GLFW_KEY_B: return KeyBoard::Key::B;
        case GLFW_KEY_C: return KeyBoard::Key::C;
        case GLFW_KEY_D: return KeyBoard::Key::D;
        case GLFW_KEY_E: return KeyBoard::Key::E;
        case GLFW_KEY_F: return KeyBoard::Key::F;
        case GLFW_KEY_G: return KeyBoard::Key::G;
        case GLFW_KEY_H: return KeyBoard::Key::H;
        case GLFW_KEY_I: return KeyBoard::Key::I;
        case GLFW_KEY_J: return KeyBoard::Key::J;
        case GLFW_KEY_K: return KeyBoard::Key::K;
        case GLFW_KEY_L: return KeyBoard::Key::L;
        case GLFW_KEY_M: return KeyBoard::Key::M;
        case GLFW_KEY_N: return KeyBoard::Key::N;
        case GLFW_KEY_O: return KeyBoard::Key::O;
        case GLFW_KEY_P: return KeyBoard::Key::P;
        case GLFW_KEY_Q: return KeyBoard::Key::Q;
        case GLFW_KEY_R: return KeyBoard::Key::R;
        case GLFW_KEY_S: return KeyBoard::Key::S;
        case GLFW_KEY_T: return KeyBoard::Key::T;
        case GLFW_KEY_U: return KeyBoard::Key::U;
        case GLFW_KEY_V: return KeyBoard::Key::V;
        case GLFW_KEY_W: return KeyBoard::Key::W;
        case GLFW_KEY_X: return KeyBoard::Key::X;
        case GLFW_KEY_Y: return KeyBoard::Key::Y;
        case GLFW_KEY_Z: return KeyBoard::Key::Z;

        case GLFW_KEY_LEFT_BRACKET: return KeyBoard::Key::LEFT_BRACKET;
        case GLFW_KEY_BACKSLASH: return KeyBoard::Key::BACKSLASH;
        case GLFW_KEY_RIGHT_BRACKET: return KeyBoard::Key::RIGHT_BRACKET;
        case GLFW_KEY_GRAVE_ACCENT: return KeyBoard::Key::GRAVE_ACCENT;

        case GLFW_KEY_ESCAPE: return KeyBoard::Key::ESCAPE; // 不支持游戏按键映射

        case GLFW_KEY_ENTER: return KeyBoard::Key::ENTER;
        case GLFW_KEY_TAB: return KeyBoard::Key::TAB;
        case GLFW_KEY_BACKSPACE: return KeyBoard::Key::BACKSPACE;
        case GLFW_KEY_INSERT: return KeyBoard::Key::INSERT;
        case GLFW_KEY_DELETE: return KeyBoard::Key::DELETE_;

        case GLFW_KEY_RIGHT: return KeyBoard::Key::RIGHT;
        case GLFW_KEY_LEFT: return KeyBoard::Key::LEFT;
        case GLFW_KEY_DOWN: return KeyBoard::Key::DOWN;
        case GLFW_KEY_UP: return KeyBoard::Key::UP;

        case GLFW_KEY_PAGE_UP: return KeyBoard::Key::PAGE_UP;
        case GLFW_KEY_PAGE_DOWN: return KeyBoard::Key::PAGE_DOWN;
        case GLFW_KEY_HOME: return KeyBoard::Key::HOME;
        case GLFW_KEY_END: return KeyBoard::Key::END;
        
        //case GLFW_KEY_CAPS_LOCK: return KeyBoard::Key::CAPS_LOCK;      // 不支持游戏按键映射
        //case GLFW_KEY_SCROLL_LOCK: return KeyBoard::Key::SCROLL_LOCK;  // 不支持游戏按键映射
        //case GLFW_KEY_NUM_LOCK: return KeyBoard::Key::NUM_LOCK;        // 不支持游戏按键映射
        //case GLFW_KEY_PRINT_SCREEN: return KeyBoard::Key::PRINT_SCREEN;// 不支持游戏按键映射
        //case GLFW_KEY_PAUSE: return KeyBoard::Key::PAUSE;              // 不支持游戏按键映射

        case GLFW_KEY_F1: return KeyBoard::Key::F1;
        case GLFW_KEY_F2: return KeyBoard::Key::F2;
        case GLFW_KEY_F3: return KeyBoard::Key::F3;
        case GLFW_KEY_F4: return KeyBoard::Key::F4;
        case GLFW_KEY_F5: return KeyBoard::Key::F5;
        case GLFW_KEY_F6: return KeyBoard::Key::F6;
        case GLFW_KEY_F7: return KeyBoard::Key::F7;
        case GLFW_KEY_F8: return KeyBoard::Key::F8;
        case GLFW_KEY_F9: return KeyBoard::Key::F9;
        case GLFW_KEY_F10: return KeyBoard::Key::F10;
        case GLFW_KEY_F11: return KeyBoard::Key::F11;
        case GLFW_KEY_F12: return KeyBoard::Key::F12;

        case GLFW_KEY_LEFT_SHIFT: return KeyBoard::Key::LEFT_SHIFT;
        case GLFW_KEY_LEFT_CONTROL: return KeyBoard::Key::LEFT_CONTROL;
        case GLFW_KEY_LEFT_ALT: return KeyBoard::Key::LEFT_ALT;
        case GLFW_KEY_LEFT_SUPER: return KeyBoard::Key::LEFT_SUPER;
        case GLFW_KEY_RIGHT_SHIFT: return KeyBoard::Key::RIGHT_SHIFT;
        case GLFW_KEY_RIGHT_CONTROL: return KeyBoard::Key::RIGHT_CONTROL;
        case GLFW_KEY_RIGHT_ALT: return KeyBoard::Key::RIGHT_ALT;
        case GLFW_KEY_RIGHT_SUPER: return KeyBoard::Key::RIGHT_SUPER;
        case GLFW_KEY_MENU: return KeyBoard::Key::MENU;

        case 0: 
            // 不存在 0 值
            tprAssert(0);
            return KeyBoard::Key::NIL; // never reach

        default:
            // glfw 认可的键，但本游戏不关心
            return KeyBoard::Key::NOT_CARE;
    }
}











#endif 


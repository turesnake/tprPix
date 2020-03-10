/*
 * ======================= esrc_window.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.06.03
 *                                        MODIFY --
 * ----------------------------------------------------------
 */
#include "pch.h"
#include "esrc_window.h"

namespace esrc {//------------------ namespace: esrc -------------------------//


namespace window_inn {//-------- namespace: window_inn --------------//

    //-- 一个 类实例 维护一个 window。
    //-- window 数据结构的 具体内容由 glfw库管理。
    //-- 用户只需保存一个指针。用来访问这个 window。
    GLFWwindow  *windowPtr {};

}//------------- namespace: window_inn end --------------//

GLFWwindow *get_windowPtr(){
    return window_inn::windowPtr;
}

void set_windowPtr( GLFWwindow *newPtr_ ){
    window_inn::windowPtr = newPtr_;
}

}//---------------------- namespace: esrc -------------------------//

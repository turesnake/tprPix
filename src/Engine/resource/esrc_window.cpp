/*
 * ======================= esrc_window.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.06.03
 *                                        MODIFY --
 * ----------------------------------------------------------
 */
#include "esrc_window.h"

namespace esrc {//------------------ namespace: esrc -------------------------//


namespace {//-------- namespace: --------------//

    //-- 一个 类实例 维护一个 window。
    //-- window 数据结构的 具体内容由 glfw库管理。
    //-- 用户只需保存一个指针。用来访问这个 window。
    GLFWwindow  *windowPtr {};

}//------------- namespace: end --------------//

GLFWwindow *get_windowPtr(){
    return windowPtr;
}

void set_windowPtr( GLFWwindow *_newPtr ){
    windowPtr = _newPtr;
}

}//---------------------- namespace: esrc -------------------------//

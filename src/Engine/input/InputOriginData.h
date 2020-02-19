/*
 * ======================== InputOriginData.h ==========================
 *                          -- tpr --
 *                                        创建 -- 2020.01.25
 *                                        修改 -- 
 * ----------------------------------------------------------
 * 仅在 firstPlayInputSet 阶段启用。
 * 每一帧，都完整地读取，所有支持地 外设的 数据
 */
#ifndef TPR_INPUT_ORIGIN_DATA_H
#define TPR_INPUT_ORIGIN_DATA_H

//--- glm - 0.9.9.5 ---
#include "glm_no_warnings.h"


//-------------------- CPP --------------------//
#include <string>
#include <vector>
#include <set>
#include <memory>
#include <unordered_map>

//-------------------- Engine --------------------//
#include "tprAssert.h"

#include "KeyBoard.h"
#include "Joystick.h"
#include "Mouse.h"



class InputOriginData{
public:
    InputOriginData()=default;

    // 读取系统中，所有连接的，支持 SDL_GameControllerDB 的 手柄信息
    std::unordered_map<int, std::unique_ptr<Joystick>> joysticks {};
    Mouse       mouse {};
    KeyBoard    keyboard {};
};



// GLFW_RELEASE == 0
// GLFW_PRESS == 1
// GLFW_REPEAT == 2  本系列函数无视  GLFW_REPEAT
inline bool is_GLFW_PRESS( int glfwRet_ )noexcept{
    return (glfwRet_==GLFW_PRESS) ? true : false;
}
inline bool is_GLFW_PRESS( unsigned char glfwRet_ )noexcept{
    return (glfwRet_==GLFW_PRESS) ? true : false;
}


#endif 


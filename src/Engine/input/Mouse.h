/*
 * ========================= Mouse.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2020.01.27
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#ifndef TPR_Mouse_H
#define TPR_Mouse_H
//--- glm - 0.9.9.5 ---
#include "glm_no_warnings.h"



class Mouse{
public:
    Mouse()=default;
    glm::dvec2  cursorDPos {};
    bool        leftButton {};  // true: press
    bool        rightButton {}; // true: press
    bool        midButton   {}; // true: press
    // 屏蔽鼠标滚轮 功能 ...
};


#endif 


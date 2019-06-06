/*
 * ===================== JoyStickButton.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.05.13
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#ifndef TPR_JOY_STICK_BUTTON_H_
#define TPR_JOY_STICK_BUTTON_H_


//-- 目前的 次序，完全按照 北通pc手柄 --
enum class JoyStickButton : int{
    BACK = 0,
    //--
    AXES_L_MID,   
    AXES_R_MID,   
    //--
    START,  
    //--
    HAT_UP,    
    HAT_RIGHT,   
    HAT_DOWN,    
    HAT_LEFT,  
    //--
    L2,   
    R2,   
    L1,   
    R1,    
    //--
    Y,  
    B,   
    A,   
    X,  
    //--
    HOME,
    //--
    NIL = -1 //- null
};


#endif 


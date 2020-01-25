/*
 * ===================== JoyStickButton.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.05.13
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#ifndef TPR_JOY_STICK_BUTTON_H
#define TPR_JOY_STICK_BUTTON_H


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


// mac 上的 国产手柄，
// 检测测试版数据 tmp ...
/*
enum class JoyStickButton : int{

    HAT_UP = 0,  
    HAT_DOWN = 1,  
    HAT_LEFT = 2,
    HAT_RIGHT = 3,   
        
    START = 4, 
    BACK = 5,

    //--
    AXES_L_MID = 6,   
    AXES_R_MID = 7,   
    //--
     
    L1 = 8,   
    R1 = 9,

    HOME = 10,

    A = 12,   
    B = 13, 
    X = 14,
    Y = 15, 
     
    L2,   
    R2,   
        
    //--

    //--
    
    //--
    NIL = -1 //- null
};
*/









#endif 


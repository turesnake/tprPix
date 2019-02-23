/*
 * ========================= EcoSys.h =======================
 *                          -- tpr --
 *                                        CREATE -- 2019.02.22
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *    ecosystem 
 * ----------------------------
 */
#ifndef _TPR_ECOSYS_H_
#define _TPR_ECOSYS_H_


//-------------------- Engine --------------------//
#include "RGBA.h" 


//-- 一种 生态群落 --
//  简易版，容纳最基础的数据
//  在未来一点点丰富细节
class EcoSys{
public:

    //-- 根据 每个 mapent 的 alti，填上对应的颜色 --
    RGBA  color_low  {}; //- alti: 0 - 7 (8)
    RGBA  color_mid  {}; //- alti: 8 -10 (3)
    RGBA  color_high {}; //- alti: 11-15 (5) 

private:
};


#endif 


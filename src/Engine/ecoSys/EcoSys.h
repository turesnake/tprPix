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

    RGBA  color_underWater {}; //- 水下的地面颜色(会在这个颜色上做 水色叠加)

    //-- 用于计算 pix.alti --
    //  注意，下面这些值 都不能 硬使用，而是一个 “变化趋向值”
    float  altiPerlin_freqBig;
    float  altiPerlin_freqSml;

    float  seaLvl {0}; // [-100,100] 
                        // 海平面高度，
                        // 值越低，地图中 land 比例越大
                        // 值越高，地图中 water 比例越大

private:
};


#endif 


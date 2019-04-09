/*
 * ====================== Density.h =======================
 *                          -- tpr --
 *                                        CREATE -- 2019.03.23
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *  密度 一个 世界区域划分规则。
 *  密度将游戏空间划分为 数段等高线（>=0 为 高地，<0 为低地）
 *  只有 field 才需要记录自己的 density 值
 * ----------------------------
 */
#ifndef _TPR_DENSITY_H_
#define _TPR_DENSITY_H_


//------------------- Engine --------------------//
#include "IntVec.h" 


//--- [mem] ---
class Density{
public:
    Density() = default;
    void set( const IntVec2 &_fieldMPos );

    //===== vals =====//
    int lvl {0}; // [-3, 3]
};
 

#endif 


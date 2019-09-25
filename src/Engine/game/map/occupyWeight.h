/*
 * ========================== occupyWeight.h =======================
 *                          -- tpr --
 *                                        CREATE -- 2019.03.22
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *   那些通过 "覆盖法／抢占法" 来对规划一个区域的 模块，
 *   需要一种机制来区分 区域四顶点 的 “抢占强势程度”。
 *   4顶点 的 occupyWeight 值不应该 出现相同。
 * ------------------------
 */
#ifndef TPR_OCCUPY_WEIGHT_H
#define TPR_OCCUPY_WEIGHT_H

//-------------------- Engine --------------------//
#include "IntVec.h"


using occupyWeight_t = int;

occupyWeight_t calc_occupyWeight( IntVec2 oddEven_, size_t idx_ );

#endif


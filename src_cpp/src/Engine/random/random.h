/*
 * ========================= random.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2018.11.29
 *                                        MODIFY --
 * ----------------------------------------------------------
 *    随机数 模块
 * ----------------------------
 */
#ifndef TPR_RANDOM_H
#define TPR_RANDOM_H

//------------------- CPP --------------------//
#include <random>

//------------------- Libs --------------------//
#include "tprDataType.h" 


std::default_random_engine &get_dRandEng();
u32_t get_new_seed();

#endif

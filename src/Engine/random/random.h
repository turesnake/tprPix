/*
 * ========================= random.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2018.11.29
 *                                        MODIFY --
 * ----------------------------------------------------------
 *    随机数 模块
 * ----------------------------
 */
#ifndef _TPR_RANDOM_H_
#define _TPR_RANDOM_H_

#include <random>

std::default_random_engine &get_dRandEng();
unsigned int get_new_seed();

#endif

/*
 * ===================== calc_colliPoints.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.11.17
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#ifndef TPR_CALC_COLLI_POINTS_H
#define TPR_CALC_COLLI_POINTS_H

//--- glm - 0.9.9.5 ---
#include "glm_no_warnings.h"

//-------------------- CPP --------------------//
#include <vector>


void calc_colliPoints_for_circular(  std::vector<glm::dvec2> &container_, double radius_ );

#endif 


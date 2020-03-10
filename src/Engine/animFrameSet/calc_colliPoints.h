/*
 * ===================== calc_colliPoints.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.11.17
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#ifndef TPR_CALC_COLLI_POINTS_H
#define TPR_CALC_COLLI_POINTS_H

#include "pch.h"


void calc_colliPoints_for_circular(  std::vector<glm::dvec2> &container_, double radius_ );

const std::vector<glm::dvec2> &get_colliPointDPosOffsRef_for_cirDogo()noexcept;


#endif 


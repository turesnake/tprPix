/*
 * ====================== circularCast.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.08.30
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *   
 * ----------------------------
 */
#ifndef TPR_CIRCULAR_CAST_H
#define TPR_CIRCULAR_CAST_H

//--- glm - 0.9.9.5 ---
#include "glm_no_warnings.h"

//-------------------- C --------------------//
#include <cmath>



/* ===========================================================
 *                 circularCast   [traditional solution]
 * -----------------------------------------------------------
 *  if collision has been confirmed between bego and dogo.
 *  call this func to calculate the colliPoint in motion vector.
 *  ---
 *  return:
 *    t is a scale
 */
inline double circularCast( const glm::dvec2 &to_bego_,
                            const glm::dvec2 &to_target_,
                            double sum_of_2_CircularRadius_ ){

    double cosValue = glm::dot( glm::normalize(to_bego_), 
                                glm::normalize(to_target_) ); //- cos值
    double sinValue = sin( acos(cosValue) );
    //---
    double to_bego_len = glm::length( to_bego_ );
    double to_VerticalPoint_len = to_bego_len * cosValue;
    double verticalLine_len     = to_bego_len * sinValue;
    double colliPoint_2_verticalPoint_len = 
        sqrt(sum_of_2_CircularRadius_*sum_of_2_CircularRadius_ - verticalLine_len*verticalLine_len);
    double to_colliPoint_len = to_VerticalPoint_len - colliPoint_2_verticalPoint_len;

    return (to_colliPoint_len / glm::length(to_target_));
}










#endif 


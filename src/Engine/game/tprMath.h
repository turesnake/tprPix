/*
 * ======================== tprMath.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.08.21
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#ifndef TPR_MATH_H
#define TPR_MATH_H


#include <cmath>


#ifndef TPR_PI
#define TPR_PI 3.14159
#endif

#ifndef TPR_2PI
#define TPR_2PI 6.28319
#endif



inline bool is_closeEnough( double a_, double b_, double threshold_=0.01 ){
    return ( std::abs(a_-b_) < threshold_ );
}







#endif 


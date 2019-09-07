/*
 * ======================= tprCast.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.06.09
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 * type cast
 * ---------------------
 */
#ifndef TPR_CAST_H
#define TPR_CAST_H

#include "tprAssert.h"


/* ===========================================================
 *                    cast_2_size_t
 * -----------------------------------------------------------
 * signed_integer -> size_t
 * 如果参数为负，直接 tprAssert 退出
 */
inline size_t assert_and_return_( const char *file_, int line_ )noexcept{
    tprAssert_inn(0, file_,  line_);
    return 0;
}
#define cast_2_size_t(e)  (((e)>=0) ? static_cast<size_t>(e) : assert_and_return_(__FILE__, __LINE__))
                            // MUST wrap a "(...)" outside !!!


#endif 


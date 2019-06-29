/*
 * ======================= tprCast.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.06.09
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 * type cast
 * ---------------------
 */
#ifndef TPR_CAST_H_
#define TPR_CAST_H_

#include "tprAssert.h"


/* ===========================================================
 *                    to_size_t_cast
 * -----------------------------------------------------------
 * signed_integer -> size_t
 * 如果参数为负，直接 tprAssert 退出
 */
inline size_t assert_and_return_( const char *file_, int line_ ){
    tprAssert_inn(0, file_,  line_);
    return 0;
}
#define to_size_t_cast(e)  ((e)>=0) ? static_cast<size_t>(e) : assert_and_return_(__FILE__, __LINE__)



#endif 


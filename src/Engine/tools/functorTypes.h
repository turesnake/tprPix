/*
 * ========================= functorTypes.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.10.20
 *                                        MODIFY --
 * ----------------------------------------------------------
 */
#ifndef TPR_FUNCTOR_TYPES_H
#define TPR_FUNCTOR_TYPES_H
//--- glm - 0.9.9.5 ---
#include "glm_no_warnings.h"

//-------------------- CPP --------------------//
#include <functional>


using F_void            = std::function<void()>;
using F_bool            = std::function<bool()>;

using F_R_int           = std::function<int()>;
using F_R_double        = std::function<double()>;

using F_R_void_P_double = std::function<void( double )>;

using F_R_c_dvec2Ref    = std::function<const glm::dvec2 &()>;
using F_P_c_dvec2Ref    = std::function<void(const glm::dvec2 &)>;



#endif 


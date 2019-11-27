/*
 * ===================== create_go_oth.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.04.12
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *  通用的 辅助函数
 * ---------------
 */
#ifndef TPR_CREATE_GO_OTH_H
#define TPR_CREATE_GO_OTH_H

//-------------------- CPP --------------------//
#include <string>

//-------------------- Engine --------------------//
#include "tprAssert.h"
#include "tprCast.h"
#include "IntVec.h"
#include "GameObjType.h"
#include "Density.h"


namespace gameObjs{//------------- namespace gameObjs ----------------


/* ===========================================================
 *                   apply_isFlipOver
 * -----------------------------------------------------------
 * 是否左右翻转
 * param: fieldWeight_ -- [-100.0, 100.0]
 */
/*
inline bool apply_isFlipOver( double fieldWeight_ ){
    size_t randV = cast_2_size_t(floor(fieldWeight_ * 3.1 + 911.3));
    return ((randV%10)<5);
}
*/


/* ===========================================================
 *                   apply_isSingleTRunk
 * -----------------------------------------------------------
 * 树，主干是否分叉
 * param: fieldWeight_ -- [-100.0, 100.0]
 */
/*
inline bool apply_isSingleTRunk( double fieldWeight_ )noexcept{
    size_t randV = cast_2_size_t(floor(fieldWeight_ * 3.7 + 701.7));
    return ((randV%10)<5);
}
*/


/* ===========================================================
 *                     apply_a_oakId   tmp
 * -----------------------------------------------------------
 * 这组方法很临时。不够好...
 * param: fieldWeight_ -- [-100.0, 100.0]
 */
/*
inline size_t apply_a_simpleId( double fieldWeight_, size_t _totalNum )noexcept{
    size_t randV = cast_2_size_t(floor(fieldWeight_ * 5.3 + 977.1));
    return randV % _totalNum;
}
*/


/* ===========================================================
 *             apply_treeAge_by_density  tmp
 * -----------------------------------------------------------
 */
inline int apply_treeAge_by_density( Density _density )noexcept{
    switch( _density.get_lvl() ){
        case -3: return 3; //- tmp
        case -2: return 2; //- tmp
        case -1: return 1; //- tmp

        case 1: return 1;
        case 2: return 2;
        case 3: return 3;
        default:
            tprAssert(0);
            return 3; //- never reach
    }
}



inline size_t apply_randUVal_in_range( size_t randUVal_, size_t begVal_, size_t endVal_,
                                        bool isIncludebeg_=true, 
                                        bool isIncludeEnd_=true  )noexcept{
    size_t fst = isIncludebeg_ ? begVal_ : (begVal_+1); // include fst
    size_t lst = isIncludeEnd_ ? endVal_ : (endVal_-1); // include lst
    size_t off = lst - fst + 1; // include lst
    size_t ret = ((randUVal_+25533) % off) + fst;
    return ret;
}






}//------------- namespace gameObjs: end ----------------
#endif 


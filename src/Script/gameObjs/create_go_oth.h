/*
 * ===================== create_go_oth.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.04.12
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *  通用的 辅助函数
 * ---------------
 */
#ifndef _TPR_CREATE_GO_OTH_H_
#define _TPR_CREATE_GO_OTH_H_


//-------------------- Engine --------------------//
#include "IntVec.h"
#include "GameObjType.h"
#include "Density.h"


namespace gameObjs{//------------- namespace gameObjs ----------------



/* ===========================================================
 *                   apply_isFlipOver
 * -----------------------------------------------------------
 * 是否左右翻转
 * param: _fieldWeight -- [-100.0, 100.0]
 */
inline bool apply_isFlipOver( float _fieldWeight ){
    int randV = static_cast<int>(floor(_fieldWeight)) * 3 + 911;
    return ((randV%10)<5);
}


/* ===========================================================
 *                   apply_isSingleTRunk
 * -----------------------------------------------------------
 * 树，主干是否分叉
 * param: _fieldWeight -- [-100.0, 100.0]
 */
inline bool apply_isSingleTRunk( float _fieldWeight ){
    int randV = static_cast<int>(floor(_fieldWeight)) + 701;
    return ((randV%10)<5);
}


/* ===========================================================
 *                     apply_a_oakId   tmp
 * -----------------------------------------------------------
 * 这组方法很临时。不够好...
 * param: _fieldWeight -- [-100.0, 100.0]
 */
inline int apply_a_simpleId( float _fieldWeight, int _totalNum ){
    int randV = static_cast<int>(floor(_fieldWeight)) * 3 + 977;
    return randV % _totalNum;
}


/* ===========================================================
 *             apply_treeAge_by_density  tmp
 * -----------------------------------------------------------
 */
inline int apply_treeAge_by_density( const Density &_density ){
    switch( _density.lvl ){
        case 1: return 1;
        case 2: return 2;
        case 3: return 3;
        default:
            assert(0);
            return 3; //- never reach
    }
}








}//------------- namespace gameObjs: end ----------------
#endif 


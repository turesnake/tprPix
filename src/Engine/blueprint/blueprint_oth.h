/*
 * ===================== blueprint_oth.h =======================
 *                          -- tpr --
 *                                        CREATE -- 2019.12.02
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#ifndef TPR_BLUE_PRINT_OTH_H
#define TPR_BLUE_PRINT_OTH_H


//-------------------- Engine --------------------//
#include "blueprintId.h"
#include "GameObjType.h"
#include "AnimLabel.h"
#include "NineDirection.h"
#include "BrokenLvl.h"
#include "IntVec.h"
#include "config.h"
#include "tprAssert.h"


namespace blueprint {//------------------ namespace: blueprint start ---------------------//


// 为了简化实现，yard 强制规定，必须为 正方形 
enum class YardSize{
    _1m1=1,
    _2m2=2,
    _3m3=3,
    _4m4=4,
    _5m5=5,
    _6m6=6,
};


inline YardSize sizeByMapEnt_2_yardSize( IntVec2 size_ )noexcept{

    tprAssert(  (size_.x == size_.y) &&
                (size_.x > 0) &&
                ((size_.x % ENTS_PER_FIELD) == 0) );
    
    switch ( size_.x / ENTS_PER_FIELD ){
        case 1: return YardSize::_1m1;
        case 2: return YardSize::_2m2;
        case 3: return YardSize::_3m3;
        case 4: return YardSize::_4m4;
        case 5: return YardSize::_5m5;
        case 6: return YardSize::_6m6;
        default:
            tprAssert(0); // 值太大了，暂未支持
            return YardSize::_6m6; // never reach
    }
}



//-- 获得不同尺寸的 yard 的边长 （以 mapent 为单位）
inline int yardSize_2_sideLen( YardSize type_ )noexcept{
    return static_cast<int>(type_) * ENTS_PER_FIELD;
}




IntVec2 parse_png(  std::vector<MapData> &mapDatasRef_,
                    const std::string &pngPath_M_,
                    IntVec2 frameNum_,
                    size_t totalFrameNum_,
                    bool isVillage_ );



}//--------------------- namespace: blueprint end ------------------------//
#endif 


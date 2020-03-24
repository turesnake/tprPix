/*
 * ===================== blueprint_oth.h =======================
 *                          -- tpr --
 *                                        CREATE -- 2019.12.02
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#ifndef TPR_BLUE_PRINT_OTH_H
#define TPR_BLUE_PRINT_OTH_H

#include "pch.h"

//-------------------- Engine --------------------//
#include "blueprintId.h"
#include "GameObjType.h"
#include "NineDirection.h"
#include "BrokenLvl.h"


namespace blueprint {//------------------ namespace: blueprint start ---------------------//


enum class BlueprintType{
    Plot,
    Yard,
    Village
};


// 为了简化实现，yard 强制规定，必须为 正方形,
// 且必须对其与 field 
enum class YardSize{
    _1f1 = 1,
    _2f2 = 2,
    _3f3 = 3,
    _4f4 = 4,
    _5f5 = 5,
    _6f6 = 6,
};


inline YardSize sizeByMapEnt_2_yardSize( IntVec2 size_ )noexcept{

    tprAssert(  (size_.x == size_.y) &&
                (size_.x > 0) &&
                ((size_.x % ENTS_PER_FIELD<>) == 0) );
    
    switch ( size_.x / ENTS_PER_FIELD<> ){
        case 1: return YardSize::_1f1;
        case 2: return YardSize::_2f2;
        case 3: return YardSize::_3f3;
        case 4: return YardSize::_4f4;
        case 5: return YardSize::_5f5;
        case 6: return YardSize::_6f6;
        default:
            tprAssert(0); // 值太大了，暂未支持
            return YardSize::_6f6; // never reach
    }
}


//-- 获得不同尺寸的 yard 的边长 （以 mapent 为单位）
inline int yardSize_2_mapEnt_sideLen( YardSize type_ )noexcept{
    return static_cast<int>(type_) * ENTS_PER_FIELD<>;
}




IntVec2 parse_png_for_plot(  std::vector<MapData> &mapDatasRef_,
                    const std::string &pngPath_M_,
                    IntVec2 frameNum_,
                    size_t totalFrameNum_ );


class YardBlueprint;

IntVec2 parse_png_for_yard(  YardBlueprint &yardRef_,
                        const std::string &pngPath_M_,
                        const std::vector<size_t> &frameAllocateTimes_,
                        IntVec2 frameNum_,
                        size_t totalFrameNum_,
                        size_t fstFrameIdx_,
                        size_t frameNums_ );

IntVec2 parse_png_for_village(  std::vector<MapData> &mapDatasRef_,
                    const std::string &pngPath_M_,
                    IntVec2 frameNum_,
                    size_t totalFrameNum_,
                    bool isHaveRoad );



// "" / "Default", 将被统一替换为 "DEFAULT" 
// 以此来表示，一种 默认 label 
// 不考虑 运行性能
inline std::string check_and_unify_default_labels( const std::string &label_ )noexcept{
    if( (label_=="") || 
        (label_=="default") ||
        (label_=="Default") ||
        (label_=="DEFAULT") ){
        return "_DEFAULT_";
    }else{
        return label_; // copy，无需考虑性能
    }
}


}//--------------------- namespace: blueprint end ------------------------//
#endif 


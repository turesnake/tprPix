/*
 * ====================== EcoObjBorder.h =======================
 *                          -- tpr --
 *                                        CREATE -- 2020.02.14
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#ifndef TPR_ECO_OBJ_BORDER_H
#define TPR_ECO_OBJ_BORDER_H

#include "pch.h"

//-------------------- CPP --------------------//
#include <tuple>

//-------------------- Engine --------------------//
#include "NineDirection.h"



// 单帧 位图数据，可以分配 ecoObj-边界（mapent为单位）
// 
class EcoObjBorder{
public:
    EcoObjBorder()
        {
            this->frame.resize( ENTS_PER_SECTION<size_t> * ENTS_PER_SECTION<size_t>, NineDirection::Center );
        }

    
    // param: mapent 相对与 ecoobj 左下角的 mposOff
    inline NineDirection assign_mapent_to_nearFour_ecoObjs_dir( IntVec2 mposOff_ )const noexcept{
        tprAssert(  (mposOff_.x>=0) && (mposOff_.x<ENTS_PER_SECTION<>) &&
                    (mposOff_.y>=0) && (mposOff_.y<ENTS_PER_SECTION<>) );
        size_t idx = cast_2_size_t(mposOff_.y * ENTS_PER_SECTION<> + mposOff_.x);
        return this->frame.at( idx );
    }

    
    //======= static =======//

    // 从 ecoObjBorder.png 读取数据
    static void init();

    inline static const EcoObjBorder *apply_rand_ecoObjBorderPtr( size_t uWeight_ )noexcept{
        size_t idx = ( uWeight_ + 1504471 ) % EcoObjBorder::frames.size();
        return EcoObjBorder::frames.at(idx).get();
    }

private:
    static std::tuple<IntVec2, size_t, std::string> parse_plotJsonFile();

    inline static EcoObjBorder &create_new_ecoObjBorder(){
        EcoObjBorder::frames.push_back( std::make_unique<EcoObjBorder>() );
        return *(EcoObjBorder::frames.back());
    }

    //----- vals -----//
    std::vector<NineDirection> frame {}; // 单帧数据

    //======= static =======//
    static std::vector<std::unique_ptr<EcoObjBorder>> frames; // 存储 若干帧
};





#endif 


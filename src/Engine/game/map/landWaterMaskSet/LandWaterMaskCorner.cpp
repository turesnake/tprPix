/*
 * ==================== LandWaterMaskCorner.cpp =======================
 *                          -- tpr --
 *                                        CREATE -- 2019.03.13
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *  
 * ----------------------------
 */
#include "LandWaterMaskCorner.h"

//-------------------- CPP --------------------//
#include <unordered_map>

//-------------------- Engine --------------------//
#include "random.h"


namespace{//-------- namespace: --------------//

    std::default_random_engine  randEngine; //-通用 随机数引擎实例
    std::uniform_int_distribution<size_t> uDistribution( 0, 999 ); 
    bool  is_first {true};

    //-- 几个 数据容器，被每个 实例使用 --//
    //- 原版数据 做xy轴翻转
    std::vector<LandWaterMaskEnt> originData {};
    std::vector<LandWaterMaskEnt> originData_Xflip {};
    std::vector<LandWaterMaskEnt> originData_Yflip {};
    std::vector<LandWaterMaskEnt> originData_XYflip {};

    //- 原版数据 沿 "y=x" 轴线翻转后，再做xy轴翻转
    std::vector<LandWaterMaskEnt> tiltData {}; 
    std::vector<LandWaterMaskEnt> tiltData_Xflip {};
    std::vector<LandWaterMaskEnt> tiltData_Yflip {};
    std::vector<LandWaterMaskEnt> tiltData_XYflip {};

    //-- 预制件 资源 ---
    std::unordered_map<landWaterMaskCornerId_t, LandWaterMaskCorner> landWaterMaskCorners {};

    //====== funcs =======//
    void handle_each_container( const std::vector<LandWaterMaskEnt> &_container );

}//------------- namespace: end --------------//


/* ===========================================================
 *             clear_for_LandWaterMaskCorner
 * -----------------------------------------------------------
 */
void clear_for_LandWaterMaskCorner(){
    originData.clear();
    originData_Xflip.clear();
    originData_Yflip.clear();
    originData_XYflip.clear();
    tiltData.clear();
    tiltData_Xflip.clear();
    tiltData_Yflip.clear();
    tiltData_XYflip.clear();
}

/* ===========================================================
 *       push_back_originData_for_LandWaterMaskCorner
 * -----------------------------------------------------------
 * -- 将原版数据，暂存在 文件容器中
 */
void push_back_originData_for_LandWaterMaskCorner( const LandWaterMaskEnt &_ent ){
    originData.push_back( _ent ); //- copy
}


/* ===========================================================
 *     build_all_mutant_datas_for_LandWaterMaskCorner
 * -----------------------------------------------------------
 * -- 根据原始数据，通过各种翻转，生成所有 变种 数据。
 */
void build_all_mutant_datas_for_LandWaterMaskCorner(){

    if( is_first ){
        is_first = false;
        randEngine.seed( get_new_seed() ); //- tmp
    }

    IntVec2   XYflipEntWH;  //- 原始数据 xy翻转后的 相对坐标 
    IntVec2   tiltEntWH;    //- 原始数据 沿 "y=x" 轴翻转后 相对坐标 
    IntVec2   tiltXYflipEntWH;

    for( const auto &entRef : originData ){ //- each ent in frame
        const IntVec2 &originEntWH = entRef.lMPosOff;

        XYflipEntWH.x = ENTS_PER_LANDWATERMASK - 1 - originEntWH.x;
        XYflipEntWH.y = ENTS_PER_LANDWATERMASK - 1 - originEntWH.y;

        tiltEntWH.x = originEntWH.y;
        tiltEntWH.y = originEntWH.x;

        tiltXYflipEntWH.x = ENTS_PER_LANDWATERMASK - 1 - tiltEntWH.x;
        tiltXYflipEntWH.y = ENTS_PER_LANDWATERMASK - 1 - tiltEntWH.y;
        
        originData_Xflip.push_back( LandWaterMaskEnt{ XYflipEntWH.x, originEntWH.y, entRef.is_major } ); //- copy
        originData_Yflip.push_back( LandWaterMaskEnt{ originEntWH.x, XYflipEntWH.y, entRef.is_major } ); //- copy
        originData_XYflip.push_back( LandWaterMaskEnt{ XYflipEntWH.x, XYflipEntWH.y, entRef.is_major } ); //- copy

        tiltData.push_back(       LandWaterMaskEnt{ tiltEntWH.x, tiltEntWH.y, entRef.is_major } ); //- copy
        tiltData_Xflip.push_back( LandWaterMaskEnt{ tiltXYflipEntWH.x, tiltEntWH.y, entRef.is_major } ); //- copy
        tiltData_Yflip.push_back( LandWaterMaskEnt{ tiltEntWH.x, tiltXYflipEntWH.y, entRef.is_major } ); //- copy
        tiltData_XYflip.push_back( LandWaterMaskEnt{ tiltXYflipEntWH.x, tiltXYflipEntWH.y, entRef.is_major } ); //- copy
    }

    //-- 逐个处理8个容器，将每个容器的数据，拆分为 4份 --
    handle_each_container( originData );
    handle_each_container( originData_Xflip );
    handle_each_container( originData_Yflip );
    handle_each_container( originData_XYflip );

    handle_each_container( tiltData );
    handle_each_container( tiltData_Xflip );
    handle_each_container( tiltData_Yflip );
    handle_each_container( tiltData_XYflip );

}


/* ===========================================================
 *         apply_a_rand_landWaterMaskCornerId
 * -----------------------------------------------------------
 * -- 最简模式...
 */
landWaterMaskCornerId_t apply_a_rand_landWaterMaskCornerId(){
    return ( uDistribution(randEngine) % landWaterMaskCorners.size() ); //- mod
}


/* ===========================================================
 *              get_landWaterMaskCorner
 * -----------------------------------------------------------
 * param: _quad -- 4个象限，分配对应的数据集
 */
const std::vector<LandWaterMaskEnt> &get_landWaterMaskCorner( landWaterMaskCornerId_t _id, QuadType _quad ){
    assert( landWaterMaskCorners.find(_id) != landWaterMaskCorners.end() ); //- tmp
    switch( _quad ){
        case QuadType::Left_Bottom:   return landWaterMaskCorners.at(_id).leftBottoms;
        case QuadType::Right_Bottom:  return landWaterMaskCorners.at(_id).rightBottoms;
        case QuadType::Left_Top:      return landWaterMaskCorners.at(_id).leftTops;
        case QuadType::Right_Top:     return landWaterMaskCorners.at(_id).rightTops;
        default:
            assert(0);
            return landWaterMaskCorners.at(_id).leftBottoms; //- never touch
    }
}



namespace{//-------- namespace: --------------//


/* ===========================================================
 *             handle_each_container
 * -----------------------------------------------------------
 * -- 将目标容器中的数据 正式存储到 全局容器中。
 */
void handle_each_container( const std::vector<LandWaterMaskEnt> &_container ){

    // ***| INSERT FIRST, INIT LATER  |***
    LandWaterMaskCorner  lwmc {};
    landWaterMaskCornerId_t id_ = LandWaterMaskCorner::id_manager.apply_a_u32_id();
    lwmc.id = id_;
        assert( landWaterMaskCorners.find(id_) == landWaterMaskCorners.end() );//- must
    landWaterMaskCorners.insert({id_, lwmc}); //- copy
    LandWaterMaskCorner &cornerRef = landWaterMaskCorners.at(id_);

    for( const auto &entRef : _container ){ //- each ent 
        const IntVec2 &wh = entRef.lMPosOff;
        if( (wh.x>=0) && (wh.y>=0) ){ //- right-top
            cornerRef.rightTops.push_back( entRef ); //- copy
        }else if( wh.y >= 0 ){ //- left-top
            cornerRef.leftTops.push_back( entRef ); //- copy
        }else if( wh.x >= 0 ){ //- right-bottom
            cornerRef.rightBottoms.push_back( entRef ); //- copy
        }else{ //- left-bottom
            cornerRef.leftBottoms.push_back( entRef ); //- copy
        }
    }
}


}//------------- namespace: end --------------//



/*
 * ==================== LandWaterMaskEdge.cpp =======================
 *                          -- tpr --
 *                                        CREATE -- 2019.03.14
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *  
 * ----------------------------
 */
#include "LandWaterMaskEdge.h"

//-------------------- CPP --------------------//
#include <vector>
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
    std::unordered_map<landWaterMaskEdgeId_t, LandWaterMaskEdge> landWaterMaskEdges {};

    //-- 区分对待 左右／上下 两种数据，当随机抽取 id时，从两个容器中抽 --
    std::vector<landWaterMaskEdgeId_t> leftRight_ids {};
    std::vector<landWaterMaskEdgeId_t> topBottom_ids {};

    //====== funcs =======//
    void handle_each_container( const std::vector<LandWaterMaskEnt> &_container, bool _is_leftRight );

}//------------- namespace: end --------------//


/* ===========================================================
 *             clear_for_LandWaterMaskEdge
 * -----------------------------------------------------------
 */
void clear_for_LandWaterMaskEdge(){
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
 *       push_back_originData_for_LandWaterMaskEdge
 * -----------------------------------------------------------
 * -- 将原版数据，暂存在 文件容器中
 */
void push_back_originData_for_LandWaterMaskEdge( const LandWaterMaskEnt &_ent ){
    originData.push_back( _ent ); //- copy
}



/* ===========================================================
 *     build_all_mutant_datas_for_LandWaterMaskEdge
 * -----------------------------------------------------------
 * -- 根据原始数据，通过各种翻转，生成所有 变种 数据。
 */
void build_all_mutant_datas_for_LandWaterMaskEdge(){

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
    handle_each_container( originData, true );
    handle_each_container( originData_Xflip, true );
    handle_each_container( originData_Yflip, true );
    handle_each_container( originData_XYflip, true );

    handle_each_container( tiltData, false );
    handle_each_container( tiltData_Xflip, false );
    handle_each_container( tiltData_Yflip, false );
    handle_each_container( tiltData_XYflip, false );
}



/* ===========================================================
 *           apply_a_rand_landWaterMaskEdgeId
 * -----------------------------------------------------------
 * -- 最简模式...
 */
landWaterMaskEdgeId_t apply_a_rand_landWaterMaskEdgeId( bool _is_leftRight ){
    size_t idx;
    if( _is_leftRight ){
        idx = uDistribution(randEngine) % leftRight_ids.size();
        return leftRight_ids.at(idx);
    }else{
        idx = uDistribution(randEngine) % topBottom_ids.size();
        return topBottom_ids.at(idx);
    }
}


/* ===========================================================
 *              get_landWaterMaskEdge
 * -----------------------------------------------------------
 * param: _quad -- 4个象限，分配对应的数据集.
 */
const std::vector<LandWaterMaskEnt> &get_landWaterMaskEdge( landWaterMaskEdgeId_t _id, QuadType _quad ){
    assert( landWaterMaskEdges.find(_id) != landWaterMaskEdges.end() ); //- tmp
    const LandWaterMaskEdge &edgeRef = landWaterMaskEdges.at(_id);
    if( edgeRef.is_leftRight ){ //- left-right
        if( (_quad==QuadType::Left_Bottom) || (_quad==QuadType::Left_Top) ){
            return edgeRef.left_or_tops;
        }else{
            return edgeRef.right_or_bottoms;
        }

    }else{ //- top-bottom
        if( (_quad==QuadType::Left_Top) || (_quad==QuadType::Right_Top) ){
            return edgeRef.left_or_tops;
        }else{
            return edgeRef.right_or_bottoms;
        }
    }
}




namespace{//-------- namespace: --------------//


/* ===========================================================
 *             handle_each_container
 * -----------------------------------------------------------
 * -- 将目标容器中的数据 正式存储到 全局容器中。
 */
void handle_each_container( const std::vector<LandWaterMaskEnt> &_container, bool _is_leftRight ){

    // ***| INSERT FIRST, INIT LATER  |***
    LandWaterMaskEdge  lwme {};
    landWaterMaskEdgeId_t id_ = LandWaterMaskEdge::id_manager.apply_a_u32_id();
    lwme.id = id_;
        assert( landWaterMaskEdges.find(id_) == landWaterMaskEdges.end() );//- must
    landWaterMaskEdges.insert({id_, lwme}); //- copy
    LandWaterMaskEdge &edgeRef = landWaterMaskEdges.at(id_);

    //-------
    edgeRef.is_leftRight = _is_leftRight;
    (_is_leftRight) ?
        leftRight_ids.push_back( edgeRef.id ) :
        topBottom_ids.push_back( edgeRef.id );

    //-------
    for( const auto &entRef : _container ){ //- each ent 
        const IntVec2 &wh = entRef.lMPosOff;

        if( _is_leftRight ){ //- left-right
            ( wh.x >= 0 ) ?
                edgeRef.right_or_bottoms.push_back( entRef ) :  //- copy
                edgeRef.left_or_tops.push_back( entRef );       //- copy
        }else{ //- top-bottom
            ( wh.y >= 0 ) ?
                edgeRef.left_or_tops.push_back( entRef ) :    //- copy
                edgeRef.right_or_bottoms.push_back( entRef ); //- copy
        }
    } //-- each ent end --
}


}//------------- namespace: end --------------//



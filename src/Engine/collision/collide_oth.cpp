/*
 * ==================== collide_oth.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.08.30
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#include "pch.h"
#include "collide_oth.h"

//-------------------- Engine --------------------//
#include "NineDirection.h"
#include "MapCoord.h"

#include "esrc_coordinate.h"


namespace colliOth_inn {//----------- namespace: colliOth_inn ----------------//


    //- 计算 moveVec 与一个 mapent 的 t值时
    //  根据 moveVec 方向，获得 它可以碰撞的 mapent 中的 哪几条边
    const std::unordered_map<NineDirection, std::vector<NineDirection>> sides_need_2_be_cast{
        { NineDirection::Left,          
            std::vector<NineDirection>{ NineDirection::Right } },

        { NineDirection::LeftTop,       
            std::vector<NineDirection>{ NineDirection::Right, NineDirection::Bottom } },

        { NineDirection::Top,           
            std::vector<NineDirection>{ NineDirection::Bottom } },

        { NineDirection::RightTop,      
            std::vector<NineDirection>{ NineDirection::Left, NineDirection::Bottom } },

        { NineDirection::Right,         
            std::vector<NineDirection>{ NineDirection::Left } },

        { NineDirection::RightBottom,   
            std::vector<NineDirection>{ NineDirection::Left, NineDirection::Top } },

        { NineDirection::Bottom,        
            std::vector<NineDirection>{ NineDirection::Top } },

        { NineDirection::LeftBottom,    
            std::vector<NineDirection>{ NineDirection::Right, NineDirection::Top } },
    };
    
    // Separate:  ()
    // Adjacent:  []
    // Intersect: ()
    double boundary_from_Separate_2_Adjacent {};
    double boundary_from_Adjacent_2_Intersect {};

    std::unordered_set<NineDirection> outs {};

    bool isInit {false}; // tmp


}//-------------- namespace: colliOth_inn end ----------------//


void init_for_colliOth_inn()noexcept{
    double threshold = 0.8;

    double AdjacentLine = HALF_PIXES_PER_MAPENT_D - Circular::radius_for_dogo; // 32.0 - 24.0
    //---
    colliOth_inn::boundary_from_Separate_2_Adjacent = AdjacentLine - threshold;
    colliOth_inn::boundary_from_Adjacent_2_Intersect = AdjacentLine + threshold;

    colliOth_inn::isInit = true; // tmp
}






/* ===========================================================
 *           collect_Adjacent_nearbyMapEnts
 * -----------------------------------------------------------
 * 针对 moveCollide 第一阶段 特制的函数
 * 仅 收集，所有和 dogo 相邻的 mapents
 * 相离，相交的 一律无视
 */
const std::unordered_set<NineDirection> &collect_Adjacent_nearbyMapEnts(    const glm::dvec2 &dogoDPos_, 
                                                                            IntVec2 dogoMPos_ ){

    colliOth_inn::outs.clear();

    //- mapEntMid_2_dogo
    glm::dvec2 off = dogoDPos_ - mpos_2_midDPos( dogoMPos_ );

    if( (off.x > -colliOth_inn::boundary_from_Separate_2_Adjacent) &&
        (off.x <  colliOth_inn::boundary_from_Separate_2_Adjacent) &&
        (off.y > -colliOth_inn::boundary_from_Separate_2_Adjacent) &&
        (off.y <  colliOth_inn::boundary_from_Separate_2_Adjacent) ){
        return colliOth_inn::outs; // empty
    }

    double sum_of_2_radius = HALF_PIXES_PER_MAPENT_D + Circular::radius_for_dogo;

    glm::dvec2 mpMidDPos {};
    glm::dvec2 absOff {};
    for( int j=-1; j<=1; j++ ){
        for( int i=-1; i<=1; i++ ){ // nearby 8 mapents

            if( (j==0) && (i==0) ){
                continue; // skip Mid
            }

            mpMidDPos = mpos_2_midDPos(dogoMPos_ + IntVec2{i,j});
            absOff.x = std::abs( mpMidDPos.x - dogoDPos_.x );
            absOff.y = std::abs( mpMidDPos.y - dogoDPos_.y );

            if( (is_closeEnough(absOff.x, sum_of_2_radius, 0.8) && (absOff.y<sum_of_2_radius)) ||
                (is_closeEnough(absOff.y, sum_of_2_radius, 0.8) && (absOff.x<sum_of_2_radius)) ){
                
                colliOth_inn::outs.insert( intVec2_2_nineDirection( IntVec2{i,j} ) );
            }
        }
    }

    return colliOth_inn::outs; // [0,2] ents
}



/* ===========================================================
 *    calc_obstructNormalVec_from_AdjacentMapEnts
 * -----------------------------------------------------------
 * 根据参数：1～2 个确认会碰撞的 相邻 mapents（square）
 * 计算出 它们作用于 dogo 的 墙壁法向量
 * ---
 * 本函数 通过 穷举的方式来实现
 */
glm::dvec2 calc_obstructNormalVec_from_AdjacentMapEnts( const glm::dvec2 &moveVec_,
                                                        const glm::dvec2 &dogoDPos_,
                                                        IntVec2 dogoMPos_,
                                                        const std::set<NineDirection> &AdjacentMapEnts_ )noexcept{
    glm::dvec2 tv {};
    //===== 1-ents =====//
    if( AdjacentMapEnts_.size() == 1 ){
        //- 只可能有 4 种情况
        switch ( *AdjacentMapEnts_.cbegin() ){
            case NineDirection::Top:    return nineDirection_2_dVec2( NineDirection::Bottom ); // anti
            case NineDirection::Bottom: return nineDirection_2_dVec2( NineDirection::Top );   // anti
            case NineDirection::Left:   return nineDirection_2_dVec2( NineDirection::Right ); // anti
            case NineDirection::Right:  return nineDirection_2_dVec2( NineDirection::Left ); // anti

            case NineDirection::LeftTop:     
                tv = calc_innVec( nineDirection_2_dVec2(NineDirection::LeftTop), moveVec_ );   
                return (tv.y >= 0.0) ?
                            nineDirection_2_dVec2( NineDirection::Right ) : // anti
                            nineDirection_2_dVec2( NineDirection::Bottom ); // anti

            case NineDirection::RightTop:       
                tv = calc_innVec( nineDirection_2_dVec2(NineDirection::RightTop), moveVec_ );   
                return (tv.y >= 0.0) ?
                            nineDirection_2_dVec2( NineDirection::Bottom ) : // anti
                            nineDirection_2_dVec2( NineDirection::Left ); // anti

            case NineDirection::RightBottom:    
                tv = calc_innVec( nineDirection_2_dVec2(NineDirection::RightBottom), moveVec_ );   
                return (tv.y >= 0.0) ?
                            nineDirection_2_dVec2( NineDirection::Left ) : // anti
                            nineDirection_2_dVec2( NineDirection::Top ); // anti

            case NineDirection::LeftBottom:     
                tv = calc_innVec( nineDirection_2_dVec2(NineDirection::LeftBottom), moveVec_ );   
                return (tv.y >= 0.0) ?
                            nineDirection_2_dVec2( NineDirection::Top ) : // anti
                            nineDirection_2_dVec2( NineDirection::Right ); // anti

            default:
                tprAssert(0);
                return glm::dvec2{}; // never reach
        }
    }

    //===== 2-ents =====//
    //- dogo 相对于 自身 mapent 的方位
    glm::dvec2 off = dogoDPos_ - mpos_2_midDPos( dogoMPos_ );   

    //-- 只可能是 4个 斜角方向
    switch ( dpos_2_nineDirection(off) ){
        case NineDirection::LeftTop:
            if( AdjacentMapEnts_.find(NineDirection::LeftTop) == AdjacentMapEnts_.end() ){

                tv = calc_innVec( nineDirection_2_dVec2(NineDirection::LeftTop), moveVec_ );   
                return (tv.y >= 0.0) ?
                            nineDirection_2_dVec2( NineDirection::Right ) : // anti
                            nineDirection_2_dVec2( NineDirection::Bottom ); // anti
            }

            if( AdjacentMapEnts_.find(NineDirection::Left) != AdjacentMapEnts_.end() ){
                return nineDirection_2_dVec2( NineDirection::Right ); // anti
            }else{
                return nineDirection_2_dVec2( NineDirection::Bottom ); // anti
            }

           
        case NineDirection::RightTop:
            if( AdjacentMapEnts_.find(NineDirection::RightTop) == AdjacentMapEnts_.end() ){

                tv = calc_innVec( nineDirection_2_dVec2(NineDirection::RightTop), moveVec_ );   
                return (tv.y >= 0.0) ?
                            nineDirection_2_dVec2( NineDirection::Bottom ) : // anti
                            nineDirection_2_dVec2( NineDirection::Left ); // anti
            }

            if( AdjacentMapEnts_.find(NineDirection::Right) != AdjacentMapEnts_.end() ){
                return nineDirection_2_dVec2( NineDirection::Left ); // anti
            }else{
                return nineDirection_2_dVec2( NineDirection::Bottom ); // anti
            }

        case NineDirection::RightBottom:
            if( AdjacentMapEnts_.find(NineDirection::RightBottom) == AdjacentMapEnts_.end() ){

                tv = calc_innVec( nineDirection_2_dVec2(NineDirection::RightBottom), moveVec_ );   
                return (tv.y >= 0.0) ?
                            nineDirection_2_dVec2( NineDirection::Left ) : // anti
                            nineDirection_2_dVec2( NineDirection::Top ); // anti
            }

            if( AdjacentMapEnts_.find(NineDirection::Right) != AdjacentMapEnts_.end() ){
                return nineDirection_2_dVec2( NineDirection::Left ); // anti
            }else{
                return nineDirection_2_dVec2( NineDirection::Top ); // anti
            }

        case NineDirection::LeftBottom:
            if( AdjacentMapEnts_.find(NineDirection::LeftBottom) == AdjacentMapEnts_.end() ){

                tv = calc_innVec( nineDirection_2_dVec2(NineDirection::LeftBottom), moveVec_ );   
                return (tv.y >= 0.0) ?
                            nineDirection_2_dVec2( NineDirection::Top ) : // anti
                            nineDirection_2_dVec2( NineDirection::Right ); // anti
            }

            if( AdjacentMapEnts_.find(NineDirection::Left) != AdjacentMapEnts_.end() ){
                return nineDirection_2_dVec2( NineDirection::Right ); // anti
            }else{
                return nineDirection_2_dVec2( NineDirection::Top ); // anti
            }

        default:
            tprAssert(0);
            return glm::dvec2{}; // never reach
    }
}



/* ===========================================================
 *                 cast_with_mapent
 * -----------------------------------------------------------
 * 计算 位移向量 和目标 mapent 的碰撞 t 值
 * return:
 *    bool:    isHaveT 
 *    double:  t-val
 */
std::pair<bool,double> cast_with_mapent(    const glm::dvec2 &moveVec_,
                                            const glm::dvec2 &dogoDPos_,
                                            IntVec2 targetMPos_ )noexcept{
    
    glm::dvec2 dogoTargetDPos = dogoDPos_ + moveVec_;

    //------------------------------//
    //   优先过滤掉 贴着墙壁移动的 情况
    //------------------------------//
    IntVec2 dogoTargetMPos = dpos_2_mpos(dogoTargetDPos);
    //- 说明 此时正贴着墙壁 上下移动 
    if( is_closeEnough<double>( moveVec_.x, 0.0, 0.001 ) ){
        if( dogoTargetMPos.x != targetMPos_.x ){
            //tprDebug::console( "--- X ---" );
            return { false, 1.0 }; // 不相交
        }
    }
    //- 说明 此时正贴着墙壁 左右移动 
    if( is_closeEnough<double>( moveVec_.y, 0.0, 0.001 ) ){
        if( dogoTargetMPos.y != targetMPos_.y ){
            //tprDebug::console( "--- Y ---" );
            return { false, 1.0 }; // 不相交
        }
    }

    //------------------------------//
    //         剩余情况
    //------------------------------//
    double sum_of_2_radius = HALF_PIXES_PER_MAPENT_D + Circular::radius_for_dogo;

    glm::dvec2 mpMidDPos = mpos_2_midDPos( targetMPos_ );
    double leftLine = mpMidDPos.x - sum_of_2_radius;
    double rightLine = mpMidDPos.x + sum_of_2_radius;
    double bottomLine = mpMidDPos.y - sum_of_2_radius;
    double topLine = mpMidDPos.y + sum_of_2_radius;

    std::multiset<double> tVals {};
    
    if( moveVec_.x != 0.0 ){
        auto [newY1, t1] = cast_with_verticalLine( moveVec_, dogoDPos_, leftLine );
        if( (newY1>bottomLine) && (newY1<topLine) ){
            tVals.insert( t1 );
        }

        auto [newY2, t2] = cast_with_verticalLine( moveVec_, dogoDPos_, rightLine );
        if( (newY2>bottomLine) && (newY2<topLine) ){
            tVals.insert( t2 );
        }
    }
    
    if( moveVec_.y != 0.0 ){
        auto [newX1, t1] = cast_with_horizonLine( moveVec_, dogoDPos_, bottomLine );
        if( (newX1>leftLine) && (newX1<rightLine) ){
            tVals.insert( t1 );
        }

        auto [newX2, t2] = cast_with_horizonLine( moveVec_, dogoDPos_, topLine );
        if( (newX2>leftLine) && (newX2<rightLine) ){
            tVals.insert( t2 );
        }
    }

    if( tVals.empty() ){
        return { false, 1.0 }; // 不相交
    }

    double tMin = *tVals.begin();

    return { true, tMin };
}




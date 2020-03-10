/*
 * ====================== collide_oth.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.08.30
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *    Only used in Collision.cpp !!!
 * ------------
 */
#ifndef TPR_COLLIDE_OTH_H
#define TPR_COLLIDE_OTH_H

#include "pch.h"

//-------------------- Engine --------------------//
#include "ColliderType.h"
#include "NineDirection.h"
#include "MapCoord.h"
#include "SpeedLevel.h" // tmp


void init_for_colliOth_inn()noexcept;


inline double calc_cos( const glm::dvec2 &a_, const glm::dvec2 &b_ ) noexcept {
    return glm::dot(glm::normalize(a_), glm::normalize(b_) );
}


/* ===========================================================
 *       collideCheck_between_2_arcs_in_same_circular
 * -----------------------------------------------------------
 * return:
 *  -- true:  collide
 *  -- flase: not collide
 */
inline bool collideCheck_between_2_arcs_in_same_circular(   const glm::dvec2 &forward_1_,
                                                            double halfRadian_1_,
                                                            const glm::dvec2 &forward_2_,
                                                            double halfRadian_2_   )noexcept{
        tprAssert( (halfRadian_1_>=0.0) && (halfRadian_2_>=0.0) );
    double cosV = calc_cos( forward_1_, forward_2_ ); //- 为负值时，角度大于 90 度
    double radian = acos(cosV); 
    return (radian < (halfRadian_1_+halfRadian_2_)) ? true : false;
}


/* ===========================================================
 *       calc_halfRadian_in_2_intersect_circulars
 * -----------------------------------------------------------
 * 2个确认相交的圆（do,be）计算 doCir 相交圆弧 半弧度值
 */
inline double calc_halfRadian_in_2_intersect_circulars( double rd_, double rb_, double totalLen_  ){

    tprAssert( (rd_+rb_) > totalLen_ );
    double cosVal = (rd_*rd_ - rb_*rb_ + totalLen_*totalLen_) / (2.0 * totalLen_ * rd_);
    return acos(cosVal);
}


/* ===========================================================
 *             collideState_from_circular_2_circular
 * -----------------------------------------------------------
 * just check if dogoCir is collide with begoCir
 * 优化: dogo 使用统一的 半径，不再需要 参数提供
 */
inline CollideState collideState_from_circular_2_circular(  const glm::dvec2 &dogoCirDPos_,
                                                            const Circular &begoCir_,
                                                            double threshold_ ) noexcept {

    glm::dvec2 offVec = begoCir_.dpos - dogoCirDPos_;
    double     sum_of_two_raidus = Circular::radius_for_dogo + begoCir_.radius;
    //-- Avoid Radical Sign / 避免开根号 --
    double off = (offVec.x * offVec.x) + (offVec.y * offVec.y) -
                (sum_of_two_raidus * sum_of_two_raidus);
    if( is_closeEnough<double>(off, 0.0, threshold_*threshold_) ){
        return CollideState::Adjacent;
    }else if( off < 0.0 ){
        return CollideState::Intersect;
    }else{
        return CollideState::Separate;
    }
}



const std::unordered_set<NineDirection> &collect_Adjacent_nearbyMapEnts( const glm::dvec2 &dogoDPos_, IntVec2 dogoMPos_ );

glm::dvec2 calc_obstructNormalVec_from_AdjacentMapEnts( const glm::dvec2 &moveVec_,
                                                        const glm::dvec2 &dogoDPos_,
                                                        IntVec2 dogoMPos_,
                                                        const std::set<NineDirection> &AdjacentMapEnts_ )noexcept;



/* ===========================================================
 *       fastCollideCheck_from_arc_2_circular
 * -----------------------------------------------------------
 * return:
 *  -- true:  collide
 *  -- flase: not collide
 */
inline bool fastCollideCheck_from_arc_2_circular(  const ArcLine &dogoArc_,
                                            const Circular &begoCir_,
                                            double threshold_ ) noexcept{
    
    double rd = dogoArc_.radius;
    double rb = begoCir_.radius;
    double sum_of_two_raidus = rd + rb;
    glm::dvec2 offVec = begoCir_.dpos - dogoArc_.dpos;
    //-- Avoid Radical Sign / 避免开根号 --
    double lenSquare =  (offVec.x * offVec.x) + (offVec.y * offVec.y);
    double lenImprecise = lenSquare - (sum_of_two_raidus * sum_of_two_raidus);

    if( is_closeEnough<double>(lenImprecise, 0.0, threshold_*threshold_) ){ // Adjacent
        return collideCheck_between_2_arcs_in_same_circular(offVec, 
                                                            0.0,
                                                            dogoArc_.forward, 
                                                            dogoArc_.halfRadian );
    }else if( lenImprecise > 0.0 ){ //Separate
        return false;

    }else{ //  Intersect
        double len = sqrt(lenSquare); //- 开根号开销略大...
        double minRadius = tprMin( rd, rb );
        double maxRadius = tprMax( rd, rb );

        if( (len+minRadius) <= maxRadius ){ //- 小圆被大圆彻底包含
            return true; // IMM
        }

        double halfRadian = calc_halfRadian_in_2_intersect_circulars(rd, rb, len);
        return collideCheck_between_2_arcs_in_same_circular(offVec, 
                                                            halfRadian,
                                                            dogoArc_.forward, 
                                                            dogoArc_.halfRadian );
    }
}

/* ===========================================================
 *               is_dogoCircular_leave_begoCircular
 * -----------------------------------------------------------
 * 当方向背离时，将不进行碰撞检测
 * 优化: dogo 使用统一的 半径，不再需要 参数提供
 */
inline bool is_dogoCircular_leave_begoCircular( const glm::dvec2 &moveVec_,
                                                const glm::dvec2 &dogoCirDPos_,
                                                const Circular &begoCir_ ) noexcept {
    glm::dvec2 dogo_2_bego = begoCir_.dpos - dogoCirDPos_;
    double cosVal = calc_cos( moveVec_, dogo_2_bego );
    return (cosVal <= 0.0) ? true : false;
}


//-- 仅用于 第一阶段，此时知道现成的 墙壁法向量，直接使用即可 
inline bool is_dogo_leave_begoSquares_easy(  const glm::dvec2 &moveVec_,
                                        const glm::dvec2 &obstructNormalVec_ ) noexcept {
                                            
    double cosVal = calc_cos( moveVec_, (-1.0 * obstructNormalVec_) );
    return (cosVal <= 0.0) ? true : false;
}


//-- 仅用于 第二阶段，此时的 墙壁法向量，需要重新计算
inline bool is_dogo_leave_begoSquares_2( const glm::dvec2 &moveVec_,
                                        const glm::dvec2 &dogoDPos_,
                                        IntVec2 dogoMPos_,
                                        IntVec2 targetMPos_
                                        ) noexcept {
    glm::dvec2 obNormVec {};
    //-- dogo 在 mp 的那个方位
    auto dir = intVec2_2_nineDirection( dogoMPos_ - targetMPos_ );
    switch (dir){
        case NineDirection::Left:   
        case NineDirection::Right:
        case NineDirection::Top:
        case NineDirection::Bottom:
            obNormVec = nineDirection_2_dVec2( dir );
            break;
        case NineDirection::LeftTop:
        case NineDirection::LeftBottom:
        case NineDirection::RightTop:
        case NineDirection::RightBottom:
        case NineDirection::Center: // Center 也许不该这么处理 ...
            obNormVec = dogoDPos_ - mpos_2_midDPos(targetMPos_);
            break;
        default:
            tprAssert(0);
            break;
    }
    //----
    double cosVal = calc_cos( moveVec_, (-1.0 * obNormVec) );
    return (cosVal <= 0.0) ? true : false;
}



/* ===========================================================
 *                 circularCast   [traditional solution]
 * -----------------------------------------------------------
 *  if collision has been confirmed between bego and dogo.
 *  call this func to calculate the colliPoint in motion vector.
 *  优化: dogo 使用统一的 半径，不再需要 参数提供
 *  ---
 *  return:
 *    t is a scale
 */
inline double circularCast( const glm::dvec2 &moveVec_,
                            const glm::dvec2 &dogoCirDPos_,
                            const Circular &begoCir_  ) noexcept {

    double sum_of_2_radius = Circular::radius_for_dogo + begoCir_.radius;
    glm::dvec2 dogo_2_bego = begoCir_.dpos - dogoCirDPos_;
    //---
    glm::dvec2 dogo_2_begoInn = calc_innVec( moveVec_, dogo_2_bego );
    double thirdEdge = sqrt(sum_of_2_radius*sum_of_2_radius - dogo_2_begoInn.y*dogo_2_begoInn.y);
    return (( std::abs(dogo_2_begoInn.x) - thirdEdge) / glm::length(moveVec_));
}




/* ===========================================================
 *                 calc_slideMoveVec
 * -----------------------------------------------------------
 * 位移向量 会被 阻挡向量 阻挡，退化为一个 平行与 阻挡向量的 分量。
 * 当 位移向量 和 阻挡向量 夹角很小时，会主动对结果叠加一个增值，让它滑动得快些
 */
inline glm::dvec2 calc_slideMoveVec(   const glm::dvec2 &moveVec_,
                                const glm::dvec2 &obstructVec_  ) noexcept {
    
    glm::dvec2 dogo_2_oth = -1.0 * obstructVec_; // anti
    double pct = std::abs(calc_innVec(dogo_2_oth, moveVec_).x) / glm::length(dogo_2_oth);
    glm::dvec2 newMoveVec = (moveVec_ - dogo_2_oth*pct) * 1.0;
    return limit_moveSpeed( newMoveVec );
                //-- 偏转后速度会下降，尤其在与 cir_bego 的碰撞中，游戏体验不够好
                //   人为地增加速度
                //   同时确保 速度不越界
}


/* ===========================================================
 *               calc_intersectX 
 * -----------------------------------------------------------
 * 位移向量，与水平线: y=y_ 相交，求 碰撞 t值
 * 求直线 y=y_ 与向量 root_2_tail_ 所在直线的交点，的 x 值
 * return:
 *    double:  newX
 *    double:  t
 */
inline std::pair<double,double> cast_with_horizonLine(  const glm::dvec2 &moveVec_,
                                                        const glm::dvec2 &dogoDPos_,
                                                        double y_ ) noexcept {
    tprAssert( moveVec_.y != 0.0 );
    double t = (y_ - dogoDPos_.y) / moveVec_.y; // 允许为负值，以便 dogo 从相交状态后退  
    double newX = dogoDPos_.x + moveVec_.x*t;
    return { newX, t };
}
inline std::pair<double,double> cast_with_verticalLine( const glm::dvec2 &moveVec_,
                                                        const glm::dvec2 &dogoDPos_,
                                                        double x_ ) noexcept {
    tprAssert( moveVec_.x != 0.0 );
    double t = (x_ - dogoDPos_.x) / moveVec_.x; // 允许为负值，以便 dogo 从相交状态后退  
    double newY = dogoDPos_.y + moveVec_.y*t;
    return { newY, t };
}


std::pair<bool,double> cast_with_mapent(    const glm::dvec2 &moveVec_,
                                            const glm::dvec2 &dogoDPos_,
                                            IntVec2 mpos_ )noexcept;



#endif 


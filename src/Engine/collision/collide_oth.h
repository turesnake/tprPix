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

//--- glm - 0.9.9.5 ---
#include "glm_no_warnings.h"

//-------------------- CPP --------------------//
#include <cmath>
#include <utility> // pair

//-------------------- Engine --------------------//
#include "ColliderType.h"
#include "tprMath.h"



/* ===========================================================
 *                     isPass_Check
 * -----------------------------------------------------------
 * -- 本实例为 主动go，参数为 被动go。检测此次碰撞 是否能 穿过
 * -- return:
 *     - true:   说明本次碰撞 可以穿过
 *     - false:  说明本次碰撞 无法穿过
 */
inline bool isPass_Check( bool isDoPass_, bool isBePass_ ) noexcept {

    //--- *** 本流程可能存在逻辑问题... *** ---//
    if( isDoPass_ == true ){
        //-- 此时，主动go比较特殊：火球／箭 等，允许穿过一切物体
        return true;
    }
    if( isBePass_ == true ){
        //-- 说明 被动go 允许别人穿过自己。比如是：草，地上的液体等
        return true;
    }else{
        //-- 主动go／被动go 都不运行穿过，才判定为 “无法穿过”
        return false;
    }
}



inline double calc_cos( const glm::dvec2 &a_, const glm::dvec2 &b_ ) noexcept {
    return glm::dot(glm::normalize(a_), glm::normalize(b_) );
}



/* ===========================================================
 *               calc_intersectX 
 * -----------------------------------------------------------
 * 求直线 y=y_ 与向量 root_2_tail_ 所在直线的交点，的 x 值
 * 
 *    将被弃用 。。。。。
 */
inline double calc_intersectX(  const glm::dvec2 &rootPoint_,
                                const glm::dvec2 &root_2_tail_,
                                double y_) noexcept {
        tprAssert( root_2_tail_.y != 0.0 );
    double pct = (rootPoint_.y - y_) / -root_2_tail_.y;    
    return (rootPoint_.x + (root_2_tail_.x * pct)); //- 允许返回 反方向的值（从相交中退出
}

/*
// 求直线 x=x_ 与向量 root_2_tail_ 所在直线的交点，的 y 值
inline double calc_intersectY(  const glm::dvec2 &rootPoint_,
                                const glm::dvec2 &root_2_tail_,
                                double x_) noexcept {
        tprAssert( root_2_tail_.x != 0.0 );       
    double pct = (rootPoint_.x - x_) / -root_2_tail_.x; 
    return ( rootPoint_.y + (root_2_tail_.y * pct) ); //- 允许返回 反方向的值（从相交中退出
}
*/

/* ===========================================================
 *               calc_percent_in_yLine
 * -----------------------------------------------------------
 * 求直线 y=y_ 与向量 root_2_tail_ 所在直线的交点，的 x 值
 */
inline double calc_percent_in_yLine(    const glm::dvec2 &rootPoint_,
                                        const glm::dvec2 &root_2_tail_,
                                        double y_) noexcept {
        tprAssert( root_2_tail_.y != 0.0 );
    return  (rootPoint_.y - y_) / -root_2_tail_.y;  //允许为负值，表示 相交点，在位移向量的 反向延长线上 
}
// 求直线 x=x_ 与向量 root_2_tail_ 所在直线的交点，的 y 值
inline double calc_percent_in_xLine(    const glm::dvec2 &rootPoint_,
                                        const glm::dvec2 &root_2_tail_,
                                        double x_) noexcept {
        tprAssert( root_2_tail_.x != 0.0 );       
    return (rootPoint_.x - x_) / -root_2_tail_.x; //允许为负值，表示 相交点，在位移向量的 反向延长线上 
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
 */
inline CollideState collideState_from_circular_2_circular(  const Circular &dogoCir_,
                                                            const Circular &begoCir_,
                                                            double threshold_ ) noexcept {
    glm::dvec2 offVec = begoCir_.dpos - dogoCir_.dpos;
    double     sum_of_two_raidus = dogoCir_.radius + begoCir_.radius;
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



std::pair<CollideState, glm::dvec2> collideState_from_circular_2_square(   const Circular &dogoCir_,
                                                    const Square   &begoSqu_,
                                                    double threshold_ )noexcept;


CollideState collideState_from_circular_2_square_simple(    const Circular &dogoCir_,
                                                            const Square   &begoSqu_,
                                                            double threshold_ ) noexcept;



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
 */
inline bool is_dogoCircular_leave_begoCircular( const glm::dvec2 &moveVec_,
                                                const Circular &dogoCir_,
                                                const Circular &begoCir_ ) noexcept {
    glm::dvec2 dogo_2_bego = begoCir_.dpos - dogoCir_.dpos;
    double cosVal = calc_cos( moveVec_, dogo_2_bego );
    return (cosVal <= 0.0) ? true : false;
}


inline bool is_dogoCircular_leave_begoSquare(   const glm::dvec2 &moveVec_,
                                                const Circular &dogoCir_,
                                                const Square &begoSqu_ ) noexcept {
    glm::dvec2 dogo_2_bego = begoSqu_.dpos - dogoCir_.dpos;
    double cosVal = calc_cos( moveVec_, dogo_2_bego );
    return (cosVal <= 0.0) ? true : false;
}




/* ===========================================================
 *                 circularCast   [traditional solution]
 * -----------------------------------------------------------
 *  if collision has been confirmed between bego and dogo.
 *  call this func to calculate the colliPoint in motion vector.
 *  ---
 *  return:
 *    t is a scale
 */
inline double circularCast( const glm::dvec2 &moveVec_,
                            const Circular &dogoCir_,
                            const Circular &begoCir_  ) noexcept {

    double sum_of_2_radius = dogoCir_.radius + begoCir_.radius;
    glm::dvec2 dogo_2_bego = begoCir_.dpos - dogoCir_.dpos;
    //---
    glm::dvec2 dogo_2_begoInn = calc_innVec( moveVec_, dogo_2_bego );
    double thirdEdge = sqrt(sum_of_2_radius*sum_of_2_radius - dogo_2_begoInn.y*dogo_2_begoInn.y);
    return (( std::abs(dogo_2_begoInn.x) - thirdEdge) / glm::length(moveVec_));
}


double squareCast(  const glm::dvec2 &moveVec_,
                    const Circular &dogoCir_,
                    const Square &begoSqu_  ) noexcept;




/* ===========================================================
 *                 calc_slideMoveVec
 * -----------------------------------------------------------
 * 位移向量 会被 阻挡向量 阻挡，退化为一个 平行与 阻挡向量的 分量。
 * 当 位移向量 和 阻挡向量 夹角很小时，会主动对结果叠加一个增值，让它滑动得快些
 */
inline glm::dvec2 calc_slideMoveVec(   const glm::dvec2 &moveVec_,
                                const glm::dvec2 &obstructVec_  ) noexcept {
                                    
    double pct = std::abs(calc_innVec(obstructVec_, moveVec_).x) / glm::length(obstructVec_);
    return (moveVec_ - obstructVec_*pct);
}





#endif 


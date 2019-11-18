/*
 * ==================== collide_oth.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.08.30
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#include "collide_oth.h"

#include "tprDebug.h"


//-------------------- Engine --------------------//
#include "NineDirection.h"

#include "esrc_coordinate.h"


//namespace colliOth_inn {//----------- namespace: colliOth_inn ----------------//
//}//-------------- namespace: colliOth_inn end ----------------//


/* ===========================================================
 *             collideState_from_circular_2_square
 * -----------------------------------------------------------
 *  return:
 *    collideState
 *    self_2_oth: dogo 指向 bego 的法向量，仅用于 Adjacent 模式
 */
std::pair<CollideState, glm::dvec2> collideState_from_circular_2_square(   const Circular &dogoCir_,
                                                    const Square   &begoSqu_,
                                                    double threshold_ ) noexcept {
    
    // 简化模型，暂把 dogocir 也看作一个 square
    const auto &worldCoord = esrc::get_worldCoordRef();
    //---
    double     sum_of_two_raidus = dogoCir_.radius + begoSqu_.radius;
    glm::dvec2 bego_2_dogoInn = worldCoord.calc_innDPos( dogoCir_.dpos - begoSqu_.dpos );

    double bigRadius = sum_of_two_raidus + threshold_; // 相邻 和 相离的边界

    if( (bego_2_dogoInn.y >= bigRadius) ||
        (bego_2_dogoInn.y <= -bigRadius) ||
        (bego_2_dogoInn.x >= bigRadius) ||
        (bego_2_dogoInn.x <= -bigRadius) ){
        return {CollideState::Separate, glm::dvec2{0.0, 0.0} }; // pair.second is useless

    }else if( is_closeEnough<double>( bego_2_dogoInn.x, -sum_of_two_raidus, threshold_ ) ){
        //== inn Left ==
        return { CollideState::Adjacent, worldCoord.get_normalVec_in_outCoord(NineDirection::Left) * -1.0 };

    }else if( is_closeEnough<double>( bego_2_dogoInn.x, sum_of_two_raidus, threshold_ ) ){
        //== inn Right ==
        return { CollideState::Adjacent, worldCoord.get_normalVec_in_outCoord(NineDirection::Right) * -1.0 };

    }else if( is_closeEnough<double>( bego_2_dogoInn.y, -sum_of_two_raidus, threshold_ ) ){
        //== inn Bottom ==
        return { CollideState::Adjacent, worldCoord.get_normalVec_in_outCoord(NineDirection::Bottom) * -1.0 };

    }else if( is_closeEnough<double>( bego_2_dogoInn.y, sum_of_two_raidus, threshold_ ) ){
        //== inn Top ==
        return { CollideState::Adjacent, worldCoord.get_normalVec_in_outCoord(NineDirection::Top) * -1.0 };

    }else{
        return {CollideState::Intersect, glm::dvec2{0.0, 0.0} }; // pair.second is useless
    }
}


//-- 简易版，仅仅判断是否 相交/相邻，并不计算 墙壁法向量
CollideState collideState_from_circular_2_square_simple(    const Circular &dogoCir_,
                                                            const Square   &begoSqu_,
                                                            double threshold_ ) noexcept {
    // 简化模型，暂把 dogocir 也看作一个 square
    const auto &worldCoord = esrc::get_worldCoordRef();
    //---
    double     sum_of_two_raidus = dogoCir_.radius + begoSqu_.radius;
    glm::dvec2 bego_2_dogoInn = worldCoord.calc_innDPos( dogoCir_.dpos - begoSqu_.dpos );

    double bigRadius = sum_of_two_raidus + threshold_; // 相邻 和 相离的边界

    if( (bego_2_dogoInn.y >= bigRadius) ||
        (bego_2_dogoInn.y <= -bigRadius) ||
        (bego_2_dogoInn.x >= bigRadius) ||
        (bego_2_dogoInn.x <= -bigRadius) ){
        return CollideState::Separate; 

    }else if(   is_closeEnough<double>( bego_2_dogoInn.x, -sum_of_two_raidus, threshold_ )  ||
                is_closeEnough<double>( bego_2_dogoInn.x, sum_of_two_raidus, threshold_ )   ||
                is_closeEnough<double>( bego_2_dogoInn.y, -sum_of_two_raidus, threshold_ ) ||
                is_closeEnough<double>( bego_2_dogoInn.y, sum_of_two_raidus, threshold_ ) ){
        //== inn Left ==
        return CollideState::Adjacent;

    }else{
        return CollideState::Intersect;
    }
}








// 调用此函数时，已经确认相交
double squareCast(  const glm::dvec2 &moveVec_,
                    const Circular &dogoCir_,
                    const Square &begoSqu_  ) noexcept{

    // 简化模型，暂把 dogocir 看作一个 square
    const auto &worldCoord = esrc::get_worldCoordRef();
    //---
    double     sum_of_two_raidus = dogoCir_.radius + begoSqu_.radius;
    glm::dvec2 moveVecInn = worldCoord.calc_innDPos( moveVec_ );
    glm::dvec2 bego_2_dogoInn = worldCoord.calc_innDPos( dogoCir_.dpos - begoSqu_.dpos );


    //-- 计算 位移向量 与 bego 的哪条边相交 --
    NineDirection intersectSide {};
    if( std::abs(bego_2_dogoInn.y) < sum_of_two_raidus ){
        intersectSide = (bego_2_dogoInn.x > 0.0) ?
                    NineDirection::Right :
                    NineDirection::Left;
    }else if( std::abs(bego_2_dogoInn.x) < sum_of_two_raidus ){
        intersectSide = (bego_2_dogoInn.y > 0.0) ?
                    NineDirection::Top :
                    NineDirection::Bottom;
    }else{

        glm::dvec2 moveTargetInn = worldCoord.calc_innDPos( dogoCir_.dpos + moveVec_ );
        glm::dvec2 moveTarget_2_corner {};

        if( bego_2_dogoInn.y > 0.0 ){
            if( bego_2_dogoInn.x > 0.0 ){
                //== RightTop ==
                moveTarget_2_corner = glm::dvec2{ sum_of_two_raidus, sum_of_two_raidus } - moveTargetInn;
                glm::dvec2 trans = calc_innVec( moveVecInn * -1.0, moveTarget_2_corner );
                intersectSide = (trans.y < 0.0) ?
                                    NineDirection::Top :
                                    NineDirection::Right;
            }else{
                //== LeftTop ==
                moveTarget_2_corner = glm::dvec2{ -sum_of_two_raidus, sum_of_two_raidus } - moveTargetInn;
                glm::dvec2 trans = calc_innVec( moveVecInn * -1.0, moveTarget_2_corner );
                intersectSide = (trans.y < 0.0) ?
                                    NineDirection::Left :
                                    NineDirection::Top;
            }
        }else{
            if( bego_2_dogoInn.x > 0.0 ){
                //== RightBottom ==
                moveTarget_2_corner = glm::dvec2{ sum_of_two_raidus, -sum_of_two_raidus } - moveTargetInn;
                glm::dvec2 trans = calc_innVec( moveVecInn * -1.0, moveTarget_2_corner );
                intersectSide = (trans.y < 0.0) ?
                                    NineDirection::Right :
                                    NineDirection::Bottom;
            }else{
                //== LeftBottom ==
                moveTarget_2_corner = glm::dvec2{ -sum_of_two_raidus, -sum_of_two_raidus } - moveTargetInn;
                glm::dvec2 trans = calc_innVec( moveVecInn * -1.0, moveTarget_2_corner );
                intersectSide = (trans.y < 0.0) ?
                                    NineDirection::Bottom :
                                    NineDirection::Left;
            }
        }
    }


    switch (intersectSide){
        case NineDirection::Top:    return calc_percent_in_yLine( bego_2_dogoInn, moveVecInn, sum_of_two_raidus );
        case NineDirection::Bottom: return calc_percent_in_yLine( bego_2_dogoInn, moveVecInn, -sum_of_two_raidus );
        case NineDirection::Left:   return calc_percent_in_xLine( bego_2_dogoInn, moveVecInn, -sum_of_two_raidus );
        case NineDirection::Right:  return calc_percent_in_xLine( bego_2_dogoInn, moveVecInn, sum_of_two_raidus );
        default:
            tprAssert(0);
            return 0.0; // never reach
    }

}







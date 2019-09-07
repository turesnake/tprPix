/*
 * ==================== collide_oth.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.08.30
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#include "collide_oth.h"

#include "tprDebug.h"

/* ===========================================================
 *             collideState_from_circular_2_capsule 
 * -----------------------------------------------------------
 *  return:
 *    collideState
 *    self_2_oth
 */
std::pair<CollideState, glm::dvec2> collideState_from_circular_2_capsule(   const Circular &dogoCir_,
                                                            const Capsule  &begoCap_,
                                                            double threshold_ ) noexcept {
    
    double sum_of_two_raidus = dogoCir_.radius + begoCap_.radius;
    double len {};
    glm::dvec2 capRoot_2_cirRoot = dogoCir_.dpos - begoCap_.dpos;
    glm::dvec2 capRoot_2_cirRootInn = calc_innVec( begoCap_.root_2_tail, capRoot_2_cirRoot );
    glm::dvec2 self_2_oth {};

    if( capRoot_2_cirRootInn.x < 0.0 ){
        len = glm::length( capRoot_2_cirRoot );
        self_2_oth = begoCap_.dpos - dogoCir_.dpos;

    }else if( capRoot_2_cirRootInn.x > begoCap_.longLen ){
        len = glm::length( capRoot_2_cirRoot - begoCap_.root_2_tail );
        self_2_oth = begoCap_.dpos + begoCap_.root_2_tail - dogoCir_.dpos;

    }else{
        len = std::abs(capRoot_2_cirRootInn.y);
        if( capRoot_2_cirRootInn.y > 0.0 ){
            self_2_oth = glm::dvec2{ begoCap_.root_2_tail.y, -begoCap_.root_2_tail.x };
        }else{
            self_2_oth = glm::dvec2{ -begoCap_.root_2_tail.y, begoCap_.root_2_tail.x };
        }
    }
    len -= sum_of_two_raidus;

    if( is_closeEnough(len, 0.0, threshold_) ){
        return { CollideState::Adjacent, self_2_oth };
    }else if( len < 0.0 ){
        return { CollideState::Intersect, self_2_oth };
    }else{
        return { CollideState::Separate, self_2_oth };
    }   
}


/* ===========================================================
 *                    capsuleCast 
 * -----------------------------------------------------------
 */
double capsuleCast( const glm::dvec2 &moveVec_,
                    const Circular &dogoCir_,
                    const Capsule  &begoCap_ ) noexcept {

    glm::dvec2 capRoot_2_cirRoot = dogoCir_.dpos - begoCap_.dpos;
    double sum_of_2_radius = dogoCir_.radius + begoCap_.radius;
    //-----
    //-- capsule 坐标系体内的 move 两端点值 --
    glm::dvec2 cirRootInn = calc_innVec( begoCap_.root_2_tail, capRoot_2_cirRoot );
    glm::dvec2 cirTailInn = calc_innVec( begoCap_.root_2_tail, capRoot_2_cirRoot + moveVec_ );
    glm::dvec2 moveVecInn = cirTailInn - cirRootInn;
    //--
    tprAssert( (cirTailInn.y >= -sum_of_2_radius) && (cirTailInn.y <= sum_of_2_radius) );

    double x {}; // root_2_tail 和线 y=n 的交点 x值
    bool is_rootCircular {false};//-- 相交与 root端 圆 --
    bool is_tailCircular {false};//-- 相交与 tail端 圆 --
    bool is_x            {false};//-- 相交与 x 所在点  --
    
    if( cirRootInn.y > sum_of_2_radius ){
        //==1== 上区间
        x = calc_intersectX( cirRootInn, moveVecInn, sum_of_2_radius );
        if( x < 0.0 ){
            is_rootCircular = true;
        }else if( x > begoCap_.longLen ){
            is_tailCircular = true;
        }else{
            is_x = true;
        }
    }else if( cirRootInn.y < -sum_of_2_radius ){
        //==2== 下区间
        x = calc_intersectX( cirRootInn, moveVecInn, -sum_of_2_radius );
        if( x < 0.0 ){
            is_rootCircular = true;
        }else if( x > begoCap_.longLen ){
            is_tailCircular = true;
        }else{
            is_x = true;
        }
    }else{  
        //==3== 中区间
        if( cirRootInn.x < 0.0 ){
            is_rootCircular = true;
        }else if( cirRootInn.x > begoCap_.longLen ){
            is_tailCircular = true;
        }else{
            //-- 此时，dogoCir 正与 begoCap 相交
            //   使用一个简略的方法 让其 退回 
            if( cirRootInn.y > 0.0 ){
                x = calc_intersectX( cirRootInn, moveVecInn, sum_of_2_radius );
            }else{
                x = calc_intersectX( cirRootInn, moveVecInn, -sum_of_2_radius );
            }
            is_x = true;
        }
    }

    if( is_rootCircular ){
        return circularCast(moveVecInn, 
                            Circular{ cirRootInn, dogoCir_.radius },
                            Circular{ glm::dvec2{}, begoCap_.radius } );
    }else if( is_tailCircular ){
        return circularCast(moveVecInn,
                            Circular{ cirRootInn, dogoCir_.radius },
                            Circular{ glm::dvec2{begoCap_.longLen, 0.0}, begoCap_.radius } );
    }else if( is_x ){
        return ((x-cirRootInn.x) / moveVecInn.x);
    }else{
        tprAssert(0);
        return 0.0; //never
    }
}



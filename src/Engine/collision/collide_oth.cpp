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
    bool isNeedSquare {false}; //- threshold_ 是否要做平方

    if( capRoot_2_cirRootInn.x < 0.0 ){
        self_2_oth = begoCap_.dpos - dogoCir_.dpos;
        //-- Avoid Radical Sign / 避免开根号 --
        const auto &l = capRoot_2_cirRoot;
        len = (l.x*l.x + l.y*l.y) - (sum_of_two_raidus*sum_of_two_raidus); 
        isNeedSquare = true;

    }else if( capRoot_2_cirRootInn.x > begoCap_.longLen ){
        self_2_oth = begoCap_.dpos + begoCap_.root_2_tail - dogoCir_.dpos;
        //-- Avoid Radical Sign / 避免开根号 --
        glm::dvec2 l = capRoot_2_cirRoot - begoCap_.root_2_tail;
        len = (l.x*l.x + l.y*l.y) - (sum_of_two_raidus*sum_of_two_raidus);
        isNeedSquare = true;

    }else{
        if( capRoot_2_cirRootInn.y > 0.0 ){
            self_2_oth = glm::dvec2{ begoCap_.root_2_tail.y, -begoCap_.root_2_tail.x };
        }else{
            self_2_oth = glm::dvec2{ -begoCap_.root_2_tail.y, begoCap_.root_2_tail.x };
        }
        len = std::abs(capRoot_2_cirRootInn.y) - sum_of_two_raidus;
        isNeedSquare = false;
    }

    double threshold = threshold_;
    if( isNeedSquare ){
        threshold *= threshold;
    }

    if( is_closeEnough<double>(len, 0.0, threshold) ){
        return { CollideState::Adjacent, self_2_oth };
    }else if( len < 0.0 ){
        return { CollideState::Intersect, self_2_oth };
    }else{
        return { CollideState::Separate, self_2_oth };
    }   
}





/* ===========================================================
 *            fastCollideCheck_from_arc_2_capsule
 * -----------------------------------------------------------
 */
bool fastCollideCheck_from_arc_2_capsule( const Arc &dogoArc_,
                                        const Capsule &begoCap_,
                                        double threshold_ ){
    
    double rd = dogoArc_.radius;
    double rb = begoCap_.radius;
    double sum_of_two_raidus = rd + rb;

    glm::dvec2 capRoot_2_arcRootInn = calc_innVec( begoCap_.root_2_tail, dogoArc_.dpos - begoCap_.dpos );

    if( capRoot_2_arcRootInn.x < 0.0 ){//  left Cir
        return fastCollideCheck_from_arc_2_circular( dogoArc_,
                                        begoCap_.get_rootCir(),
                                        threshold_ );

    }else if( capRoot_2_arcRootInn.x > begoCap_.longLen ){// right Cir
        return fastCollideCheck_from_arc_2_circular( dogoArc_,
                                        begoCap_.get_tailCir(),
                                        threshold_ );

    
    }else{// Mid box
        glm::dvec2 shortV = glm::dvec2{ 0.0, -capRoot_2_arcRootInn.y };

        double len = std::abs(capRoot_2_arcRootInn.y);
        

        if( is_closeEnough<double>(len, sum_of_two_raidus, threshold_) ){// Adjacent

            return collideCheck_between_2_arcs_in_same_circular(shortV, 
                                                            0.0,
                                                            dogoArc_.forward, 
                                                            dogoArc_.halfRadian );

        }else if( len > sum_of_two_raidus ){ // Separate
            return false;

        }else{ // Intersect

            //- 计算 capsule 中央盒 与 圆弧圆 的相互包围问题时，
            //  把 中央盒 也看作一个圆，一个与 圆弧圆 x值相同的 圆
            double minRadius = tprMin( rd, rb );
            double maxRadius = tprMax( rd, rb );        
            if( (len+minRadius) <= maxRadius ){ //- 小体被大体彻底包含
                return true; // IMM
            }

            double halfRadian = calc_halfRadian_in_2_intersect_circulars(rd, rb, len);
            return collideCheck_between_2_arcs_in_same_circular(shortV, 
                                                            halfRadian,
                                                            dogoArc_.forward, 
                                                            dogoArc_.halfRadian );
            
        }
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



/*
 * ====================== AnimActionPos.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.08.31
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#include "AnimActionPos.h"

//------------------- Engine --------------------//
#include "tprAssert.h"
#include "config.h" 
#include "tprMath.h"


#include "tprDebug.h" 


namespace fp_inn {//----------- namespace: fp_inn -------------//

    //======= circular =========//

    const std::vector<glm::dvec2> scales_xy { //- 
        glm::dvec2{  1.0,  0.0 },
        glm::dvec2{ -1.0,  0.0 },
        glm::dvec2{  0.0,  1.0 },
        glm::dvec2{  0.0, -1.0 }
    };

    const std::vector<glm::dvec2> scales_45_degree { //- 
        glm::dvec2{  0.8,  0.8 },
        glm::dvec2{  0.8, -0.8 },
        glm::dvec2{ -0.8,  0.8 },
        glm::dvec2{ -0.8, -0.8 }
    };

    const std::vector<glm::dvec2> scales_2m2_oth { //- 
        //-- 将 2m2圆每个象限 分为 4份 --
        glm::dvec2{  0.9,  0.4 },
        glm::dvec2{  0.9, -0.4 },
        glm::dvec2{ -0.9,  0.4 },
        glm::dvec2{ -0.9, -0.4 },
        //---
        glm::dvec2{  0.4,  0.9 },
        glm::dvec2{  0.4, -0.9 },
        glm::dvec2{ -0.4,  0.9 },
        glm::dvec2{ -0.4, -0.9 }
    };

    const std::vector<glm::dvec2> scales_3m3_oth { //- 
        //-- 将 3m3圆每个象限 分为 5份 --
        glm::dvec2{  0.95,  0.31 },
        glm::dvec2{  0.95, -0.31 },
        glm::dvec2{ -0.95,  0.31 },
        glm::dvec2{ -0.95, -0.31 },
        //---
        glm::dvec2{  0.31,  0.95 },
        glm::dvec2{  0.31, -0.95 },
        glm::dvec2{ -0.31,  0.95 },
        glm::dvec2{ -0.31, -0.95 },
        //---
        glm::dvec2{  0.81,  0.59 },
        glm::dvec2{  0.81, -0.59 },
        glm::dvec2{ -0.81,  0.59 },
        glm::dvec2{ -0.81, -0.59 },
        //---
        glm::dvec2{  0.59,  0.81 },
        glm::dvec2{  0.59, -0.81 },
        glm::dvec2{ -0.59,  0.81 },
        glm::dvec2{ -0.59, -0.81 }
    };

    std::vector<glm::dvec2> colliPointScales_cir_1m1 {};
    std::vector<glm::dvec2> colliPointScales_cir_2m2 {};
    std::vector<glm::dvec2> colliPointScales_cir_3m3 {};


    //======= capsule =========//
    const std::vector<glm::dvec2> scales_cap_leftCir { 
        glm::dvec2{  -1.0,  0.0 },
        //---
        glm::dvec2{  -0.9,  0.4 },
        glm::dvec2{  -0.4,  0.9 },
        //---
        glm::dvec2{  -0.9, -0.4 },
        glm::dvec2{  -0.4, -0.9 }
    };

    const std::vector<glm::dvec2> scales_cap_rightCir { 
        glm::dvec2{  1.0,  0.0 },
        //---
        glm::dvec2{  0.9,  0.4 },
        glm::dvec2{  0.4,  0.9 },
        //---
        glm::dvec2{  0.9, -0.4 },
        glm::dvec2{  0.4, -0.9 }
    };

    const std::vector<glm::dvec2> scales_cap_midVertical { 
        glm::dvec2{  0.0,  1.0 },
        glm::dvec2{  0.0,  0.0 },
        glm::dvec2{  0.0, -1.0 }
    };

    std::vector<glm::dvec2> colliPoints_cap_tmp {}; //- 存储中间件


}//--------------- namespace: fp_inn end -----------------//

/* ===========================================================
 *               prepare_colliPointOffs
 * -----------------------------------------------------------
 */
void AnimActionPos::prepare_colliPointOffs(){

    //----- colliPointScales_cir_1m1 ------//
    fp_inn::colliPointScales_cir_1m1.push_back( glm::dvec2{ 0.0, 0.0 } );
    fp_inn::colliPointScales_cir_1m1.insert( fp_inn::colliPointScales_cir_1m1.end(),
                                                    fp_inn::scales_xy.cbegin(), 
                                                    fp_inn::scales_xy.cend() );

    fp_inn::colliPointScales_cir_1m1.insert( fp_inn::colliPointScales_cir_1m1.end(),
                                                    fp_inn::scales_45_degree.cbegin(), 
                                                    fp_inn::scales_45_degree.cend() );

        cout << "   colliPointScales_cir_1m1.size() = " << fp_inn::colliPointScales_cir_1m1.size() << endl;

    //----- colliPointScales_cir_2m2 ------//
    //-- 直接继承 2m2 的数据，缩小为 1/2 ----
    for( const auto &i : fp_inn::colliPointScales_cir_1m1 ){
        fp_inn::colliPointScales_cir_2m2.push_back( i * 0.5 );
    }

    //-- 再添加 2m2 特有的 外层一圈 --
    fp_inn::colliPointScales_cir_2m2.insert( fp_inn::colliPointScales_cir_2m2.end(),
                                                    fp_inn::scales_xy.cbegin(), 
                                                    fp_inn::scales_xy.cend() );

    fp_inn::colliPointScales_cir_2m2.insert( fp_inn::colliPointScales_cir_2m2.end(),
                                                    fp_inn::scales_45_degree.cbegin(), 
                                                    fp_inn::scales_45_degree.cend() );

    fp_inn::colliPointScales_cir_2m2.insert( fp_inn::colliPointScales_cir_2m2.end(),
                                                    fp_inn::scales_2m2_oth.cbegin(), 
                                                    fp_inn::scales_2m2_oth.cend() );


        cout << "   colliPointScales_cir_2m2.size() = " << fp_inn::colliPointScales_cir_2m2.size() << endl;

    //----- colliPointScales_cir_3m3 ------//
    //-- 直接继承 2m2 的数据，缩小为 2/3 ----
    for( const auto &i : fp_inn::colliPointScales_cir_2m2 ){
        fp_inn::colliPointScales_cir_3m3.push_back( i * 0.66 );
    }

    //-- 再添加 3m3 特有的 外层一圈 --
    fp_inn::colliPointScales_cir_3m3.insert( fp_inn::colliPointScales_cir_3m3.end(),
                                                    fp_inn::scales_xy.cbegin(), 
                                                    fp_inn::scales_xy.cend() );

    fp_inn::colliPointScales_cir_3m3.insert( fp_inn::colliPointScales_cir_3m3.end(),
                                                    fp_inn::scales_3m3_oth.cbegin(), 
                                                    fp_inn::scales_3m3_oth.cend() );


    cout << "   colliPointScales_cir_3m3.size() = " << fp_inn::colliPointScales_cir_3m3.size() << endl;

}


/* ===========================================================
 *                  init_from_semiData
 * -----------------------------------------------------------
 */
void AnimActionPos::init_from_semiData( const AnimActionSemiData &semiData_ ){

        this->rootAnchorDPosOff = semiData_.get_rootAnchor();
        this->lGoAltiRange = semiData_.get_lGoAltiRange();

        auto colliderType = semiData_.get_colliderType();
        if( colliderType == ColliderType::Nil ){

            std::unique_ptr<ColliDataFromJ_Nil> nilUPtr = std::make_unique<ColliDataFromJ_Nil>();
            auto *nilPtr = nilUPtr.get();
            nilPtr->colliderType = ColliderType::Nil;
            this->colliDataFromJUPtr.reset( nilUPtr.release() );//- move uptr

        }else if( colliderType == ColliderType::Circular ){

            
            std::unique_ptr<ColliDataFromJ_Circular> cirUPtr = std::make_unique<ColliDataFromJ_Circular>();
            auto *cirPtr = cirUPtr.get();
            cirPtr->colliderType = ColliderType::Circular;
            cirPtr->moveColliRadius  = glm::length( semiData_.get_moveColliRadiusAnchor() - this->rootAnchorDPosOff );
            cirPtr->skillColliRadius = glm::length( semiData_.get_skillColliRadiusAnchor() - this->rootAnchorDPosOff );
            //-- colliPoints --
            this->calc_colliPoints_for_circular( cirPtr, tprMax(cirPtr->moveColliRadius, cirPtr->skillColliRadius));

            cirPtr->makeSure_colliPointDPosOffs_isNotEmpty();
            this->colliDataFromJUPtr.reset( cirUPtr.release() );//- move uptr

        }else if( colliderType == ColliderType::Capsule ){


            std::unique_ptr<ColliDataFromJ_Capsule> capUPtr = std::make_unique<ColliDataFromJ_Capsule>();
            auto *capPtr = capUPtr.get();
            capPtr->colliderType = ColliderType::Capsule;
            capPtr->rootAnchor_2_tailAnchor = semiData_.get_tailAnchor() - this->rootAnchorDPosOff;
            capPtr->moveColliRadius = glm::length( semiData_.get_moveColliRadiusAnchor() - this->rootAnchorDPosOff );
            capPtr->skillColliRadius = capPtr->moveColliRadius;
            capPtr->longLen = glm::length( capPtr->rootAnchor_2_tailAnchor );
            //-- colliPoints --
            this->calc_colliPoints_for_capsule( capPtr, capPtr->moveColliRadius, capPtr->longLen, capPtr->rootAnchor_2_tailAnchor );
            capPtr->makeSure_colliPointDPosOffs_isNotEmpty();
            this->colliDataFromJUPtr.reset( capUPtr.release() );//- move uptr

        }else{
            tprAssert(0);
        }
}


/* ===========================================================
 *              calc_colliPoints_for_circular
 * -----------------------------------------------------------
 */
void AnimActionPos::calc_colliPoints_for_circular(  ColliDataFromJ_Circular *cirPtr_,
                                                    double radius_ ){

    tprAssert( radius_ > 0.0 );

    double radius = radius_ + 10.0; //- 适当向外延伸 
    double scale  = radius / static_cast<double>(PIXES_PER_MAPENT);

    cirPtr_->colliPointDPosOffs.clear();

    if( scale <= 1.0 ){
        for( const auto &i : fp_inn::colliPointScales_cir_1m1 ){
            cirPtr_->colliPointDPosOffs.push_back( i * radius );
        }
    }else if( scale <= 2.0 ){
        for( const auto &i : fp_inn::colliPointScales_cir_2m2 ){
            cirPtr_->colliPointDPosOffs.push_back( i * radius );
        }
    }else if( scale <= 3.0 ){
        for( const auto &i : fp_inn::colliPointScales_cir_3m3 ){
            cirPtr_->colliPointDPosOffs.push_back( i * radius );
        }
    }else{
        tprAssert(0); // not support jet
    }
}


/* ===========================================================
 *             calc_colliPoints_for_capsule
 * -----------------------------------------------------------
 */
void AnimActionPos::calc_colliPoints_for_capsule(ColliDataFromJ_Capsule *capPtr_,
                                                double radius_, 
                                                double longLen_,
                                                const glm::dvec2 &rootAnchor_2_tailAnchor_ ){

    tprAssert( (radius_>0.0) && (longLen_>0.0) );
    tprAssert( radius_ <= static_cast<double>(PIXES_PER_MAPENT) ); //- 暂不支持更大的 capsule
    double radius = radius_ + 20.0; //- 适当向外衍生
    double scale = radius / static_cast<double>(PIXES_PER_MAPENT);

    double blockNum = ceil(longLen_ / static_cast<double>(PIXES_PER_MAPENT)); //- capsule中间的长方体,被切成几块
    double step = longLen_ / blockNum; //- 切割后，每段间距

    glm::dvec2 secRootOff { longLen_, 0.0 };

    glm::dvec2 cutOff { 0.0, 0.0 };

    //--- 将 capsule 躺平在x轴，收集 collipoints ---//
    fp_inn::colliPoints_cap_tmp.clear();

    for( const auto &i : fp_inn::scales_cap_leftCir ){
        fp_inn::colliPoints_cap_tmp.push_back( i * radius );
    }
    for( const auto &i : fp_inn::scales_cap_rightCir ){
        fp_inn::colliPoints_cap_tmp.push_back( (i*radius) + secRootOff );
    }

    for( int i=0; i <= static_cast<int>(blockNum); i++ ){
        cutOff.x = static_cast<double>(i) * step;
        for( const auto &i : fp_inn::scales_cap_midVertical ){
            fp_inn::colliPoints_cap_tmp.push_back( (i*radius) + cutOff );
        }
    }

    //---- 将 临时容器中的 collipoint，旋转一个角度，变成最终值 -----//
    capPtr_->colliPointDPosOffs.clear();

    for( const auto &i : fp_inn::colliPoints_cap_tmp ){
        capPtr_->colliPointDPosOffs.push_back( rotate_vec(i, rootAnchor_2_tailAnchor_) );
    }
}








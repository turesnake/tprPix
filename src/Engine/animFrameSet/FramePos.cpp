/*
 * ======================== FramePos.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.08.31
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#include "FramePos.h"

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
void FramePos::prepare_colliPointOffs(){

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
void FramePos::init_from_semiData( const FramePosSemiData &semiData_ ){

        this->colliderType = semiData_.get_colliderType();
        this->rootAnchorDPosOff = semiData_.get_rootAnchor();
        this->lGoAltiRange = semiData_.get_lGoAltiRange();

        if( this->colliderType == ColliderType::Nil ){
            this->circularUPtr = nullptr;
            this->capsuleUPtr  = nullptr;
            //---- functors -----//
            this->get_rootAnchor_2_tailAnchor = nullptr;
            this->get_moveColliRadius        = nullptr;
            this->get_skillColliRadius       = nullptr;
            this->get_colliPointDPosOffsRef  = nullptr;

        }else if( this->colliderType == ColliderType::Circular ){
            this->circularUPtr = std::make_unique<JData_InCircular>();
            this->capsuleUPtr  = nullptr;
            JData_InCircular *cirPtr = this->circularUPtr.get();
            //==
            cirPtr->moveColliRadius  = glm::length( semiData_.get_moveColliRadiusAnchor() - this->rootAnchorDPosOff );
            cirPtr->skillColliRadius = glm::length( semiData_.get_skillColliRadiusAnchor() - this->rootAnchorDPosOff );
            //-- colliPoints --

                        //  这组点将被 自动生成 ....
            /*
            for( const auto &i : semiData_.get_colliPointsRef() ){
                cirPtr->colliPointDPosOffs.push_back( i - this->rootAnchorDPosOff );
            }
            cirPtr->colliPointDPosOffs.push_back( glm::dvec2{0.0,0.0} );   //- rootAnchor
            */

            this->calc_colliPoints_for_circular(tprMax(cirPtr->moveColliRadius, cirPtr->skillColliRadius));



            //---- functors -----//
            this->get_rootAnchor_2_tailAnchor = nullptr;
            this->get_moveColliRadius        = std::bind( &JData_InCircular::get_moveColliRadius, cirPtr );
            this->get_skillColliRadius       = std::bind( &JData_InCircular::get_skillColliRadius, cirPtr );
            this->get_colliPointDPosOffsRef  = std::bind( &JData_InCircular::get_colliPointDPosOffs, cirPtr );

                tprAssert( !this->get_colliPointDPosOffsRef().empty() );


        }else if( this->colliderType == ColliderType::Capsule ){
            this->circularUPtr = nullptr;
            this->capsuleUPtr = std::make_unique<JData_InCapsule>();
            JData_InCapsule *capPtr = this->capsuleUPtr.get();
            //==
            capPtr->rootAnchor_2_tailAnchor = semiData_.get_tailAnchor() - this->rootAnchorDPosOff;
            capPtr->moveColliRadius = glm::length( semiData_.get_moveColliRadiusAnchor() - this->rootAnchorDPosOff );
            capPtr->skillColliRadius = capPtr->moveColliRadius;
            capPtr->longLen = glm::length( capPtr->rootAnchor_2_tailAnchor );


            //-- colliPoints --
            /*
            for( const auto &i : semiData_.get_colliPointsRef() ){
                capPtr->colliPointDPosOffs.push_back( i - this->rootAnchorDPosOff );
            }
            capPtr->colliPointDPosOffs.push_back( glm::dvec2{0.0,0.0} );            //- rootAnchor
            capPtr->colliPointDPosOffs.push_back( capPtr->rootAnchor_2_tailAnchor );//- tailAnchor
            \*/
            
            this->calc_colliPoints_for_capsule( capPtr->moveColliRadius, capPtr->longLen, capPtr->rootAnchor_2_tailAnchor );



                /*
                cout<< "capsule: moveColliRadius = " << capPtr->moveColliRadius << "\n"
                    << "capsule: skillColliRadius = " << capPtr->skillColliRadius << "\n"
                    << "capsule: colliPointDPosOffs.size() = " << capPtr->colliPointDPosOffs.size() 
                    << endl;
                */

            //---- functors -----//
            this->get_rootAnchor_2_tailAnchor= std::bind( &JData_InCapsule::get_rootAnchor_2_tailAnchor, capPtr );
            this->get_moveColliRadius        = std::bind( &JData_InCapsule::get_moveColliRadius, capPtr );
            this->get_skillColliRadius       = std::bind( &JData_InCapsule::get_skillColliRadius, capPtr );
            this->get_colliPointDPosOffsRef  = std::bind( &JData_InCapsule::get_colliPointDPosOffs, capPtr );

                tprAssert( !this->get_colliPointDPosOffsRef().empty() );


        }else{
            tprAssert(0);
        }
}


/* ===========================================================
 *              calc_colliPoints_for_circular
 * -----------------------------------------------------------
 */
void FramePos::calc_colliPoints_for_circular( double radius_ ){

    tprAssert( radius_ > 0.0 );

    double radius = radius_ + 10.0; //- 适当向外延伸 
    double scale  = radius / static_cast<double>(PIXES_PER_MAPENT);

    JData_InCircular *cirPtr = this->circularUPtr.get();
    cirPtr->colliPointDPosOffs.clear();

    if( scale <= 1.0 ){
            //cout << "   1.0" << endl;
        for( const auto &i : fp_inn::colliPointScales_cir_1m1 ){
            cirPtr->colliPointDPosOffs.push_back( i * radius );
        }

    }else if( scale <= 2.0 ){
            //cout << "   2.0" << endl;
        for( const auto &i : fp_inn::colliPointScales_cir_2m2 ){
            cirPtr->colliPointDPosOffs.push_back( i * radius );
        }

    }else if( scale <= 3.0 ){
            //cout << "   3.0" << endl;
        for( const auto &i : fp_inn::colliPointScales_cir_3m3 ){
            cirPtr->colliPointDPosOffs.push_back( i * radius );
        }

    }else{
        tprAssert(0); // not support jet
    }
}


/* ===========================================================
 *             calc_colliPoints_for_capsule
 * -----------------------------------------------------------
 */
void FramePos::calc_colliPoints_for_capsule(double radius_, 
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
    JData_InCapsule *capPtr = this->capsuleUPtr.get();
    capPtr->colliPointDPosOffs.clear();

    for( const auto &i : fp_inn::colliPoints_cap_tmp ){
        capPtr->colliPointDPosOffs.push_back( rotate_vec(i, rootAnchor_2_tailAnchor_) );
    }
}








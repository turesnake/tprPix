/*
 * ================== BioSoupParticle.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2020.03.05
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */

//-------------------- CPP --------------------//
#include <vector>


//-------------------- Engine --------------------//
#include "tprAssert.h"
#include "config.h"
#include "IntVec.h"
#include "random.h"
#include "GameObj.h"
#include "assemble_go.h"
#include "GoDataForCreate.h"
#include "GoSpecFromJson.h"

#include "esrc_coordinate.h"
#include "esrc_animFrameSet.h"

#include "tprDebug.h"


//-------------------- Script --------------------//
#include "Script/gameObjs/bioSoup/BioSoupDataForCreate.h"


namespace gameObjs::bioSoup {//------------- namespace gameObjs::bioSoup ----------------
namespace bioSoupP_inn {//------------------ namespace: bioSoupP_inn ---------------------//


    std::vector<animSubspeciesId_t> animSubspeciesIds {};


    // 若干份数据，每一份，都包含一组 打乱了次序的点，在这个点上 创建 particle-gomesh
    std::vector<std::vector<glm::dvec2>> goMesh_dposOffSets_2m2 {};
    std::vector<std::vector<glm::dvec2>> goMesh_dposOffSets_4m4 {};

    
    std::vector<std::vector<size_t>> createStepSets {}; 






}//--------------------- namespace: bioSoupP_inn end ------------------------//



void init_for_particle(){

    std::default_random_engine  randEngine;
    randEngine.seed( get_new_seed() );
    

    {
        const auto &worldCoorldRef = esrc::get_worldCoordRef();

        std::vector<glm::dvec2> goMesh_dposOffs_2m2 {}; // 单组
        std::vector<glm::dvec2> goMesh_dposOffs_4m4 {}; // 单组

        
        constexpr glm::dvec2 centerDposOff_2m2 { PIXES_PER_MAPENT_D, PIXES_PER_MAPENT_D };
        constexpr glm::dvec2 centerDposOff_4m4 { HALF_PIXES_PER_FIELD_D, HALF_PIXES_PER_FIELD_D };


        for( int j=0; j<HALF_ENTS_PER_FIELD; j++ ){
            for( int i=0; i<HALF_ENTS_PER_FIELD; i++ ){

                glm::dvec2 point { static_cast<double>(i), static_cast<double>(j) }; // base on left=bottom
                point *= PIXES_PER_MAPENT_D;
                glm::dvec2 off = point - centerDposOff_2m2;
                //--
                goMesh_dposOffs_2m2.push_back( worldCoorldRef.calc_outDPos( off ) );
            }
        }

        for( int j=0; j<ENTS_PER_FIELD; j++ ){
            for( int i=0; i<ENTS_PER_FIELD; i++ ){

                glm::dvec2 point { static_cast<double>(i), static_cast<double>(j) }; // base on left=bottom
                point *= PIXES_PER_MAPENT_D;
                glm::dvec2 off = point - centerDposOff_4m4;
                //--
                goMesh_dposOffs_4m4.push_back( worldCoorldRef.calc_outDPos( off ) );
            }
        }
        
        // copy and shuffle

        // 生成 21 份数据 2m2
        for( int n=0; n<21; n++ ){
            bioSoupP_inn::goMesh_dposOffSets_2m2.push_back( std::vector<glm::dvec2>{} ); // empty
            auto &v2Ref = bioSoupP_inn::goMesh_dposOffSets_2m2.back();
            v2Ref.insert( v2Ref.end(), goMesh_dposOffs_2m2.begin(), goMesh_dposOffs_2m2.end() );
            v2Ref.insert( v2Ref.end(), goMesh_dposOffs_2m2.begin(), goMesh_dposOffs_2m2.end() ); // 复制两份
            std::shuffle( v2Ref.begin(), v2Ref.end(), randEngine );
        }

        // 生成 17 份数据 4m4
        for( int n=0; n<17; n++ ){
            bioSoupP_inn::goMesh_dposOffSets_4m4.push_back( std::vector<glm::dvec2>{} ); // empty
            auto &v4Ref = bioSoupP_inn::goMesh_dposOffSets_4m4.back();
            v4Ref.insert( v4Ref.end(), goMesh_dposOffs_2m2.begin(), goMesh_dposOffs_2m2.end() ); // 只复制一份
            std::shuffle( v4Ref.begin(), v4Ref.end(), randEngine );
        }
    }

    //===========//
    bioSoupP_inn::animSubspeciesIds.push_back( esrc::apply_a_random_animSubspeciesId( "bioSoup_particle", "Rise_1", 7 ) );
    //... 


    //===========//
    std::uniform_int_distribution<size_t> uDistribution( 31, 91 );

    for( int n=0; n<33; n++ ){

        bioSoupP_inn::createStepSets.push_back( std::vector<size_t>{} );
        auto &vRef = bioSoupP_inn::createStepSets.back();

        // 一份数据，17个 值
        for( int i=0; i<17; i++ ){
            vRef.push_back( uDistribution(randEngine) );
        }
    }

    cout << "bioSoup::particle: init done" << endl;
}
























}//------------- namespace gameObjs::bioSoup: end ----------------


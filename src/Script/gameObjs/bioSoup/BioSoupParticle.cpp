/*
 * ================== BioSoupParticle.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2020.03.05
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#include "pch.h"
#include "Script/gameObjs/bioSoup/BioSoupParticle.h"


//-------------------- Engine --------------------//
#include "random.h"
#include "assemble_go.h"
#include "GoDataForCreate.h"
#include "GoSpecFromJson.h"
#include "GoAssemblePlan.h"

#include "esrc_coordinate.h"
#include "esrc_animFrameSet.h"


//-------------------- Script --------------------//
#include "Script/gameObjs/bioSoup/BioSoupDataForCreate.h"




namespace gameObjs::bioSoup {//------------- namespace gameObjs::bioSoup ----------------
//namespace bioSoupP_inn {//------------------ namespace: bioSoupP_inn ---------------------//
//}//--------------------- namespace: bioSoupP_inn end ------------------------//


//======= static =======//
bool                                        BioSoupParticle::isStaticInit {false};
std::vector<std::vector<animSubspeciesId_t>> BioSoupParticle::animSubspeciesIdSets {};
std::vector<std::vector<glm::dvec2>>        BioSoupParticle::goMeshDposOffSets_2m2 {};
std::vector<std::vector<glm::dvec2>>        BioSoupParticle::goMeshDposOffSets_4m4 {};
std::vector<std::vector<size_t>>            BioSoupParticle::createStepSets {}; 


void BioSoupParticle::init_for_static(){

    std::default_random_engine  randEngine;
    randEngine.seed( get_new_seed() );
    

    {
        const auto &worldCoorldRef = esrc::get_worldCoordRef();

        std::vector<glm::dvec2> goMesh_dposOffs_2m2 {}; // 单组
        std::vector<glm::dvec2> goMesh_dposOffs_4m4 {}; // 单组

        
        constexpr glm::dvec2 centerDposOff_2m2 { PIXES_PER_MAPENT_D, PIXES_PER_MAPENT_D };
        constexpr glm::dvec2 centerDposOff_4m4 { HALF_PIXES_PER_FIELD_D, HALF_PIXES_PER_FIELD_D };


        for( int j=0; j<HALF_ENTS_PER_FIELD<>; j++ ){
            for( int i=0; i<HALF_ENTS_PER_FIELD<>; i++ ){

                glm::dvec2 point { static_cast<double>(i), static_cast<double>(j) }; // base on left=bottom
                point *= PIXES_PER_MAPENT_D;
                glm::dvec2 off = point - centerDposOff_2m2;
                //--
                goMesh_dposOffs_2m2.push_back( worldCoorldRef.calc_outDPos( off ) );
            }
        }

        for( int j=0; j<ENTS_PER_FIELD<>; j++ ){
            for( int i=0; i<ENTS_PER_FIELD<>; i++ ){

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
            //BioSoupParticle::goMeshDposOffSets_2m2.push_back( std::vector<glm::dvec2>{} ); // empty
            auto &v2Ref = BioSoupParticle::goMeshDposOffSets_2m2.emplace_back(); // empty
            //auto &v2Ref = BioSoupParticle::goMeshDposOffSets_2m2.back();
            v2Ref.insert( v2Ref.end(), goMesh_dposOffs_2m2.begin(), goMesh_dposOffs_2m2.end() );
            v2Ref.insert( v2Ref.end(), goMesh_dposOffs_2m2.begin(), goMesh_dposOffs_2m2.end() ); // 复制两份
            std::shuffle( v2Ref.begin(), v2Ref.end(), randEngine );
        }

        // 生成 17 份数据 4m4
        for( int n=0; n<17; n++ ){
            //BioSoupParticle::goMeshDposOffSets_4m4.push_back( std::vector<glm::dvec2>{} ); // empty
            auto &v4Ref = BioSoupParticle::goMeshDposOffSets_4m4.emplace_back(); // empty
            //auto &v4Ref = BioSoupParticle::goMeshDposOffSets_4m4.back();
            v4Ref.insert( v4Ref.end(), goMesh_dposOffs_2m2.begin(), goMesh_dposOffs_2m2.end() ); // 只复制一份
            std::shuffle( v4Ref.begin(), v4Ref.end(), randEngine );
        }
    }

    //===========//
    {
        std::vector<animSubspeciesId_t> allIds;
        allIds.insert( allIds.end(), 2, esrc::apply_a_random_animSubspeciesId( "bioSoup_particle", "Rise_1", 11 ) );
        allIds.insert( allIds.end(), 1, esrc::apply_a_random_animSubspeciesId( "bioSoup_particle", "Rise_2", 11 ) );
        allIds.insert( allIds.end(), 1, esrc::apply_a_random_animSubspeciesId( "bioSoup_particle", "Rise_3", 11 ) );
        allIds.insert( allIds.end(), 2, esrc::apply_a_random_animSubspeciesId( "bioSoup_particle", "Rise_4", 11 ) );
        allIds.insert( allIds.end(), 1, esrc::apply_a_random_animSubspeciesId( "bioSoup_particle", "Rise_5", 11 ) );

        // 17 种 方案
        for( int n=0; n<17; n++ ){
            //BioSoupParticle::animSubspeciesIdSets.push_back( std::vector<animSubspeciesId_t>{} );
            auto &vRef = BioSoupParticle::animSubspeciesIdSets.emplace_back();
            //auto &vRef = BioSoupParticle::animSubspeciesIdSets.back();
            vRef.insert( vRef.end(), allIds.begin(), allIds.end() );
            vRef.insert( vRef.end(), allIds.begin(), allIds.end() ); // twice
            std::shuffle( vRef.begin(), vRef.end(), randEngine );
        }
    }


    // more ...

    //===========//
    std::uniform_int_distribution<size_t> uDistribution_fst( 1, 51 );
    std::uniform_int_distribution<size_t> uDistribution( 111, 311 );

    for( int n=0; n<33; n++ ){
        //BioSoupParticle::createStepSets.push_back( std::vector<size_t>{} );
        auto &vRef = BioSoupParticle::createStepSets.emplace_back();
        //auto &vRef = BioSoupParticle::createStepSets.back();
        // 一份数据，17个 值
        vRef.push_back( uDistribution_fst(randEngine) ); 
        vRef.push_back( uDistribution_fst(randEngine) ); // 前2个值通常比较小
        for( int i=0; i<15; i++ ){
            vRef.push_back( uDistribution(randEngine) );
        }
    }

    //===========//


}
























}//------------- namespace gameObjs::bioSoup: end ----------------


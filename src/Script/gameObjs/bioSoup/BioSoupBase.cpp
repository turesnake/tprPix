/*
 * ===================== BioSoupBase.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2020.03.06
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#include "pch.h"
#include "Script/gameObjs/bioSoup/BioSoupBase.h"

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

//======= static =======//
bool                                            BioSoupBase::isStaticInit {};
std::vector<std::vector<animSubspeciesId_t>>    BioSoupBase::animSubspeciesIdSets_1m1 {};
std::vector<std::vector<animSubspeciesId_t>>    BioSoupBase::animSubspeciesIdSets_2m2 {};
std::vector<std::vector<animSubspeciesId_t>>    BioSoupBase::animSubspeciesIdSets_4m4 {};



void BioSoupBase::init_for_static(){

    std::default_random_engine  randEngine;
    randEngine.seed( get_new_seed() );

    // 快捷函数
    auto f_insert = [ &randEngine ](std::vector<std::vector<animSubspeciesId_t>>    &targetV_,
                        std::vector<animSubspeciesId_t>                 &originIds_,
                        int                                             num_ ){
        for( int n=0; n<num_; n++ ){
            auto &vRef = targetV_.emplace_back();
            //auto &vRef = targetV_.back();
            vRef.insert( vRef.end(), originIds_.begin(), originIds_.end() );
            vRef.insert( vRef.end(), originIds_.begin(), originIds_.end() ); // twice
            std::shuffle( vRef.begin(), vRef.end(), randEngine );
        }
    };

    {// 1m1
        std::vector<animSubspeciesId_t> ids;
        ids.insert( ids.end(), 1, esrc::apply_a_random_animSubspeciesId( "bioSoup_active_1", "MapEnt_1m1_1", 11 ) );
        ids.insert( ids.end(), 1, esrc::apply_a_random_animSubspeciesId( "bioSoup_active_1", "MapEnt_1m1_2", 11 ) );
        ids.insert( ids.end(), 1, esrc::apply_a_random_animSubspeciesId( "bioSoup_active_1", "MapEnt_1m1_3", 11 ) );
        ids.insert( ids.end(), 1, esrc::apply_a_random_animSubspeciesId( "bioSoup_active_1", "MapEnt_1m1_4", 11 ) );
        ids.insert( ids.end(), 1, esrc::apply_a_random_animSubspeciesId( "bioSoup_active_1", "MapEnt_1m1_5", 11 ) );
        ids.insert( ids.end(), 1, esrc::apply_a_random_animSubspeciesId( "bioSoup_active_1", "MapEnt_1m1_6", 11 ) );
        ids.insert( ids.end(), 1, esrc::apply_a_random_animSubspeciesId( "bioSoup_active_1", "MapEnt_1m1_7", 11 ) );
        //=
        f_insert( BioSoupBase::animSubspeciesIdSets_1m1, ids, 17 );
    }

    {// 2m2
        std::vector<animSubspeciesId_t> ids;
        ids.insert( ids.end(), 1, esrc::apply_a_random_animSubspeciesId( "bioSoup_active_1", "MapEnt_2m2_1", 11 ) );
        ids.insert( ids.end(), 1, esrc::apply_a_random_animSubspeciesId( "bioSoup_active_1", "MapEnt_2m2_2", 11 ) );
        ids.insert( ids.end(), 1, esrc::apply_a_random_animSubspeciesId( "bioSoup_active_1", "MapEnt_2m2_3", 11 ) );
        ids.insert( ids.end(), 1, esrc::apply_a_random_animSubspeciesId( "bioSoup_active_1", "MapEnt_2m2_4", 11 ) );
        ids.insert( ids.end(), 1, esrc::apply_a_random_animSubspeciesId( "bioSoup_active_1", "MapEnt_2m2_5", 11 ) );
        ids.insert( ids.end(), 1, esrc::apply_a_random_animSubspeciesId( "bioSoup_active_1", "MapEnt_2m2_6", 11 ) );
        ids.insert( ids.end(), 1, esrc::apply_a_random_animSubspeciesId( "bioSoup_active_1", "MapEnt_2m2_7", 11 ) );
        //=
        f_insert( BioSoupBase::animSubspeciesIdSets_2m2, ids, 17 );
    }

    {// 4m4
        std::vector<animSubspeciesId_t> ids;
        ids.insert( ids.end(), 1, esrc::apply_a_random_animSubspeciesId( "bioSoup_active_1", "MapEnt_4m4_1", 11 ) );
        ids.insert( ids.end(), 1, esrc::apply_a_random_animSubspeciesId( "bioSoup_active_1", "MapEnt_4m4_2", 11 ) );
        ids.insert( ids.end(), 1, esrc::apply_a_random_animSubspeciesId( "bioSoup_active_1", "MapEnt_4m4_3", 11 ) );
        //=
        f_insert( BioSoupBase::animSubspeciesIdSets_4m4, ids, 17 );
    }

}


}//------------- namespace gameObjs::bioSoup: end ----------------


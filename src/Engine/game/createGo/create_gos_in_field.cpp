/*
 * ================= create_gos_in_field.cpp ======================
 *                          -- tpr --
 *                                        CREATE -- 2019.09.27
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */



//-------------------- Engine --------------------//
#include "tprAssert.h"
#include "esrc_field.h"
#include "config.h"
#include "chunkKey.h"
#include "dyParams.h"
#include "GameObj.h"
#include "create_goes.h"

#include "esrc_field.h"
#include "esrc_ecoObj.h"
#include "esrc_gameObj.h" 
#include "esrc_time.h" 
#include "esrc_chunk.h" 
#include "esrc_job_chunk.h"
#include "esrc_mapSurfaceRand.h"


//-------------------- Script --------------------//
#include "Script/resource/ssrc_all.h" 


/* ===========================================================
 *                create_gos_in_field     
 * -----------------------------------------------------------
 * only called in chunkCreate
 * all kinds of gos 
 */
void create_gos_in_field(   fieldKey_t      fieldKey_, 
                            const Chunk     &chunkRef_,
                            const Job_Chunk &job_chunkRef_ ){

    const auto &fieldRef = esrc::get_field(fieldKey_);
    const auto *job_fieldPtr = job_chunkRef_.get_job_fieldPtr(fieldKey_);

    //----- ground go ------//
    {
        //--- dyParam ---//
        DyParam dyParam {};
        auto gUPtr = std::make_unique<DyParams_GroundGo>();
        gUPtr->fieldUWeight = fieldRef.get_uWeight();
        gUPtr->job_fieldPtr = job_fieldPtr;
        dyParam.insert_ptr<DyParams_GroundGo>( gUPtr.get() );
        //--- 
        gameObjs::create_a_Go(  ssrc::get_goSpecId( "groundGo" ),
                                    fieldRef.get_midDPos(),
                                    dyParam );
    }

    //----- mapsurface go ------//
    {               
        auto entId = chunkRef_.get_mapSurfaceRandEntId();
        const auto &outPair = esrc::get_mapSurfaceRandEntData( entId, fieldRef.calc_fieldIdx_in_chunk() );
        const auto &mapSurfaceLvl = outPair.first;
        const auto &dposOff = outPair.second;

        if( mapSurfaceLvl != mapSurface::RandEntLvl::Nil ){
            //--- dyParam ---//
            DyParam dyParam {};     
            auto mUPtr = std::make_unique<DyParams_MapSurface>();
            mUPtr->spec = MapSurfaceLowSpec::WhiteRock; //- tmp，其实要根据 eco 来分配 ...
            mUPtr->lvl = mapSurfaceLvl;
            mUPtr->randUVal = fieldRef.get_uWeight();
            dyParam.insert_ptr<DyParams_MapSurface>( mUPtr.get() );
            //--- 
            gameObjs::create_a_Go(  ssrc::get_goSpecId( "mapSurfaceLower" ),
                                    fieldRef.get_dpos() + dposOff,
                                    //fieldRef.get_midDPos(),
                                    dyParam );
        }
    }

    //----- fieldRim go [-DEBUG-] ------//
    bool isFieldRimGoCreate { true };
    if( isFieldRimGoCreate ){
        gameObjs::create_a_Go(  ssrc::get_goSpecId( "fieldRim" ),
                                fieldRef.get_midDPos(),
                                emptyDyParam );
    }
    
    //----- land go -----//
    for( const auto &i : job_fieldPtr->get_job_goDatas() ){
        const auto &animLabels = i.goSpecDataPtr->get_animLabels();
        //--- dyParam ---//
        DyParam dyParam {};
        auto fUPtr = std::make_unique<DyParams_Field>();
        fUPtr->uWeight = i.job_mapEntPtr->uWeight;
        fUPtr->mapEntAlti = i.job_mapEntPtr->alti;
        fUPtr->mapEntDensity = i.job_mapEntPtr->density;
        for( const auto &ent : animLabels ){
            fUPtr->animLabels.push_back( ent );
        }
        dyParam.insert_ptr<DyParams_Field>( fUPtr.get() );
        //---
        gameObjs::create_a_Go(  i.goSpecDataPtr->get_goSpecId(),
                                fieldRef.get_midDPos() + i.dposOff,
                                dyParam );
    }

}



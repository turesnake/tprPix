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
#include "ParamBinary.h"
#include "GameObj.h"
#include "create_goes.h"

#include "esrc_field.h"
#include "esrc_ecoObj.h"
#include "esrc_gameObj.h" 
#include "esrc_time.h" 
#include "esrc_chunk.h" 
#include "esrc_chunkData.h"
#include "esrc_mapSurfaceRand.h"


//-------------------- Script --------------------//
#include "Script/resource/ssrc.h" 


/* ===========================================================
 *                create_gos_in_field     
 * -----------------------------------------------------------
 * only called in chunkCreate
 * all kinds of gos 
 */
void create_gos_in_field( fieldKey_t fieldKey_, const Chunk &chunkRef_ ){

    const auto &fieldRef = esrc::atom_get_field(fieldKey_);

    sectionKey_t ecoObjKey = fieldRef.get_ecoObjKey();

    double randV = fieldRef.get_uWeight() * 0.35 + 313.17; //- 确保大于0
    double fract = randV - floor(randV); //- 小数部分
    tprAssert( (fract>=0.0) && (fract<=1.0) );

    //----- ground go ------//
    {
        fieldKey_t  fieldKey {};
        auto &chunkDataRef = esrc::atom_getnc_chunkDataCRef( chunkRef_.get_key() );
        //--- dyParam ---//
        ParamBinary dyParam {};
        auto *BinaryPtr = dyParam.init_binary<DyParams_GroundGo>( ParamBinaryType::GroundGo );
        BinaryPtr->fieldUWeight = fieldRef.get_uWeight();
        BinaryPtr->job_fieldPtr = chunkDataRef.get_job_fieldPtr(fieldKey_);
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
            ParamBinary dyParam {};
            auto *BinaryPtr = dyParam.init_binary<DyParams_MapSurface>( ParamBinaryType::MapSurface );
            BinaryPtr->spec = MapSurfaceLowSpec::WhiteRock; //- tmp，其实要根据 eco 来分配 ...
            BinaryPtr->lvl = mapSurfaceLvl;
            BinaryPtr->randVal = fieldRef.get_uWeight();
            //--- 
            gameObjs::create_a_Go(  ssrc::get_goSpecId( "mapSurfaceLower" ),
                                    fieldRef.get_dpos() + dposOff,
                                    dyParam );
        }
    }

    //----- fieldRim go ------//
    bool isFieldRimGoCreate {true};
    if( isFieldRimGoCreate ){
        gameObjs::create_a_Go(  ssrc::get_goSpecId( "fieldRim" ),
                                fieldRef.get_dpos(),
                                emptyParamBinary );
    }
    


    //----- land go -----//
    if( fieldRef.is_land() ){
        const auto &goSpecData = esrc::atom_ecoObj_apply_a_rand_goSpecData( ecoObjKey,
                                                            fieldRef.get_density().get_idx(),
                                                            fieldRef.get_uWeight() );
        const auto &animLabels = goSpecData.get_animLabels();

        if( fract <= esrc::atom_ecoObj_get_applyPercent( ecoObjKey, fieldRef.get_density()) ){

            //--- dyParam ---//
            ParamBinary dyParam {};
            auto *BinaryPtr = dyParam.init_binary<DyParams_Field>( ParamBinaryType::Field );
            BinaryPtr->fieldUWeight = fieldRef.get_uWeight();
            BinaryPtr->fieldNodeMapEntAlti = fieldRef.get_nodeMapAlti(); //- tmp 有问题
            BinaryPtr->fieldDensity = fieldRef.get_density();
            BinaryPtr->animLabels.insert( BinaryPtr->animLabels.end(), animLabels.cbegin(), animLabels.cend() );//- maybe empty

            gameObjs::create_a_Go(  goSpecData.get_goSpecId(),
                                    fieldRef.get_nodeDPos(),
                                    dyParam );
        }
    }
}







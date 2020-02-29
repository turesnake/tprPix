/*
 * ====================== Chunk.cpp =======================
 *                          -- tpr --
 *                                        CREATE -- 2018.12.09
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#include "Chunk.h"

//-------------------- CPP --------------------//
#include <cmath>
#include <map>

//-------------------- Engine --------------------//
#include "tprCast.h"
#include "ViewingBox.h"
#include "EcoObj.h"
#include "random.h"
#include "occupyWeight.h"
#include "Quad.h"
#include "MapField.h"
#include "Job_Chunk.h"
#include "simplexNoise.h"
#include "create_goes.h"

#include "esrc_shader.h"
#include "esrc_ecoObj.h"
#include "esrc_camera.h"
#include "esrc_field.h"
#include "esrc_gameSeed.h"
#include "esrc_job_chunk.h"


void Chunk::init(){

    this->uWeight = calc_simple_uWeight( this->mpos );

    //  mapents
    auto &job_chunkRef = esrc::atom_getnc_job_chunkRef( this->chunkKey );
    this->memMapEnts.reserve( cast_2_size_t( ENTS_PER_CHUNK * ENTS_PER_CHUNK ) ); // reserve FIRST
    for( int h=0; h<ENTS_PER_CHUNK; h++ ){
        for( int w=0; w<ENTS_PER_CHUNK; w++ ){//- each mapent
            const auto &jobMapEnt = job_chunkRef.getnc_mapEntInnRef( IntVec2{w,h} );
            this->memMapEnts.push_back( std::make_unique<MemMapEnt>( 
                                                        this->mpos + IntVec2{w,h},
                                                        jobMapEnt.get_chunkKey(),
                                                        jobMapEnt.get_ecoObjKey(),
                                                        jobMapEnt.get_colorTableId(),
                                                        jobMapEnt.get_density(),
                                                        jobMapEnt.get_alti(),
                                                        jobMapEnt.get_bioSoupState(),
                                                        jobMapEnt.get_uWeight(),
                                                        jobMapEnt.get_isEcoBorder()
                                                    ) );
        }
    }
}



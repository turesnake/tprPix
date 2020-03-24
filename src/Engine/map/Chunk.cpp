/*
 * ====================== Chunk.cpp =======================
 *                          -- tpr --
 *                                        CREATE -- 2018.12.09
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#include "pch.h"
#include "Chunk.h"

//-------------------- Engine --------------------//
#include "ViewingBox.h"
#include "EcoObj.h"
#include "random.h"
#include "occupyWeight.h"
#include "Quad.h"
#include "MapField.h"
#include "Job_Chunk.h"
#include "simplexNoise.h"
#include "create_goes.h"

#include "esrc_job_chunk.h"


void Chunk::init(){

    this->uWeight = calc_simple_mapent_uWeight( this->mpos );

    //  mapents
    auto job_chunk_opt = esrc::atom_getnc_job_chunk_ptr( this->chunkKey );
    tprAssert( job_chunk_opt.has_value() );
    auto &job_chunkRef = *(job_chunk_opt.value());

    this->memMapEnts.reserve( ENTS_PER_CHUNK<size_t> * ENTS_PER_CHUNK<size_t> ); // reserve FIRST
    for( int h=0; h<ENTS_PER_CHUNK<>; h++ ){
        for( int w=0; w<ENTS_PER_CHUNK<>; w++ ){//- each mapent
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



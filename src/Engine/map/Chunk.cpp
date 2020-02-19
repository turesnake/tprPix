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


#include "tprDebug.h"



//namespace chunk_inn {//-------- namespace: chunk_inn --------------//
//}//------------- namespace: chunk_inn end --------------//


/* ===========================================================
 *                        init
 * -----------------------------------------------------------
 */
void Chunk::init(){

    this->init_memMapEnts();    

    //------------------------------//
    //      random vals
    //------------------------------//
    this->CDPos = this->mcpos.get_dpos() * 0.037 + esrc::get_gameSeed().get_chunk_dposOff();
    
    double freq = 1.0 / 7.0; //- tmp 7*7 个 field 组成一个 pn晶格
    this->originPerlin = simplex_noise2( this->CDPos * freq );  //- [-1.0, 1.0]
    this->uWeight = blender_the_perlinNoise( this->originPerlin, 791613.7, 10000 ); //[0, 9999]

    //------------------------------//
    //       read job_chunk
    //------------------------------//
    auto &job_chunkRef = esrc::atom_getnc_job_chunkRef( this->chunkKey );
    
    //----- mapents -------//
    size_t entIdx {};
    for( int h=0; h<ENTS_PER_CHUNK; h++ ){
        for( int w=0; w<ENTS_PER_CHUNK; w++ ){//- each mapent

            entIdx = cast_2_size_t( h * ENTS_PER_CHUNK + w );
            auto &mapEnt = this->getnc_mapEntRef( entIdx );
            const auto &mapEntInn = job_chunkRef.getnc_mapEntInnRef( IntVec2{w,h} );
            mapEntInn.write_2_mapEnt( mapEnt );

        }
    }

    //----- fieldKeys -------//
    IntVec2     fieldMPos   {};
    fieldKey_t  fieldKey    {};
    for( int h=0; h<FIELDS_PER_CHUNK; h++ ){
        for( int w=0; w<FIELDS_PER_CHUNK; w++ ){ //- each field in chunk
            fieldMPos = this->get_mpos() + IntVec2{ w*ENTS_PER_FIELD, h*ENTS_PER_FIELD };
            fieldKey = fieldMPos_2_fieldKey( fieldMPos );
            //---
            this->fieldKeys.push_back(fieldKey);
        }
    } //- each field in chunk
}


/* ===========================================================
 *                     init_memMapEnts
 * -----------------------------------------------------------
 * -- 向 memMapEnts 填入每个mapent，并设置它们的 mcpos
 * --- 除此之外，这些 mapent 数据都是空的
 */
void Chunk::init_memMapEnts(){
    this->memMapEnts.reserve( cast_2_size_t( ENTS_PER_CHUNK * ENTS_PER_CHUNK ) ); // reserve FIRST
    for( int h=0; h<ENTS_PER_CHUNK; h++ ){
        for( int w=0; w<ENTS_PER_CHUNK; w++ ){
            auto mapEntUPtr = std::make_unique<MemMapEnt>();
            mapEntUPtr->set_mcpos( mcpos + MapCoord{w, h} );
            this->memMapEnts.push_back( std::move(mapEntUPtr) ); //-move
        }
    }
}


/* ===========================================================
 *               get_mapEntIdx_in_chunk
 * -----------------------------------------------------------
 * -- 传入任意 mpos，获得其在 本chunk 中的 idx（访问 vector 用）
 */
size_t Chunk::get_mapEntIdx_in_chunk( IntVec2 anyMPos_ ){
    IntVec2 mposOff = anyMPos_ - this->mcpos.get_mpos();
    int w = mposOff.x;
    int h = mposOff.y;
        tprAssert( (w>=0) && (w<ENTS_PER_CHUNK) &&
                (h>=0) && (h<ENTS_PER_CHUNK) ); //- tmp
    return cast_2_size_t(h*ENTS_PER_CHUNK + w);
}


/* ===========================================================
 *               get_pixIdx_in_chunk
 * -----------------------------------------------------------
 * -- 传入任意 ppos 绝对值，获得 此pix 在 本chunk 中的 idx（访问 mapTex 用）
 */
size_t Chunk::get_pixIdx_in_chunk( IntVec2 anyPPos_ ){
    IntVec2 pposOff = anyPPos_ - this->mcpos.get_ppos();
    int w = pposOff.x;
    int h = pposOff.y;
        tprAssert( (w>=0) && (w<PIXES_PER_CHUNK) &&
                (h>=0) && (h<PIXES_PER_CHUNK) ); //- tmp
    return cast_2_size_t( h*PIXES_PER_CHUNK + w );
}


/*
 * ====================== Chunk.cpp =======================
 *                          -- tpr --
 *                                        CREATE -- 2018.12.09
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#include "Chunk.h"

//-------------------- C --------------------//
#include <cmath>

//-------------------- CPP --------------------//
#include <map>

//-------------------- Engine --------------------//
#include "tprCast.h"
#include "ViewingBox.h"
#include "EcoObj.h"
#include "random.h"
#include "occupyWeight.h"
#include "Quad.h"
#include "MapField.h"
#include "ChunkData.h"
#include "simplexNoise.h"
#include "create_goes.h"

#include "esrc_shader.h"
#include "esrc_ecoObj.h"
#include "esrc_camera.h"
#include "esrc_field.h"
#include "esrc_gameSeed.h"
#include "esrc_chunkData.h"
#include "esrc_mapSurfaceRand.h"


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

    this->mapSurfaceRandEntId = esrc::apply_a_mapSurfaceRandEntId( (this->originPerlin + 13.7) * 333.1 );

    //------------------------------//
    //       read chunkData
    //------------------------------//
    auto &chunkDataRef = esrc::atom_getnc_chunkDataCRef( this->chunkKey );
    
    //----- mapents -------//
    size_t entIdx {};
    for( size_t h=0; h<ENTS_PER_CHUNK; h++ ){
        for( size_t w=0; w<ENTS_PER_CHUNK; w++ ){//- each mapent

            entIdx = h * ENTS_PER_CHUNK + w;
            auto &mapEnt = this->getnc_mapEntRef( entIdx );
            const auto &mapEntInn = chunkDataRef.getnc_mapEntInnRef( IntVec2{w,h} );
            mapEntInn.write_2_mapEnt( mapEnt );

        }
    }

    //----- fields -------//
    IntVec2     fieldNodeOff {};
    fieldKey_t  fieldKey {};
    this->fieldKeys.clear();
    this->fieldKeys.reserve( FIELDS_PER_CHUNK * FIELDS_PER_CHUNK );

    for( const auto &pair : chunkDataRef.get_fieldDatas() ){//- each fieldData

        fieldKey = pair.first;
        this->fieldKeys.push_back(fieldKey);
        //--
        const auto &fieldRef = esrc::atom_get_field(fieldKey);
        fieldNodeOff = fieldRef.get_nodeMPos() - anyMPos_2_chunkMPos(fieldRef.get_mpos());
        const auto &mapEntInnRef = chunkDataRef.getnc_mapEntInnRef( fieldNodeOff );

        esrc::atom_write_2_field_from_jobData(  fieldKey,
                                                mapEntInnRef.ecoObjKey,
                                                mapEntInnRef.colorRableId,
                                                mapEntInnRef.density,
                                                mapEntInnRef.alti,
                                                pair.second->minFieldAlti,
                                                pair.second->maxFieldAlti,
                                                pair.second->is_crossEcoObj(),
                                                pair.second->is_crossColorTable() );
    }
}


/* ===========================================================
 *                     init_memMapEnts
 * -----------------------------------------------------------
 * -- 向 memMapEnts 填入每个mapent，并设置它们的 mcpos
 * --- 除此之外，这些 mapent 数据都是空的
 */
void Chunk::init_memMapEnts(){
    if( this->is_memMapEnts_set ){
        return;
    }
    this->memMapEnts.reserve( ENTS_PER_CHUNK * ENTS_PER_CHUNK ); // reserve FIRST
    for( int h=0; h<ENTS_PER_CHUNK; h++ ){
        for( int w=0; w<ENTS_PER_CHUNK; w++ ){
            auto mapEntUPtr = std::make_unique<MemMapEnt>();
            mapEntUPtr->set_mcpos( mcpos + MapCoord{w, h} );
            this->memMapEnts.push_back( std::move(mapEntUPtr) ); //-copy
        }
    }
    this->is_memMapEnts_set = true;
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


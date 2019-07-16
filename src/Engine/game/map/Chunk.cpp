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
#include "MapAltitude.h"
#include "Quad.h"
#include "FieldBorderSet.h"
#include "MapField.h"
#include "ChunkData.h"

#include "esrc_shader.h"
#include "esrc_ecoObj.h"
#include "esrc_camera.h"
#include "esrc_field.h"
#include "esrc_gameSeed.h"
#include "esrc_chunkData.h"


#include "tprDebug.h"

//-------------------- Script --------------------//
#include "Script/gameObjs/create_goes.h"


namespace chunk_inn {//-------- namespace: chunk_inn --------------//
    
    //--- 定值: chunk-mesh scale --
    const glm::vec3  mesh_scaleVal {PIXES_PER_CHUNK_IN_TEXTURE,
                                    PIXES_PER_CHUNK_IN_TEXTURE,
                                    1.0f };

    //-- 根据 奇偶性，来分配每个 chunk 的 zOff值 --
    const std::vector<double> zOffs{
        0.1, 0.2, 0.3, 0.4
    };

}//------------- namespace: chunk_inn end --------------//


/* ===========================================================
 *                        init
 * -----------------------------------------------------------
 */
void Chunk::init(){

    //--- mesh.shaderProgram ---
    this->mesh.set_shader_program( &(esrc::get_rect_shader()) ); //- default
    
    //--- mesh.scale ---
    mesh.set_scale(chunk_inn::mesh_scaleVal);

    //---
    this->init_memMapEnts();

    // 根据 本chunk 在 2*2 chunk 中的位置
    // 来分配 zoff 值。 
    //-- 本 chunk 在 世界坐标中的 奇偶性 --
    // 得到的值将会是 {0,0}; {1,0}; {0,1}; {1,1} 中的一种
    IntVec2 v = floorDiv( this->get_mpos(), static_cast<double>(ENTS_PER_CHUNK) );
    IntVec2 oddEven = floorMod( v, 2.0 );
    this->zOff = chunk_inn::zOffs.at( cast_2_size_t(oddEven.y * 2 + oddEven.x) );
    
    //------------------------------//
    //  从 chunkData 中 copy: 
    //  mapEntAltis / fieldKeys
    //------------------------------//
    auto &chunkDataRef = esrc::atom_getnc_chunkDataCRef( this->chunkKey );
    {
        const auto &mapEntAltis = chunkDataRef.get_mapEntAltis();
            tprAssert( mapEntAltis.size() == this->memMapEnts.size() ); //- tmp
        size_t entIdx {};
        for( size_t h=0; h<ENTS_PER_CHUNK; h++ ){
            for( size_t w=0; w<ENTS_PER_CHUNK; w++ ){//- each mapent
                entIdx = h * ENTS_PER_CHUNK + w;
                this->memMapEnts.at(entIdx)->mapAlti = mapEntAltis.at(entIdx);
            }
        }
    }

    //------------------------//
    //      fieldKeys
    //  设置 field.nodeMapAlti
    //------------------------//
    IntVec2       tmpFieldMpos {};
    fieldKey_t    tmpFieldKey  {};
    this->fieldKeys.clear();
    this->fieldKeys.reserve( FIELDS_PER_CHUNK * FIELDS_PER_CHUNK );
    for( int h=0; h<FIELDS_PER_CHUNK; h++ ){
        for( int w=0; w<FIELDS_PER_CHUNK; w++ ){ //- each field in 8*8
            tmpFieldMpos = this->get_mpos() + IntVec2{  w*ENTS_PER_FIELD,
                                                        h*ENTS_PER_FIELD };
            tmpFieldKey = fieldMPos_2_fieldKey(tmpFieldMpos);
            this->fieldKeys.push_back( tmpFieldKey );
            //----
            esrc::atom_field_set_nodeAlti_2( tmpFieldKey, this->memMapEnts );
                            //- 在这里，直接传递 容器引用，很粗暴。
                            //  可以做的更精细一些
        }
    }

    //------------------------------//
    //        mapTex, mesh
    //------------------------------//
    this->mapTex.swap_texBuf_from( chunkDataRef.getnc_texBuf() );
    this->mapTex.creat_texName();
    this->mesh.init( mapTex.get_texName() ); //- auto
    this->mesh.isVisible = true;  //- 一定可见

    //- mapTex 直接坐标于 camera 的 远平面上
    //  此值 需要跟随 camera 每一帧都调整。主要是 camera.get_zFar() 这个值
    this->refresh_translate_auto();
}



/* ===========================================================
 *                  refresh_translate_auto
 * -----------------------------------------------------------
 */
void Chunk::refresh_translate_auto(){
    const IntVec2 &ppos = mcpos.get_ppos();
    mesh.set_translate(glm::vec3{   static_cast<float>(ppos.x),
                                    static_cast<float>(ppos.y),
                                    static_cast<float>(esrc::get_camera().get_zFar() + ViewingBox::chunks_zOff +
                                        this->zOff)  //-- MUST --
                                    });
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
            mapEntUPtr->mcpos = mcpos + MapCoord{ w, h };
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
size_t Chunk::get_mapEntIdx_in_chunk( const IntVec2 &anyMPos_ ){
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
size_t Chunk::get_pixIdx_in_chunk( const IntVec2 &anyPPos_ ){
    IntVec2 pposOff = anyPPos_ - this->mcpos.get_ppos();
    int w = pposOff.x;
    int h = pposOff.y;
        tprAssert( (w>=0) && (w<PIXES_PER_CHUNK) &&
                (h>=0) && (h<PIXES_PER_CHUNK) ); //- tmp
    return cast_2_size_t( h*PIXES_PER_CHUNK + w );
}


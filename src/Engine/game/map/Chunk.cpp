/*
 * ====================== Chunk.cpp =======================
 *                          -- tpr --
 *                                        CREATE -- 2018.12.09
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *    map chunk 一个地图区域。 左下坐标系
 *    ----------
 *    
 * ----------------------------
 */
#include "Chunk.h"

//-------------------- C --------------------//
#include <cassert>

//-------------------- Engine --------------------//
#include "ViewingBox.h"
#include "srcs_engine.h"
#include "MapEnt.h"
#include "ChunkFieldSet.h"


/* ===========================================================
 *                        init
 * -----------------------------------------------------------
 */
void Chunk::init(){

    //--- mesh.scale ---
    mesh.set_scale(glm::vec3{   (float)(ENTS_PER_CHUNK * PIXES_PER_MAPENT),
                                (float)(ENTS_PER_CHUNK * PIXES_PER_MAPENT),
                                1.0f });
}


/* ===========================================================
 *                  refresh_translate_auto
 * -----------------------------------------------------------
 */
void Chunk::refresh_translate_auto(){
    const IntVec2 &ppos = mcpos.get_ppos();
    mesh.set_translate(glm::vec3{   (float)ppos.x,
                                    (float)ppos.y,
                                    esrc::camera.get_zFar() + ViewingBox::chunks_zOff //-- MUST --
                                    });
}


/* ===========================================================
 *                     init_memMapEnts
 * -----------------------------------------------------------
 * -- 向 memMapEnts 填入每个mapent，并设置它们的 mcpos
 * --- 除此之外，这些 mapent 数据都是空的
 */
void Chunk::init_memMapEnts(){
    for( int h=0; h<ENTS_PER_CHUNK; h++ ){
        for( int w=0; w<ENTS_PER_CHUNK; w++ ){
            MemMapEnt mapEnt {};
            mapEnt.mcpos = mcpos + MapCoord{ w, h };
            memMapEnts.push_back( mapEnt ); //-copy
        }
    }
}



/* ===========================================================
 *               assign_mapEnts_2_field
 * -----------------------------------------------------------
 * -- 将本 chunk 的每个 mapent，根据 cell-noise, 分配给最近的 field
 */
void Chunk::assign_mapEnts_2_field(){

    ChunkFieldSet  *chunkFieldSetPtr = esrc::get_chunkFieldSetPtr( this->chunkKey );
    IntVec2         tmpEntMPos;
    IntVec2         tmpEntMidPPos; //- mapent 中间pix的 ppos
    size_t          tmpEntIdx;
    MemMapEnt       *tmpMapEntPtr;

    int         off; //- 两个 ppos 间距离（未开根号）

    fieldKey_t  minFieldKey;
    int         minOff = 0;

    for( const auto &fieldPair : chunkFieldSetPtr->fields ){ //- each field (16*16)
        const MapField &fieldRef = fieldPair.second;
        assert( fieldRef.is_firstOrderData_set );
        assert( fieldRef.is_secondOrderData_set );

        for( int h=0; h<ENTS_PER_FIELD; h++ ){
            for( int w=0; w<ENTS_PER_FIELD; w++ ){ //- each mapent in field (4*4)

                tmpEntMPos = fieldRef.mcpos.get_mpos() + IntVec2{ w, h };
                tmpEntIdx = get_mapEntIdx_in_chunk( tmpEntMPos );
                tmpEntMidPPos = mpos_2_midPPos( tmpEntMPos );
                tmpMapEntPtr = esrc::get_memMapEntPtr( tmpEntMPos );

                minOff = 0;

        
                for( auto &nodePPosPair : fieldRef.nearby_field_nodePPoses ){ //- each nearby field nodeppos

                    off = calc_fast_ppos_distance( nodePPosPair.second, tmpEntMidPPos );

                    if( minOff == 0 ){
                        minOff = off;
                        minFieldKey = nodePPosPair.first;
                    }else{
                        if( off < minOff ){
                            minOff = off;
                            minFieldKey = nodePPosPair.first;
                        }
                    }
                }//-- for each nearby field nodeppos end ---
                tmpMapEntPtr->fieldKey = minFieldKey;
                

            }
        }//- for each mapent in field (4*4) end ---


    }//- for each field (16*16) end --

}


/* ===========================================================
 *               assign_pixels_2_mapent
 * -----------------------------------------------------------
 * -- 最简模式，每个 mapent 根据所属field.color, 来涂颜色
 */
void Chunk::assign_pixels_2_mapent(){

    RGBA    *pixBufHeadPtr;
    RGBA    *tmpPixPtr;

    IntVec2  tmpPixPPos;
    size_t   tmpPixIdx;


    //---------
    this->mapTex.resize_texBuf();
    pixBufHeadPtr = this->mapTex.get_texBufHeadPtr();
    
    for( const auto &mapEntRef : this->memMapEnts ){ //- each mapent
        const RGBA  &color = get_fieldPtr_by_key(mapEntRef.fieldKey)->color; //- 目标颜色

        for( int h=0; h<PIXES_PER_MAPENT; h++ ){
            for( int w=0; w<PIXES_PER_MAPENT; w++ ){ //- each pix in mapent

                tmpPixPPos = mapEntRef.mcpos.get_ppos() + IntVec2{ w, h };
                tmpPixIdx = this->get_pixIdx_in_chunk( tmpPixPPos );
                tmpPixPtr = pixBufHeadPtr + tmpPixIdx;

                tmpPixPtr->set( color.r,
                                color.g,
                                color.b,
                                color.a );
            }
        } //- for each pix in mapent end ---
    } //- for each mapent end --


    //-- 测试，将 chunk nodeppos 画到图上。
    /*
    tmpPixIdx = this->get_pixIdx_in_chunk( nodePPos );
    tmpPixPtr = pixBufHeadPtr + tmpPixIdx;
    tmpPixPtr->set( 0,0,0,255 );
    */




    //---------------------------//
    //   正式用 texture 生成 name
    //---------------------------//
    this->mapTex.creat_texName();

}



/* ===========================================================
 *               get_mapEntIdx_in_chunk
 * -----------------------------------------------------------
 * -- 传入任意 mpos，获得其在 本chunk 中的 idx（访问 vector 用）
 */
size_t Chunk::get_mapEntIdx_in_chunk( const IntVec2 &_anyMPos ){
    IntVec2 mposOff = _anyMPos - this->mcpos.get_mpos();
    int w = mposOff.x;
    int h = mposOff.y;
        assert( (w>=0) && (w<ENTS_PER_CHUNK) &&
                (h>=0) && (h<ENTS_PER_CHUNK) ); //- tmp
    return (size_t)(h*ENTS_PER_CHUNK + w);
}


/* ===========================================================
 *               get_pixIdx_in_chunk
 * -----------------------------------------------------------
 * -- 传入任意 ppos 绝对值，获得 此pix 在 本chunk 中的 idx（访问 mapTex 用）
 */
size_t Chunk::get_pixIdx_in_chunk( const IntVec2 &_anyPPos ){
    IntVec2 pposOff = _anyPPos - this->mcpos.get_ppos();
    int w = pposOff.x;
    int h = pposOff.y;
        assert( (w>=0) && (w<PIXES_PER_CHUNK) &&
                (h>=0) && (h<PIXES_PER_CHUNK) ); //- tmp
    return (size_t)( h*PIXES_PER_CHUNK + w );
}





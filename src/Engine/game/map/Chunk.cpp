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

    IntVec2     tmpPPosOff;
    int         off; //- 距离（未开根号）

    fieldKey_t  minFieldKey;
    int         minOff = 0;

    for( const auto &fieldPair : chunkFieldSetPtr->fields ){ //- each field (16*16)
            const MapField &fieldRef = fieldPair.second;
            assert( fieldRef.is_firstOrderData_init );
            assert( fieldRef.is_secondOrderData_init );

        for( int h=0; h<ENTS_PER_FIELD; h++ ){
            for( int w=0; w<ENTS_PER_FIELD; w++ ){ //- each mapent in field (4*4)

                tmpEntMPos = fieldRef.mcpos.get_mpos() + IntVec2{ w, h };
                tmpEntIdx = get_mapEntIdx_in_chunk( tmpEntMPos );
                tmpEntMidPPos = mpos_2_midPPos( tmpEntMPos );

                minOff = 0;

        
                for( auto &nodePPosPair : fieldRef.nearby_field_nodePPoses ){ //- each nearby field nodeppos

                    tmpPPosOff = nodePPosPair.second - tmpEntMidPPos;
                    off = tmpPPosOff.x*tmpPPosOff.x + tmpPPosOff.y*tmpPPosOff.y;

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
                

            }
        }



    }



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
        assert( (w>=0) && (w<ENTS_PER_FIELD) &&
                (h>=0) && (h<ENTS_PER_FIELD) ); //- tmp
    return (size_t)(h*ENTS_PER_FIELD + w);
}







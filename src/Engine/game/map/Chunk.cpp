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
#include <math.h>

//-------------------- CPP --------------------//
#include <map>

//-------------------- Engine --------------------//
#include "ViewingBox.h"
#include "srcs_engine.h"
#include "MapEnt.h"
#include "ChunkFieldSet.h"

//#include "fieldBorderEntPixMasks.h" //- tmp


namespace{//-------- namespace: --------------//

    std::map<int,fieldKey_t> mapEnt_2_field_distances {}; //- < distance, fieldKey >
                            //- 记录每个 mapent，距离周边9个field 的距离值. 被反复使用
                                               


}//------------- namespace: end --------------//


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
    if( this->is_memMapEnts_set ){
        return;
    }
    for( int h=0; h<ENTS_PER_CHUNK; h++ ){
        for( int w=0; w<ENTS_PER_CHUNK; w++ ){
            MemMapEnt mapEnt {};
            mapEnt.mcpos = mcpos + MapCoord{ w, h };
            this->memMapEnts.push_back( mapEnt ); //-copy
        }
    }
    this->is_memMapEnts_set = true;
}



/* ===========================================================
 *               assign_mapEnts_2_field
 * -----------------------------------------------------------
 * -- 将本 chunk 的每个 mapent，根据 cell-noise, 分配给最近的 field
 */
void Chunk::assign_mapEnts_2_field(){
    if( this->is_assign_mapEnts_2_field_done ){
        return;
    }

    ChunkFieldSet  *chunkFieldSetPtr = esrc::get_chunkFieldSetPtr( this->chunkKey );
    IntVec2         tmpEntMPos;
    //size_t          tmpEntIdx;
    MemMapEnt       *tmpMapEntPtr;

    int         off; //- 两个 mpos 间距离（未开根号）

    int        fst_near_distance;
    int        sec_near_distance;
    fieldKey_t fst_near_fieldKey;
    fieldKey_t sec_near_fieldKey;

    int    relatedDistance; //- 最近距离 和 次近距离 之差（abs）


    for( const auto &fieldPair : chunkFieldSetPtr->fields ){ //- each field (16*16)
        const MapField &fieldRef = fieldPair.second;
        assert( fieldRef.is_firstOrderData_set );
        assert( fieldRef.is_secondOrderData_set );

        for( int h=0; h<ENTS_PER_FIELD; h++ ){
            for( int w=0; w<ENTS_PER_FIELD; w++ ){ //- each mapent in field (4*4)

                tmpEntMPos = fieldRef.mcpos.get_mpos() + IntVec2{ w, h };
                //tmpEntIdx = get_mapEntIdx_in_chunk( tmpEntMPos );
                tmpMapEntPtr = esrc::get_memMapEntPtr( tmpEntMPos );



                mapEnt_2_field_distances.clear();
        
                for( auto &nodeMPosPair : fieldRef.nearby_field_nodeMPoses ){ //- each nearby field nodeMPos

                    off = calc_fast_mpos_distance( nodeMPosPair.second, tmpEntMPos );
                    mapEnt_2_field_distances.insert({ off, nodeMPosPair.first }); 


                }//-- for each nearby field nodeMPos end ---

                auto it = mapEnt_2_field_distances.begin();
                fst_near_distance = (*it).first;
                fst_near_fieldKey = (*it).second;
                it++;
                sec_near_distance = (*it).first;
                sec_near_fieldKey = (*it).second;


                //-- 在后续版本中，会将边缘ent 分成几个等级 --
                // 不同等级的，使用对应的 mask
                // 越靠近 field 内部的，点越少，而且 点的颜色也会更接近 本field 颜色


                relatedDistance = abs(fst_near_distance-sec_near_distance);
                if( relatedDistance < 6 ){
                    tmpMapEntPtr->fieldBorderType = FieldBorderType::Nearest;
                }else if( relatedDistance < 10 ){
                    tmpMapEntPtr->fieldBorderType = FieldBorderType::Sec;
                }else if( relatedDistance < 26 ){
                    tmpMapEntPtr->fieldBorderType = FieldBorderType::Thd;
                }else{
                    tmpMapEntPtr->fieldBorderType = FieldBorderType::Inner;
                }

                tmpMapEntPtr->fieldKey = fst_near_fieldKey;
                tmpMapEntPtr->nearbyFieldKey = sec_near_fieldKey;
            }
        }//- for each mapent in field (4*4) end ---


    }//- for each field (16*16) end --

    this->is_assign_mapEnts_2_field_done = true;
}


/* ===========================================================
 *               assign_pixels_2_mapent
 * -----------------------------------------------------------
 * -- 最简模式，每个 mapent 根据所属field.color, 来涂颜色
 */
void Chunk::assign_pixels_2_mapent(){
    if( this->is_assign_pixels_2_mapent_done ){
        return;
    }

    RGBA     color;
    RGBA     nearColor;

    RGBA    *pixBufHeadPtr;
    RGBA    *tmpPixPtr;

    IntVec2  tmpPixPPos;
    size_t   tmpPixIdx;

    size_t   maskIdx; 


    //------ 简易方案 -------//
    this->mapTex.resize_texBuf();
    pixBufHeadPtr = this->mapTex.get_texBufHeadPtr();
    
    for( const auto &mapEntRef : this->memMapEnts ){ //- each mapent



        color = get_fieldPtr_by_key(mapEntRef.fieldKey)->color; //- 目标颜色
        nearColor = get_fieldPtr_by_key(mapEntRef.nearbyFieldKey)->color; //- 副色


        //--- 先整体涂一遍 主色 ---//
        for( int h=0; h<PIXES_PER_MAPENT; h++ ){
            for( int w=0; w<PIXES_PER_MAPENT; w++ ){ //- each pix in mapent

                tmpPixPPos = mapEntRef.mcpos.get_ppos() + IntVec2{ w, h };
                tmpPixIdx = this->get_pixIdx_in_chunk( tmpPixPPos );
                tmpPixPtr = pixBufHeadPtr + tmpPixIdx;
                *tmpPixPtr = color;
            }
        } //- for each pix in mapent end ---


        //--- 再涂 副色 ---//

        for( const auto &pixWH : esrc::fieldBorderEntPixMaskSet.get_rand_maskSet( mapEntRef.fieldBorderType ) ){

            tmpPixPPos = mapEntRef.mcpos.get_ppos() + IntVec2{ pixWH.x, pixWH.y };
            tmpPixIdx = this->get_pixIdx_in_chunk( tmpPixPPos );
            tmpPixPtr = pixBufHeadPtr + tmpPixIdx;
            *tmpPixPtr = nearColor;
        }





        //--- 若为 border mapEnt -----//
        /*
        if( mapEntRef.is_fieldBorder ){
            color = get_fieldPtr_by_key(mapEntRef.fieldKey)->color; //- 目标颜色
            nearColor = get_fieldPtr_by_key(mapEntRef.nearbyFieldKey)->color; //- 副色
            maskIdx = apply_a_fieldBorderEntMask_idx();

            for( int h=0; h<PIXES_PER_MAPENT; h++ ){
                for( int w=0; w<PIXES_PER_MAPENT; w++ ){ //- each pix in mapent

                    tmpPixPPos = mapEntRef.mcpos.get_ppos() + IntVec2{ w, h };
                    tmpPixIdx = this->get_pixIdx_in_chunk( tmpPixPPos );
                    tmpPixPtr = pixBufHeadPtr + tmpPixIdx;

                    if( get_fieldBorderEntMask_val( maskIdx, w, h ) == true ){
                        *tmpPixPtr = nearColor;
                    }else{
                        *tmpPixPtr = color;
                    }
                    
                }
            } //- for each pix in mapent end ---


        //--- 若不是 border mapEnt -----//
        }else{
            color = get_fieldPtr_by_key(mapEntRef.fieldKey)->color; //- 目标颜色
            for( int h=0; h<PIXES_PER_MAPENT; h++ ){
                for( int w=0; w<PIXES_PER_MAPENT; w++ ){ //- each pix in mapent

                    tmpPixPPos = mapEntRef.mcpos.get_ppos() + IntVec2{ w, h };
                    tmpPixIdx = this->get_pixIdx_in_chunk( tmpPixPPos );
                    tmpPixPtr = pixBufHeadPtr + tmpPixIdx;
                    *tmpPixPtr = color;
                }
            } //- for each pix in mapent end ---
        }
        */



        
    } //- for each mapent end --



        //--- 测试，绘制 chunk nodeMPos ----
        /*
        tmpPixIdx = this->get_pixIdx_in_chunk( mpos_2_ppos(this->nodeMPos) );
        tmpPixPtr = pixBufHeadPtr + tmpPixIdx;
        tmpPixPtr->set( 0,0,0,255 );

        //--- 绘制 chunk 左下角 ---
        tmpPixIdx = this->get_pixIdx_in_chunk( this->mcpos.get_ppos() );
        tmpPixPtr = pixBufHeadPtr + tmpPixIdx;
        tmpPixPtr->set( 0,255,0,255 );

        //--- 绘制 section 左下角 ---
        IntVec2  thisChunkMPos = this->mcpos.get_mpos();
        IntVec2  sectionMPos = anyMPos_2_sectionMPos( thisChunkMPos );
        if( sectionMPos == thisChunkMPos ){
            tmpPixIdx = this->get_pixIdx_in_chunk( mpos_2_ppos(thisChunkMPos) );
            tmpPixPtr = pixBufHeadPtr + tmpPixIdx;
            tmpPixPtr->set( 255,0,0,255 );
        }
        */
    


    //---------------------------//
    //   正式用 texture 生成 name
    //---------------------------//
    this->mapTex.creat_texName();

    this->is_assign_pixels_2_mapent_done = true;
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





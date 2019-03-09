/*
 * =================== ChunkFieldSet.cpp =======================
 *                          -- tpr --
 *                                        CREATE -- 2019.02.27
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *   chunk "二级距离场"集 
 * ----------------------------
 */
#include "ChunkFieldSet.h"


//-------------------- C --------------------//
#include <cassert>
#include <math.h>

//-------------------- CPP --------------------//
#include <unordered_map>
#include <utility> //- pair

//-------------------- Engine --------------------//
#include "random.h"
#include "IntVec.h"
#include "sectionKey.h"
#include "srcs_engine.h"
#include "Section.h"
#include "chunkKey.h"

#include "debug.h"

namespace{//----------- namespace ---------------//

    std::default_random_engine  randEngine; //-通用 随机数引擎实例
    std::uniform_int_distribution<char> uDistribution_color(-3,3); // [-7,7] 


    using chunkData_t = std::pair<sectionKey_t, IntVec2>; //- <EcoSysInMapKey, chunkNodePPos>
    std::unordered_map<chunkKey_t, chunkData_t> nearby_chunk_datas {}; //- 周边 9个chunk 的 距离场点。
                                                    //- 被每个 chunk 反复使用


    //-- 临时颜色 --
    RGBA color_forst { 130, 150, 109, 255 };
    RGBA color_darkForst { 106, 130, 113, 255 };
    RGBA color_plain { 155, 159, 134, 255 };
    RGBA color_swamp { 142, 148, 149, 255 };
    RGBA color_desert { 176, 167, 140, 255 };

}//-------------- namespace : end ---------------//




/* ===========================================================
 *                  init_fields
 * -----------------------------------------------------------
 */
void ChunkFieldSet::init_fields(){
    if( this->is_fields_set ){
        return;
    }
    //-------
    this->fields.clear();
    IntVec2    fieldMPos {};
    for( int h=0; h<FIELDS_PER_CHUNK; h++ ){  //- each field
        for( int w=0; w<FIELDS_PER_CHUNK; w++ ){

            fieldMPos.set(  mcpos.get_mpos().x + w*ENTS_PER_FIELD,
                            mcpos.get_mpos().y + h*ENTS_PER_FIELD );

            MapField   field  {};
            field.init_firstOrderData( fieldMPos, this->mcpos.get_mpos() ); 
                                                    //- 仅初始化 field 一阶数据
            this->fields.insert({ anyMPos_2_fieldKey(fieldMPos), field }); //- copy
        }
    }
    //----
    this->is_fields_set = true; 
}


/* ===========================================================
 *               assign_fields_2_chunk
 * -----------------------------------------------------------
 * -- 通过 cell noise, 将 本chunk 内的所有 field，分配给不同的 chunk
 */
void ChunkFieldSet::assign_fields_2_chunk(){
    if( this->is_assign_fields_2_chunk_done ){
        return;
    }
    assert( this->is_fields_set );
    //-------------------------------//
    //    rand engine
    //-------------------------------//
    randEngine.seed( get_new_seed() ); //- tmp

    //-------------------------------//
    //    收集 周边 9个chunk nodeppos
    //-------------------------------//
    this->reset_nearby_chunk_datas();

    //-------------------------------//
    //    目前这部分实现 有点低效 ...
    //-------------------------------//
    int         off; //- field 与周边 各chunk.nodeppos 间距离（未开根号）
    //---
    sectionKey_t  minEcosysInMapKey;
    chunkKey_t    minChunkKey;
    int           minOff = 0; 

    RGBA          targetColor; //- tmp
    EcoSysInMap  *ecoSysInMapPtr; //- tmp

    IntVec2       tmpFieldMidMPos; //- field 中间位置的 mpos
    IntVec2       midMPosOff  { 2, 2 }; //- field中点 mpos 到 fieldMPos 位移

    for( auto &fieldPair : this->fields ){ //- each field
        MapField &fieldRef = fieldPair.second;
        tmpFieldMidMPos = fieldRef.mcpos.get_mpos() + midMPosOff;
            assert( fieldRef.is_firstOrderData_set );
            
        minOff = 0; //- reset
        for( const auto &chunkPair : nearby_chunk_datas ){ //- each chunk
            const chunkData_t &chunkDataRef = chunkPair.second;

            off = calc_fast_ppos_distance( mpos_2_ppos(tmpFieldMidMPos), chunkDataRef.second );

            if( minOff == 0 ){
                minOff = off;
                minChunkKey = chunkPair.first;
                minEcosysInMapKey = chunkDataRef.first;
            }else{
                if( off < minOff ){
                    minOff = off;
                    minChunkKey = chunkPair.first;
                    minEcosysInMapKey = chunkDataRef.first;
                }
            }
        }//----- for each chunk end -----

        fieldRef.chunkKey = minChunkKey;
        fieldRef.ecoSysInMapKey = minEcosysInMapKey;
        fieldRef.is_chunkKey_set = true;
        fieldRef.is_ecoSysInMapKey_set = true;


            //-------------- ... tmp ... ---------------//
  
            //--- 根据 ecoSysInMapKey，为每个 field 分配一个颜色 tmp... --
            ecoSysInMapPtr = esrc::get_ecoSysInMapPtr( fieldRef.ecoSysInMapKey );
            switch (ecoSysInMapPtr->ecoSysType){
                case EcoSysType::Forst:
                    targetColor = color_forst;
                    break;
                case EcoSysType::DarkForst:
                    targetColor = color_darkForst;
                    break;
                case EcoSysType::Plain:
                    targetColor = color_plain;
                    break;
                case EcoSysType::Swamp:
                    targetColor = color_swamp;
                    break;
                case EcoSysType::Desert:
                    targetColor = color_desert;
                    break;
                default:
                    assert(0);
                    break;
            }

            fieldRef.color.set( targetColor.r + uDistribution_color(randEngine),
                                targetColor.g + uDistribution_color(randEngine),
                                targetColor.b + uDistribution_color(randEngine),
                                targetColor.a );


            fieldRef.is_color_set = true;

    }//----- for each field -----
    this->is_assign_fields_2_chunk_done = true;
}




/* ===========================================================
 *                get_fieldPtr_by_mpos
 * -----------------------------------------------------------
 * 传入本chunk中的任意一个 mpos，获得其对应的 MapField 的 访问权 
 * param: _anyMPos -- 这个 chunk 中的任意 mapent.mpos 
 */
MapField *ChunkFieldSet::get_fieldPtr_by_mpos( const IntVec2 &_anyMPos ){
    fieldKey_t key = anyMPos_2_fieldKey( _anyMPos );
    return (MapField*)&(fields.at(key));
}
MapField *ChunkFieldSet::get_fieldPtr_by_key( fieldKey_t _key ){
        assert( this->fields.find(_key) != this->fields.end() );
    return (MapField*)&(fields.at(_key));
}


/* ===========================================================
 *                reset_nearby_chunk_datas
 * -----------------------------------------------------------
 * - 收集 周边 9个chunk nodeppos
 */
void ChunkFieldSet::reset_nearby_chunk_datas(){
    nearby_chunk_datas.clear();
    //--------
    IntVec2      currentChunkMPos = this->mcpos.get_mpos();
    IntVec2      tmpChunkMPos;
    size_t       tmpChunkIdx; //- chunk 在其所属section 中的 idx
    Section     *tmpSectionPtr;
    chunkData_t  tmpChunkData;

    for( int h=-1; h<=1; h++ ){
        for( int w=-1; w<=1; w++ ){ //- 周边 9 个 chunk
            tmpChunkMPos.set(   currentChunkMPos.x + w*ENTS_PER_CHUNK,
                                currentChunkMPos.y + h*ENTS_PER_CHUNK );
            tmpChunkIdx = get_chunkIdx_in_section( tmpChunkMPos );
            tmpSectionPtr = esrc::get_sectionPtr( anyMPos_2_sectionKey(tmpChunkMPos) );
            //------
            tmpChunkData.first = tmpSectionPtr->get_chunkEcoSysInMapKey( tmpChunkIdx );
            tmpChunkData.second = tmpSectionPtr->get_chunkNodePPos(tmpChunkIdx); //- copy
            nearby_chunk_datas.insert({ chunkMPos_2_key(tmpChunkMPos), tmpChunkData }); //- copy
        }
    }
}




/* ===========================================================
 *              get_fieldPtr_by_key
 * -----------------------------------------------------------
 * 全局函数版
 * -1- 找到 field 所在 chunkFieldSet
 * -2- 找到 field 实例
 */
MapField *get_fieldPtr_by_key( fieldKey_t _key ){

    chunkKey_t chunkKey = anyMPos_2_chunkKey( fieldKey_2_mpos(_key) );
    ChunkFieldSet *chunkFieldSetPtr = esrc::get_chunkFieldSetPtr( chunkKey );
    const auto &fieldsRef = chunkFieldSetPtr->fields;
        assert( fieldsRef.find(_key) != fieldsRef.end() ); //- tmp
    return (MapField*)&(fieldsRef.at(_key));
}


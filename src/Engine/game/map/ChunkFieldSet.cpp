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


namespace{//----------- namespace ---------------//


    //std::default_random_engine  randEngine; //-随机数引擎实例
    //std::uniform_int_distribution<int> uDistribution(0,
                                       //(ENTS_PER_CHUNK*PIXES_PER_MAPENT)-1); 
                                        // [0,319] 
    
    //std::unordered_map<chunkKey_t, IntVec2>  nearby_chunk_nodePPoses {};

    using chunkData_t = std::pair<sectionKey_t, IntVec2>; //- <EcoSysInMapKey, chunkNodePPos>
    std::unordered_map<chunkKey_t, chunkData_t> nearby_chunk_datas {}; //- 周边 9个chunk 的 距离场点。
                                                    //- 被每个 chunk 反复使用


    std::vector<ChunkFieldSet*> nearby_chunkFieldSetPtrs {}; //- 周边9个 ChunkFieldSet 实例指针
                                                    //- 被每个 chunk 反复使用

}//-------------- namespace : end ---------------//




/* ===========================================================
 *                  init_firstOrderData
 * -----------------------------------------------------------
 *  仅生成 一阶数据
 * param: _anyMPos -- 这个 chunk 中的任意 mapent.mpos 
 */
void ChunkFieldSet::init_firstOrderData( const IntVec2 &_anyMPos ){

    //-------------------------------//
    // 每初始化一个 chunk，就重分配一个 seed
    // 但这个 chunk内部。则使用这个 连续的 引擎
    //       (临时简易方案...)
    //-------------------------------// 
    //randEngine.seed(get_new_seed());

    //-------------------------------//
    //          chunkKey
    //-------------------------------//
    IntVec2 mpos = anyMPos_2_chunkMPos( _anyMPos );
    mcpos.set_by_mpos( mpos );
    //chunkKey.init_by_anyMPos( mcpos.get_mpos() ); 
    chunkKey = anyMPos_2_chunkKey( mcpos.get_mpos() );
                    //-- 这里的计算重复了，但问题不大。

    //-------------------------------//
    //  制作 16*16 个 距离场
    //  填入 fields 容器中
    //-------------------------------//
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
    this->is_firstOrderData_init = true; //- MUST
}


/* ===========================================================
 *               assign_fields_2_chunk
 * -----------------------------------------------------------
 * -- 通过 cell noise, 将 本chunk 内的所有 field，分配给不同的 chunk
 */
void ChunkFieldSet::assign_fields_2_chunk(){

    assert( this->is_firstOrderData_init );

    //-------------------------------//
    //    收集 周边 9个chunk nodeppos
    //-------------------------------//
    this->reset_nearby_chunk_datas();


    //-------------------------------//
    //    目前这部分实现 有点低效 ...
    //-------------------------------//
    IntVec2     tmpPPosOff;//- field 与周边 各chunk.nodeppos 的 距离向量（未开根号）
    int         off; //- 距离（未开根号）
    //---
    sectionKey_t  minEcosysInMapKey;
    chunkKey_t    minChunkKey;
    int           minOff = 0; 

    for( auto &fieldPair : this->fields ){ //- each field
            MapField &fieldRef = fieldPair.second;
            assert( fieldRef.is_firstOrderData_init );
            //assert( fieldRef.is_secondOrderData_init );
        minOff = 0; //- reset
        for( const auto &chunkPair : nearby_chunk_datas ){ //- each chunk

            const chunkData_t &chunkDataRef = chunkPair.second;

            tmpPPosOff = fieldRef.nodePPos - chunkDataRef.second;
            off = tmpPPosOff.x*tmpPPosOff.x + tmpPPosOff.y*tmpPPosOff.y;

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
    }//----- for each field -----
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
    IntVec2      nearbyChunkMPos;
    size_t       tmpChunkIdx; //- chunk 在其所属section 中的 idx
    Section     *tmpSectionPtr;
    chunkData_t  tmpChunkData;

    for( int h=-1; h<=1; h++ ){
        for( int w=-1; w<=1; w++ ){ //- 周边 9 个 chunk
            nearbyChunkMPos.set(currentChunkMPos.x + w*ENTS_PER_CHUNK,
                                currentChunkMPos.y + h*ENTS_PER_CHUNK );
            tmpChunkIdx = get_chunkIdx_in_section( nearbyChunkMPos );
            tmpSectionPtr = esrc::get_sectionPtr( anyMPos_2_sectionKey(nearbyChunkMPos) );
            //------
            tmpChunkData.first = tmpSectionPtr->get_chunkEcoSysInMapKey( tmpChunkIdx );
            tmpChunkData.second = tmpSectionPtr->get_chunkNodePPos(tmpChunkIdx);
            nearby_chunk_datas.insert({ chunkMPos_2_key(nearbyChunkMPos), tmpChunkData }); //- copy
        }
    }
}


/* ===========================================================
 *                get_nearby_chunkFieldSetPtrs
 * -----------------------------------------------------------
 * - 收集 周边 9个 chunkFieldSet 实例指针，并返回容器访问权
 */
std::vector<ChunkFieldSet*> &get_nearby_chunkFieldSetPtrs( const IntVec2 &_anyMPos ){

    nearby_chunkFieldSetPtrs.clear();
    //--------
    IntVec2        currentChunkMPos = anyMPos_2_chunkMPos( _anyMPos );
    IntVec2        nearbyChunkMPos;
    ChunkFieldSet *tmpChunkFieldSetPtr;

    for( int h=-1; h<=1; h++ ){
        for( int w=-1; w<=1; w++ ){ //- 周边 9 个 chunk
            nearbyChunkMPos.set(currentChunkMPos.x + w*ENTS_PER_CHUNK,
                                currentChunkMPos.y + h*ENTS_PER_CHUNK );
            tmpChunkFieldSetPtr = esrc::get_chunkFieldSetPtr( chunkMPos_2_key(nearbyChunkMPos) );
            nearby_chunkFieldSetPtrs.push_back( tmpChunkFieldSetPtr ); //- copy
        }
    }
    //------
    return nearby_chunkFieldSetPtrs;
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


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
#include "PerlinNoise3D.h"

#include "debug.h"

namespace{//----------- namespace ---------------//

    std::default_random_engine  randEngine; //-通用 随机数引擎实例
    std::uniform_int_distribution<char> uDistribution_color(-3,3); // [-7,7] 


    PerlinNoise3D pn_alti;      //- perlin，制作 alti数据
    u32_t         pn_alti_seed; //- 临时存在在此，每次游戏重启，都随机生成
    bool          is_pn_alti_init {false}; //- 临时flag



    using chunkData_t = std::pair<sectionKey_t, IntVec2>; //- <EcoSysInMapKey, chunkNodeMPos>
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
    //    收集 周边 9个chunk nodeMPos
    //-------------------------------//
    this->reset_nearby_chunk_datas();

    //-------------------------------//
    //    目前这部分实现 有点低效 ...
    //-------------------------------//
    int         off; //- field 与周边 各chunk.nodeMPos 间距离（未开根号）
    //---
    sectionKey_t  minEcosysInMapKey;
    chunkKey_t    minChunkKey;
    int           minOff = 0; 
    bool          is_first_set {true};

    RGBA          targetColor; //- tmp
    EcoSysInMap  *ecoSysInMapPtr; //- tmp

    IntVec2       tmpFieldMidMPos; //- field 中间位置的 mpos
    IntVec2       midMPosOff  { HALF_ENTS_PER_FIELD, 
                                HALF_ENTS_PER_FIELD }; //- field中点 mpos 到 fieldMPos 位移

    for( auto &fieldPair : this->fields ){ //- each field
        MapField &fieldRef = fieldPair.second;
        tmpFieldMidMPos = fieldRef.mcpos.get_mpos() + midMPosOff;
            assert( fieldRef.is_firstOrderData_set );
            
        //--- reset ---
        minOff       = 0; 
        is_first_set = true;

        for( const auto &chunkPair : nearby_chunk_datas ){ //- each chunk
            const chunkData_t &chunkDataRef = chunkPair.second;

            off = calc_fast_mpos_distance( tmpFieldMidMPos, chunkDataRef.second );

            if( is_first_set ){
                is_first_set = false;
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

            fieldRef.color.set( targetColor.r + (u8_t)(uDistribution_color(randEngine) * 2.2),
                                targetColor.g + (u8_t)(uDistribution_color(randEngine) * 1.8),
                                targetColor.b + (u8_t)(uDistribution_color(randEngine) * 0.6),
                                targetColor.a );

            fieldRef.is_color_set = true;

    }//----- for each field -----
    this->is_assign_fields_2_chunk_done = true;
}


/* ===========================================================
 *                assign_alti_in_field
 * -----------------------------------------------------------
 * 根据 perlin-noise，为每个 field 分配一个 alti
 */
void ChunkFieldSet::assign_alti_in_field(){
    if( this->is_assign_alti_in_field_done ){
        return;
    }

    //-------------------------------//
    //    perlin-noise alti
    //-------------------------------//
    if( is_pn_alti_init == false ){
        is_pn_alti_init = true;
        pn_alti_seed = get_new_seed();
        pn_alti.init( pn_alti_seed );  //- 只用初始化一次
    }
    

    float   val;
    int     ival;

    float   w;
    float   h;

    float rad = 1.5; //- perlin-noise 图比例

    for( auto &fieldPair : this->fields ){ //- each field
        MapField &fieldRef = fieldPair.second;

        w = (float)fieldRef.nodeMPos.x / (float)ENTS_PER_CHUNK;
        h = (float)fieldRef.nodeMPos.y / (float)ENTS_PER_CHUNK;

        //-- [0.0, 1.0]
        val = pn_alti.noise( w*rad, h*rad, 0.1f );

        //--- val 的常见区间在 [30.0,70.0],将其映射到 [0,15] ---
        val *= 100.0;
        ival = (int)floor((val-30)*0.375); 
        if( ival < 0 ){
            ival = 0;
        }else if( ival > 15 ){
            ival = 15;
        }

        fieldRef.alti = (alti_t)ival;

            //-- 根据 alti，改动 颜色。
            int off = (fieldRef.alti-8);

            fieldRef.color.r -=   (u8_t)(off * 2.2 ); 
            fieldRef.color.g -=   (u8_t)(off * 1.8 ); 
            fieldRef.color.b -=   (u8_t)(off * 0.6 );  
                        //--- 历史最好效果 -----
                        // { 2.2 - 1.8 - 0.6 }


    }

        

        


    this->is_assign_alti_in_field_done = true;
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
 * - 收集 周边 9个chunk nodeMPos
 */
void ChunkFieldSet::reset_nearby_chunk_datas(){
    nearby_chunk_datas.clear();
    //--------
    const IntVec2   currentChunkMPos = this->mcpos.get_mpos();
    IntVec2         tmpChunkMPos;
    size_t          tmpChunkIdx; //- chunk 在其所属section 中的 idx
    Section         *tmpSectionPtr;
    chunkData_t     tmpChunkData;

    for( int h=-1; h<=1; h++ ){
        for( int w=-1; w<=1; w++ ){ //- 周边 9 个 chunk
            tmpChunkMPos.set(   currentChunkMPos.x + w*ENTS_PER_CHUNK,
                                currentChunkMPos.y + h*ENTS_PER_CHUNK );
            tmpChunkIdx = get_chunkIdx_in_section( tmpChunkMPos );
            tmpSectionPtr = esrc::get_sectionPtr( anyMPos_2_sectionKey(tmpChunkMPos) );
            //------
            tmpChunkData.first = tmpSectionPtr->get_chunkEcoSysInMapKey( tmpChunkIdx );
            tmpChunkData.second = tmpSectionPtr->get_chunkNodeMPos(tmpChunkIdx); //- copy
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


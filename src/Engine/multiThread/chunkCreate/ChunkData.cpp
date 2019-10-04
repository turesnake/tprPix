/*
 * ========================== ChunkData.cpp =======================
 *                          -- tpr --
 *                                        CREATE -- 2019.09.26
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *  以 chunk 为单位的，需要被 job线程 计算生成的 数据集 
 * ----------------------------
 */
#include "ChunkData.h"


void ChunkData::init( chunkKey_t chunkKey_ )noexcept{

    this->mapEntInns.resize( (ENTS_PER_CHUNK+2) * (ENTS_PER_CHUNK+2) );// 34*34 mapents
    this->fieldDatas.reserve( FIELDS_PER_CHUNK * FIELDS_PER_CHUNK );

    IntVec2 chunkMPos = chunkKey_2_mpos(chunkKey_);
    IntVec2 tmpFieldMPos {};
    fieldKey_t  tmpFieldKey {};

    for( size_t h=0; h<FIELDS_PER_CHUNK; h++ ){
        for( size_t w=0; w<FIELDS_PER_CHUNK; w++ ){
            tmpFieldMPos = chunkMPos + IntVec2{w*ENTS_PER_FIELD, h*ENTS_PER_FIELD};
            tmpFieldKey = fieldMPos_2_fieldKey(tmpFieldMPos);
            auto outPair = this->fieldDatas.insert({ tmpFieldKey, std::make_unique<Job_Field>() });
            tprAssert( outPair.second );
        }
    }
}


// param: mposOff_ [0, 31]
bool ChunkData::is_borderMapEnt( IntVec2 mposOff_ )noexcept{

    tprAssert(  (mposOff_.x>=0) && (mposOff_.x<ENTS_PER_CHUNK) &&
                (mposOff_.y>=0) && (mposOff_.y<ENTS_PER_CHUNK) ); //- [0,31]

    IntVec2 offV = mposOff_ + IntVec2{1,1}; //- 现在的 mapEntInns 外凸了一圈 mapent [1,32]
    size_t idx = cast_2_size_t(offV.y * (ENTS_PER_CHUNK+2) + offV.x);
    tprAssert( idx < this->mapEntInns.size() );


    //---
    //colorTableId_t selfColorId = this->mapEntInns.at(idx).colorRableId;
    //colorTableId_t tmpColorId {};

    sectionKey_t selfEcoKey = this->mapEntInns.at(idx).ecoObjKey;
    sectionKey_t tmpEcoKey {};
    
    IntVec2 tmpOffV {};
    size_t  tmpIdx  {};

    //-- nearby_9_mapent --
    for( int j=-1; j<=1; j++ ){
        for( int i=-1; i<=1; i++ ){
            
            //-- skip self --
            if( (j==0) && (i==0) ){
                continue;
            }
            //-- skip outEnt --
            tmpOffV = offV + IntVec2{i,j}; // [0,33]

            tprAssert(  (tmpOffV.x>=0) && (tmpOffV.x<ENTS_PER_CHUNK+2) &&
                        (tmpOffV.y>=0) && (tmpOffV.y<ENTS_PER_CHUNK+2) ); //- [0,34]

            //-- do check --
            tmpIdx = static_cast<size_t>(tmpOffV.y * (ENTS_PER_CHUNK+2) + tmpOffV.x);

            tmpEcoKey = this->mapEntInns.at(tmpIdx).ecoObjKey;
            if( selfEcoKey != tmpEcoKey ){
                return true;
            }

        }
    }
    return false;
}



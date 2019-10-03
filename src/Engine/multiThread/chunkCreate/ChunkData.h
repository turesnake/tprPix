/*
 * ========================== ChunkData.h =======================
 *                          -- tpr --
 *                                        CREATE -- 2019.04.24
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *  以 chunk 为单位的，需要被 job线程 计算生成的 数据集 
 * ----------------------------
 */
#ifndef TPR_CHUNK_DATA_H
#define TPR_CHUNK_DATA_H

//-------------------- CPP --------------------//
#include <vector>

//-------------------- Engine --------------------//
#include "tprCast.h"
#include "tprAssert.h"
#include "config.h"
#include "MapTexture.h"
#include "MapAltitude.h"
#include "fieldKey.h"
#include "chunkKey.h"
#include "Job_ChunkCreate.h"



class ChunkData{
public:
    ChunkData( chunkKey_t chunkKey_ )
        {
            this->init( chunkKey_ );
        }
    void init( chunkKey_t chunkKey_ )noexcept;

    //- param: offset from chunk.mpos [-1,32]
    inline Job_MapEntInn &getnc_mapEntInnRef( IntVec2 mposOff_ )noexcept{

        tprAssert(  (mposOff_.x>=-1) && (mposOff_.x<=ENTS_PER_CHUNK) &&
                    (mposOff_.y>=-1) && (mposOff_.y<=ENTS_PER_CHUNK) ); // [-1,32]

        IntVec2 offV = mposOff_ + IntVec2{1,1}; //- 现在的 mapEntInns 外凸了一圈 mapent
        size_t idx = cast_2_size_t(offV.y * (ENTS_PER_CHUNK+2) + offV.x);
        tprAssert( idx < this->mapEntInns.size() );
        return this->mapEntInns.at(idx);
    }

    bool is_borderMapEnt( IntVec2 mposOff_ )noexcept;
    
    inline void insert_a_entInnPtr_2_field( fieldKey_t fieldKey_, 
                                            IntVec2 mposOff_,
                                            Job_MapEntInn* entPtr_ )noexcept{
        tprAssert( this->fieldDatas.find(fieldKey_) != this->fieldDatas.end() );
        this->fieldDatas.at(fieldKey_)->insert_a_entInnPtr( mposOff_, entPtr_ );
    }

    inline void apply_field_job_groundGoEnts( fieldKey_t fieldKey_ )noexcept{
        tprAssert( this->fieldDatas.find(fieldKey_) != this->fieldDatas.end() );
        this->fieldDatas.at(fieldKey_)->apply_job_groundGoEnts();
    }
    

    inline void set_field_min_max_altis( fieldKey_t fieldKey_, MapAltitude min_, MapAltitude max_ )noexcept{
        tprAssert( this->fieldDatas.find(fieldKey_) != this->fieldDatas.end() );
        auto *fieldPtr = this->fieldDatas.at(fieldKey_).get();
        fieldPtr->minFieldAlti = min_;
        fieldPtr->maxFieldAlti = max_;
    }

    inline const std::unordered_map<fieldKey_t, std::unique_ptr<Job_Field>> &get_fieldDatas()const noexcept{
        return this->fieldDatas;
    }

    inline const Job_Field *get_job_fieldPtr( fieldKey_t fieldKey_ )const noexcept{
        tprAssert( this->fieldDatas.find(fieldKey_) != this->fieldDatas.end() );
        return this->fieldDatas.at(fieldKey_).get();
    }

private:
    std::vector<Job_MapEntInn> mapEntInns {};// [34 * 34 mapents] 朝四周外凸了 1-mapent
    std::unordered_map<fieldKey_t, std::unique_ptr<Job_Field>> fieldDatas {};
};


#endif 


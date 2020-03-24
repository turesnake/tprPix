/*
 * ======================= Job_Chunk.h =======================
 *                          -- tpr --
 *                                        CREATE -- 2019.04.24
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *  以 chunk 为单位的，需要被 job线程 计算生成的 数据集 
 * ----------------------------
 */
#ifndef TPR_JOB_CHUNK_H
#define TPR_JOB_CHUNK_H

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
#include "Job_MapEnt.h"
#include "Job_Field.h"

#include "MapField.h"



class Job_Chunk{
public:
    explicit Job_Chunk( chunkKey_t chunkKey_ ):
        chunkKey(chunkKey_),
        chunkMPos(chunkKey_2_mpos(chunkKey_))
        {
            this->init();
        }

    void write_2_field_from_jobData();
    void create_field_goSpecDatas();
    bool is_mapEnt_a_ecoBorder( IntVec2 mposOff_ )noexcept;

    inline chunkKey_t   get_chunkKey()const noexcept{ return this->chunkKey; }
    inline IntVec2      get_chunkMPos()const noexcept{ return this->chunkMPos; }
    
    inline Job_MapEnt &getnc_mapEntInnRef( IntVec2 mposOff_ )noexcept{

        tprAssert(  (mposOff_.x>=0) && (mposOff_.x<ENTS_PER_CHUNK<>) &&
                    (mposOff_.y>=0) && (mposOff_.y<ENTS_PER_CHUNK<>) );

        size_t idx = cast_2_size_t(mposOff_.y * ENTS_PER_CHUNK<> + mposOff_.x);
        tprAssert( idx < this->mapEntInns.size() );
        return *(this->mapEntInns.at(idx));
    }

    inline void insert_a_entInnPtr_2_field( fieldKey_t fieldKey_, 
                                            IntVec2 mposOff_,
                                            Job_MapEnt* entPtr_ )noexcept{
        tprAssert( this->job_fields.find(fieldKey_) != this->job_fields.end() );
        this->job_fields.at(fieldKey_)->insert_a_entInnPtr( mposOff_, entPtr_ );
    }

    /*
    inline void apply_field_job_groundGoEnts( fieldKey_t fieldKey_ )noexcept{
        tprAssert( this->job_fields.find(fieldKey_) != this->job_fields.end() );
        this->job_fields.at(fieldKey_)->apply_groundGoEnts();
    }
    */
    

    inline void set_field_min_max_altis( fieldKey_t fieldKey_, MapAltitude min_, MapAltitude max_ )noexcept{
        tprAssert( this->fields.find(fieldKey_) != this->fields.end() );
        auto *fieldPtr = this->fields.at(fieldKey_).get();
        fieldPtr->init_minAlti( min_ );
        fieldPtr->init_maxAlti( max_ );
    }

    inline const Job_Field *get_job_fieldPtr( fieldKey_t fieldKey_ )const noexcept{
        tprAssert( this->job_fields.find(fieldKey_) != this->job_fields.end() );
        return this->job_fields.at(fieldKey_).get();
    }

    inline MapField &getnc_fieldRef( fieldKey_t fieldKey_ )noexcept{
        tprAssert( this->fields.find(fieldKey_) != this->fields.end() );
        return *(this->fields.at(fieldKey_));
    }

    inline std::unordered_map<fieldKey_t, std::unique_ptr<MapField>> &
    get_fields()noexcept{ return this->fields; }


private:
    void init()noexcept;
    //------
    chunkKey_t chunkKey     {};
    IntVec2    chunkMPos    {};

    std::vector<std::unique_ptr<Job_MapEnt>> mapEntInns {};// [32 * 32 mapents]

    std::unordered_map<fieldKey_t, std::unique_ptr<Job_Field>> job_fields {};
    std::unordered_map<fieldKey_t, std::unique_ptr<MapField>> fields {};
                        // 在 job线程 直接创建 field 实例
                        // 最后一股脑传递给 主线程
};


#endif 


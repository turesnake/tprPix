/*
 * ========================== Job_Field.h =======================
 *                          -- tpr --
 *                                        CREATE -- 2019.10.08
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#ifndef TPR_JOB_FIELD_H
#define TPR_JOB_FIELD_H
//--- glm - 0.9.9.5 ---
#include "glm_no_warnings.h"

//-------------------- CPP --------------------//
#include <vector>
#include <unordered_map>
#include <set>

//-------------------- Engine --------------------//
#include "Job_MapEnt.h"
#include "animSubspeciesId.h"


class Job_GoMesh{
public:
    Job_GoMesh( animSubspeciesId_t  id_,
                const glm::vec2     &fposOff_ ):
        subspecId(id_),
        fposOff(fposOff_)
        {}
    //----- vals -----//
    animSubspeciesId_t  subspecId {};
    glm::vec2           fposOff   {}; // gomesh-dposoff based on go
};




// datas to support the field_gos create in main-thread 
class Job_GoData{
public:
    
    goSpecId_t              goSpecId {};
    glm::dvec2              goDposOff {}; // go-dposoff based on field-middpos
    size_t                  mapEntUWeight {};

    std::vector<Job_GoMesh> job_goMeshs {};
};



class Job_Field{
public:
    Job_Field()
        {
            //-- 二维数组 --
            this->mapEntPtrs.resize( ENTS_PER_FIELD ); // h
            for( auto &c : this->mapEntPtrs ){
                c.resize( ENTS_PER_FIELD ); // w
            }
            //---
            this->halfFields.resize( HALF_ENTS_PER_FIELD * HALF_ENTS_PER_FIELD );
        }

    
    // param: mposOff_ base on field.mpos
    inline void insert_a_entInnPtr( IntVec2 mposOff_, Job_MapEnt *entPtr_ )noexcept{
        tprAssert(  (mposOff_.x>=0) && (mposOff_.x<ENTS_PER_FIELD) &&
                    (mposOff_.y>=0) && (mposOff_.y<ENTS_PER_FIELD));

        //size_t entIdx = cast_2_size_t(mposOff_.y * ENTS_PER_FIELD + mposOff_.x);
        size_t hIdx = Job_Field::get_halfFieldIdx(mposOff_);

        //--- mapEntPtrs ---
        this->mapEntPtrs.at(static_cast<size_t>(mposOff_.y)).at(static_cast<size_t>(mposOff_.x)) = entPtr_;
        //--- is have border ent ---
        if( (entPtr_->isBorder) && (!this->isHaveBorderEnt) ){
            this->isHaveBorderEnt = true;
        }
        //--- half field container ---
        this->halfFields.at(hIdx).insert( entPtr_->colorTableId ); // maybe
        //--- field container ---
        this->fields.insert( entPtr_->colorTableId ); // maybe
        //--- ecoObjKey container ---
        this->ecoObjKeys.insert( entPtr_->ecoObjKey ); // maybe
    }

    void apply_job_groundGoEnts()noexcept;

    inline const std::vector<std::unique_ptr<Job_GroundGoEnt>> &get_job_groundGoEnts()const noexcept{
        return this->groundGoEnts;
    }

    inline bool is_crossEcoObj()const noexcept{
        return (this->ecoObjKeys.size() > 1);
    }
    inline bool is_crossColorTable()const noexcept{
        return (this->fields.size() > 1);
    }


    inline Job_GoData &insert_new_job_goData()noexcept{
        this->job_goDatas.push_back( Job_GoData{} );
        auto &target = this->job_goDatas.back();
        return target;
    }



    inline const std::vector<Job_GoData> &get_job_goDatas()const noexcept{
        return this->job_goDatas;
    }



private:

    inline static size_t get_halfFieldIdx( IntVec2 mposOff_ )noexcept{
        IntVec2 halfFieldPos = mposOff_.floorDiv(static_cast<double>(HALF_ENTS_PER_FIELD));
        return cast_2_size_t( halfFieldPos.y * HALF_ENTS_PER_FIELD + halfFieldPos.x );
    }

    //=== datas passed to the main thread  ===//
    std::vector<std::unique_ptr<Job_GroundGoEnt>> groundGoEnts {};


    std::vector<Job_GoData> job_goDatas {}; // datas need to be create in main-thread



    //=== datas just used for inner calc ===
    std::vector<std::vector<Job_MapEnt*>> mapEntPtrs {}; // 二维数组 [h,w]
    //-- 在未来，元素type 会被改成 colorTableId_t ---
    std::set<sectionKey_t> ecoObjKeys {};
    std::set<colorTableId_t> fields {};
    std::vector<std::set<colorTableId_t>> halfFields {}; // 4 containers
            // leftBottom, rightBottom, leftTop, rightTop

    //===== flags =====//
    bool isHaveBorderEnt    {false}; //- 只要发现 border
};


#endif 


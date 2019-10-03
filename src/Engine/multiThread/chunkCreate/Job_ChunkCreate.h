/*
 * ========================== Job_ChunkCreate.h =======================
 *                          -- tpr --
 *                                        CREATE -- 2019.09.26
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#ifndef TPR_JOB_CHUNK_CREATE_H
#define TPR_JOB_CHUNK_CREATE_H
//--- glm - 0.9.9.5 ---
#include "glm_no_warnings.h"

//-------------------- CPP --------------------//
#include <vector>
#include <unordered_map>
#include <set>

//-------------------- Engine --------------------//
#include "tprAssert.h"
#include "tprCast.h"
#include "config.h"
#include "IntVec.h"
#include "sectionKey.h"
#include "fieldKey.h"
#include "colorTableId.h"
#include "Density.h"
#include "MapAltitude.h"

#include "Job_GroundGoEnt.h"


class MemMapEnt;


class Job_MapEntInn{
public:
    
    void init( IntVec2 mpos_ )noexcept;
    //====== vals ======//
    IntVec2    mpos         {};
    IntVec2    midPPos      {}; // 中间点

    sectionKey_t        ecoObjKey {};
    colorTableId_t      colorRableId {}; // same as ecoObj.colorTableId
    Density             density {};
    MapAltitude         alti {};

    double  originPerlin {}; // [-1.0, 1.0]
    double  uWeight      {}; // [0.0, 97.0]

    bool  isBorder {false}; //- 是否为 eco边缘go

                             // 在未来，将被拓展为 一个 具体的数字，表示自己离 border 的距离（mapents）...
                              

    void write_2_mapEnt( MemMapEnt &entRef_ )const noexcept;

private:
    double calc_pixAlti( IntVec2 pixPPos_ )noexcept;

    //===== static =====//
    static const IntVec2 pixesPerHalfMapent;
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
    inline void insert_a_entInnPtr( IntVec2 mposOff_, Job_MapEntInn *entPtr_ )noexcept{
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
        this->halfFields.at(hIdx).insert( entPtr_->colorRableId ); // maybe
        //--- field container ---
        this->fields.insert( entPtr_->colorRableId ); // maybe
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


    MapAltitude  minFieldAlti {};  
    MapAltitude  maxFieldAlti {};

private:

    inline static size_t get_halfFieldIdx( IntVec2 mposOff_ )noexcept{
        IntVec2 halfFieldPos = mposOff_.floorDiv(static_cast<double>(HALF_ENTS_PER_FIELD));
        return cast_2_size_t( halfFieldPos.y * HALF_ENTS_PER_FIELD + halfFieldPos.x );
    }

    //=== datas passed to the main thread  ===//
    std::vector<std::unique_ptr<Job_GroundGoEnt>> groundGoEnts {};


    //=== datas just used for inner calc ===
    std::vector<std::vector<Job_MapEntInn*>> mapEntPtrs {}; // 二维数组 [h,w]
    //-- 在未来，元素type 会被改成 colorTableId_t ---
    std::set<sectionKey_t> ecoObjKeys {};
    std::set<colorTableId_t> fields {};
    std::vector<std::set<colorTableId_t>> halfFields {}; // 4 containers
            // leftBottom, rightBottom, leftTop, rightTop

    //===== flags =====//
    bool isHaveBorderEnt    {false}; //- 只要发现 border
};



#endif 


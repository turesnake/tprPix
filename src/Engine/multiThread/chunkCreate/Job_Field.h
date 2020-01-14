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
#include "mapEntKey.h"

#include "GoDataForCreate.h"


class Job_Field{
public:
    Job_Field()
        {
            //===== static =====//
            if( !Job_Field::isStaticInit ){
                Job_Field::isStaticInit = true;
                Job_Field::init_for_static();
            }
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
    inline void insert_2_majorGoDataPtrs( const GoDataForCreate *ptr_ )noexcept{
        this->majorGoDataPtrs.push_back( ptr_ );
    }
    inline void insert_2_floorGoDataPtrs( const GoDataForCreate *ptr_ )noexcept{
        this->floorGoDataPtrs.push_back( ptr_ );
    }
    inline const std::vector<const GoDataForCreate*> &get_majorGoDataPtrs()const noexcept{ 
        return this->majorGoDataPtrs; 
    }
    inline const std::vector<const GoDataForCreate*> &get_floorGoDataPtrs()const noexcept{ 
        return this->floorGoDataPtrs; 
    }
    inline std::unordered_map<mapEntKey_t, std::unique_ptr<GoDataForCreate>> &get_nature_majorGoDatas()noexcept{ 
        return this->nature_majorGoDatas;
    }
    inline std::unordered_map<mapEntKey_t, std::unique_ptr<GoDataForCreate>> &get_nature_floorGoDatas()noexcept{ 
        return this->nature_floorGoDatas;
    }


    inline void copy_nature_majorGoDataPtrs()noexcept{
        for( const auto &[iKey, iUPtr] : this->nature_majorGoDatas ){
            this->majorGoDataPtrs.push_back( iUPtr.get() );
        }
    }
    inline void copy_nature_floorGoDataPtrs()noexcept{
        for( const auto &[iKey, iUPtr] : this->nature_floorGoDatas ){
            this->floorGoDataPtrs.push_back( iUPtr.get() );
        }
    }


private:

    inline static size_t get_halfFieldIdx( IntVec2 mposOff_ )noexcept{
        IntVec2 halfFieldPos = mposOff_.floorDiv(static_cast<double>(HALF_ENTS_PER_FIELD));
        return cast_2_size_t( halfFieldPos.y * HALF_ENTS_PER_FIELD + halfFieldPos.x );
    }

    //=== datas passed to the main thread  ===//
    std::vector<std::unique_ptr<Job_GroundGoEnt>> groundGoEnts {};


    // 同时包含 artifact/nature 两种蓝图数据
    // 供 具象go类 访问，创建 go实例
    std::vector<const GoDataForCreate*> majorGoDataPtrs {};
    std::vector<const GoDataForCreate*> floorGoDataPtrs {};


    // 人造物蓝图数据 实际存储区，不像人造物数据，被存储在 ecoobj 中
    // 但是为了对外统一接口，还是会把 ptr 存储在一个容器中，以便具象类集中访问
    std::unordered_map<mapEntKey_t, std::unique_ptr<GoDataForCreate>> nature_majorGoDatas {};
    std::unordered_map<mapEntKey_t, std::unique_ptr<GoDataForCreate>> nature_floorGoDatas {};


    //=== datas just used for inner calc ===
    std::vector<std::vector<Job_MapEnt*>> mapEntPtrs {}; // 二维数组 [h,w]
    //-- 在未来，元素type 会被改成 colorTableId_t ---
    std::set<sectionKey_t> ecoObjKeys {};
    std::set<colorTableId_t> fields {};
    std::vector<std::set<colorTableId_t>> halfFields {}; // 4 containers
            // leftBottom, rightBottom, leftTop, rightTop

    //===== flags =====//
    bool isHaveBorderEnt    {false}; //- 只要发现 border

    //===== static =====//
    static void init_for_static()noexcept;
    static bool isStaticInit;
};


#endif 


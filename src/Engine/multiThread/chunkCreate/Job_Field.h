/*
 * ========================== Job_Field.h =======================
 *                          -- tpr --
 *                                        CREATE -- 2019.10.08
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#ifndef TPR_JOB_FIELD_H
#define TPR_JOB_FIELD_H

#include "pch.h"

//-------------------- Engine --------------------//
#include "Job_MapEnt.h"
#include "animSubspeciesId.h"
#include "mapEntKey.h"
#include "goLabelId.h"
#include "fieldFractType.h"

#include "GoDataForCreate.h"

// need:
class Job_Chunk;


// tmp data. created in job threads
class Job_Field{

    // 按照规则将 mapents，合并为 halfField / Field 
    // 从而减少 go 的创建数量
    template< typename T >
    class Fract{
    public:
        Fract()
            {
                this->inHalfFields.resize( HALF_ENTS_PER_FIELD<size_t> * HALF_ENTS_PER_FIELD<size_t> );
            }

        inline void insert( IntVec2 mposOff_, const T &val_ )noexcept{
            tprAssert(  (mposOff_.x>=0) && (mposOff_.x<ENTS_PER_FIELD<>) &&
                        (mposOff_.y>=0) && (mposOff_.y<ENTS_PER_FIELD<>));
            //--- field ---//
            this->inField.insert(val_); // maybe
            //--- halfField ---//
            size_t hIdx = Fract::calc_halfFieldIdx(mposOff_);
            tprAssert( this->inHalfFields.size() > hIdx );
            this->inHalfFields.at(hIdx).insert(val_);
        }


        inline size_t get_inField_size()const noexcept{ return this->inField.size(); }
        inline const std::set<T> &get_inField()const noexcept{ return this->inField; }
        inline const std::vector<std::set<T>> &get_inHalfFields()const noexcept{ return this->inHalfFields; }

    private:
        inline static size_t calc_halfFieldIdx( IntVec2 mposOff_ )noexcept{
            IntVec2 halfFieldPos = mposOff_.floorDiv( HALF_ENTS_PER_FIELD_D );
            return cast_2_size_t( halfFieldPos.y * HALF_ENTS_PER_FIELD<> + halfFieldPos.x );
        }
        //========== vals ==========//
        std::set<T> inField {};        
        std::vector<std::set<T>> inHalfFields {}; // 4 containers
                            // leftBottom, rightBottom, leftTop, rightTop
    };



public:
    //========== Self Vals ==========//
    Job_Field( Job_Chunk &jChunkRef_, fieldKey_t fieldKey_ ):
        fieldKey(fieldKey_)
        {
            //-- 二维数组 --
            this->mapEntPtrs.resize( ENTS_PER_FIELD<size_t> ); // h
            for( auto &c : this->mapEntPtrs ){
                c.resize( ENTS_PER_FIELD<size_t> ); // w
            }
        }

    
    // param: mposOff_ base on field.mpos
    inline void insert_a_entInnPtr( IntVec2 mposOff_, Job_MapEnt *entPtr_ )noexcept{
        tprAssert(  (mposOff_.x>=0) && (mposOff_.x<ENTS_PER_FIELD<>) &&
                    (mposOff_.y>=0) && (mposOff_.y<ENTS_PER_FIELD<>));

        //--- mapEntPtrs ---
        this->mapEntPtrs.at(static_cast<size_t>(mposOff_.y)).at(static_cast<size_t>(mposOff_.x)) = entPtr_;
        //--- is have border ent ---
        if( entPtr_->get_isEcoBorder() && (!this->isHaveBorderEnt) ){
            this->isHaveBorderEnt = true;
        }
        
        //--- ecoObjKey container ---
        this->ecoObjKeys.insert( entPtr_->get_ecoObjKey() ); // maybe

        colorTableIdFract.insert( mposOff_, entPtr_->get_colorTableId() );
        bioSoupFract.insert( mposOff_, entPtr_->get_bioSoupState() );
    }


    void apply_bioSoupEnts();
    void apply_groundGoEnts();


    inline bool is_crossEcoObj()const noexcept{
        return (this->ecoObjKeys.size() > 1);
    }
    inline bool is_crossColorTable()const noexcept{
        return ( this->colorTableIdFract.get_inField_size() > 1 );
    }
    inline void insert_2_majorGoDataPtrs( const GoDataForCreate *ptr_ )noexcept{
        this->majorGoDataPtrs.push_back( ptr_ );
    }
    inline void insert_2_floorGoDataPtrs( const GoDataForCreate *ptr_ )noexcept{
        this->floorGoDataPtrs.push_back( ptr_ );
    }

    inline void insert_2_bioSoupGoDatas( std::unique_ptr<GoDataForCreate> uptr_ )noexcept{
        this->bioSoupGoDatas.push_back( std::move(uptr_) );
    }



    inline const std::vector<const GoDataForCreate*> &get_majorGoDataPtrs()const noexcept{ 
        return this->majorGoDataPtrs; 
    }
    inline const std::vector<const GoDataForCreate*> &get_floorGoDataPtrs()const noexcept{ 
        return this->floorGoDataPtrs; 
    }
    inline const std::vector<const GoDataForCreate*> &get_bioSoupGoDataPtrs()const noexcept{ 
        return this->bioSoupGoDataPtrs;
    }
    inline std::optional<const GoDataForCreate*> get_groundGoDataPtr()const noexcept{
        if( this->groundGoData ){
            return { this->groundGoData.get() };
        }else{
            return std::nullopt;
        }
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
    

    inline fieldKey_t   get_fieldKey()const noexcept{ return this->fieldKey; }
    inline bool         get_isCoveredBy_InertiaBioSoup()const noexcept{ return this->isCoveredBy_InertiaBioSoup; }
    inline IntVec2      get_fieldMidMPos()const noexcept{ return (fieldKey_2_mpos(this->fieldKey) + IntVec2{ HALF_ENTS_PER_FIELD<>, HALF_ENTS_PER_FIELD<> }); }


    //===== static =====//
    static void init_for_static()noexcept; // MUST CALL IN MAIN !!! 

private:

    void create_bioSoupDataUPtr(    FieldFractType fieldFractType_,
                                    IntVec2 mpos_, 
                                    gameObjs::bioSoup::State bioSoupState_,
                                    MapAltitude mapEntAlti_ );

    inline void copy_bioSoupGoDataPtrs()noexcept{
        for( const auto &uptr : this->bioSoupGoDatas ){
            this->bioSoupGoDataPtrs.push_back( uptr.get() );
        }
    }


    // 同时包含 artifact/nature 两种蓝图数据
    // 供 具象go类 访问，创建 go实例
    std::vector<const GoDataForCreate*> majorGoDataPtrs {};
    std::vector<const GoDataForCreate*> floorGoDataPtrs {};
    std::vector<const GoDataForCreate*> bioSoupGoDataPtrs {};


    // 人造物蓝图数据 实际存储区，不像人造物数据，被存储在 ecoobj 中
    // 但是为了对外统一接口，还是会把 ptr 存储在一个容器中，以便具象类集中访问
    std::unordered_map<mapEntKey_t, std::unique_ptr<GoDataForCreate>> nature_majorGoDatas {};
    std::unordered_map<mapEntKey_t, std::unique_ptr<GoDataForCreate>> nature_floorGoDatas {};


    // 临时方案，单独存储 bioSoup mp-go
    std::vector<std::unique_ptr<GoDataForCreate>> bioSoupGoDatas {};

    // 1个field，只能拥有一个 groundGo
    // 如果本 field，被 Inertia-BioSoup 完全覆盖，则不会分配
    std::unique_ptr<GoDataForCreate> groundGoData {};


    //=== datas just used for inner calc ===
    std::vector<std::vector<Job_MapEnt*>> mapEntPtrs {}; // 二维数组 [h,w]
    //-- 在未来，元素type 会被改成 colorTableId_t ---
    std::set<sectionKey_t> ecoObjKeys {};



    Fract<gameObjs::bioSoup::State>     bioSoupFract {};
    Fract<colorTableId_t>               colorTableIdFract {};


    
    fieldKey_t  fieldKey;

    //===== flags =====//
    bool isHaveBorderEnt            {false}; //- 只要发现 border. 暂时无用
    bool isCoveredBy_InertiaBioSoup {false};
                // 若为 true，本 field 不再创建 groundGo, floorGo

};



#endif 


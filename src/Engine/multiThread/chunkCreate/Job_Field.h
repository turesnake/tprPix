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
#include <map>
#include <set>
#include <functional>

//-------------------- Engine --------------------//
#include "Job_MapEnt.h"
#include "animSubspeciesId.h"
#include "mapEntKey.h"
#include "tprCast.h"
#include "goLabelId.h"
#include "fieldFractType.h"


#include "GoDataForCreate.h"


class Job_Chunk;


class Job_Field{


    template< typename T >
    class Fract{
    public:
        Fract()
            {
                this->inHalfFields.resize( cast_2_size_t( HALF_ENTS_PER_FIELD * HALF_ENTS_PER_FIELD ) );
            }

        inline void insert( IntVec2 mposOff_, const T &val_ )noexcept{
            tprAssert(  (mposOff_.x>=0) && (mposOff_.x<ENTS_PER_FIELD) &&
                        (mposOff_.y>=0) && (mposOff_.y<ENTS_PER_FIELD));
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
            IntVec2 halfFieldPos = mposOff_.floorDiv(static_cast<double>(HALF_ENTS_PER_FIELD));
            return cast_2_size_t( halfFieldPos.y * HALF_ENTS_PER_FIELD + halfFieldPos.x );
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
            this->mapEntPtrs.resize( cast_2_size_t(ENTS_PER_FIELD) ); // h
            for( auto &c : this->mapEntPtrs ){
                c.resize( cast_2_size_t(ENTS_PER_FIELD) ); // w
            }
        }

    
    // param: mposOff_ base on field.mpos
    inline void insert_a_entInnPtr( IntVec2 mposOff_, Job_MapEnt *entPtr_ )noexcept{
        tprAssert(  (mposOff_.x>=0) && (mposOff_.x<ENTS_PER_FIELD) &&
                    (mposOff_.y>=0) && (mposOff_.y<ENTS_PER_FIELD));

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
    

    inline fieldKey_t get_fieldKey()const noexcept{ return this->fieldKey; }
    inline size_t get_leftBottomMapEnt_uWeight()const noexcept{ return this->mapEntPtrs.at(0).at(0)->get_uWeight(); } // 生成 groundGo 时使用，

    //===== static =====//
    static void init_for_static()noexcept; // MUST CALL IN MAIN !!! 

private:

    void create_bioSoupDataUPtr(    IntVec2 mpos_, 
                                    FieldFractType fieldFractType_,
                                    BioSoupState bioSoupState_ );

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
    // 如果本 field，被 BioSoup 占据，甚至不会分配 groundGo
    std::unique_ptr<GoDataForCreate> groundGoData {};


    //=== datas just used for inner calc ===
    std::vector<std::vector<Job_MapEnt*>> mapEntPtrs {}; // 二维数组 [h,w]
    //-- 在未来，元素type 会被改成 colorTableId_t ---
    std::set<sectionKey_t> ecoObjKeys {};



    Fract<BioSoupState>     bioSoupFract {};
    Fract<colorTableId_t>   colorTableIdFract {};


    
    fieldKey_t  fieldKey;

    //===== flags =====//
    bool isHaveBorderEnt    {false}; //- 只要发现 border

};



#endif 


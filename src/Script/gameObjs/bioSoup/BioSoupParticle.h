/*
 * ==================== BioSoupParticle.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2020.03.06
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#ifndef TPR_GO_BIO_SOUP_PARTICLE_H
#define TPR_GO_BIO_SOUP_PARTICLE_H
//--- glm - 0.9.9.5 ---
#include "glm_no_warnings.h"


//-------------------- CPP --------------------//
#include <cmath>
#include <vector>
#include <map>

//-------------------- Engine --------------------//
#include "tprAssert.h"
#include "animSubspeciesId.h"
#include "fieldFractType.h"
#include "MapAltitude.h"
#include "SignInMapEnts_Square_Type.h"


//-------------------- Script --------------------//
#include "Script/gameObjs/bioSoup/BioSoupState.h"



namespace gameObjs::bioSoup {//------------- namespace gameObjs::bioSoup ----------------




class BioSoupParticle{
public:
    BioSoupParticle()
        {
            if( !BioSoupParticle::isStaticInit ){
                BioSoupParticle::isStaticInit = true;
                BioSoupParticle::init_for_static();
            }
        }


    inline void init(   size_t uWeight_, 
                        State bioSoupState_, 
                        SignInMapEnts_Square_Type squType_,
                        MapAltitude mpAlti_
                    )noexcept{
        this->uWeight = uWeight_;
        this->bioSoupState = bioSoupState_;


        {// extraCreateStepDelay 
            double d = static_cast<double>(std::abs(mpAlti_.get_val())) * 1.7;
            this->extraCreateStepDelay = static_cast<size_t>( std::floor(d) );
        }

        //-- fieldFractType --
        switch( squType_ ){
            case SignInMapEnts_Square_Type::T_1m1: this->fieldFractType = FieldFractType::MapEnt;   break;
            case SignInMapEnts_Square_Type::T_2m2: this->fieldFractType = FieldFractType::HalfField;break;
            case SignInMapEnts_Square_Type::T_4m4: this->fieldFractType = FieldFractType::Field;    break;
            default: tprAssert(0);
        }

        {// animSubspeciesId
            size_t idx = (uWeight_ + 9976173) % BioSoupParticle::animSubspeciesIdSets.size();
            this->animSubspeciesPtr = &BioSoupParticle::animSubspeciesIdSets.at(idx);
        }

        // goMesh_dposOffsPtr
        if( this->fieldFractType == FieldFractType::MapEnt ){
            // do nothing
        }else if( this->fieldFractType == FieldFractType::HalfField ){
            size_t idx = (uWeight_ + 9276115) % BioSoupParticle::goMeshDposOffSets_2m2.size();
            this->goMeshDposOffsPtr = &BioSoupParticle::goMeshDposOffSets_2m2.at(idx);

        }else if( this->fieldFractType == FieldFractType::Field ){
            size_t idx = (uWeight_ + 11976115) % BioSoupParticle::goMeshDposOffSets_4m4.size();
            this->goMeshDposOffsPtr = &BioSoupParticle::goMeshDposOffSets_4m4.at(idx);
        }else{
            tprAssert(0);
        }
            
        {// createStepsPtr 
            size_t idx = (uWeight_ + 9976133) % BioSoupParticle::createStepSets.size();
            this->createStepsPtr = &BioSoupParticle::createStepSets.at(idx);
        }
        
    }

    // 永远循环使用一组数据
    inline const glm::dvec2 &get_next_goMeshDposOff()noexcept{
        if( this->fieldFractType == FieldFractType::MapEnt ){
            return BioSoupParticle::goMeshDposOff_1m1;

        }else{
            this->goMeshDposOffCount++;
            if( this->goMeshDposOffCount >= this->goMeshDposOffsPtr->size() ){
                this->goMeshDposOffCount = 0;
            }
            return this->goMeshDposOffsPtr->at( this->goMeshDposOffCount ); 
        }
    }

    inline size_t get_next_createStep()noexcept{
        this->createStepCount++;
        if( this->createStepCount >= this->createStepsPtr->size() ){
            this->createStepCount = 0;
        }
        return this->createStepsPtr->at(this->createStepCount) + this->extraCreateStepDelay;
    }


    inline animSubspeciesId_t get_next_animSubspeciesId()noexcept{
        this->animSubspecieCount++;
        if( this->animSubspecieCount >= this->animSubspeciesPtr->size() ){
            this->animSubspecieCount = 0;
        }
        return this->animSubspeciesPtr->at(this->animSubspecieCount);
    }
    

private: 
    static void init_for_static();

    size_t          uWeight {};
    State           bioSoupState {}; // 暂时无用
    FieldFractType  fieldFractType {};
    
    //-- 一个 biosoup 实例，分配到一份随机数据后，就不再变动了
    std::vector<animSubspeciesId_t> *animSubspeciesPtr {nullptr};
    size_t                          animSubspecieCount {0};

    std::vector<glm::dvec2> *goMeshDposOffsPtr  {nullptr};
    size_t                   goMeshDposOffCount {0};

    std::vector<size_t>     *createStepsPtr     {nullptr};
    size_t                   createStepCount {0};
    size_t                   extraCreateStepDelay {}; // 越远离 边界的 区域，生成 particle 越缓慢


    //======= static =======//
    static bool isStaticInit;
    static std::vector<std::vector<animSubspeciesId_t>> animSubspeciesIdSets;

    // 若干份数据，每一份，都包含一组 打乱了次序的点，在这个点上 创建 particle-gomesh
    static constexpr glm::dvec2 goMeshDposOff_1m1 { 0.0, 0.0 };
    static std::vector<std::vector<glm::dvec2>> goMeshDposOffSets_2m2;
    static std::vector<std::vector<glm::dvec2>> goMeshDposOffSets_4m4;
    
    static std::vector<std::vector<size_t>>     createStepSets; 


};


}//------------- namespace gameObjs::bioSoup: end ----------------
#endif 



/*
 * ======================= BioSoupBase.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2020.03.06
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#ifndef TPR_GO_BIO_SOUP_BASE_H
#define TPR_GO_BIO_SOUP_BASE_H
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



class BioSoupBase{
public:
    BioSoupBase()
        {
            if( !BioSoupBase::isStaticInit ){
                BioSoupBase::isStaticInit = true;
                BioSoupBase::init_for_static();
            }
        }

    inline void init(   size_t uWeight_, 
                        State bioSoupState_, 
                        SignInMapEnts_Square_Type squType_
                    )noexcept{
        this->uWeight = uWeight_;
        this->bioSoupState = bioSoupState_;

        // fieldFractType 
        // animSubspeciesId
        size_t idx {};
        switch( squType_ ){
            case SignInMapEnts_Square_Type::T_1m1: 
                this->fieldFractType = FieldFractType::MapEnt;   
                idx = (uWeight_ + 69761733) % BioSoupBase::animSubspeciesIdSets_1m1.size();
                this->animSubspeciesIdsPtr = &BioSoupBase::animSubspeciesIdSets_1m1.at(idx);
                break;

            case SignInMapEnts_Square_Type::T_2m2: 
                this->fieldFractType = FieldFractType::HalfField;
                idx = (uWeight_ + 69761733) % BioSoupBase::animSubspeciesIdSets_2m2.size();
                this->animSubspeciesIdsPtr = &BioSoupBase::animSubspeciesIdSets_2m2.at(idx);
                break;

            case SignInMapEnts_Square_Type::T_4m4: 
                this->fieldFractType = FieldFractType::Field;   
                idx = (uWeight_ + 69761733) % BioSoupBase::animSubspeciesIdSets_4m4.size();
                this->animSubspeciesIdsPtr = &BioSoupBase::animSubspeciesIdSets_4m4.at(idx); 
                break;
            default: tprAssert(0);
        }
    }


    inline animSubspeciesId_t get_next_animSubspeciesId()noexcept{
        this->animSubspeciesIdCount++;
        if( this->animSubspeciesIdCount >= this->animSubspeciesIdsPtr->size() ){
            this->animSubspeciesIdCount = 0;
        }
        return this->animSubspeciesIdsPtr->at(this->animSubspeciesIdCount);
    }


private:
    static void init_for_static();

    size_t          uWeight {};
    State           bioSoupState {}; // 暂时无用
    FieldFractType  fieldFractType {};

    std::vector<animSubspeciesId_t> *animSubspeciesIdsPtr {nullptr};
    size_t                          animSubspeciesIdCount {0};

    //======= static =======//
    static bool isStaticInit;
    static std::vector<std::vector<animSubspeciesId_t>> animSubspeciesIdSets_1m1;
    static std::vector<std::vector<animSubspeciesId_t>> animSubspeciesIdSets_2m2;
    static std::vector<std::vector<animSubspeciesId_t>> animSubspeciesIdSets_4m4;

};






}//------------- namespace gameObjs::bioSoup: end ----------------
#endif 


/*
 * ======================= DensityPool.h =======================
 *                          -- tpr --
 *                                        CREATE -- 2019.10.07
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#ifndef TPR_DENSITY_POOL_H
#define TPR_DENSITY_POOL_H

//-------------------- CPP --------------------//
#include <vector>
#include <unordered_map>
#include <memory>

//-------------------- Engine --------------------//
#include "tprAssert.h"
#include "GoSpecData.h"
#include "random.h"

#include "FieldDistributePlan.h"
#include "FieldDistributeType.h"

//--- need ---//
//class EcoEnt;
class FieldDistributePlan;



class DensityPool{
public:
    DensityPool()=default;


    inline void insert_type( FieldDistributeType type_, size_t nums_ )noexcept{
        this->types.insert( this->types.begin(), nums_, type_ );
    }



    void insert_goSpecData( BodySize bodySize_,  std::unique_ptr<GoSpecData> uptr_  )noexcept;



    inline void shuffle( std::default_random_engine &engine_ )noexcept{

        std::shuffle(  this->types.begin(), this->types.end(), engine_ );
        for( auto &pair : this->goSpecDatas ){
            std::shuffle( pair.second.begin(), pair.second.end(), engine_ );
        }
    }

    inline void set_applyPercent( double percent_ )noexcept{ this->applyPercent = percent_; }


    const FieldDistributePlan &apply_a_fieldDistributePlan( size_t randUVal_ )const noexcept;


    // param: randUVal_ [0, 9999]
    inline bool isNeed2Apply( size_t randUVal_ )const noexcept{
        double randV = static_cast<double>(randUVal_) * 0.3717 + 313.717;
        double fract = randV - floor(randV);
        return ((fract <= this->applyPercent) ? true : false);
    }


    // param: randUVal_ [0, 9999]
    inline const GoSpecData *apply_a_goSpecDataPtr( BodySize bodySize_, size_t randUVal_  )const noexcept{
        size_t randIdx = randUVal_ + 99173;
        tprAssert( this->goSpecDatas.find(bodySize_) != this->goSpecDatas.end() );
        const auto &vec = this->goSpecDatas.at(bodySize_);
        return vec.at(randIdx % vec.size()).get();
    }


private:
    double  applyPercent {}; //- [0.0, 1.0]
    
    std::vector<FieldDistributeType> types {};

    std::unordered_map<BodySize, std::vector<std::unique_ptr<GoSpecData>>> goSpecDatas {};

};



#endif 


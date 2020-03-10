/*
 * ======================= DensityPool.h =======================
 *                          -- tpr --
 *                                        CREATE -- 2019.10.07
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#ifndef TPR_DENSITY_POOL_H
#define TPR_DENSITY_POOL_H

#include "pch.h"

//-------------------- Engine --------------------//
#include "GoSpecData.h"
#include "random.h"

#include "blueprintId.h"

//--- need ---//
class FieldDistributePlan;



class DensityPool{
public:
    DensityPool()=default;

    inline void set_applyPercent( double percent_ )noexcept{ this->applyPercent = percent_; }

    inline void insert_2_yardBlueprintIds( blueprint::yardBlueprintId_t yardId_, size_t num_ )noexcept{
        this->yardBlueprintIds.insert( this->yardBlueprintIds.end(), num_, yardId_ );
    }

    inline void shuffle( std::default_random_engine &engine_ )noexcept{
        std::shuffle(  this->yardBlueprintIds.begin(), this->yardBlueprintIds.end(), engine_ );
    }

    // param: randUVal_ [0, 9999]
    inline bool isNeed2Apply( size_t randUVal_ )const noexcept{
        if( this->yardBlueprintIds.empty() ){
            return false;
        }
        if( is_closeEnough<double>(this->applyPercent, 0.0, 0.001) ){
            return false;
        }
        double randV = static_cast<double>(randUVal_) * 0.3717 + 313.717;
        double fract = randV - floor(randV);
        return ((fract < this->applyPercent) ? true : false);
    }

   inline blueprint::yardBlueprintId_t apply_a_yardBlueprintId( size_t uWeight_ )const noexcept{
       //-- 应该已经被 isNeed2Apply 过滤掉了
       tprAssert( !this->yardBlueprintIds.empty() );

       size_t idx = (uWeight_ + 17595441) % this->yardBlueprintIds.size();
       return this->yardBlueprintIds.at( idx );
   }
    
private:
    double  applyPercent {}; //- [0.0, 1.0]
    std::vector<blueprint::yardBlueprintId_t> yardBlueprintIds {};
};



#endif 


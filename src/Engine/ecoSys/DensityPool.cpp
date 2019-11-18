/*
 * ======================= DensityPool.cpp =======================
 *                          -- tpr --
 *                                        CREATE -- 2019.10.07
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#include "DensityPool.h"

//-------------------- C --------------------//
#include <cmath>

//-------------------- Engine --------------------//
#include "EcoSysPlan.h"
#include "tprCast.h"

#include "esrc_fieldDistributePlan.h"


//-------------------- Script --------------------//
#include "Script/resource/ssrc_all.h" 


#include "tprDebug.h"


void DensityPool::insert_goSpecData( BodySize bodySize_, std::unique_ptr<GoSpecData> uptr_ )noexcept{

    if( this->goSpecDatas.find(bodySize_) == this->goSpecDatas.end() ){
        this->goSpecDatas.insert({ bodySize_, std::vector<std::unique_ptr<GoSpecData>>{} });
    }
    auto &pool = this->goSpecDatas.at(bodySize_);
    //--
    pool.push_back( std::move(uptr_) );

    //goSpecId_t goSpecId = ssrc::str_2_goSpecId(ecoEnt_.goSpecName);
    //pool.insert( pool.begin(), ecoEnt_.idNum, GoSpecData{goSpecId, ecoEnt_.labels} ); // 不能这么写 
}



// param: randUVal_ [0, 9999]
const FieldDistributePlan &DensityPool::apply_a_fieldDistributePlan( size_t randUVal_ )const noexcept{
    size_t randIdx = randUVal_ + 1937173;
    auto type = this->types.at( randIdx % this->types.size() );
    auto &planSet = esrc::get_fieldDistributePlanSet();
    return planSet.apply_a_fieldDistributePlan(type, randIdx);
}




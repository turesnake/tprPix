/*
 * =================== FieldDistributePlan.cpp ==========================
 *                          -- tpr --
 *                                        创建 -- 2019.10.07
 *                                        修改 -- 
 * ----------------------------------------------------------
 */
#include "FieldDistributePlan.h"


#include "tprDebug.h"



void FieldDistributePlanSet::final_check()const noexcept{

    tprAssert( this->is_find_type( FieldDistributeType::Sml1 ) );
    tprAssert( this->is_find_type( FieldDistributeType::Sml2 ) );
    tprAssert( this->is_find_type( FieldDistributeType::Sml3 ) );
    tprAssert( this->is_find_type( FieldDistributeType::Mid1 ) );
    tprAssert( this->is_find_type( FieldDistributeType::Mid2 ) );
    tprAssert( this->is_find_type( FieldDistributeType::Big1 ) );
    tprAssert( this->is_find_type( FieldDistributeType::Sml1_Mid1 ) );
    tprAssert( this->is_find_type( FieldDistributeType::Sml2_Mid1 ) );

    cout << "subVectors.size(): " 
        << "\n      Sml1: "<< this->plans.at(FieldDistributeType::Sml1).size()
        << "\n      Sml2: "<< this->plans.at(FieldDistributeType::Sml2).size()
        << "\n      Sml3: "<< this->plans.at(FieldDistributeType::Sml3).size()
        << "\n      Mid1: "<< this->plans.at(FieldDistributeType::Mid1).size()
        << "\n      Mid2: "<< this->plans.at(FieldDistributeType::Mid2).size()
        << "\n      Big1: "<< this->plans.at(FieldDistributeType::Big1).size()
        << "\n      Sml1_Mid1: "<< this->plans.at(FieldDistributeType::Sml1_Mid1).size()
        << "\n      Sml2_Mid1: "<< this->plans.at(FieldDistributeType::Sml2_Mid1).size()
        << endl;




}









/*
 * =================== FieldDistributePlan.h ==========================
 *                          -- tpr --
 *                                        创建 -- 2019.10.07
 *                                        修改 -- 
 * ----------------------------------------------------------
 */
#ifndef TPR_FIELD_DISTRIBUTE_PLAN_H
#define TPR_FIELD_DISTRIBUTE_PLAN_H
//--- glm - 0.9.9.5 ---
#include "glm_no_warnings.h"

//-------------------- CPP --------------------//
#include <vector>
#include <utility>
#include <unordered_map>
#include <memory>

//-------------------- Engine --------------------//
#include "tprAssert.h"
#include "BodySize.h"
#include "FieldDistributeType.h"


class FieldDistributePlan{
public:
    FieldDistributePlan()=default;

    inline void push_back_point( BodySize bs_, const glm::dvec2 &dposOff_  )noexcept{
        this->points.push_back({ bs_, dposOff_ });
    }

    inline const std::vector<std::pair<BodySize, glm::dvec2>> &
    get_points()const noexcept{ return this->points; }
    

private:
    std::vector<std::pair<BodySize, glm::dvec2>> points {};
    
};




class FieldDistributePlanSet{
public:


    inline void perpare_targetType( FieldDistributeType type_ )noexcept{
        tprAssert( !this->is_find_type(type_) );
        this->plans.insert({ type_, std::vector<std::unique_ptr<FieldDistributePlan>>{} });
    }


    inline FieldDistributePlan &create_new_fieldDistributePlan( FieldDistributeType type_ )noexcept{
        tprAssert( this->is_find_type(type_) );
        auto &vec = this->plans.at(type_);
        //---
        auto uptr = std::make_unique<FieldDistributePlan>();
        vec.push_back( std::move(uptr) );
        return *(vec.back());
    }

    inline const FieldDistributePlan &apply_a_fieldDistributePlan( FieldDistributeType type_, size_t randUVal_ )const noexcept{
        //tprAssert( this->is_find_type(type_) );
        auto &container = this->plans.at(type_);
        size_t idx = (randUVal_*3+197507) % container.size();
        return *(container.at(idx));
    }

    void final_check()const noexcept;


private:    

    bool is_find_type( FieldDistributeType type_ )const noexcept{
        return (this->plans.find(type_) != this->plans.end());
    }

    std::unordered_map< FieldDistributeType, std::vector<std::unique_ptr<FieldDistributePlan>> > plans {};
};






#endif 


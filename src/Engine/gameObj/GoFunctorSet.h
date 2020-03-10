/*
 * ====================== GoFunctorSet.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2020.02.12
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#ifndef TPR_GO_FUNCTOR_SET_H
#define TPR_GO_FUNCTOR_SET_H


//-------------------- CPP --------------------//
#include <unordered_map>
#include <functional>


//-------------------- Engine --------------------//
#include "tprAssert.h"
#include "DyFunctor.h"
#include "GoFunctorLabel.h"


class GoFunctorSet{
public:
    GoFunctorSet()=default;

    template<typename T>
    inline void insert( GoFunctorLabel label_, T functor_ )noexcept{
        auto [insertIt, insertBool] = this->functors.emplace( label_, DyFunctor{} );
        tprAssert( insertBool );
        DyFunctor &f = insertIt->second;
        //---
        f.init<T>( functor_ );
    }

    // 返回值是 std::funtion<> 的 ref
    template<typename T>
    inline T &get( GoFunctorLabel label_ )noexcept{
        tprAssert( this->functors.find(label_) != this->functors.end() );
        DyFunctor &f = this->functors.at(label_);
        //---
        return *(f.get<T>());
    }

private:
    std::unordered_map<GoFunctorLabel, DyFunctor> functors {};
};



#endif


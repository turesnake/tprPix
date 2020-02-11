/*
 * ======================= DyFunctor.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2020.02.12
 *                                        MODIFY --
 * ----------------------------------------------------------
 */
#ifndef TPR_DY_FUNCTOR_H
#define TPR_DY_FUNCTOR_H

//-------------------- CPP --------------------//
#include <any>

//-------------------- Engine --------------------//
#include "tprAssert.h"

// 通用万能 函数对象
class DyFunctor{
public:
    DyFunctor()=default;

    template<typename T>
    inline T *init( T functor_ )noexcept{
        this->data = std::make_any<T>( functor_ );
        return std::any_cast<T>( &(this->data) );
    }

    template<typename T>
    inline T *get()noexcept{
        tprAssert( this->data.has_value() );
        tprAssert( this->data.type().hash_code() == typeid(T).hash_code() );
        return std::any_cast<T>( &(this->data) );
    }

private:
    std::any data; // 存储一枚 functor，允许复制
};  

#endif 


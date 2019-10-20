/*
 * ======================= DyBinary.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.10.18
 *                                        MODIFY --
 * ----------------------------------------------------------
 */
#ifndef TPR_DY_BINARY_H
#define TPR_DY_BINARY_H

//-------------------- CPP --------------------//
#include <any>

//-------------------- Engine --------------------//
#include "tprAssert.h"


class DyBinary{
public:
    DyBinary()=default;

    template<typename T>
    inline T *init()noexcept{
        this->data = std::make_any<T>();
        return std::any_cast<T>( &(this->data) );
    }

    template<typename T>
    inline T *get()noexcept{
        tprAssert( this->data.has_value() );
        tprAssert( this->data.type().hash_code() == typeid(T).hash_code() );
        return std::any_cast<T>( &(this->data) );
    }

private:
    std::any data;
};

#endif 


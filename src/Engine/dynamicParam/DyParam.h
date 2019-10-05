/*
 * ========================= DyParam.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.08.25
 *                                        MODIFY --
 * ----------------------------------------------------------
 *  fast dynamic param
 */
#ifndef TPR_DY_PARAM_H
#define TPR_DY_PARAM_H

//-------------------- CPP --------------------//
#include <vector>
//-------------------- Engine --------------------//
#include "tprAssert.h"


//-- only used by create_gos_in_field()
//   the paramData's lifetime is managed by caller
//   DyParam instance only transfer the data-ptr
class DyParam{
public:
    DyParam() = default;

    template< typename T >
    inline void insert_ptr( T *ptr_ )noexcept{
        this->typeHash = typeid(T).hash_code();
        this->ptr = static_cast<void*>(ptr_);
    }

    template< typename T >
    inline const T*get_binaryPtr()const noexcept{
        tprAssert( this->ptr != nullptr );
        tprAssert( typeid(T).hash_code() == this->typeHash );
        return static_cast<T*>( this->ptr );
    }

    inline size_t   get_typeHash()const noexcept{ return this->typeHash; }
    inline bool     is_Nil()const noexcept{ return (this->ptr == nullptr); }
private:
    void    *ptr     {nullptr};
    size_t  typeHash {0};
};

extern DyParam emptyDyParam;

#endif 


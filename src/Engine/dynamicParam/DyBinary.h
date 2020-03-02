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

// 通用万能数据包
class DyBinary{
public:

    //========== Self ==========//
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

    template<typename T>
    inline const T *get()const noexcept{
        tprAssert( this->data.has_value() );
        tprAssert( this->data.type().hash_code() == typeid(T).hash_code() );
        return std::any_cast<T>( &(this->data) );
    }

private:
    std::any data;
            // 尽管这里的 data 并不会被复制，但 std::any 不支持 move_only type class
            // 所以，T 中禁止出现 unique_ptr (嵌套)
            // 解法有二：
            // -1- 将 unique_ptr 替换为 shadred_ptr
            // -2- 自定义 T 的 构造函数 / 移动构造函数 / 移动赋值运算符:
            //   T(){...}
            //   T( const T & ){ tprAssert(0); }
            //   T &operator=( const T & );  // 定义需要在 class 外部
};  

#endif 


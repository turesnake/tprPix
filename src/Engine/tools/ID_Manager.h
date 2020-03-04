/*
 * ========================= ID_Manager.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2018.12.10
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#ifndef TPR_ID_MANAGER_H
#define TPR_ID_MANAGER_H

//------------------- CPP --------------------//
#include <cstdint> // uint8_t

//------------------- Engine --------------------//
#include "tprAssert.h"


//-- id 默认从 1 开始增长。0号id 被保留，表示空id --
//extern const int NULLID;
#define NULLID 0
        //-- int 类型有麻烦...


//-- id类型（取值范围）--
enum class ID_TYPE : uint8_t {
    U8  = 1, //- [1,255]
    U16 = 2, //- [1,65535]
    U32 = 3, //- [1,4294967295]
    U64 = 4  //- [1, infinity...]
};

//----- id 管理器 -----
// 通常作为 其他类的 静态成员 存在。
// 0 号 id 一定不会被分配到
class ID_Manager{
public:
    ID_Manager( ID_TYPE id_type_=ID_TYPE::U64,
                uint64_t   max_id_=0 ):
        id_scope(id_type_),
        max_id(max_id_)
        {
            switch( this->id_scope ){
                case ID_TYPE::U8:  this->id_limit = static_cast<uint8_t>(-1); break;
                case ID_TYPE::U16: this->id_limit = static_cast<uint16_t>(-1); break;
                case ID_TYPE::U32: this->id_limit = static_cast<uint32_t>(-1); break;
                case ID_TYPE::U64: this->id_limit = static_cast<uint64_t>(-1); break; //- 用不到
                default:
                    tprAssert(0); //- 不会运行到此行
            }
        }

    inline uint8_t apply_a_u8_id() noexcept {
        
        tprAssert( this->id_scope == ID_TYPE::U8 );
        this->max_id++;
        tprAssert( this->max_id <= this->id_limit );//-- 2^8 --
        return static_cast<uint8_t>(this->max_id);
    }

    inline uint16_t apply_a_u16_id() noexcept {
        
        tprAssert( this->id_scope == ID_TYPE::U16 );
        this->max_id++;
        tprAssert( this->max_id <= this->id_limit );//-- 2^16 --
        return static_cast<uint16_t>(this->max_id);
    }

    inline uint32_t apply_a_u32_id() noexcept {
        
        tprAssert( this->id_scope == ID_TYPE::U32 );
        this->max_id++;
        tprAssert( this->max_id <= this->id_limit );//-- 2^32 --
        return static_cast<uint32_t>(this->max_id);
    }

    inline uint64_t apply_a_u64_id() noexcept {
        
        tprAssert( this->id_scope == ID_TYPE::U64 );
        this->max_id++;
        //-- 2^64 个id，永远也用不完。
        return this->max_id;
    }

    inline void set_max_id( uint64_t max_id_ ) noexcept {
        this->max_id = max_id_;
    }

    inline uint64_t get_max_id() const noexcept {
        return this->max_id;
    }

private:
    ID_TYPE     id_scope;  //- id类型
    uint64_t    max_id;    //- 当前 使用的 数值最大的 id号
    uint64_t    id_limit;  //- id号 上限
};

#endif


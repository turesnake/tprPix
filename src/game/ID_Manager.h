/*
 * ========================= ID_Manager.h ==========================
 *                          -- tpr --
 *                                        创建 -- 2018.12.10
 *                                        修改 -- 2018.12.10
 * ----------------------------------------------------------
 *   通用的 id 管理器
 * 
 * ----------------------------
 */
#ifndef _TPR_ID_MANAGER_H_
#define _TPR_ID_MANAGER_H_

//-------------------- C ----------------------//
#include <cassert> //-- assert

//------------------- Libs --------------------//
#include "tprDataType.h" 


//-- id 默认从 1 开始增长。0号id 被保留，表示空id --
inline constexpr int NULLID = 0;


//-- id类型（取值范围）--
enum class ID_TYPE : u8 {
    U8  = 1, //- [1,255]
    U16 = 2, //- [1,65535]
    U32 = 3, //- [1,4294967295]
    U64 = 4  //- [1, infinity...]
};

//----- id 管理器 -----
// 通常作为 其他类的 静态成员 存在。
// 0 号 id 就是空id
class ID_Manager{
public:
    explicit ID_Manager(ID_TYPE _id_type=ID_TYPE::U64,
                        u64     _max_id=1 ):
        id_scope(_id_type),
        max_id(_max_id)
        {
            switch( id_scope ){
                case ID_TYPE::U8:  id_limit = (u8)(-1); break;
                case ID_TYPE::U16: id_limit = (u16)(-1); break;
                case ID_TYPE::U32: id_limit = (u32)(-1); break;
                case ID_TYPE::U64: id_limit = (u64)(-1); break; //- 用不到
                default:
                    assert(0); //- 不会运行到此行
            }
        }

    //-- 申请分配一个 新 u8 id --
    inline u8 apply_a_u8_id(){
        
        assert( id_scope == ID_TYPE::U8 );
        max_id++;
        assert( max_id <= id_limit );//-- 2^8 --
        return (u8)max_id;
    }

    //-- 申请分配一个 新 u16 id --
    inline u16 apply_a_u16_id(){
        
        assert( id_scope == ID_TYPE::U16 );
        max_id++;
        assert( max_id <= id_limit );//-- 2^16 --
        return (u16)max_id;
    }

    //-- 申请分配一个 新 u32 id --
    inline u32 apply_a_u32_id(){
        
        assert( id_scope == ID_TYPE::U32 );
        max_id++;
        assert( max_id <= id_limit );//-- 2^32 --
        return (u32)max_id;
    }

    //-- 申请分配一个 新 u64 id --
    inline u64 apply_a_u64_id(){
        
        assert( id_scope == ID_TYPE::U64 );
        max_id++;
        //-- 2^64 个id，永远也用不完。
        return max_id;
    }

private:
    ID_TYPE id_scope; //- id类型
    u64     max_id;      //- 当前 使用的 数值最大的 id号
    u64     id_limit;  //- id号 上限
};

#endif


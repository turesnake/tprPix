/*
 * ========================= ScriptBuf.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.01.01
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *    Engine-Script:  params/retVal  buffer
 *  -----
 * 
 * 
 * 
 *                      目前，此装置未被使用.....
 * 
 * 
 * 
 * 
 * ----------------------------
 */
#ifndef TPR_SCRIPT_BUF_H
#define TPR_SCRIPT_BUF_H

//-------------------- C --------------------//
#include <cstring> //- memcpy

//-------------------- CPP --------------------//
#include <vector> 
#include <string>

//------------------- Engine --------------------//
#include "tprAssert.h"


//--- 一个更加强大，专业的 引擎-脚本 参数／返回值 传递工具 ---
//  每种类型拥有 独立的容器 和 状态flag检测
//  允许向 同一个容器 多次 push， 而不做任何 pop 操作
//  但是如果一个 容器未被 push，就调用 pop 操作 将报错
//  pop 只能执行一次。 之后容器中的数据就被 “销毁”
class ScriptBuf{
public:
    //============== bool ==============//
    inline void push_bool( bool b_ )noexcept{
        boolval = b_;
        is_boolval_push = true;
    }

    inline bool pop_bool()noexcept{
        tprAssert( is_boolval_push == true );
        is_boolval_push = false;
        return boolval;
    }

    //============== int ==============//
    inline void push_int( int i_ )noexcept{
        i32val = i_;
        is_i32val_push = true;
    }

    inline int pop_int()noexcept{
        tprAssert( is_i32val_push == true );
        is_i32val_push = false;
        return i32val;
    }

    //============== uint64_t ==============//
    inline void push_u64( uint64_t u_ )noexcept{
        u64val = u_;
        is_u64val_push = true;
    }

    inline uint64_t pop_u64()noexcept{
        tprAssert( is_u64val_push == true );
        is_u64val_push = false;
        return u64val;
    }

    //============== string ==============//
    inline void push_str( std::string str_ )noexcept{
        str = str_;
        is_str_push = true;
    }

    inline std::string pop_str()noexcept{
        tprAssert( is_str_push == true );
        is_str_push = false;
        return str; //- copy
    }

    //============== binary ==============//
    inline void push_binary( void *buf_, int len_ )noexcept{
        binary.resize(len_);
        memcpy( (void*)&(binary.at(0)),
                buf_,
                (size_t)len_ );
        is_binary_push = true;
    }

    inline void pop_binary( void *buf_, int len_ )noexcept{
        //- 非常严格，buf长度不匹配也将 报错 
        tprAssert( is_binary_push == true );
        is_binary_push = false;
        tprAssert( binary.size() == len_ );
        memcpy( buf_,
                (void*)&(binary.at(0)),
                (size_t)len_ );
        binary.clear();
    }
    
private:
    //------ val container ---
    bool   boolval  {};
    int    i32val   {};
    uint64_t  u64val   {};
    std::vector<uint8_t> binary {};
    std::string       str    {};

    //======== flags ========//
    bool   is_boolval_push {false};
    bool   is_i32val_push  {false};
    bool   is_u64val_push  {false};
    bool   is_str_push     {false};
    bool   is_binary_push  {false};
};


//-- 本类唯一的实例，全局 ----
inline  ScriptBuf  scriptBuf {};



#endif


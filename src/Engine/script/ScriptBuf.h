/*
 * ========================= ScriptBuf.h ==========================
 *                          -- tpr --
 *                                        创建 -- 2019.01.01
 *                                        修改 -- 2019.01.01
 * ----------------------------------------------------------
 *    Engine-Script:  params/retVal  buffer
 * ----------------------------
 */
#ifndef _TPR_SCRIPT_BUF_H_
#define _TPR_SCRIPT_BUF_H_

//-------------------- C --------------------//
#include <string.h> //- memcpy
#include <cassert> //- assert

//-------------------- CPP --------------------//
#include <vector> 
#include <string>

//------------------- Libs --------------------//
#include "tprDataType.h" 


//--- 一个更加强大，专业的 引擎-脚本 参数／返回值 传递工具 ---
//  每种类型拥有 独立的容器 和 状态flag检测
//  允许向 同一个容器 多次 push， 而不做任何 pop 操作
//  但是如果一个 容器未被 push，就调用 pop 操作 将报错
//  pop 只能执行一次。 之后容器中的数据就被 “销毁”
class ScriptBuf{
public:
    //============== bool ==============//
    inline void push_bool( bool _b ){
        boolval = _b;
        is_boolval_push = true;
    }

    inline bool pop_bool(){
        assert( is_boolval_push == true );
        is_boolval_push = false;
        return boolval;
    }

    //============== int ==============//
    inline void push_int( int _i ){
        i32val = _i;
        is_i32val_push = true;
    }

    inline int pop_int(){
        assert( is_i32val_push == true );
        is_i32val_push = false;
        return i32val;
    }

    //============== u64 ==============//
    inline void push_u64( u64 _u ){
        u64val = _u;
        is_u64val_push = true;
    }

    inline u64 pop_u64(){
        assert( is_u64val_push == true );
        is_u64val_push = false;
        return u64val;
    }

    //============== string ==============//
    inline void push_str( std::string _str ){
        str = _str;
        is_str_push = true;
    }

    inline std::string pop_str(){
        assert( is_str_push == true );
        is_str_push = false;
        return str; //- copy
    }

    //============== binary ==============//
    inline void push_binary( void *_buf, int _len ){
        binary.resize(_len);
        memcpy( (void*)&(binary[0]),
                _buf,
                (size_t)_len );
        is_binary_push = true;
    }

    inline void pop_binary( void *_buf, int _len ){
        //- 非常严格，buf长度不匹配也将 报错 
        assert( is_binary_push == true );
        is_binary_push = false;
        assert( binary.size() == _len );
        memcpy( _buf,
                (void*)&(binary[0]),
                (size_t)_len );
        binary.clear();
    }
    
private:
    //------ val container ---
    bool   boolval {};
    int    i32val  {};
    u64    u64val  {};
    std::vector<u8> binary;
    std::string     str;

    //-------- flag -------
    bool   is_boolval_push {false};
    bool   is_i32val_push  {false};
    bool   is_u64val_push  {false};
    bool   is_str_push  {false};
    bool   is_binary_push  {false};
};


//-- 本类唯一的实例，全局 ----
inline  ScriptBuf  scriptBuf {};



#endif


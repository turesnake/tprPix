/*
 * ========================= RcyPool.h ==========================
 *                          -- tpr --
 *                                        创建 -- 2018.11.04
 *                                        修改 -- 2018.11.04
 * ----------------------------------------------------------
 * 
 *   一个 文件地址偏移 回收池，可分别支持 定长数据 和 变长数据 的回收
 * 
 * 
 * ----------------------------
 * 依赖的其它库：
 *       
 * ----------------------------
 */
#ifndef _TPR_DB_RCYPOOL_H_
#define _TPR_DB_RCYPOOL_H_

//------------ CPP -------------//
#include <string>
//#include <iostream> 
#include <vector> 
#include <deque> 

//------------ SELF -------------//
#include "tprDB_type.h" 
 

namespace tpr { //--------------- namespace: tpr -------------------//
namespace DB {//--------------- namespace: DB -------------------//

class RcyPool{

public:
    //---- 构造函数 ----//
    explicit RcyPool()
        :
        pool_limit( 100 *_TPR_MB_ )
        {}

    //---- init -----//
    void init( bool _is_fixed, len_t _fix_blk_bytes );

    //------ FIX / VAR 兼容 -------//
    bool is_empty( len_t _blk_bytes );
    void push( base_t _base, len_t _blk_bytes );
    base_t pop( len_t _blk_bytes );

    //------------ info ----------------//
    void info_var();
    void info_fix();

private:
    //++++++++++++++++| var rcyPool  |++++++++++++++++//
    //-- 回收池中 只存放 各空置 block 的文件 地址偏移
    //-- 共有 15 个 var 回收池:
    //  字节数      idx
    //-- 0      --- 0 闲置 仅用于映射对称 --- 
    //-- 2      --- 1 闲置 仅用于映射对称 ---
    //-- 4      --- 2 闲置 仅用于映射对称 ---
    //-- 8      --- 3 闲置 仅用于映射对称 ---
    //-- 16        -4- ...特殊，此时，strData 为空，只有 32-bytes 的头部...
    //-- 32        -5- 
    //-- 64        -6-
    //-- 128       -7-
    //-- 256       -8-
    //-- 512       -9-
    //-- 1024     -10-  只在 单条 str长度 超过 1KB时，才启动 链表式存储
    //-- 2048     -11-
    //-- 4096     -12-
    //-- 8192     -13-
    //-- 16384    -14-
    std::vector< std::deque<base_t> > varPools;

    //++++++++++++++++| fixed rcyPool  |++++++++++++++++//
    //-- 定长数据 回收池 只有一个。
    std::deque<base_t>                fixPool; 

    len_t fix_blk_bytes {0}; //-- fix pool 中，一个 blk 的字节数

    u64   all_pool_bytes {0}; //-- 回收池中，所有 空置block 的 字节数总和
    u64   pool_limit     {0}; //-- 回收池 字节数 上限，当超出此值，将 触发 文件整理
    bool  is_pool_fixed; //-- true  --> 定长数据，blk 采用 4字节对齐
                         //-- false --> 变长数据，blk 采用 2^n 对齐
};


}//------------------- namespace: DB ------------------------//
}//------------------- namespace: tpr ------------------------//
#endif

/*
 * ====================== tprMemAllor.h =======================
 *                          -- tpr --
 *                                        创建 -- 2018.11.24
 *                                        修改 -- 2018.11.24
 * ----------------------------------------------------------
 *    简易的 内存分配器
 *    --------
 *    暂时只适合 定长数据集 的分配
 *    不支持 多线程
 * 
 * 
 *    -------
 *    id分配方式：
 *    -- 有限id制
 *         id号就是 ent 在 mem 中的存储序号
 *         优点是，id存储空间小。缺点是，废旧id会被回收，容易被串用
 *    -- 无限id制
 *         id 为一个 u64数据，意味着 几乎永远不会被分配完
 *         内存池不容易遭污染。但，id 需要较大存储空间
 *    id 号从 1 分配起，永不为0
 * 
 * ----------------------------
 */
#ifndef _TPR_MEM_ALLOR_H_
#define _TPR_MEM_ALLOR_H_

//------------------- Libs --------------------//
#include "tprDataType.h" 


//-------------------- CPP --------------------//
#include <string>
#include <vector>
#include <unordered_map>
#include <deque>

namespace tpr { //-------------- namespace : tpr ------------//
namespace MemAllor{ //------- namespace : MemAllor ----------//

using eid_t = u32_t; //- 内存池ent id。 个数上限就是 ent_nums 个
using len_t = u32_t;
using ptr_t = void*;


struct MemEnt{
    eid_t   id  {0};
    ptr_t   ptr {0}; //- 指向 ent 存储地址
};


}//-------------------- namespace : MemAllor end ------------//


class tprMemAllor{

public:
    explicit tprMemAllor( MemAllor::len_t _ent_bytes, 
                        MemAllor::len_t _ent_nums,
                        bool _is_limit_id_mod=true )
        :
        ent_bytes(_ent_bytes),
        ent_nums(_ent_nums),
        is_limit_id_mod(_is_limit_id_mod)
        {}

    void init();

    // ---< 增 >---
    MemAllor::eid_t insert( const void *_buf, MemAllor::len_t _len );
    // ---< 删 >---
    int erase( MemAllor::eid_t _id );
    // ---< 查 >---
    ssize_t read( MemAllor::eid_t _id, void *_buf, size_t _len );
    // ---< 改 >---
    ssize_t write( MemAllor::eid_t _id, const void *_buf, MemAllor::len_t _len );

    //-----------------//
    bool check_id( MemAllor::eid_t _id ); //-- 检测 目标_id 是否在 内存池中

    void info(); //- 内存池状态打印

private:
    std::vector<u8_t> mem; //-- 真正分配到的 内存空间

    std::unordered_map< MemAllor::eid_t, MemAllor::ptr_t > busy_hash; //- 在用池
    std::deque< MemAllor::MemEnt >                         idle_pool; //- 未用池（回收池）
                                                                      //  尾进头出

    MemAllor::len_t  ent_bytes; //- 单个 ent 字节数
    MemAllor::eid_t  ent_nums;  //- ent 个数

    bool is_limit_id_mod; //- true  -- 有限 id制 
                          //- false -- 无限 id制

    MemAllor::ptr_t busy_hash_find( MemAllor::eid_t _id ); 
};


} //-------------- namespace : tpr end --------------//
#endif


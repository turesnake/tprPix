/*
 * ========================= tprDB_VAR.h ===================
 *                          -- tpr --
 *                                        创建 -- 2018.11.08
 *                                        修改 -- 2018.11.08
 * ----------------------------------------------------------
 * 
 *   基类，被 tprDB_VAR, tprDB_FIX 继承
 * 
 * ----------------------------
 * 依赖的其它库：
 *       wrapFuncs 
 *       tprFileSys
 *       tprFileModeT
 *       tprCrypto
 *       tprDataType
 *       tprBinary (临时)
 * ----------------------------
 */
#ifndef _TPR_TPRDB_ORIG_H_
#define _TPR_TPRDB_ORIG_H_

//------------ CPP -------------//
#include <string>
#include <iostream> 
#include <unordered_map>
#include <list>

//------------ libs -------------//
#include "wrapFuncs.h"
#include "tprFileSys.h"
#include "tprCrypto.h" 
#include "tprDataType.h" 
#include "tprBinary.h"  //- 临时
#include "nocopyable.h"

//------------ SELF -------------//
#include "tprDB_type.h" 
#include "RcyPool.h" 

namespace tpr { //--------------- namespace: tpr -------------------//

class tprDB_Orig : nocopyable {
    //============== friend ==============
public:

    //---- 构造函数 ----//
    explicit tprDB_Orig( bool _is_fix )
        :
        is_fix( _is_fix ),
        cache_limit( 100 * _TPR_MB_ )
        {}

    //---- 析构函数 ----//    *** 需要析构函数的类，也需要 拷贝构造函数 和 拷贝赋值运算符 *** --
    ~tprDB_Orig(){
        flush();
    }

    //----- init ------//
    void init( int _fd, const std::string &_path, DB::len_t _len=0 );

    // -< 增 >-
    DB::eid_t insert( const void *_buf, DB::len_t _len ); //-- 添加一条 数据，返回为其分配的id
    // -< 删 >-
    int erase( DB::eid_t _id ); //-- 彻底删除 一条 varEnt
    // -< 查 >-
    ssize_t read( DB::eid_t _id, void *_buf, size_t _len, bool _is_fast=false ); //- 读取一条entData
    // -< 改 >-
    ssize_t write( DB::eid_t _id, const void *_buf, const DB::len_t _len ); //- 写入一条entData
    // -< 冲 >-
    void flush(); //-- 将所有 dirty entry 写回文件中。只能在派生类中调用

    
    //-----------------//
    bool check_id( DB::eid_t _id ); //-- 检测 目标_id 是否在 数据库中
    ssize_t get_entData_len( DB::eid_t _id );

    void info(); //-- 打印数据库信息


private:
//---- 不可被类外部访问，被派生类访问 ---

    bool is_fix; //-- FIX --> true 
                 //-- VAR --> false

    //++++++++++++++++|  FILE  |++++++++++++++++//
    std::string path_file {};  //-- 文件 路径名 -- 通常由 tprDB 提供  
    int         fd_file   {};  //-- 文件 fd    -- 通常由 tprDB 提供

    DB::file_FixOptions fOpt;    //- FIX 特有，文件头部后方的 opt段
    DB::len_t  fix_blk_bytes {}; //- FIX 特有，blk 字节数

    DB::base_t  data_base {};   //-- 文件中，数据段首地址。
                        //- VAR， 头部 尾后
                        //- FIX， 头部+opt段 尾后
    //------------------
    void _1_read_file();  //- 从文件中读取所有 Ent 到 mem


    //++++++++++++++++|  MEM  |++++++++++++++++//
    std::unordered_map<DB::eid_t, DB::mem_Ent> mem_hash; //-- 每条ent 的 综合信息。

    DB::eid_t     max_id    {}; //-- 当前数据库实例中，最大的id值 (删掉的也算。一个id永远不会被使用两次)
    DB::base_t   next_base {}; //-- 当前数据库实例中，文件中最后一个 ent 所在 blk 的 尾后字节地址
                         //  （可能远大于文件尾后字节...）
    
    u64      all_data_bytes {}; //-- 所有 entData 字节数 总和。（不在 mem 中的也算）
    u64      all_blk_bytes  {}; //-- 所有 Ent 的 block 字节数 总和 (空置blk 不算)

    //------------------
    DB::mem_Ent *mem_hash_find( DB::eid_t _id ); //-- 从 mem_hash 中获得一个元素的迭代器

    void refresh_max_id( DB::eid_t _id );  //-- 输入新 entry 的 id，来更新 max_id
    void refresh_next_base( DB::base_t _base, DB::len_t _blk_bytes );
                //-- 输入新 entry 的 base, blk_bytes，来更新 next_base
    DB::eid_t apply_a_id();            //-- 申请一个id，
    void free_a_id( DB::eid_t _id ){}  //-- 释放一个id （其实什么也不做）

    bool is_last_mEnt( DB::base_t _base, DB::len_t _blk_bytes ); //-- 判断自己是不是文件中排最后的Ent

    void load_a_entData( DB::mem_Ent *_mEntp ); //- 从文件读取一条 entData 到 mem --
    void save_a_entData(DB::eid_t _id); //- 将 mem 中的一个 entData，存储到 文件中 --


    void writeback_file_enthead( const DB::mem_Ent &_ent,
                                const std::string &_err_info);
    
    void writeback_file_empty_enthead( DB::base_t _base,
                                        DB::len_t _blk_bytes,
                                    const std::string &_err_info);


    //++++++++++++++++|  rcyPool  |++++++++++++++++//
    DB::RcyPool rcy_pools {};
    //------------------
    void divide_the_block( DB::mem_Block *_mb, DB::len_t _blk_bytes ); 
                        //-- 递归式地 将一个 大的 blk，拆封为数个 小的block。
                        //-- 拆分到最后，目标blk 的尺寸 将和  _blk_bytes 一样大，然后结束递归
                        //-- 中途被拆分出来的 各个block，都将 独立成 新空置blk，
                        //-- 并被 存入 文件 和 rcyPools 中。


    //++++++++++++++++|  cache  |++++++++++++++++//
    //-- varEnt 缓存区。
    //-- 变长数据本体 记录在 mem_hash entdata 中。
    //-- 本 缓存区 用来管理这些 字符串的 载入／释放／写回...
    std::list<DB::eid_t>                                      cache_history; 
                        //-- 缓存区全体 ent 的id 都存储在此，
                        //-- 以 访问时间来排列，越旧越没人访问的 ent 排在队首，越新的排在 队尾
    std::unordered_map<DB::eid_t, std::list<DB::eid_t>::iterator>  cache_history_iters;
                        //-- 存储所有 迭代器，用来快速访问 cache_history

    u64   all_cache_bytes {}; //-- cache中，所有entData 的 字节数总和
    u64   cache_limit;        //-- cache 字节数 上限。 当超出此值，将释放 cache 的旧元素
    //------------------
    void cache_push( DB::eid_t _id, DB::len_t _len ); 
    void cache_release( u64 _size );
    int del_cacheEnt( DB::eid_t _id ); //-- 彻底删除 一条 ent，不管它是否存在。erase函数中 调用


};


}//------------------- namespace: tpr ------------------------//
#endif


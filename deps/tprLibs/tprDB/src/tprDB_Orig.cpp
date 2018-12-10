/*
 * ==================== tprDB_Orig.cpp ========================
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
#include "tprDB_Orig.h"

//------------ C -------------//
#include <string.h> //- memcpy
#include <cassert> //- assert
#include <errno.h> //- errno

//------------ CPP -------------//
#include <sstream> //- stringstream

using std::cout;
using std::endl;
using std::string;

namespace tpr { //--------------- namespace: tpr -------------------//

using DB::BlockState;

using DB::BlkState_2_idx;
using DB::idx_2_BlkState;

using DB::varEntHead_f2m;
using DB::varEntHead_m2f;
using DB::fixEntHead_f2m;
using DB::fixEntHead_m2f;

using DB::mem_Block;
using DB::mem_Ent;

using DB::find_suited_Blk_bytes;

using DB::aligns_2_Blk_bytes;

using DB::file_EntHead;
using DB::file_FixOptions;

using DB::eid_t;
using DB::len_t;
using DB::base_t;
using DB::FILE_ENT_HEAD_LEN;
using DB::FILE_FIXOPTION_LEN;

using DB::tprDB_File_Head;

using DB::mk_options;



/* ===========================================================
 *                         init
 * -----------------------------------------------------------
 * -- FIX / VAR 通用
 */
void tprDB_Orig::init( int _fd, const std::string &_path, DB::len_t _len ){

    string err_info = "tprDB_Orig::init(): ";
    //-----------------------//
    fd_file   = _fd;
    path_file = _path;

    if( is_fix == true ){
        //---------------------//
        //         FIX
        //---------------------//
        fOpt = mk_options( _len );
        fix_blk_bytes = aligns_2_Blk_bytes( fOpt.blk_aligns );
        data_base = sizeof(tprDB_File_Head) + FILE_FIXOPTION_LEN;
        next_base = data_base;

        //-- 从文件读取 opt 段 ---//
        //     并检测其 正确性
        Lseek( fd_file, (off_t)sizeof(tprDB_File_Head), SEEK_SET, err_info );

        file_FixOptions opt; //- tmp
        ssize_t rlen;
        rlen = Read(fd_file, (void*)&opt, FILE_FIXOPTION_LEN, err_info);
        assert( rlen == FILE_FIXOPTION_LEN );

        //-- 确保 文件的 opt段 是正确的 ---//
        assert( opt.ent_len == fOpt.ent_len );
        assert( opt.blk_aligns == fOpt.blk_aligns );

        //--- 初始化 rcy_pools ---//
        assert( (fOpt.blk_aligns * 4) < 65536 );
        rcy_pools.init( true, (fOpt.blk_aligns * 4) );

    }else{
        //---------------------//
        //         VAR
        //---------------------//
        data_base = sizeof(tprDB_File_Head);
        next_base = data_base;

        //--- 初始化 rcy_pools ---//
        rcy_pools.init(  false, 0 );
    }

    //--- 正式读取 var文件数据 ---//
    _1_read_file();
}

/* ===========================================================
 *                   ent_nums
 * -----------------------------------------------------------
 */
size_t tprDB_Orig::ent_nums(){

    return (size_t)mem_hash.size();
}

/* ===========================================================
 *                   get_fst_id
 * -----------------------------------------------------------
 */
DB::eid_t tprDB_Orig::get_fst_id(){

    return mem_hash.begin()->first;
}

/* ===========================================================
 *                      insert   [1]
 * -----------------------------------------------------------
 * -- 添加一条 entry
 * 
 */
DB::eid_t tprDB_Orig::insert( const void *_buf, DB::len_t _len ){

    //----- 申请 分配一个 新id -----//
    eid_t id;
    id = apply_a_id();

    //------- 制作 memblk ------//
    mem_Block memblk;

    size_t bsize = FILE_ENT_HEAD_LEN + _len;
    assert( bsize < 65536 );

    if( is_fix == true ){
        //---- FIX ----//
        assert( _len == fOpt.ent_len );
        memblk.blk_bytes = fix_blk_bytes;
    }else{
        //---- VAR ----//
        memblk.blk_bytes = find_suited_Blk_bytes( (len_t)bsize );
    }

    memblk.blk_state_idx = BlkState_2_idx( BlockState::Busy );
    //------- 确定 memblk.base ------//
    if( rcy_pools.is_empty(memblk.blk_bytes) == false ){
        //-- 从缓冲池 分配 --//
        memblk.base = rcy_pools.pop( memblk.blk_bytes );

    }else{
        //-- 目标缓冲池 为空，从 文件尾分配--//
        memblk.base = next_base;
    }
    //------- 更新 max_id, next_base ------//
    refresh_max_id( id );
    refresh_next_base( memblk.base, memblk.blk_bytes );

    //----- 制作一个 mem_Ent -----//
    mem_Ent  mEnt;

    mEnt.id       = id;
    mEnt.len      = _len;
    mEnt.rd_times = 0;
    mEnt.wr_times = 0;
    mEnt.memblk   = memblk;
    mEnt.is_data_in_mem = true;
    mEnt.is_dirty      = true;

    if( _len > 0 ){
        mEnt.data.resize( _len ); //-- 复制前扩容，关键 --
        memcpy( (void*)&(mEnt.data[0]),
                (void*)_buf,
                _len
                );
    }
    //------ 写入 mem 容器 ------//
    mem_hash.insert( {id, mEnt} );

    //--- 累计 all_data_bytes ---//
    all_data_bytes += mEnt.len;

    //--- 累计 all_blk_bytes ---//
    all_blk_bytes += memblk.blk_bytes;

    //------ 写入 cache 容器 ----//
    cache_push( id, mEnt.len );

    //----------------
    return id;
}

/* ===========================================================
 *                       erase
 * -----------------------------------------------------------
 * -- 彻底删除 一条 Ent
 * -- 作为对外接口，并不会轻易终止程序，而只是报错给 用户
 * -- return:
 *     若运行正常，返回 0
 * 
 *     若出错，返回 -1， 并会设置 相应 errno:
 *         ENOENT -- 参数 _id 在数据库中不存在
 *         ...
 *     
 *     若在 处理中途，发现某容器中没有 _id, 并不立即报错终止。
 *         而是继续执行删除操作。
 *         这部分 处理 暂未实现。
 */
int tprDB_Orig::erase( DB::eid_t _id ){

    string err_info = "tprDB_Orig::erase(): ";
    //------ 确保参数 _id 有效 ------//
    if( check_id(_id) == false ){
        errno = ENOENT;
        return -1; //- 立即返回
    }
    //------------------------------//
    //        获得 mEntp
    //------------------------------//
    mem_Ent   *mEntp;
    mEntp = mem_hash_find( _id ); 

    int err_no; //-- inner errno。 

    //------------------------------//
    //    本ent 是否是 文件中最后一个 ent
    //------------------------------//
    bool is_last_ent = false;
    if( is_last_mEnt(mEntp->memblk.base,
                    mEntp->memblk.blk_bytes) ){
        is_last_ent = true;
    }
    //------------------------------//
    //  不管 本ent 是不是 文件的最后一个 ent
    //   都会向文件 写入新的 空置blk头部
    //------------------------------//
    writeback_file_empty_enthead( mEntp->memblk.base,
                                mEntp->memblk.blk_bytes,
                                err_info );
    //------------------------------//
    //    将这块 blk，送入 rcy_pools
    //------------------------------//
    rcy_pools.push( mEntp->memblk.base, 
                    mEntp->memblk.blk_bytes );
    //------------------------------//
    //  若 cache 中有 此元素，清除在 cache 中的记录
    //------------------------------//
    if( mEntp->is_data_in_mem == true ){
        if( del_cacheEnt( _id ) != 0 ){
            //-- 发现某容器中 没有 _id, 但此处不适合 报错终止。
            err_no = ENOENT;
            //...
        }
    }
    all_cache_bytes -= mEntp->len;
    //------------------------------//
    //    mem 组 若干变量的 更新
    //   在正式删除 varEnt 内容之前
    //------------------------------//
    //--- max_id 不受影响 ---//
    //--- next_base ---//
    //-- 不管是否为 文件最后一个ent，都不会改写 next_base
    //--- all_data_bytes ---//
    all_data_bytes -= mEntp->len;

    //--- all_blk_bytes ---//
    all_blk_bytes -= mEntp->memblk.blk_bytes;

    //------------------------------//
    //  从 mem 容器中 删除这个 id 以及 varEnt
    //------------------------------//
    //-- 此函数 能保障，一定会彻底删除 _id, 哪怕它在某容器中不存在
    if( mem_hash.erase(_id) == 0 ){
        //-- 发现某容器中 没有 _id, 但此处不适合 报错终止。
        err_no = ENOENT;
        //...
    }
    //------------------------------//
    //      根据 err_no 做出反馈
    //------------------------------//
    //  暂未实现
    //...
    return 0;
}

/* ===========================================================
 *                        read
 * -----------------------------------------------------------
 * -- 读取 一条 entData.
 * -- param: _id
 * -- param: _buf -- 目标字符串 将复制到这个 buf 中。
 * -- param: _len -- _buf 的大小， 调用者 可通过 
 *                          (get_entData_len(_id) + 1) 来获得
 * -- param: _is_fast -- 快速模式，此时将不会把 enthead 写回文件
 *                     而是设置 dirty位。通常用在 write前的 预read
 * 
 * -- return:
 *     若运行正常，返回 实际读取的 字节数，
 *              并通过参数 _buf, 返回 字符串数据本身
 *              entData 会被 复制到 _buf 中，所以用户无法 改写原始数据
 *     若出错，返回 -1， 并会设置 相应 errno:
 *         ENOENT -- 参数 _id 在数据库中不存在
 *         EINVAL -- 参数 _len 太短。
 *         ...
 */
ssize_t tprDB_Orig::read( eid_t _id, void *_buf, size_t _len, 
                        bool _is_fast ){

    //----- 确保参数 _id 有效 ----
    if( check_id(_id) == false ){
        errno = ENOENT;
        return -1;
    }
    //---- 获得 mEntp ---
    mem_Ent   *mEntp;
    mEntp = mem_hash_find( _id ); 

    //-- 目标 var data 为空 --
    if( mEntp->len == 0 ){
        //- 虽然数据为空，但表示读过了
        mEntp->rd_times++;
        mEntp->is_data_in_mem = true; 
        mEntp->is_dirty = true;
        return  0; //-- 读取了 0 字节
    }

    //--- 确保参数 _len 有效 ----
    if( _len < (size_t)mEntp->len ){
        errno = EINVAL;
        return -1;
    }

    //---- 如果 entData 不在 mem 中，先读取 ---
    if( mEntp->is_data_in_mem == false ){
        load_a_entData( mEntp );
    }

    //---- 将 entData 复制到 _buf 中 -----
    memcpy( _buf,
            (void*)&(mEntp->data[0]),
            (size_t)mEntp->len
            );

    //------- 读取计数器 累加 ---------
    mEntp->rd_times++;

    //-- 由于 rd_times 发生了变化 --
    // 所以需要立即 将 文件中的 头部 同步 --
    // 但这个思路存在问题， 尤其是在 多线程异步访问中
    // 这个功能可能会被取消

    if( _is_fast == false ){
        //-- 常规模式，将 enthead 写回文件 --//
        writeback_file_enthead(*mEntp, 
                    "tprDB_Orig::read(): ");
    }else{
        //-- 快速模式，只改写 dirty位 --//
        mEntp->is_dirty = true;
    }
    
    return (ssize_t)mEntp->len;
}

/* ===========================================================
 *                       write
 * -----------------------------------------------------------
 * -- FIX / VAR 兼容
 * -- 改写 一条 entData.
 * -- param: _id
 * -- param: _buf -- 新的 entData,  0~16KB 
 * -- param: _len
 * 
 * -- return:
 *     若运行正常，返回 实际写入的 字节数。
 * 
 *     若出错，返回 -1， 并会设置 相应 errno:
 *         ENOENT -- 参数 _id 在数据库中不存在
 *         EINVAL -- 参数 _len 在 FIX 中不符
 *         ...
 */
ssize_t tprDB_Orig::write(eid_t _id, const void *_buf, const DB::len_t _len){

    string err_info = "tprDB_Orig::write(): ";

    //----- 确保参数 _id 有效 ----
    if( check_id(_id) == false ){
        errno = ENOENT;
        return -1;
    }
    //---- 获得 mEntp ---
    mem_Ent   *mEntp;
    mEntp = mem_hash_find( _id ); 

    //++++++++++++++++++++++++++++++++++++++++//
    //               FIX 部分
    //++++++++++++++++++++++++++++++++++++++++//
    //  比较简单
    if( is_fix == true ){

        //--- 确保 _len 符合 --
        if( fOpt.ent_len != _len ){
            errno = EINVAL;
            return -1;
        }
        //---------------------------------//
        //      正式将 新data 复制进 mem 
        //---------------------------------//
        mEntp->data.resize( _len ); //- 多余
        if( _len != 0 ){
            memcpy( (void*)&(mEntp->data[0]),
                    (void*)_buf,
                    (size_t)_len
                    );
        }
        //-- 改写参数 --
        mEntp->wr_times++;
        mEntp->is_dirty       = true; 
        mEntp->is_data_in_mem = true; 
        //-----------------------------------//
        //          更新 cache 
        //-----------------------------------//
        cache_push( _id, mEntp->len );
        //-----------
        return (ssize_t)(mEntp->len);
    }

    //++++++++++++++++++++++++++++++++++++++++//
    //               VAR 部分
    //++++++++++++++++++++++++++++++++++++++++//

    //------  改写 all_data_bytes ------//
    all_data_bytes -= mEntp->len;
    all_data_bytes += _len;

    //---------------------------------//
    //      正式将 新data 复制进 mem 
    //---------------------------------//
    mEntp->data.resize( _len );
    if( _len != 0 ){
        memcpy( (void*)&(mEntp->data[0]),
                (void*)_buf,
                (size_t)_len
                );
    }

    //-- 改写参数 --
    mEntp->len = (len_t)_len;
    mEntp->wr_times++;
    mEntp->is_dirty          = true; 
    mEntp->is_data_in_mem = true;  
    //-----------------------------------//
    //      重新 整理／分配 block
    //-----------------------------------//
    len_t old_blk_bytes = mEntp->memblk.blk_bytes;
    len_t new_blk_bytes = find_suited_Blk_bytes( FILE_ENT_HEAD_LEN+_len );

    //------  改写 all_blk_bytes ------//
    if( new_blk_bytes != old_blk_bytes ){
        all_blk_bytes -= old_blk_bytes;
        all_blk_bytes += new_blk_bytes;
    }
    //------  正式分配 blk ------//
    if( new_blk_bytes > old_blk_bytes ){
        //=== 若 新字符串 blk 级数 变大了 ===

        if( is_last_mEnt(mEntp->memblk.base, old_blk_bytes)==false ){
            //-- 若自己不是文件中排最后的 varEnt，迁徙到 文件 blk尾后 --

            //-- 为原有 blk，制作一个 新空置头部，立即写入文件 --
            writeback_file_empty_enthead( mEntp->memblk.base,
                                            old_blk_bytes,
                                            err_info );
            //-- 将原有 blk 存入 rcyPool ---
            rcy_pools.push( mEntp->memblk.base, old_blk_bytes );
            //-- varEnt base 迁移到文件 blk尾后
            mEntp->memblk.base = next_base; 
        }

        mEntp->memblk.blk_bytes     = new_blk_bytes;
        mEntp->memblk.blk_state_idx = BlkState_2_idx(BlockState::Busy);

        //---- 更新 next_base 的值 ----
        //-- 上诉两种情况里，next_base 都将后移
        refresh_next_base( mEntp->memblk.base, new_blk_bytes );

        //-- 制作一个 新blk 的 头部，立即写入文件 --
        writeback_file_enthead( *mEntp, err_info );

    }else if( new_blk_bytes < old_blk_bytes ){
        //=== 若 新字符串 blk 级数 变小了，释放 多出来的 文件空间 ===

        //-- 递归式地释放 多余空间，并改写 varEnt 的 blk_bytes --
        divide_the_block( &(mEntp->memblk), new_blk_bytes );
    }

    //-----------------------------------//
    //          更新 cache 
    //-----------------------------------//
    cache_push( _id, mEntp->len );
    //-------------------------
    return (ssize_t)(mEntp->len);
}

/* ===========================================================
 *                     flush
 * -----------------------------------------------------------
 * -- 将所有 dirty entry 写回文件中。
 */
void tprDB_Orig::flush(){

    for( const auto& i : mem_hash ){
        if( i.second.is_dirty == true ){
            save_a_entData( i.first );
        }
    }
}

/* ===========================================================
 *                   _1_read_file
 * -----------------------------------------------------------
 * -- FIX / VAR 兼容
 * -- 调用本函数前，默认 文件的 head / opt 已经通过检测了
 */
void tprDB_Orig::_1_read_file(){

    std::string err_info = "tprDB_Orig::_1_read_file(): ";

    //------ 获得 文件 尾后字节 地址偏移 -------//
    base_t file_next_byte = (base_t)get_file_size(fd_file, err_info);

    //-- 每个 blk 的首字节的 地址偏移, 先指向 第一个 blk
    base_t blk_base = data_base;
    Lseek( fd_file, (off_t)blk_base, SEEK_SET, err_info );
    //----------------------------------//
    //       读取   Ent blocks
    //----------------------------------//
    file_EntHead    fEnt; //-- 每条 Ent 的 文件态 头部
    mem_Ent         mEnt; //-- 每条 Ent 的 内存态 头部

    ssize_t rlen;
    while((rlen=Read(fd_file, (void*)fEnt.binary, FILE_ENT_HEAD_LEN, err_info))!=0){

        //-- 如果没能读到 一个 头部的长度，说明文件出了问题
        assert( rlen == FILE_ENT_HEAD_LEN );

        //----------------------------------//
        //    将 head 从 file态 转入 mem态
        //----------------------------------//
        if( is_fix == true ){
            //----- FIX ----//
            fixEntHead_f2m( fEnt, fOpt, &mEnt );
        }else{
            //----- VAR ----//
            varEntHead_f2m( fEnt, &mEnt );
        }
        //-- 自行填充剩余 field --
            mEnt.memblk.base = blk_base;
            mEnt.is_data_in_mem = false;
            mEnt.is_dirty = false;
            mEnt.data.clear();
        //----------------------------------//
        //   根据 此block 的 state 来做不同操作 
        //----------------------------------//
        if( idx_2_BlkState(mEnt.memblk.blk_state_idx) == BlockState::Idle ){
            //-- 若为 空闲 block ---//
            // 添加到 回收池，然后进入下一 回合
            rcy_pools.push( blk_base, mEnt.memblk.blk_bytes );

        }else{
            //-- 若为 非空闲 block ---//
            //--- 确保 mem 容器中 没有 _id ---
            assert( check_id(mEnt.id) == false );

            //---- 将 varEnt 正式添加进 mem 容器中 ---
            mem_hash.insert( {mEnt.id, mEnt} );

            //--- 累计 all_data_bytes,all_blk_bytes ---//
            all_data_bytes += mEnt.len;
            all_blk_bytes += mEnt.memblk.blk_bytes;

            //-- 更新 max_id, max_base --
            refresh_max_id( mEnt.id );
            refresh_next_base( blk_base, mEnt.memblk.blk_bytes );
        }

        //-- 更新 blk_base，指向 下一个 block 的首字节 地址偏移 --
        blk_base += mEnt.memblk.blk_bytes;
        if( blk_base >= file_next_byte ){
            //-- 若下一个 blk的 首地址 已经越过了 文件尾后字节地址 --
            break; //-- 跳出 while 循环
        }
        //--- lseek --
        Lseek( fd_file, (off_t)blk_base, SEEK_SET, err_info );
    }
}

/* ===========================================================
 *                     check_id
 * -----------------------------------------------------------
 * -- 直接 检查 hash 表,
 */
bool tprDB_Orig::check_id( eid_t _id ){
    if( mem_hash.find(_id) == mem_hash.end() ){
        return false;
    }else{
        return true;
    }
}

/* ===========================================================
 *                    get_entData_len    
 * -----------------------------------------------------------
 * -- 获得 一个 ent 的 data 的字节数
 * -- return :
 *     若正确，返回 目标字符串的 字节数。
 *     若出错，返回 -1. 且设置 errno
 *          ENOENT -- 参数 _id 在数据库中不存在
 */
ssize_t tprDB_Orig::get_entData_len( eid_t _id ){

    if( check_id(_id) == false ){
        errno = ENOENT;
        return -1; //-- public 函数，不适合报错终止
    }
    //-----------
    mem_Ent   *mEntp;
    mEntp = mem_hash_find( _id );

    return (ssize_t)(mEntp->len);
}

/* ===========================================================
 *                refresh_max_id
 * -----------------------------------------------------------
 * -- 输入新 entry 的 id，来更新 max_id
 */
void tprDB_Orig::refresh_max_id( eid_t _id ){

    if( _id > max_id ){
        max_id = _id;
    }
}

/* ===========================================================
 *                refresh_max_base  [1]
 * -----------------------------------------------------------
 * -- 输入新 entry 的 base, blk_type，来更新 next_base
 */
void tprDB_Orig::refresh_next_base(base_t _base, len_t _blk_bytes){

    base_t b = _base + _blk_bytes;
    if( b > next_base ){
        next_base = b;
    }
}

/* ===========================================================
 *                  apply_a_id
 * -----------------------------------------------------------
 * -- 申请一个id. id最小为 1（永远不会等于0，为0时表示 null ）
 */
eid_t  tprDB_Orig::apply_a_id(){

    //-- eid_t 在文件中的实际存储容量是 2^48 
    //-- 2^48 是个天文数字，完全不用担心会使用完：
    //-- 如果每秒 申请 100 个 id，需要连续申请 89255 年才能申请完。
    //-- 暂不使用 任何措施 来回收 已经废弃的 id 号。

    //-- id  不做任何形式的回收，如果一个 id本属于 最大id，然后它被删除了
    //-- 此时新分配的 id也从 它之后算起，也就是说，一个 id 永远不会被使用两次
    max_id++;
    assert( max_id < 281474976710656 );
    return max_id;
}

/* ===========================================================
 *                    is_last_mEnt
 * -----------------------------------------------------------
 * -- 判断自己是不是文件中排最后的 mEnt
 */
bool tprDB_Orig::is_last_mEnt( base_t _base, len_t _blk_bytes ){

    base_t tail = _base + _blk_bytes;
    assert( tail <= next_base );

    if( tail == next_base ){
        return true;
    }else{
        return false;
    }
}

/* ===========================================================
 *                    mem_hash_find
 * -----------------------------------------------------------
 * -- 从 mem_hash 中获得一个 Ent 的指针
 */
DB::mem_Ent *tprDB_Orig::mem_hash_find( eid_t _id ){

    auto pair = mem_hash.find(_id);
    //-- 确保 mem_hash 中一定有 _id --
    assert( pair != mem_hash.end() ); 
    return &(pair->second);
}

/* ===========================================================
 *                      load_a_entData
 * -----------------------------------------------------------
 * -- 从文件读取一条 entData 到 mem
 */
void tprDB_Orig::load_a_entData( DB::mem_Ent *_mEntp ){

    string err_info = "tprDB_Orig::load_a_entData(): ";
 
    //----- 向 cache 存入 新 ent 信息 ----
    cache_push( _mEntp->id, _mEntp->len );

    //------- 正式从 文件 读取 字符串数据 -------
    _mEntp->data.resize( _mEntp->len );
    Lseek( fd_file, 
            (off_t)(_mEntp->memblk.base + FILE_ENT_HEAD_LEN), 
            SEEK_SET, 
            err_info 
            );
    ssize_t rlen;
    rlen = Read( fd_file, (void*)&(_mEntp->data[0]), _mEntp->len, err_info);
    assert( rlen == _mEntp->len );
    //------- 更新 varEnt 中状态值 -------
    _mEntp->is_data_in_mem = true;
    _mEntp->is_dirty = false; 
}

/* ===========================================================
 *                      save_a_entData
 * -----------------------------------------------------------
 * -- 将 mem 中的一个 entData，存储到 文件中
 */
void tprDB_Orig::save_a_entData(eid_t _id){

    string err_info = "tprDB_Orig::save_a_entData(): ";
    //------------------
    mem_Ent   *mEntp; 
    mEntp = mem_hash_find( _id ); 

    //----------------------------------//
    //  将 entData 写回文件，并不会改变 cache
    //  毕竟 数据仍存储在 mem 中
    //----------------------------------//

    //-- 将 varEntHead 写入文件 --
    writeback_file_enthead( *mEntp, err_info );

    //--- 将 entData 写入文件 --
    if( mEntp->len != 0 ){
        Write(  fd_file, 
                (const void*)&(mEntp->data[0]), 
                mEntp->len, 
                err_info 
                );
    }
    //--- 改写 varEnt 信息 ---
    mEntp->is_dirty = false;
}

/* ===========================================================
 *            writeback_file_enthead   
 * -----------------------------------------------------------
 * -- 将一个 mem_Ent 头部，写回 文件中，
 */
void tprDB_Orig::writeback_file_enthead( const DB::mem_Ent &_ent,
                            const std::string &_err_info){

    //--- 转换为 file态 头部 --
    file_EntHead fEnt;
    if( is_fix == true ){
        //--- FIX ---//
        fEnt = fixEntHead_m2f( _ent );
    }else{
        //--- VAR ---//
        fEnt = varEntHead_m2f( _ent );
    }

    Lseek( fd_file, _ent.memblk.base, SEEK_SET, _err_info );
    Write( fd_file, (const void*)fEnt.binary, FILE_ENT_HEAD_LEN, _err_info);
}

/* ===========================================================
 *        writeback_file_empty_enthead    
 * -----------------------------------------------------------
 * -- 将一个 mem_Ent 头部，写回 文件中，
 */
void tprDB_Orig::writeback_file_empty_enthead( base_t _base, 
                            len_t _blk_bytes,
                            const std::string &_err_info){
    mem_Ent mEnt;
        mEnt.id = 0;
        mEnt.len = 0;
        mEnt.rd_times = 0;
        mEnt.wr_times = 0;
        //mEnt.memblk.base = _base; //- 用不到
        mEnt.memblk.blk_bytes = _blk_bytes;
        mEnt.memblk.blk_state_idx = BlkState_2_idx(BlockState::Idle);
    //--- 转换为 file态 头部 --
    file_EntHead fEnt;
    if( is_fix == true ){
        //--- FIX ---//
        fEnt = fixEntHead_m2f( mEnt );
    }else{
        //--- VAR ---//
        fEnt = varEntHead_m2f( mEnt );
    }

    Lseek( fd_file, _base, SEEK_SET, _err_info );
    Write( fd_file, (const void*)fEnt.binary, FILE_ENT_HEAD_LEN, _err_info);
}

/* ===========================================================
 *                    cache_push
 * -----------------------------------------------------------
 * -- 向 cache 存入 新 ent 信息
 * -- 允许将同一 id 重复添加，函数会自动忽略它们
 */
void tprDB_Orig::cache_push( eid_t _id, len_t _len ){

    //----------------------//
    //  若 _id 已在 cache 中
    //   什么也不做，直接返回
    //----------------------//
    if( cache_history_iters.find(_id) != cache_history_iters.end() ){
        return; 
    }

    //----------------------//
    //--- 如果 cache 有过载可能，先 释放 cache ----
    if( (all_cache_bytes + _len) > cache_limit ){
        cache_release( _len );
    }

    //--- 更新 all_cache_bytes ---
    all_cache_bytes += _len;

    //--- 将 新 ent 信息 添加到 cache 容器中 ---
    cache_history.push_back( _id );
    auto it = cache_history.end();
    it--; //-- 现在，迭代器 it指向 最后一个元素
    cache_history_iters.insert( {_id, it} );
}

/* ===========================================================
 *                     cache_release
 * -----------------------------------------------------------
 * -- 从 cache 中释放 至少 _size 字节 的空间
 * -- 从 cache_history 头部 一个一个地释放，直到 释放足够空间 
 */
void tprDB_Orig::cache_release( u64 _size ){

    string err_info = "tprDB_Orig::cache_release(): ";

    //---- 确保 _size 小于 cache_limit ----//
    assert( _size <= cache_limit );

    //-----------------
    eid_t      id;        //- tmp
    mem_Ent   *mEntp;  //- tmp， 用指针 来访问 hash表元素的 seconds
    u64  usable_bytes = cache_limit - all_cache_bytes;
                            //-- 当前 可用空间

    size_t cache_nums;
    while( (cache_nums=cache_history.size()) > 0 ){

        id = cache_history.front();
        mEntp = mem_hash_find( id ); 

        //-- entData 已经被 改写，需要 写回到文件 --
        if( mEntp->is_dirty == true ){
            save_a_entData( id );
        }

        //------ 正式释放 entData -------
        mEntp->data.clear(); //-- 清空
        mEntp->is_data_in_mem = false;

        //------ 清理 cache 容器 ----
        //-- 确保 cache_history_iters 存在 id
        assert( cache_history_iters.erase(id) != 0 );

        cache_history.pop_front();

        //---- 统计 已释放字节数 ------
        usable_bytes += mEntp->len;
        if( usable_bytes >= _size ){
            break;
        }
    }//------- while end -------

    //-------- 更新 all_cache_bytes ------
    all_cache_bytes = cache_limit - usable_bytes;
}

/* ===========================================================
 *                    del_cacheEnt
 * -----------------------------------------------------------
 * -- 从 cache 容器组，删除一个 entry. [ 一定会彻底删除 ]
 *    目标 _id 有可能不在 某一容器中。或在两容器中 都不存在
 *    即便如此，我们也会 彻底遍历两个容器，彻底完成删除。
 *    并在返回中，报告查询结果。
 * -- return
 *       若正确，                                返回 0
 *       若 _id 仅仅不存在于 cache_history        返回 -1
 *       若 _id 仅仅不存在于 cache_history_iters  返回 -2
 *       若 _id 在两容器中都不存在，               返回 -3
 */
int tprDB_Orig::del_cacheEnt( eid_t _id ){

    bool is_in_history       = false;
    bool is_in_history_iters = true;

    //----- 删除 cache_history 中元素 ----
    auto it = cache_history_iters.find( _id );
    if( it == cache_history_iters.end() ){
        //-- 若 _id 不在 cache_history_iters 中 --
        is_in_history_iters = false;

        //--- 使用一种 低速但有效 的方式来 
        //    删除 cache_history 中元素 
        auto itt = cache_history.begin();
        for( ; itt != cache_history.end(); itt++ ){
            if( *itt == _id  ){
                cache_history.erase( itt );
                is_in_history = true;
                break;
            }
        }

    }else{
        //-- 若 _id 在 cache_history_iters 中 --
        is_in_history = true;
        cache_history.erase( it->second ); //-- 直接删，不检查返回值
        cache_history_iters.erase(_id);    //-- 直接删，不检查返回值
    }

    //---------------
    if( is_in_history && is_in_history_iters ){
        return 0;
    }else if( (!is_in_history) && is_in_history_iters ){
        return -1;
    }else if( is_in_history && (!is_in_history_iters) ){
        return -2;
    }else{
        return -3;
    }
}

/* ===========================================================
 *                   divide_the_block
 * -----------------------------------------------------------
 * -- 递归式地 将一个 大的 blk，拆封为数个 小的block。
 * -- 拆分到最后，目标blk 的级数将和  _blk_bytes 一样大，然后结束递归
 * -- 中途被拆分出来的 各个block，都将 独立成 新空置blk，
 *    并被 存入 文件 和 rcy_pools 中。
 * 
 * 
 * -- param: _mb        -- 指向 目标 memblk。
 * -- param: _blk_bytes -- 目标 blk级数
 */
void tprDB_Orig::divide_the_block( DB::mem_Block *_mb, len_t _blk_bytes ){

    //---- 参数检测，目标blk级数，不能小于 _blk_bytes ---
    assert( _mb->blk_bytes >= _blk_bytes );

    string err_info = "tprDB_Orig::divide_the_block(): ";
    //-------------------------
    if( _mb->blk_bytes == _blk_bytes ){
        //=== 若 目标 blk级数已经足够小了（或更小）可以退出递归 ===
        return;
    }
    //=== 目标blk 仍比 要求尺寸大，需要（再）细分 一次 ===
    //  目标blk 会被 一份为二，前半段 传入下一级递归 继续细分。
    //  后半段 被 独立为一个 新的 空置blk。
    //  这个 新空置blk 将被立即写入 文件 和 rcy_pools
    //-------------------------------------
    len_t  new_blk_bytes = _mb->blk_bytes / 2;          
    base_t new_base      = _mb->base + _mb->blk_bytes/2;
    
    //--- 将 新空置blk，送入 rcyPool --
    rcy_pools.push( new_base, new_blk_bytes );
    
    //--- 生成 新空置blk 的 head，并写入 文件 ----
    //  !!! 注意 !!!
    // 这是一个 虚函数，必须调用 派生类实现的版本
    writeback_file_empty_enthead( new_base,
                                    new_blk_bytes,
                                    err_info );
    //------!!! 递归 !!!------
    _mb->blk_bytes /= 2 ; //-- old 的 blk 缩成原来一半
    divide_the_block( _mb, _blk_bytes );
}


}//------------------- namespace: tpr ------------------------//

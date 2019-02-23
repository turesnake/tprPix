/*
 * ========================= tprDB_type.cpp ==========================
 *                          -- tpr --
 *                                        创建 -- 2018.10.20
 *                                        修改 -- 2018.10.20
 * ----------------------------------------------------------
 * 
 *   tprDB 中，和 数据类型 有关的 内容
 *  
 *   本h文件 的所有内容，应该与 模版 无关
 * 
 * ----------------------------
 * 依赖的其它库：
 *       tprFileModeT
 *       tprDataType
 * ----------------------------
 */
#include "tprDB_type.h"

//------------ C -------------//
#include <string.h> //- memcpy
#include <cassert> //- assert
#include <errno.h> //- errno
#include <iostream> //- tmp

using std::cout;
using std::endl;
using std::string;

namespace tpr { //--------------- namespace: tpr -------------------//
namespace DB {//----- namespace: DB -----//


/* ===========================================================
 *                  Blk_bytes_2_idx
 * -----------------------------------------------------------
 * -- VAR 专用，所有 blk尺寸，都是 2^n, [2, 16384]
 */
u8_t Blk_bytes_2_idx( len_t _blk_bytes ){

    switch( _blk_bytes ){
        case 2:     return 1;
        case 4:     return 2;
        case 8:     return 3;
        case 16:    return 4; 
        case 32:    return 5;
        case 64:    return 6;
        case 128:   return 7;
        case 256:   return 8;
        case 512:   return 9;
        case 1024:  return 10;
        case 2048:  return 11;
        case 4096:  return 12;
        case 8192:  return 13;
        case 16384: return 14;
        default:
            assert(0);
            return 0; //- 不会运行到此行
    }
}

/* ===========================================================
 *               idx_2_Blk_bytes
 * -----------------------------------------------------------
 * -- VAR 专用，所有 blk尺寸，都是 2^n, [2, 16384]
 */ 
len_t idx_2_Blk_bytes( u8_t _idx ){
    switch( _idx ){
        case 1:    return 2;
        case 2:    return 4;
        case 3:    return 8;
        case 4:    return 16;
        case 5:    return 32;
        case 6:    return 64;
        case 7:    return 128;
        case 8:    return 256;
        case 9:    return 512;
        case 10:   return 1024;
        case 11:   return 2048;
        case 12:   return 4096;
        case 13:   return 8192;
        case 14:   return 16384;
        default:  
            assert(0);
    }
}

/* ===========================================================
 *               Blk_bytes_2_aligns
 * -----------------------------------------------------------
 * -- FIX 专用，blk 尺寸全是 4字节对齐的。
 */
len_t Blk_bytes_2_aligns( len_t _blk_bytes ){

    //-- 确保 _blk_bytes 是 4字节对齐的
    assert( (_blk_bytes%4)==0 );
    return (_blk_bytes/4);
}

/* ===========================================================
 *               aligns_2_Blk_bytes
 * -----------------------------------------------------------
 * -- FIX 专用，blk 尺寸全是 4字节对齐的。
 */
len_t aligns_2_Blk_bytes( len_t _aligns ){

    //-- 缺环 还原后的 blk字节数 不超过 len_t 类型上限
    u32_t s = _aligns * 4;
    //assert( s < 65536 );

    return ( _aligns * 4 );
}

/* ===========================================================
 *              find_suited_Blk_bytes
 * -----------------------------------------------------------
 * -- VAR 专用， 寻找匹配的 blk 字节数
 */
len_t find_suited_Blk_bytes( len_t _size ){

    if( _size <= 2 ){           return 2;
    }else if( _size <= 4 ){     return 4;
    }else if( _size <= 8 ){     return 8;
    }else if( _size <= 16 ){    return 16;
    }else if( _size <= 32 ){    return 32;
    }else if( _size <= 64 ){    return 64;
    }else if( _size <= 128 ){   return 128;
    }else if( _size <= 256 ){   return 256;
    }else if( _size <= 512 ){   return 512;
    }else if( _size <= 1024 ){  return 1024;
    }else if( _size <= 2048 ){  return 2048;
    }else if( _size <= 4096 ){  return 4096;
    }else if( _size <= 8192 ){  return 8192;
    }else if( _size <= 16384){  return 16384;
    }else{
        //--- 暂不支持 超过 16KB 的 字符串 ---//
        assert(0);
        return 16384; //-- 不会运行到 此行 --
    }
}

/* ===========================================================
 *               BlkState_2_idx  
 * -----------------------------------------------------------
 */
u8_t BlkState_2_idx( BlockState _bstate ){
    switch( _bstate ){
        case BlockState::Idle:  return 1;
        case BlockState::Busy:  return 2;
        default:
            assert(0);
            return 0; //-- 不会运行到 此行 --
    }
}

/* ===========================================================
 *              idx_2_BlkState  
 * -----------------------------------------------------------
 */
BlockState idx_2_BlkState( u8_t _idx ){
    BlockState bs;
    switch( _idx ){ 
        case 1:  bs = BlockState::Idle;  break;
        case 2:  bs = BlockState::Busy;  break;
        default:  
            assert(0);
    }
    return bs;
}

/* ===========================================================
 *               idx_2_BlkStateStr  
 * -----------------------------------------------------------
 */
std::string idx_2_BlkStateStr( u8_t _idx ){

    switch( _idx ){ 
        case 1:  return "Idle";
        case 2:  return "Busy";
        default:  
            assert(0);
    }
}

/* ===========================================================
 *               find_suited_blk_size 
 * -----------------------------------------------------------
 * -- FIX 专用。一种 4字节对齐的 blk， 
 */
len_t find_suited_Blk_aligns( len_t _len ){

    len_t s = _len / 4;

    //-- 若无法被整除，还要扩容一次（增加 4字节）
    if( (_len%4) != 0 ){
        s++;
    }
    return s;
}


/* ===========================================================
 *             find_suited_Blk_aliBytes
 * -----------------------------------------------------------
 * -- FIX 专用。一种 4字节对齐的 blk， 
 */
len_t find_suited_Blk_aliBytes( len_t _len ){

    len_t s = _len / 4;

    //-- 若无法被整除，还要扩容一次（增加 4字节）
    if( (_len%4) != 0 ){
        s++;
    }
    //assert( (s*4) < 65536 );
    return (s*4);
}


/* ===========================================================
 *               varEntHead_f2m
 * -----------------------------------------------------------
 * -- 将 文件态的 varEnthead, 转换为 内存态的 mem_VarEnt 实例
 */
void varEntHead_f2m( const file_EntHead &_fhead, mem_Ent *_mEntp ){

    u8_t *p = (u8_t*)_fhead.binary; //- 用指针来访问 _fhead.binary

    //--- id ---//
    _mEntp->id = 0;
    memcpy( (void*)&(_mEntp->id), (void*)p, 6);
    p += 6;

    //--- len ---//
    _mEntp->len = 0;
    memcpy( (void*)&(_mEntp->len), (void*)p, 2);
    p += 2;

    //--- rd_times ---//
    _mEntp->rd_times = 0;
    memcpy( (void*)&(_mEntp->rd_times), (void*)p, 3);
    p += 3;

    //--- wr_times ---//
    _mEntp->wr_times = 0;
    memcpy( (void*)&(_mEntp->wr_times), (void*)p, 3);
    p += 3;

    //--- blk_t_idx ---//
    assert( ( *p>=1 )&&( *p<=14 ) ); //-- 检测不越界
    _mEntp->memblk.blk_bytes = idx_2_Blk_bytes( *p );
    p += 1;

    //--- blk_state_idx ---//
    assert( (*p==1)||(*p==2) ); //-- 检测不越界
    _mEntp->memblk.blk_state_idx = *p;

    //--- oth ---//
    //  以下 field 并不完整，需要调用者 自行填充 
    _mEntp->memblk.base = 0;
    _mEntp->is_data_in_mem = false;
    _mEntp->is_dirty = false;
    _mEntp->data.clear();
}

/* ===========================================================
 *               varEntHead_m2f
 * -----------------------------------------------------------
 * -- 将 内存态的 mem_VarEnt, 制作成一个 文件态的 file_EntHead
 */
file_EntHead varEntHead_m2f( const mem_Ent &_mEnt ){

    file_EntHead head; //-- 返回值
    u8_t *p = (u8_t*)head.binary;

    //--- id ---//
    // ..高位被丢弃..
    memcpy( (void*)p, (void*)&(_mEnt.id), 6);
    p += 6;

    //--- len ---//
    // ..高位被丢弃..
    memcpy( (void*)p, (void*)&(_mEnt.len), 2);
    p += 2;

    //--- rd_times ---//
    // ..高位被丢弃..
    memcpy( (void*)p, (void*)&(_mEnt.rd_times), 3);
    p += 3;

    //--- wr_times ---//
    // ..高位被丢弃..
    memcpy( (void*)p, (void*)&(_mEnt.wr_times), 3);
    p += 3;

    //--- blk_t_idx ---//
    *p = Blk_bytes_2_idx( _mEnt.memblk.blk_bytes );
    p += 1;

    //--- blk_state_idx ---//
    *p = _mEnt.memblk.blk_state_idx;

    return head;
}

/* ===========================================================
 *               fixEntHead_f2m
 * -----------------------------------------------------------
 * -- 将 文件态的 varEnthead, 转换为 内存态的 mem_VarEnt 实例
 */
void fixEntHead_f2m( const file_EntHead &_fhead, 
                    const file_FixOptions &_opt,
                    mem_Ent *_mEntp ){

    u8_t *p = (u8_t*)_fhead.binary; //- 用指针来访问 _fhead.binary

    //--- id ---//
    _mEntp->id = 0;
    memcpy( (void*)&(_mEntp->id), (void*)p, 6);
    p += 6;

    //--- len ---//
    _mEntp->len = _opt.ent_len;
    p += 2; //- 文件态 闲置

    //--- rd_times ---//
    _mEntp->rd_times = 0;
    memcpy( (void*)&(_mEntp->rd_times), (void*)p, 3);
    p += 3;

    //--- wr_times ---//
    _mEntp->wr_times = 0;
    memcpy( (void*)&(_mEntp->wr_times), (void*)p, 3);
    p += 3;

    //--- blk_t_idx ---//
    p += 1; //- 文件态 闲置

    //--- blk_state_idx ---//
    assert( (*p==1)||(*p==2) ); //-- 检测不越界
    _mEntp->memblk.blk_state_idx = *p;


    //--- blk_size ---//
    _mEntp->memblk.blk_bytes = aligns_2_Blk_bytes( _opt.blk_aligns );

    //--- oth ---//
    //  以下 field 并不完整，需要调用者 自行填充
    _mEntp->memblk.base = 0; 
    _mEntp->is_data_in_mem = false;
    _mEntp->is_dirty = false;
    _mEntp->data.clear();
}

/* ===========================================================
 *                  fixEntHead_m2f
 * -----------------------------------------------------------
 * -- 将 内存态的 mem_FixEnt, 制作成一个 文件态的 file_EntHead
 */
file_EntHead fixEntHead_m2f( const mem_Ent &_mEnt ){

    file_EntHead head; //-- 返回值
    u8_t *p = (u8_t*)head.binary;

    //--- id ---//
    // ..高位被丢弃..
    memcpy( (void*)p, (void*)&(_mEnt.id), 6);
    p += 6;

    //--- len ---//
    // ..文件态 闲置位，写入 0..
    memset( (void*)p, 0, 2);
    p += 2;

    //--- rd_times ---//
    // ..高位被丢弃..
    memcpy( (void*)p, (void*)&(_mEnt.rd_times), 3);
    p += 3;

    //--- wr_times ---//
    // ..高位被丢弃..
    memcpy( (void*)p, (void*)&(_mEnt.wr_times), 3);
    p += 3;

    //--- blk_t_idx ---//
    // ..文件态 闲置位，写入 0..
    *p = 0;
    p += 1;

    //--- blk_state_idx ---//
    *p = _mEnt.memblk.blk_state_idx;

    return head;
}

/* ===========================================================
 *                 DATA_T_2_bytes
 * -----------------------------------------------------------
 * -- 根据 DATA_T, 获得此类型 代表的 bytes（实际存储字节数）
 */
len_t DATA_T_2_bytes( DATA_T _dt ){

    //--- 此函数不可用于 HUGE_BINARY 类型 ---
    assert( _dt != DATA_T::HUGE_BINARY );

    if( (u8_t)_dt <= 4 ){
        return 4;
    }else{
        return 8;
    }
}

/* ===========================================================
 *                  is_DATA_T_a_INT
 * -----------------------------------------------------------
 */
bool is_DATA_T_a_INT( DATA_T _dt ){

    switch( _dt ){
        case DATA_T::BOOL:  return true;
        case DATA_T::I32:   return true;
        case DATA_T::U32:   return true;
        case DATA_T::FLOAT: return false;

        case DATA_T::STRID:  return false;
        case DATA_T::I64:    return true;
        case DATA_T::U64:    return true;
        case DATA_T::DOUBLE: return false;

        case DATA_T::HUGE_BINARY: return false;
        default:
            assert(0);
            return false; //- 不会运行到此行
    }
}

/* ===========================================================
 *                  is_DATA_T_a_FLOAT
 * -----------------------------------------------------------
 */
bool is_DATA_T_a_FLOAT( DATA_T _dt ){

    if( (_dt==DATA_T::FLOAT) || (_dt==DATA_T::DOUBLE) ){
        return true;
    }else{
        return false;
    }
}

/* ===========================================================
 *                  is_DATA_T_a_STRID
 * -----------------------------------------------------------
 */
bool is_DATA_T_a_STRID( DATA_T _dt ){

    if( _dt == DATA_T::STRID ){
        return true;
    }else{
        return false;
    }
}

/* ===========================================================
 *                  mk_options
 * -----------------------------------------------------------
 */
file_FixOptions mk_options( len_t _len ){

    file_FixOptions opt;
    opt.ent_len = _len;
    opt.blk_aligns = find_suited_Blk_aligns( FILE_ENT_HEAD_LEN + _len );
    return opt;
}


}//-------- namespace: DB --------------//
}//------------------- namespace: tpr ------------------------//

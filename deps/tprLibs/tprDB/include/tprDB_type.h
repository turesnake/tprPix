/*
 * ========================= tprDB_type.h ==========================
 *                          -- tpr --
 *                                        创建 -- 2018.10.20
 *                                        修改 -- 2018.10.20
 * ----------------------------------------------------------
 * 
 *   tprDB 中，和 数据类型 有关的 内容
 * 
 *   本h文件，只被 以下文件调用: 
 *          tprDB.h 
 *          tprDB_VAR.h
 *          tpDB_type.cpp
 * 
 *   本h文件 的所有内容，应该与 模版 无关
 * 
 * ----------------------------
 * 依赖的其它库：
 *       tprFileModeT
 *       tprDataType
 * ----------------------------
 */
#ifndef _TPR_TPRDB_TYPE_H_
#define _TPR_TPRDB_TYPE_H_

//------------ C -------------//
#include <cassert> //- assert

//------------ CPP -------------//
#include <string>
#include <vector>
//#include <iostream> 

//------------ libs -------------//
#include "tprFileModeT.h" 
#include "tprDataType.h" 
#include "tprCrypto.h" 

namespace tpr { //--------------- namespace: tpr -------------------//
namespace DB {//---------- namespace: DB ----------//

//--- 文件 后缀名 ---//
inline const std::string SUFFIX_TPR_FIX { ".tprFix" };
inline const std::string SUFFIX_TPR_VAR { ".tprVar" };


inline constexpr size_t TPRDB_MAGIC_NUM_BYTES = 16;
//--------------------------------"123456789012345"--16-bytes--//
inline const std::string FIX_MAGIC_NUM { "_____tprFix____" };
inline const std::string VAR_MAGIC_NUM { "_____tprVar____" };

//------ .tprFix/.tprVar 两文件 的 头部结构 --------
//--- 请确保这个 头部是 2^n 对齐的 ----
struct tprDB_File_Head{
    char         magic_num[ 16 ]; //- 16-bytes -- 魔数，验证本文件的 处理软件 是否为 tprDB
    CheckSum128  check_sum;       //- 16-bytes -- 检验和，确认 此文件拥有者身份 的唯一手段
};


using eid_t     = u64;   //-- id 的类型
using base_t   = u64;   //-- 文件内 地址偏移 --
using len_t    = u32;   //-- 数据长度类型，（ 4GB ）


//--- 当调用者 搭建 entry 的类型 ：一个 C风格的 struct 时，
//-- 要求使用 以下别名 作为 struct 内 元素的 类型，而不是原生 类型
//-- 在外部使用时，通过: tpr::T_I32 
using T_BOOL   = i32;    //--4-- 0/1
using T_I32    = i32;    //--4--
using T_U32    = u32;    //--4--
using T_FLOAT  = float;  //--4--

using T_STRID  = u64;    //--8-- 字符串／变长二进制数据
using T_I64    = i64;    //--8--
using T_U64    = u64;    //--8--
using T_DOUBLE = double; //--8--

//--- 当调用者 搭建 entry 的类型 ：一个 C风格的 struct 时，
//-- 要求使用 以下别名 作为 struct 内 元素的 类型，而不是原生 类型
//- 可归为 三类:
//-   INT
//-   FLOAT
//-   STRID
enum class DATA_T : u8{

    BOOL   = 1,  //-- 4-bytes --
    I32    = 2,  //-- 4-bytes --
    U32    = 3,  //-- 4-bytes --
    FLOAT  = 4,  //-- 4-bytes --

    STRID  = 5,  //-- 8-bytes -- 字符串／变长二进制数据
    I64    = 6,  //-- 8-bytes --
    U64    = 7,  //-- 8-bytes --
    DOUBLE = 8,  //-- 8-bytes --

    HUGE_BINARY = 9 //-- n-bytes -- 巨型二进制数据块
};
//---------------
len_t DATA_T_2_bytes( DATA_T _dt );


//-- DATA_T ( struct ) 中，每个 字段的 类型信息 --  -bytes--
struct Field_T{
    len_t    base {0};    //--4-bytes- 本字段 在 struct 中的 地址偏移
    len_t    len {0};     //--4-bytes- 本字段 的 字节数
    DATA_T   data_type;   //--1-bytes- 本字段 的 类型
    //--- padding ---//
    u8       padding[3] {0}; //--3-bytes- 
};
//---------------
bool is_DATA_T_a_INT( DATA_T _dt );
bool is_DATA_T_a_FLOAT( DATA_T _dt );
bool is_DATA_T_a_STRID( DATA_T _dt );


//--  block 的 字节数 : idx 映射关系 --- 
/*
enum class Block_T : u32{
    //      -val-     -idx-    
    B_2     = 2,     //- 1
    B_4     = 4,     //- 2
    B_8     = 8,     //- 3
    B_16    = 16,    //- 4
    B_32    = 32,    //- 5
    B_64    = 64,    //- 6
    B_128   = 128,   //- 7
    B_256   = 256,   //- 8
    B_512   = 512,   //- 9
    B_1024  = 1024,  //- 10
    B_2048  = 2048,  //- 11
    B_4096  = 4096,  //- 12
    B_8192  = 8192,  //- 13
    B_16384 = 16384  //- 14
};
*/
//-- 目前一共有 14 个 block 类型。
inline constexpr len_t BLOCK_T_NUMS = 14;


//-- 一条 strEnt 所处的 block 的 当前状态 -- 
enum class BlockState : u32{
    Idle = 1,  //-- 此块非空，读取其 字符串
    Busy = 2   //-- 此块为空，应该存入 回收池
};
//---------------
u8 BlkState_2_idx( BlockState _bstate );
BlockState idx_2_BlkState( u8 _idx );
std::string idx_2_BlkStateStr( u8 _idx );


//==================== FILE FIX / VAR ======================//
//|||||||||||||||||||||||||||||||||||||||||||||||||||||||||//

//-- 文件态，一条 Ent 的 头部 16-bytes --
//-- FIX / VAR 兼容
struct file_EntHead{
    //--------------
    // id                  -- 6-bytes: 
    // len / [-pad-]       -- 2-bytes: 65535 个字节
    // rd_times            -- 3-bytes: 1677 万次
    // wr_times            -- 3-bytes: 1677 万次
    // blk_t_idx / [-pad-] -- 1-bytes: 
    // blk_state_idx       -- 1-bytes: 
    //---------------
    u8 binary[16]{ 0 }; 
};
//-- VAR / FIX 通用的 file态，ent头部字节数 --
inline constexpr len_t FILE_ENT_HEAD_LEN = sizeof(file_EntHead);

//====================== FILE FIX  =========================//
//|||||||||||||||||||||||||||||||||||||||||||||||||||||||||//

//-- 在 .tprFix 文件中，tprDB_File_Head结构后方， --
//   跟随一个 数据块，记录有关 fixEnt 数据的信息  16-bytes
struct file_FixOptions{
    len_t   ent_len    = 0; //--4- 一个 ent 的有效数据是 多少字节
    len_t   blk_aligns = 0; //--4- 一个 blk 由多少个 "4字节" 组成
    //---- padding ----//
    u8   padding[8] { 0 };  //--8-
};
inline constexpr len_t FILE_FIXOPTION_LEN = sizeof(file_FixOptions);

file_FixOptions mk_options( len_t _len );


//========================== MEM ==========================//
//|||||||||||||||||||||||||||||||||||||||||||||||||||||||||//

//-- mem态，一条 Ent 所属的 blk 的信息 --
//-- 兼容 VAR ／ FIX --
struct mem_Block{
    base_t  base = 0;          //--8- 本 block 在 文件中的 地址偏移。（mem 特有）
    len_t   blk_bytes = 0;     //--4- blk 字节数 
    u8      blk_state_idx = 0; //--1- blk 状态: 空置，使用...
};

//-- mem态，一条 Ent 的完整信息 --
//-- 兼容 VAR ／ FIX --
struct mem_Ent{

    eid_t     id = 0;       //--8- 本条 ent 的 id
    len_t    len = 0;      //--4- 本条 ent data 的字节数（不包含 尾后0）
    //--- 读写次数 ----//
    u32      rd_times = 0;  //--4- 被读取的次数
    u32      wr_times = 0;  //--4- 被改写的次数

    mem_Block    memblk;
    bool    is_data_in_mem = false; //-- data本体 是否已存入 mem 中。 
    bool    is_dirty       = false; //-- 是否被改写。

    std::vector<u8>  data { 0 };    //-- 数据本体
};


//========================== DB TYPE ========================//
//|||||||||||||||||||||||||||||||||||||||||||||||||||||||||//
enum class DB_TYPE : u8{
    Pure_Var = 1, //- 只有 varDB 子库
    Pure_Fix = 2, //- 只有 fixDB 子库
    Regular  = 3  //- 两个 子库 都有
};


//========================= funcs ==========================//

len_t find_suited_Blk_bytes( len_t _size ); //- VAR
len_t find_suited_Blk_aligns( len_t _len ); //- FIX
len_t find_suited_Blk_aliBytes( len_t _len ); //- FIX

u8 Blk_bytes_2_idx( len_t _blk_bytes ); //- VAR
len_t  idx_2_Blk_bytes( u8 _idx );      //- VAR

len_t Blk_bytes_2_aligns( len_t _blk_bytes ); //- FIX
len_t aligns_2_Blk_bytes( len_t _aligns );    //- FIX


//------- VAR entHead 在 file态 与 mem态 的转换 ------
void varEntHead_f2m( const file_EntHead &_fhead, mem_Ent *_mEntp );
file_EntHead varEntHead_m2f( const mem_Ent &_mEnt );

//------- FIX entHead 在 file态 与 mem态 的转换 ------
void fixEntHead_f2m( const file_EntHead &_fhead, 
                    const file_FixOptions &_opt,
                    mem_Ent *_mEntp );
file_EntHead fixEntHead_m2f( const mem_Ent &_mEnt );



}//-------- namespace: DB --------------//
}//------------------- namespace: tpr ------------------------//
#endif


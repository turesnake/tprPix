/*
 * ========================= tprDB.h ==========================
 *                          -- tpr --
 *                                        创建 -- 2018.10.13
 *                                        修改 -- 2018.10.13
 * ----------------------------------------------------------
 * 
 *   一个 不支持异步访问的 小型数据库
 * 
 *   有 3 种形式的数据库：
 *     1 -- PURE_VAR 数据库 - 此时只有 varDB 子库在运行 （比如只存储 字符串）
 *     2 -- 常规数据库 - 只有 fix 子库在运行。（比如未存储字符串）
 *     3 -- 常规数据库 - var / fix 两个 子库都在运行 
 * 
 *   目前版本只提供了 非常初级的接口。
 *   需要调用者 自行封装。
 *   
 * ----------------------------
 *          使用 教程
 * 
 *   tpr::tprDB db{ false };
 *   db.init( path_parent, name, {...}, 32 );
 *   
 * 
 *   ...
 * 
 * ----------------------------
 * 依赖的其它库：
 *       wrapFuncs 
 *       tprFileSys
 *       tprFileModeT
 *       tprCrypto
 *       tprDataType
 *       tprBinary (临时)
 *       nocopyable
 * ----------------------------
 */
#ifndef _TPR_TPRDB_H_
#define _TPR_TPRDB_H_

//------------ C -------------//
#include <fcntl.h> //- open, openat,mode_t
#include <string.h> //- memcpy

#include <cassert> //- assert

//------------ CPP -------------//
#include <string>
#include <vector> 
#include <iostream> 

//------------ libs -------------//
#include "tprFileModeT.h" 
#include "wrapFuncs.h"
#include "tprFileSys.h"
#include "tprCrypto.h" 
#include "tprDataType.h" 
#include "tprBinary.h"  //- 临时
#include "nocopyable.h"

//------------ SELF -------------//
#include "tprDB_type.h" 
#include "tprDB_Orig.h" 

 
namespace tpr { //--------------- namespace: tpr -------------------//


class tprDB : nocopyable {
    //============== friend ==============
public:

    //-- 构造函数 --//
    explicit tprDB( bool _is_pure_val=false ) //- 是否为一个纯 VAR 数据库
        :
        is_have_FIX( !_is_pure_val )
        {}

    //---- 析构函数 ----//    *** 需要析构函数的类，也需要 拷贝构造函数 和 拷贝赋值运算符 *** --
    ~tprDB(){
        //flush();
    }

    //----- init ------//
    void init(  const std::string &_path_dir_parent, //- 通常为 .../data/
                const std::string &_DBname,          //- 本数据库实例的 name
                std::initializer_list<DB::DATA_T> _field_types,
                DB::len_t _DATA_T_size
                );

    //-----------------//
    // ---< 增 >---
    DB::eid_t insert( bool _is_fix, const void *_buf, DB::len_t _len );
                                            
    // ---< 删 >---
    int erase( DB::eid_t _id ); 

    // ---< 查 >---
    ssize_t read( bool _is_fix, DB::eid_t _id, void *_buf, size_t _len, bool _is_fast=false );   
    ssize_t get_entData_len( bool _is_fix, DB::eid_t _id );

    // ---< 改 >---
    ssize_t write( bool _is_fix, DB::eid_t _id, const void *_buf, const DB::len_t _len );

    // ---< 冲 >---
    void flush(); //-- 将所有 dirty entry 写回文件中。

    //-----------------//
    bool check_id( bool _is_fix, DB::eid_t _id ); //-- 检测 _id 对应的 entry 是否已存在。
    size_t ent_nums(); //- 当前有多少 dbent
    DB::eid_t get_fst_id(); //- 获得 数据库中 第一个 ent 的id 

    void info( bool _fix, bool _var );

private:
    //++++++++++++++++|  Check Sum  |++++++++++++++++//
    CheckSum128    check_sum; //- 数据库实例 的身份的唯一证明。
                            //  一个数据库实例，只有一个 checksum，在所有文件中通用
    //------------------
    void mk_checksum(); //-- 制作 本数据库实例 得 checksum

    
    //++++++++++++++++|  FILE  |++++++++++++++++//
    //----- string / path ------//
    std::string path_dir_parent; //-- /data/ 目录的 父目录 路径名。   -- 构造函数参数传入
    std::string path_dir_tprDB;  //-- /data/tprDB/ 目录的路径名。    -- 类自动合成
    std::string DBname;          //-- 数据库实例的名字。              -- 构造函数参数传入
    std::string path_dir_self; //-- /data/tprDB/DBmane/ 目录的路径名 -- 类自动合成

    std::string path_file_tprFix;     //-- .tprFix 文件 路径名 -- 类自动合成
    std::string path_file_tprVar;     //-- .tprVar 文件 路径名 -- 类自动合成
    
    //------- fd -------//
    int fd_tprFix{};   //-- .tprFix 文件 fd.  init() 后一直处于开启状态
    int fd_tprVar{};   //-- .tprVar 文件 fd.  init() 后一直处于开启状态
    
    struct DB::tprDB_File_Head  tprFix_head; //- .tprFix 头部
    struct DB::tprDB_File_Head  tprVar_head; //- .tprVar 头部
    //------------------
    void _1_creat_and_save_file_head( int _fd, bool _is_fix ); 
                                //-- 新建 文件 head，并将它写入 文件中

    int check_tprDB_file_head( bool _is_fix );
    
    void init_DATA_T( std::initializer_list<DB::DATA_T> _field_types, 
                            DB::len_t _DATA_T_size ); 
                    //-- 通过此函数，调用者 传入 DATA_T 中每个 元素的 类型
                    //-- 必须使用 tprDB 提供的 类型组:


    //++++++++++++++++|  DATA_T  |++++++++++++++++//
    std::vector<DB::Field_T>  field_types;   //- 一个类型序列，记录了 DATA_T 中每个元素的类型。
    DB::len_t  fixData_bytes {}; //-- fix DATA_T 字节数 

    bool  is_have_VAR {true}; //- 是否创建 var 文件  -[自动识别]-
    bool  is_have_FIX {true}; //- 是否创建 fix 文件. -[手动设置]-
    DB::DB_TYPE   db_type; //- 数据库类型



    //++++++++++++++++|  child DB  |++++++++++++++++//
    //-- 两个 子容器，分别存储 定长数据 和 变长数据。
    tprDB_Orig   fixDB { true }; //-- 定长，DATA_T数据 本体存储 容器
    tprDB_Orig   varDB { false }; //-- 变长，字符串 存储容器
                            //  这个实例 不一定会被完全创建。若 DATA_T 中没有 变长数据
                            //  这个实例 就可以是个 空壳。

};


}//------------------- namespace: tpr ------------------------//
#endif


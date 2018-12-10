/*
 * ========================= tprDB.cpp ==========================
 *                          -- tpr --
 *                                        创建 -- 2018.11.10
 *                                        修改 -- 2018.11.10
 * ----------------------------------------------------------
 * 
 *   一个 不支持异步访问的 小型数据库
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
#include "tprDB.h"

//------------ C -------------//
#include <string.h> //- memcpy
#include <cassert> //- assert
#include <errno.h> //- errno

//------------ CPP -------------//

using std::cout;
using std::endl;
using std::string;

namespace tpr { //--------------- namespace: tpr -------------------//

using DB::DATA_T_2_bytes;

using DB::eid_t;
using DB::len_t;

using DB::Field_T;

using DB::tprDB_File_Head;
using DB::TPRDB_MAGIC_NUM_BYTES;
using DB::FILE_FIXOPTION_LEN;

using DB::file_FixOptions;
using DB::mk_options;

using DB::DB_TYPE;

/* ===========================================================
 *                   init_DATA_T 
 * -----------------------------------------------------------
 * -- 此函数应该先于 init 函数调用，
 * 
 * -- 通过此函数，获得 DATA_T 的几乎所有信息。
 *    包括 每个字段 在 struct 中的 基址，字节数，类型。
 *    以及 DATA_T struct 自己的 字节数
 * 
 * -- param: _field_types  DATA_T struct 中，所有字段的 类型
 * -- param: _DATA_T_size  DATA_T struct 的字节数。
 */
void tprDB::init_DATA_T( std::initializer_list<DB::DATA_T> _field_types,
                                DB::len_t _DATA_T_size ){

    //------------------------------//
    //   组装出 所有 field_type, 存入容器
    //------------------------------//
    field_types.clear();
    len_t  base = 0; //- tmp
    for( const auto &i : _field_types ){

        DB::Field_T field;
        field.data_type = i;
        field.base = base;
        field.len = DATA_T_2_bytes(i);

        field_types.push_back( field ); //-- 复制，暂不优化
        //-------
        base += field.len;
    }
    //------------------------------//
    //       遍历所有 filed 类型
    // -- 确保 类型排序符合：
    //     8, 8, 8... 4, 4, 4...
    // -- 计数 有多少个 4-bytes 类型
    // -- 查看 有无 STRID 类型
    //------------------------------//
    is_have_VAR = false;
    int   nr_of_4 = 0; //-- 类型序列中，有多少个 4-bytes
    bool  find_4 = false;
    for( const auto &i : field_types ){

        assert( !((find_4==true)&&(i.len==8)) );
                //-- 若排序出错，属于 调用者代码错误，直接报错终止
        if( i.len == 4 ){
            find_4 = true;
            nr_of_4++;
        }

        if( i.data_type == DB::DATA_T::STRID ){
            is_have_VAR = true;
        }
    }
    //------------------------------//
    //   校准／获得 D_size
    //------------------------------//
    auto last = field_types.back();
    assert( (last.base+last.len) < 65536 ); //- 不越界
    len_t  D_size = last.base + last.len;

    //-- 若有 奇数个 4-bytes 类型 --
    //-- DATA_T struct 需要加上 padding：4-bytes
    if( (nr_of_4%2)!=0 ){
        D_size += 4;
    }
    assert( D_size == _DATA_T_size );
    //------------------------------//
    //     初始化 fixData_bytes
    //------------------------------//
    fixData_bytes = D_size;

    //------------------------------//
    //     初始化  db_type
    //------------------------------//
    if( is_have_FIX && is_have_VAR ){
        db_type = DB_TYPE::Regular;
    }else if( is_have_FIX ){
        db_type = DB_TYPE::Pure_Fix;
    }else{
        db_type = DB_TYPE::Pure_Var;
    }
}

/* ===========================================================
 *                     init  
 * -----------------------------------------------------------
 */
void tprDB::init(const std::string &_path_dir_parent,
                const std::string &_DBname,
                std::initializer_list<DB::DATA_T> _field_types,
                DB::len_t _DATA_T_size ){

    std::string err_info = "tprDB::init(): ";
    //--------------------------------------//
    //               file 
    //--------------------------------------//
    path_dir_parent = _path_dir_parent;
    DBname = _DBname;

    //--------------------------------------//
    //          init_DATA_T
    //--------------------------------------//
    init_DATA_T( _field_types, _DATA_T_size );
  
    //--------------------------------------//
    //   确保 path_dir_parent 是 已存在的 目录
    //   通常为: .../data/
    //--------------------------------------//
    Is_path_valid( path_dir_parent.c_str(), err_info );

    //--------------------------------------//
    //  查找／创建 path_dir_tprDB
    //   通常为: .../data/tprDB/
    //--------------------------------------//
    path_dir_tprDB = mk_dir( path_dir_parent.c_str(), 
                            "tprDB",
                            RWXR_XR_X,
                            err_info
                            );
    //--------------------------------------//
    //  查找／创建 path_dir_self
    //   通常为: .../data/tprDB/DBname/
    //--------------------------------------//
    path_dir_self = mk_dir( path_dir_tprDB.c_str(), 
                            DBname.c_str(),
                            RWXR_XR_X,
                            err_info
                            );
    //--------------------------------------//
    //  查找／创建 各个 文件
    //  通常为: 
    //     .../data/tprDB/DBname/DBname.tprVar
    //     .../data/tprDB/DBname/DBname.tprFix
    //--------------------------------------//
    std::string path_tmp;

    //--- .tprFix ---//
    if( is_have_FIX == true ){

        path_tmp = DBname + DB::SUFFIX_TPR_FIX;
        path_file_tprFix = path_combine( path_dir_self, path_tmp );
        fd_tprFix = Open( path_file_tprFix.c_str(), O_RDWR|O_CREAT, RW_R__R__, err_info);
    }

    //--- .tprVar ---//
    if( is_have_VAR == true ){

        path_tmp = DBname + DB::SUFFIX_TPR_VAR;
        path_file_tprVar = path_combine( path_dir_self, path_tmp );
        fd_tprVar = Open( path_file_tprVar.c_str(), O_RDWR|O_CREAT, RW_R__R__, err_info);
    }

    //--------------------------------------//
    //    制作 本数据库实例的 唯一 checksum
    //--------------------------------------//
    mk_checksum();
    //--------------------------------------//
    //    检查 .tprFix 文件 的 头部
    //--------------------------------------//
    if( is_have_FIX == true ){

        DB::file_FixOptions  opt; 
        std::string   str_fix;
        len_t         d_size = fixData_bytes;

        switch( check_tprDB_file_head(true) ){
            case 1: //--- 文件头部正确
                str_fix = "tprFix_head is corrent. ";
                break;

            case 0: //--- 文件是空的（新创建的）
                str_fix = "file .tprFix is empty. ";

                //--- 制作并写入 head段 ---//
                _1_creat_and_save_file_head( fd_tprFix, true );

                //--- 制作并写入 opt段 ---//
                opt = mk_options( d_size );
                Lseek( fd_tprFix, (off_t)sizeof(tprDB_File_Head), SEEK_SET, err_info );
                Write( fd_tprFix, (const void*)&opt, FILE_FIXOPTION_LEN, err_info );
                break;

            case -1: //--- 文件长度 短于 头部
            case -2: //--- 魔数 不正确
            case -3: //--- 魔数 正确，但 checksum 不正确
                //-- 推荐的处理:
                //--     将错误文件 重新命名，将错误记录到 log文件
                //--     制作一个新的 .tprFix 文件
            default: //--- 其他值（不可能，直接报错终止吧）
                assert(0);
                break;
        }

        //--- fix init ---//
        fixDB.init( fd_tprFix, path_file_tprVar, d_size );
        cout << str_fix << endl;
    }

    //--------------------------------------//
    //    检查 .tprVar 文件 的 头部
    //--------------------------------------//
    if( is_have_VAR == true ){

        std::string str_var;
        switch( check_tprDB_file_head(false) ){
            case 1: //--- 文件头部正确
                str_var = "tprVar_head is corrent. ";
                break;

            case 0: //--- 文件是空的（新创建的）
                str_var = "file .tprVar is empty. ";

                //--- 制作并写入 head段 ---//
                _1_creat_and_save_file_head( fd_tprVar, false );
                break;

            case -1: //--- 文件长度 短于 头部
            case -2: //--- 魔数 不正确
            case -3: //--- 魔数 正确，但 checksum 不正确
                //-- 推荐的处理:
                //--     将错误文件 重新命名，将错误记录到 log文件
                //--     制作一个新的 .tprVar 文件
            default: //--- 其他值（不可能，直接报错终止吧）
                assert(0);
                break;
        }
        //--- var init ---//
        varDB.init( fd_tprVar, path_file_tprVar );
        cout << str_var << endl;
    }
}

/* ========================================================
 *               check_tprDB_file_head   
 * --------------------------------------------------------
 * -- 检查 文件（头部），确认其数据和格式 是否正确。
 *    这个函数，会将 文件的 各种状态 如实表现在 返回值中，
 *    至于具体处理手段，由调用者实现。
 * 
 * -- param: _is_fix -- 二选一，确定 检验的 魔数字符串 
 * 
 * -- return:
 *   若文件头部正常，
 *              -- 返回 1. 并将 读取的 头部，存入 hp 中
 *   若 文件长度为0. 说明这个 数据库实例是新创建的。
 *              -- 返回 0.
 *   若文件太短，长度小于 头部长度，说明此文件数据不匹配，
 *              -- 返回 -1
 *   若 文件头部的 magic_num 不正确。
 *              -- 返回 -2
 *   若 文件头部的 magic_num 正确，但是 checksum 不匹配
 *      说明，这个文件至少是一个 tprDB 文件。
 *      只是出于某种原因，它并不是 本实例 拥有的 文件
 *              -- 返回 -3 
 */
int tprDB::check_tprDB_file_head( bool _is_fix ){
    
    std::string err_info = "check_tprDB_file_head(): ";

    //--------- FIX or VAR ---------//
    int              fd; 
    tprDB_File_Head *hp; 
    if( _is_fix == true ){
        fd = fd_tprFix;
        hp = &tprFix_head;
    }else{
        fd = fd_tprVar;
        hp = &tprVar_head;
    }

    //---- 获得 目标文件 字节数 -----//
    off_t file_len = get_file_size( fd, err_info );

    if( file_len == 0 ){
        //-- 新创建的 数据库实例 --//
        return 0;

    }else if( file_len < sizeof(tprDB_File_Head) ){
        //-- 如果 文件长度 小于 头部长度，说明文件数据不匹配 --//
        std::cout << err_info << "WARNNING. "
            << "file_len < sizeof(tprDB_File_Head). "
            << std::endl;
        return (-1);
    }
    //------------------------------------//
    //          检查 magic_num
    //------------------------------------//
    tprDB_File_Head head; //-- 存放从 文件中读取的 head 

    //-- 通过参数 _is_fix 确定比较的 字符串
    std::string magic_str;
    if( _is_fix == true ){
        magic_str = DB::FIX_MAGIC_NUM; //-- .tprFix 版 魔数字符串
    }else{
        magic_str = DB::VAR_MAGIC_NUM; //-- .tprVar 版 魔数字符串
    }

    //-- 暂存 旧的 “当前文件偏移量／current file offset”
    off_t cfo_old = Lseek( fd, 0, SEEK_CUR, err_info );

    Lseek( fd, 0, SEEK_SET, err_info ); //-- 将 当前偏移量 指向文件开头
    //-- 读取完整的 头部
    Read( fd, 
        (void*)&head, 
        sizeof(tprDB_File_Head), 
        err_info 
        ); 
    //-- 对 魔数 逐个字符地检查 --//
    //-- 一旦发现不匹配，立即返回
    for( size_t i=0; i<TPRDB_MAGIC_NUM_BYTES; i++ ){
        if( head.magic_num[i] != *(magic_str.c_str()+i) ){
            return (-2);
        }
    }
    //------------------------------------//
    //          检查 checksum
    //------------------------------------//
    if( cmp_checksum128( check_sum, head.check_sum ) == false ){
        return (-3);
    }
    //------------------------------------//
    //     一切都正确，说明文件没问题
    //------------------------------------//
    //-- 正式将 头部 复制到 参数提供的 容器 中
    memcpy( (void*)hp,
            (void*)&head,
            sizeof(tprDB_File_Head)
            );
    
    //-- 将 当前偏移量 设置回 原来的值。
    Lseek( fd, cfo_old, SEEK_SET, err_info );

    return 1; //-- 检测正确
}

/* ===========================================================
 *                    mk_checksum
 * -----------------------------------------------------------
 * -- 原材料：
 *   1-- path_dir_self -- 路径名 字符串
 *   2-- field_types   -- DATA_T 中，每个字段的类型信息
 */
void tprDB::mk_checksum(){

    size_t path_len = path_dir_self.size();

    //-- 制作 checksum 所依赖的 原始数据的 字节数 --//
    size_t buf_len = path_len + sizeof(Field_T) * field_types.size();

    //--- 制作 原始数据 -----//
    size_t off = 0; //- tmp
    u8 buf[ buf_len ]; 
    memcpy( (void*)buf,
            (void*)path_dir_self.c_str(),
            path_len
            );
    off += path_len;

    for( const auto &i : field_types ){

        memcpy( (void*)&(buf[off]),
                (void*)&i,
                sizeof(Field_T)
                );
        off += sizeof(Field_T);
    }

    //--- 制作 checksum ----//
    check_sum = mk_checksum128( buf, buf_len );
}

/* ===========================================================
 *            _1_creat_and_save_file_head
 * -----------------------------------------------------------
 * -- 调用此函数前，请确保 .tprFix / .tprVar 文件为空
 */
void tprDB::_1_creat_and_save_file_head( int _fd, bool _is_fix ){

    std::string err_info = "tprDB::_1_creat_and_save_file_head(): ";

    //------ 新建 head ----//
    std::string s;
    if( _is_fix == true ){
        s = DB::FIX_MAGIC_NUM;
        memcpy( (void*)tprFix_head.magic_num,
            (void*)(s.c_str()),
            TPRDB_MAGIC_NUM_BYTES
            );
        tprFix_head.check_sum = check_sum;

    }else{
        s = DB::VAR_MAGIC_NUM;
        memcpy( (void*)tprVar_head.magic_num,
            (void*)(s.c_str()),
            TPRDB_MAGIC_NUM_BYTES
            );
        tprVar_head.check_sum = check_sum;
    }

    //----- 将 head 写入 对应文件头部 -----
    Lseek( _fd, 0, SEEK_SET, err_info ); //-- 将 当前偏移量 指向 对应文件文件头部
    if( _is_fix == true ){
        Write( _fd, (void*)&tprFix_head, sizeof(tprDB_File_Head), err_info );
    }else{
        Write( _fd, (void*)&tprVar_head, sizeof(tprDB_File_Head), err_info );
    }
}

/* ===========================================================
 *                   check_id
 * -----------------------------------------------------------
 */
bool tprDB::check_id( bool _is_fix, eid_t _id ){

    if( _is_fix == true ){
        assert( is_have_FIX == true );
        return fixDB.check_id( _id );

    }else{
        assert( is_have_VAR == true );
        return varDB.check_id( _id );
    }
}


/* ===========================================================
 *                   ent_nums
 * -----------------------------------------------------------
 */
size_t tprDB::ent_nums(){

    if( db_type == DB::DB_TYPE::Pure_Var ){
        return varDB.ent_nums();
    }else{
        return fixDB.ent_nums();
    }
}

/* ===========================================================
 *                   get_fst_id
 * -----------------------------------------------------------
 */
DB::eid_t tprDB::get_fst_id(){

    if( db_type == DB::DB_TYPE::Pure_Var ){
        return varDB.get_fst_id();
    }else{
        return fixDB.get_fst_id();
    }
}

/* ===========================================================
 *                      insert
 * -----------------------------------------------------------
 */
DB::eid_t tprDB::insert( bool _is_fix, const void *_buf, DB::len_t _len ){

    if( _is_fix == true ){
        assert( is_have_FIX == true );
        return fixDB.insert( _buf, _len );
    
    }else{
        assert( is_have_VAR == true );
        return varDB.insert( _buf, _len );
    }
}

/* ===========================================================
 *                     erase
 * -----------------------------------------------------------
 * -- 先把 所有 var字段 从 varDB 中删除
 * -- 再从 fixDB 中，删除本 _id 指向的 ent
 */
int tprDB::erase( DB::eid_t _id ){

    if( db_type == DB_TYPE::Pure_Var ){
        return varDB.erase(_id);

    }else if( db_type == DB_TYPE::Pure_Fix ){
        return fixDB.erase(_id);
    }

    //-------- Regular --------//
    std::vector<u8>  fix_data;
    fix_data.resize( fixData_bytes );

    eid_t  str_id = 0;
    //-- 先读取 entData --
    ssize_t rlen;
    rlen = fixDB.read( _id,
                    (void*)&(fix_data[0]),
                    (size_t)fix_data.size(),
                    true
                    );
    assert( rlen == (ssize_t)fix_data.size() );
    //--- 删除 所有 var字段 ---//
    for( const auto &ft : field_types ){

        if( is_DATA_T_a_STRID(ft.data_type) == true ){
            str_id = *( (eid_t*)&(fix_data[ft.base]) );
            assert( varDB.erase(str_id) == 0 );
        }
    }
    //--- 删除 fix ent ---//
    return fixDB.erase(_id);
}

/* ===========================================================
 *                      write
 * -----------------------------------------------------------
 * -- PURE_VAR / PURE_FIX 专用
 */
ssize_t tprDB::write( bool _is_fix, DB::eid_t _id, const void *_buf, const DB::len_t _len ){

    if( _is_fix == true ){
        assert( is_have_FIX == true );
        return fixDB.write( _id, _buf, _len );
    
    }else{
        assert( is_have_VAR == true );
        return varDB.write( _id, _buf, _len );
    }
}

/* ===========================================================
 *                      read
 * -----------------------------------------------------------
 * -- param: _is_fast -- 快速模式，此时将不会把 enthead 写回文件
 *                     而是设置 dirty位。通常用在 write前的 预read
 */
ssize_t tprDB::read( bool _is_fix, DB::eid_t _id, void *_buf, size_t _len, bool _is_fast ){

    if( _is_fix == true ){
        assert( is_have_FIX == true );
        return fixDB.read( _id, _buf, _len, _is_fast );

    }else{
        assert( is_have_VAR == true );
        return varDB.read( _id, _buf, _len, _is_fast );
    }
}

/* ===========================================================
 *                 get_entData_len
 * -----------------------------------------------------------
 */
ssize_t tprDB::get_entData_len( bool _is_fix, DB::eid_t _id ){

    if( _is_fix == true ){
        assert( is_have_FIX == true );
        return fixDB.get_entData_len( _id );

    }else{
        assert( is_have_VAR == true );
        return varDB.get_entData_len( _id );
    }
}

/* ===========================================================
 *                      flush
 * -----------------------------------------------------------
 */
void tprDB::flush(){

    if( is_have_FIX == true ){
        fixDB.flush();
    }

    if( is_have_VAR == true ){
        varDB.flush();
    }
}

/* ===========================================================
 *                   info
 * -----------------------------------------------------------
 */
void tprDB::info( bool _fix, bool _var ){

    if( _fix == true ){
        assert( is_have_FIX == true );
        cout << "\n\n|||||=== FIX ===|||||" << endl;
        fixDB.info();
    }

    if( _var == true ){
        assert( is_have_VAR == true );
        cout << "\n\n|||||=== VAR ===|||||" << endl;
        varDB.info();
    }
}


}//------------------- namespace: tpr ------------------------//

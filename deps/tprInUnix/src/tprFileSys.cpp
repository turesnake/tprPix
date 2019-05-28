/*
 * ========================= tprFileSys.cpp ==========================
 *                          -- tpr --
 *                                        创建 -- 2018.10.14
 *                                        修改 -- 2018.10.14
 * ----------------------------------------------------------
 *    专门存放 文件系统 相关的 函数
 * ----------------------------
 *    依赖的其它库：
 *         wrapFuncs
 * ----------------------------
 */


//-------------------- C --------------------//
#include <cerrno> //- errno.h
#include <cstring> //- strerror
#include <unistd.h> //- size_t, PATH_MAX, _PC_PATH_MAX

//#include <stdio.h> //- perror, FILE
#include <cstdio> 

#include <sys/stat.h> //- stat

#include <cassert> //- assert

//-------------------- CPP --------------------//
#include <iostream> //-- cout
#include <string>
#include <sstream>

//-------------------- SELF --------------------// 
#include "wrapFuncs.h"


using std::cout;
using std::endl;
using std::string;

namespace tpr {//--------------- namespace: tpr -------------------//


//------------------- 提供给外部的 函数 ----------------
int check_st_mode( mode_t _mode );

int check_path_st_mode( const char *_path );
int check_path_st_mode( int _fd );
int check_path_st_mode( int _fd, const char *_path );

bool is_path_valid( const char *_path );
bool is_path_valid( int _fd );
bool is_path_valid( int _fd, const char *_path );

void Is_path_valid( const char *_path, const std::string &_err_info );
void Is_path_valid( int _fd, const std::string &_err_info );
void Is_path_valid( int _fd, const char *_path, const std::string &_err_info );

int is_path_a_dir( const char *_path );
int is_path_a_dir( int _fd );
int is_path_a_dir( int _fd, const char *_path );

void Is_path_a_dir( const char *_path, const std::string &_err_info );
void Is_path_a_dir( int _fd, const std::string &_err_info );
void Is_path_a_dir( int _fd, const char *_path, const std::string &_err_info );

void mk_dir( const char *_path, mode_t _mode,
            const std::string &_err_info );
const std::string mk_dir( const char *_path_dir, const char * _name, mode_t _mode,
            const std::string &_err_info );
void mk_dir( int _fd, const char * _name, mode_t _mode,
            const std::string &_err_info );

const std::string path_combine( const std::string &_pa, const std::string &_pb );
const std::string path_combine( const char *_pa, const char *_pb );

bool is_path_not_too_long( const std::string &_path );
void Is_path_not_too_long( const std::string &_path,
                            const std::string &_err_info );

off_t get_file_size( int _fd, const std::string &_err_info );
off_t get_file_size( FILE *_fp, const std::string &_err_info );
off_t get_file_size( const char *_path, const std::string &_err_info );


//------------------- 局部 函数 ----------------
namespace{

    std::stringstream ss;

    //===== funcs =====//
    bool _is_path_valid_result_check( int _r, int _fd, const char *_path );
    int  _is_path_a_dir_result_check( int _r, int _fd, const char *_path );

}

//------------------- 局部变量 ----------------
#define TPR_PATH_MAX  1024 //-- 路径名极限长度（各系统不同）
                           //-- 在此偷一个懒，选择了 几个unix 系统中的 最小值



/* ===========================================================
 *                 check_st_mode
 * -----------------------------------------------------------
 * -- 检查 st_mode 指向 什么类型的 文件
 *  return:
 *      若 _mode 存在问题，直接报错终止
 *      若 _mode 正确， 返回不同类型 对应的 int 值
 */
int check_st_mode( mode_t _mode ){

    if(       S_ISREG(_mode)  ){ return 1;
    }else if( S_ISDIR(_mode)  ){ return 2;
    }else if( S_ISCHR(_mode)  ){ return 3;
    }else if( S_ISBLK(_mode)  ){ return 4;
    }else if( S_ISFIFO(_mode) ){ return 5;
    }else if( S_ISSOCK(_mode) ){ return 6;
    }else if( S_ISLNK(_mode)  ){ return 7;
    }else{
        //-- 什么类型都不是，说明 _mode 有问题
        cout << "check_st_mode: ERROR. _mode is invalid. " 
            << endl;
        assert(0);
    }
}



/* ===========================================================
 *                  check_path_st_mode  [1]
 * -----------------------------------------------------------
 * -- 检查 _path 的 stat.st_mode
 *  return:
 *      若 调用 lstat 出错，返回 errno 的 负值。
 *      若 _path 确实指向一个文件，
 *          且为  常规文件   返回 1
 *          且为  目录      返回 2
 *          且为  字符设备   返回 3
 *          且为  块设备    返回 4
 *          且为  FIFO     返回 5
 *          且为  socket   返回 6
 *          且为  符号链接   返回 7 
 */
int check_path_st_mode( const char *_path ){
    errno = 0; //-- 清空 errno
    struct stat st;
    if( lstat(_path, &st) == -1 ){
        return (-errno);  //-- errno 的负值
    }else{
        return check_st_mode( st.st_mode );
    }
}

/* ===========================================================
 *                  check_path_st_mode  [2]
 * -----------------------------------------------------------
 * -- 重载函数，参数改为 _fd
 */
int check_path_st_mode( int _fd ){
    errno = 0; //-- 清空 errno
    struct stat st;
    if( fstat(_fd, &st) == -1 ){
        return (-errno);  //-- errno 的负值
    }else{
        return check_st_mode( st.st_mode );
    }
}


/* ===========================================================
 *                  check_path_st_mode  [3]
 * -----------------------------------------------------------
 * -- 重载函数，参数改为 (_fd, _path)
 */
int check_path_st_mode( int _fd, const char *_path ){
    errno = 0; //-- 清空 errno
    struct stat st;
    if( fstatat(_fd, _path, &st, AT_SYMLINK_NOFOLLOW) == -1 ){
        return (-errno);  //-- errno 的负值
    }else{
        return check_st_mode( st.st_mode );
    }
}


/* ===========================================================
 *             _is_path_valid_result_check  [static]
 * -----------------------------------------------------------
 * -- 为 所有 is_path_valid 函数 服务的函数，
 * -- 检测 check_path_st_mode 函数的 返回值，并提供对应的 结果
 * -- param: _r     -- check_path_st_mode 的返回值
 * -- param: _fd   -- 不需要时 写入 -1
 * -- param: _path -- 不需要时 写入 NULL
 * 
 *  return:
 *      若 _path 合法，    返回 true。
 *      若 _path格式不正确，返回 false
 *      若出现 系统级错误，直接 报错终止
 */
namespace{
bool _is_path_valid_result_check( int _r, int _fd, const char *_path ){

    int r = _r;
    if( r > 0 ){
        return true; //-- _path 确实指向某文件，为为有效路径名
    }else{
        r = -r; //-- 负值取反。
        switch( r ){
            case ENOENT:       //-  2 - _path 中某一段不存在，或 _path是空字符串
            case EBADF:        //-  9 - fd 是坏的。（目前不可能）
            case EACCES:       //- 13 - _path 中，某一级目录缺乏 执行／搜索权
            case ENOTDIR:      //- 20 - _path 的前缀 不是 目录
            case ENAMETOOLONG: //- 36 - _path 过长
            case ELOOP:        //- 40 - _path 的 符号链接 级数过多
                return false;

            default: //-- 剩余的都是 系统级error，直接报错终止
                cout << "is_path_valid: lstat: ERROR. " 
                    << "errno = " << r << " " << strerror(r) 
                    << endl;
                if( _fd != -1 ){
                    cout << "_fd = " << _fd << endl;
                }
                if( _path != NULL ){
                    cout << "_path = " << _path << endl;
                }
                assert(0);
        }
    }
}
}//----- namespace -----//


/* ===========================================================
 *                  is_path_valid  [1]
 * -----------------------------------------------------------
 * -- 检查 _path 是否 为有效路径名 
 *  return:
 *      若 _path 合法，    返回 true。
 *      若 _path格式不正确，返回 false
 *      若出现 系统级错误，直接 报错终止
 */
bool is_path_valid( const char *_path ){

    int r;
    r = check_path_st_mode( _path );
    return _is_path_valid_result_check( r, -1, _path );
}

/* ===========================================================
 *                  is_path_valid  [2]
 * -----------------------------------------------------------
 * -- 重载函数，参数改为 _fd
 */
bool is_path_valid( int _fd ){

    int r;
    r = check_path_st_mode( _fd );
    return _is_path_valid_result_check( r, _fd, NULL );
}

/* ===========================================================
 *                  is_path_valid  [3]
 * -----------------------------------------------------------
 * -- 重载函数，参数改为 (_fd, _path)
 */
bool is_path_valid( int _fd, const char *_path ){

    int r;
    r = check_path_st_mode( _fd, _path );
    return _is_path_valid_result_check( r, _fd, _path );
}


/* ===========================================================
 *                  Is_path_valid  [1]
 * -----------------------------------------------------------
 * -- is_path_valid 的 包裹函数，对于 无效的 _path, 直接报错终止
 * -- 此函数的 使用频率很高
 */
void Is_path_valid( const char *_path, const std::string &_err_info ){

    if( is_path_valid(_path) == false ){
        cout << "Is_path_valid: ERROR. "
            << "errno = " << errno << " " << strerror(errno) << "\n"
            << "_path: " << _path << "\n" 
            << _err_info
            << endl;
        assert(0);
    }
}

/* ===========================================================
 *                  Is_path_valid   [2]
 * -----------------------------------------------------------
 * -- 重载函数，参数改为 _fd
 */
void Is_path_valid( int _fd, const std::string &_err_info ){

    if( is_path_valid(_fd) == false ){
        cout << "Is_path_valid: ERROR. "
            << "errno = " << errno << " " << strerror(errno) << "\n"
            << "_fd: " << _fd << "\n" 
            << _err_info
            << endl;
        assert(0);
    }
}

/* ===========================================================
 *                  Is_path_valid   [3]
 * -----------------------------------------------------------
 * -- 重载函数，参数改为 _fd
 */
void Is_path_valid( int _fd, const char *_path, const std::string &_err_info ){

    if( is_path_valid(_fd, _path) == false ){
        cout << "Is_path_valid: ERROR. "
            << "errno = " << errno << " " << strerror(errno) << "\n"
            << "_fd   = " << _fd << "\n" 
            << "_path = " << _path
            << _err_info
            << endl;
        assert(0);
    }
}


/* ===========================================================
 *                _is_path_a_dir_result_check   [static]
 * -----------------------------------------------------------
 * -- 为 所有 is_path_a_dir 函数 服务的函数，
 * -- 检测 check_path_st_mode 函数的 返回值，并提供对应的 结果
 * -- param: _r     -- check_path_st_mode 的返回值
 * -- param: _fd   -- 不需要时 写入 -1
 * -- param: _path -- 不需要时 写入 NULL
 */
namespace{
int _is_path_a_dir_result_check( int _r, int _fd, const char *_path ){

    int r = _r;
    if( r == 2 ){        
        return 1;  //--  _path 确实指向一个 目录

    }else if( r > 0 ){   
        return 0;  //--  _path 有效，但指向的文件 不是目录

    }else{
        r = -r; //-- 负值取反。
        switch( r ){
            case ENOENT:       //-  2 - _path 中某一段不存在，或 _path是空字符串
                return (-1);

            case EBADF:        //-  9 - fd 是坏的。（目前不可能）
            case EACCES:       //- 13 - _path 中，某一级目录缺乏 执行／搜索权
            case ENOTDIR:      //- 20 - _path 的前缀 不是 目录
            case ENAMETOOLONG: //- 36 - _path 过长
            case ELOOP:        //- 40 - _path 的 符号链接 级数过多
                return (-2);

            default: //-- 剩余的都是 系统级error，直接报错终止
                cout << "is_path_a_dir: lstat: ERROR. " 
                    << "errno = " << r << " " << strerror(r) << endl;
                if( _fd != -1 ){
                    cout << "_fd = " << _fd << endl;
                }
                if( _path != NULL ){
                    cout << "_path = " << _path << endl;
                }
                assert(0);
        }
    }
}
}//----- namespace -----//

/* ===========================================================
 *                  is_path_a_dir  [1]
 * -----------------------------------------------------------
 * -- 检查 _path 是否为 目录
 * -- return:
 *      若 _path 确实指向一个 目录，                 返回 1
 *      若 _path 有效，但指向的不是目录（其它文件类型） 返回 0
 *      若 _path 指向的地址没有文件／目录，ENOENT， 返回 -1. (当检查一个 尚未新建的 路径时，这种情况可能发送)
 *      若 _path 格式错误。                      返回 -2
 *      若 _path 格式没问题，但 调用lstat 发生系统级错误，直接报错终止
 * 
 */
int is_path_a_dir( const char *_path ){

    int r;
    r = check_path_st_mode( _path );
    return _is_path_a_dir_result_check( r, -1, _path );
}

/* ===========================================================
 *                  is_path_a_dir  [2]
 * -----------------------------------------------------------
 * -- 重载函数，参数改为 _fd
 */
int is_path_a_dir( int _fd ){

    int r;
    r = check_path_st_mode( _fd );
    return _is_path_a_dir_result_check( r, _fd, NULL );
}

/* ===========================================================
 *                  is_path_a_dir  [3]
 * -----------------------------------------------------------
 * -- 重载函数，参数改为 (_fd, _path)
 */
int is_path_a_dir( int _fd, const char *_path ){

    int r;
    r = check_path_st_mode( _fd, _path );
    return _is_path_a_dir_result_check( r, _fd, _path );
}


/* ===========================================================
 *                  Is_path_a_dir  [1]
 * -----------------------------------------------------------
 * -- is_path_a_dir 的 包裹函数，对于 不是目录的 _path, 直接报错终止
 */
void Is_path_a_dir( const char *_path, const std::string &_err_info ){

    if( is_path_a_dir(_path) != 1 ){
        cout << "Is_path_a_dir: ERROR. "
            << "errno = " << errno << " " << strerror(errno) << "\n"
            << "_path = " << _path << "\n" 
            << _err_info
            << endl;
        assert(0);
    }
}

/* ===========================================================
 *                  Is_path_a_dir  [2]
 * -----------------------------------------------------------
 * -- 重载函数，参数改为 _fd
 */
void Is_path_a_dir( int _fd, const std::string &_err_info ){

    if( is_path_a_dir(_fd) != 1 ){
        cout << "Is_path_a_dir: ERROR. "
            << "errno = " << errno << " " << strerror(errno) << "\n"
            << "_fd = " << _fd << "\n" 
            << _err_info
            << endl;
        assert(0);
    }
}

/* ===========================================================
 *                  Is_path_a_dir  [3]
 * -----------------------------------------------------------
 * -- 重载函数，参数改为 (_fd, _path)
 */
void Is_path_a_dir( int _fd, const char *_path, const std::string &_err_info ){

    if( is_path_a_dir(_fd, _path) != 1 ){
        cout << "Is_path_a_dir: ERROR. "
            << "errno = " << errno << " " << strerror(errno) << "\n"
            << "_fd   = " << _fd << "\n" 
            << "_path = " << _path << "\n" 
            << _err_info
            << endl;
        assert(0);
    }
}


/* ===========================================================
 *                       mk_dir  [1]
 * -----------------------------------------------------------
 * -- param: _path  -- 新目录的 绝对路径名
 * -- return:
 *       若 _path 尚未存在，执行 Mkdir，创新 新目录
 *       若 _path 指向出，已经存在一个文件，且是目录，说明目录已存在，直接返回
 *       若 _path 指向出，已经存在一个文件，但不是目录，报错终止
 *       若 _path 存在格式错误，或系统错误，报错终止
 */
void mk_dir( const char *_path, mode_t _mode,
            const std::string &_err_info ){
    
    string err_info = _err_info + "mk_dir(): ";
    //--------------------------
    int r;
    r = is_path_a_dir( _path );
    switch( r ){
        case  1:  //-- 目标目录已经创建了，直接跳出返回
            break;

        case  0:  //-- 存在一个同名的文件，但它不是目录
            cout << err_info << "ERROR. "
                << "find a file with same name, but it's not a dir. \n" 
                << "_path   = " << _path << " \n"
                << endl;
            assert(0);

        case -1:  //-- 可以执行 新目录 的创建工作
            Mkdir( _path, _mode, err_info );
            break;

        case -2:  //-- path 存在格式错误。（但这几乎不可能）
        default:  //-- is_path_a_dir() 不可能存在 其它返回值
            cout << err_info << "ERROR. "
                << "errno = " << errno << " " << strerror(errno) << "\n"
                << "_path   = " << _path << " \n"
                << endl;
            assert(0);
    }//--------- switch end ----------//
}

/* ===========================================================
 *                       mk_dir  [2]
 * -----------------------------------------------------------
 * -- 重载版，通过两个 路径名 合成 新目录的 绝对路径名
 * -- param: _path_dir  -- 父目录，在此目录下 创建新目录
 * -- param: _name      -- 新目录的 名字（不是路径名）
 */
const std::string mk_dir( const char *_path_dir, const char * _name, mode_t _mode,
            const std::string &_err_info ){
    
    string err_info = _err_info + "mk_dir(): ";
    string err_dir = err_info + "the parent dir is error. ";

    //--------------------------//
    //  确保 _path_dir 是有效的目录
    //--------------------------//
    Is_path_a_dir( _path_dir, err_dir );

    //--------------------------//
    //  合成 新目录的 绝对路径名 
    //--------------------------//
    string path; //-- 新目录的 绝对路径名
    string dir = _path_dir;
    string name = _name;
    path = path_combine( dir, name );
    //--------------------------//
    //  检测 新的目录 是否已存在 
    //--------------------------//
    mk_dir( path.c_str(), _mode, err_info ); //-- 调用 mk_dir-[1] 
    return path;
}


/* ===========================================================
 *                       mk_dir  [3]
 * -----------------------------------------------------------
 * -- 重载函数，参数改为 _fd
 */
void mk_dir( int _fd, const char * _name, mode_t _mode,
            const std::string &_err_info ){

    string err_info = _err_info + "mk_dir(): ";
    string err_dir = err_info + "the parent dir is error. ";

    //--------------------------//
    //  确保 _path_dir 是有效的目录
    //--------------------------//
    Is_path_a_dir( _fd, err_dir );

    //--------------------------//
    //  此重载 不需要 合成 绝对路径名
    //--------------------------//
    //  测试表明，参数 _name 写成 "/name", "name", "//name"
    //      都能正确运行。所以也不需要做 "/" 的检测
    //--------------------------//
    int r;
    r = is_path_a_dir( _fd, _name);
    switch( r ){
        case  1:  //-- 目标目录已经创建了，直接跳出返回
            break;

        case  0:  //-- 存在一个同名的文件，但它不是目录
            cout << err_info << "ERROR. "
                << "find a file with same name, but it's not a dir. \n" 
                << "_fd   = " << _fd << " \n"
                << "_name = " << _name << " \n"
                << endl;
            assert(0);

        case -1:  //-- 可以执行 新目录 的创建工作
            Mkdirat( _fd, _name, _mode, err_info );
            break;

        case -2:  //-- path 存在格式错误。（但这几乎不可能）
        default:  //-- is_path_a_dir() 不可能存在 其它返回值
            cout << err_info << "ERROR. "
                << "errno = " << errno << " " << strerror(errno) << "\n"
                << "_fd   = " << _fd << "\n" 
                << "_name = " << _name << "\n"
                << endl;
            assert(0);
    }//--------- switch end ----------//

    //-- 没想好 返回值类型，暂设为 void
}







/* ===========================================================
 *                       path_combine [1]
 * -----------------------------------------------------------
 * -- 将 _pa，_pb 合成一个 合法的 路径名。（主要是 处理中间的 '/' 问题）
 * -- 目前，不管调用者在 _pa，_pb 间写没写 '/', 都能正确合成。 
 */
const std::string path_combine( const std::string &_pa, const std::string &_pb ){

    string err_info = "path_combine(): ";
    //---------------------

    string path; //-- 最终合成的 路径名
    string pa = _pa;
    string pb = _pb;

    bool is_pa_has_slash = false;
    bool is_pb_has_slash = false;
    //---------------------
    if( pa.back() == '/' ){
        is_pa_has_slash = true;
    }
    if( pb.front() == '/' ){
        is_pb_has_slash = true;
    }
    //---------------------
    if( (is_pa_has_slash ^ is_pb_has_slash) == true ){ //-- 异或
        //---- 若 只有一个 拥有 '/' ----//
    }else if( is_pa_has_slash == true ){
        //---- 若 两个都有 '/' ----//
        pa.pop_back();
    }else{
        //---- 若 两个都无 '/' ----//
        pa += "/";
    }
    //---------------------
    path = pa + pb;
    //------ 检测 path 长度 -------
    Is_path_not_too_long( path, err_info );

    return path;
}


/* ===========================================================
 *                       path_combine [2]
 * -----------------------------------------------------------
 * -- 重载版本，参数有变，不推荐使用此版 
 */
const std::string path_combine( const char *_pa, const char *_pb ){

    string a = _pa;
    string b = _pb;
    return path_combine( a, b );
}


/* ===========================================================
 *                  nameString_combine
 * -----------------------------------------------------------
 * -- 拼接一种特殊的 string，类似 "dog_2_jump" 这种
 */
const std::string nameString_combine(   const std::string &_prefix,
                                        size_t _idx,
                                        const std::string &_suffix ){
    ss.str("");
    ss << _idx;
    return (_prefix + ss.str() + _suffix);
}



/* ===========================================================
 *                   is_path_not_too_long
 * -----------------------------------------------------------
 * -- 返回 true    表示 长度允许
 * -- 返回 false， 表示 太长了
 */
bool is_path_not_too_long( const std::string &_path ){

    //-- TPR_PATH_MAX 里包含了 尾后0 的字节
    if( (_path.size()+1) <= TPR_PATH_MAX ){ 
        return true;
    }else{
        return false;
    }
}

/* ===========================================================
 *                   Is_path_not_too_long
 * -----------------------------------------------------------
 * -- 检测 _path 长度，过长的 直接报错终止
 */
void Is_path_not_too_long( const std::string &_path,
                            const std::string &_err_info ){

    //-- TPR_PATH_MAX 里包含了 尾后0 的字节
    if( (_path.size()+1) > TPR_PATH_MAX ){ 
        cout << "Is_path_not_too_long: ERROR. "
            << _err_info
            << "_path =" << _path  
            << endl;
        assert(0);
    }
}


/* ===========================================================
 *                   get_file_size  [1]
 * -----------------------------------------------------------
 * -- 获得 文件的 字节数。fd版
 */
off_t get_file_size( int _fd, const std::string &_err_info ){

    string err_info = _err_info + "get_file_size(1): ";
    //-----------------------------
    //-- 暂存 旧的 “当前文件偏移量／current file offset”
    off_t cfo = Lseek( _fd, 0, SEEK_CUR, err_info );
    //-- 将 当前偏移量 指向 文件最后一字节, 同时获得 此字节的 偏移量数值（我们要的）
    off_t r = Lseek( _fd, 0, SEEK_END, err_info );
    //-- 将 当前偏移量 设置回 原来的值。
    Lseek( _fd, cfo, SEEK_SET, err_info );
    return r;
}

/* ===========================================================
 *                   get_file_size  [2]
 * -----------------------------------------------------------
 * -- 重载2，FILE版
 */
/*
off_t get_file_size( FILE *_fp, const std::string &_err_info ){

    string err_info = _err_info + "get_file_size(2): ";
    //-----------------------------
    //-- 暂存 旧的 “当前文件偏移量／current file offset”
    off_t cfo = Ftello( _fp, err_info );

    Fseeko( _fp, 0, SEEK_END, err_info );//-- 指向 文件尾字节
    off_t r = Ftello( _fp, err_info ); //-- 获得 文件尾字节 的偏移量
    Fseeko( _fp, cfo, SEEK_SET, err_info ); //-- 恢复为 旧的偏移量
    return r;
}
*/

/* ===========================================================
 *                   get_file_size  [3]
 * -----------------------------------------------------------
 * -- 重载3，path版
 */
off_t get_file_size( const char *_path, const std::string &_err_info ){

    string err_info = _err_info + "get_file_size(3): ";
    //-----------------------------
    Is_path_valid( _path, err_info ); //-- 确保 _path 指向 已存在文件
    int fd = Open( _path, O_RDONLY, 0, err_info ); //-- 获得 fd

    off_t r = get_file_size( fd, _err_info );
    Close( fd, err_info );
    return r;
}

/* ===========================================================
 *                        file_load
 * -----------------------------------------------------------
 * -- 输入 文件路的 绝对路径名。
 *    打开并读取此文件，获得其数据
 * -- param: _pathp -- 目标文件 绝对路径名
 * -- param: _buf   -- 获得的文件数据 存入buf. (此buf的原数据将被丢弃)
 *
 */
void file_load( const char *_pathp, std::string &_buf ){

    string err_info = "file_load(): ";
    //-------
    int fd;
    off_t flen;   //- 文件 字节数
    ssize_t rlen; //- 实际读取 字节数
    
    flen = get_file_size( _pathp, err_info );
    _buf.resize( flen );
    fd = Open( _pathp, O_RDONLY, 0, err_info );
    rlen = Read( fd, (void*)_buf.c_str(), flen, err_info );
    assert( rlen == (ssize_t)flen );

    Close( fd, err_info );
}





}//------------------- namespace: tpr ------------------------//

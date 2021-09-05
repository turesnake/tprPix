/*
 * =================== tprFileSys_unix.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2018.10.14
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *    专门存放 文件系统 相关的 函数
 * ----------------------------
 */
#include "tprFileSys_unix.h"

//-------------------- C --------------------//
#include <cerrno> //- errno.h
#include <cstring> //- strerror
#include <unistd.h> //- size_t, PATH_MAX, _PC_PATH_MAX

#include <cstdio> 

#include <sys/stat.h> //- stat

#include <cassert> //- assert

//-------------------- CPP --------------------//
#include <iostream> //-- cout
#include <string>
#include <sstream>

//-------------------- SELF --------------------// 
#include "wrapUnix.h"


#include "tprGeneral.h"


using std::cout;
using std::endl;

namespace tprUnix {//--------------- namespace: tprUnix -------------------//


namespace {

    //std::stringstream ss;
    //===== funcs =====//
    bool _is_path_valid_result_check( int r_, int fd_, const char *path_ );
    int  _is_path_a_dir_result_check( int r_, int fd_, const char *path_ );

}

//------------------- 局部变量 ----------------
#define TPR_PATH_MAX  1024 //-- 路径名极限长度（各系统不同）
                           //-- 在此偷一个懒，选择了 几个unix 系统中的 最小值



/* ===========================================================
 *                 check_st_mode
 * -----------------------------------------------------------
 * -- 检查 st_mode 指向 什么类型的 文件
 *  return:
 *      若 mode_ 存在问题，直接报错终止
 *      若 mode_ 正确， 返回不同类型 对应的 int 值
 */
int check_st_mode( mode_t mode_ ){

    if(       S_ISREG(mode_)  ){ return 1;
    }else if( S_ISDIR(mode_)  ){ return 2;
    }else if( S_ISCHR(mode_)  ){ return 3;
    }else if( S_ISBLK(mode_)  ){ return 4;
    }else if( S_ISFIFO(mode_) ){ return 5;
    }else if( S_ISSOCK(mode_) ){ return 6;
    }else if( S_ISLNK(mode_)  ){ return 7;
    }else{
        //-- 什么类型都不是，说明 mode_ 有问题
        cout << "check_st_mode: ERROR. mode_ is invalid. " 
            << endl;
        assert(0);
        return 0; //- never reach
    }
}



/* ===========================================================
 *                  check_path_st_mode  [1]
 * -----------------------------------------------------------
 * -- 检查 path_ 的 stat.st_mode
 *  return:
 *      若 调用 lstat 出错，返回 errno 的 负值。
 *      若 path_ 确实指向一个文件，
 *          且为  常规文件   返回 1
 *          且为  目录      返回 2
 *          且为  字符设备   返回 3
 *          且为  块设备    返回 4
 *          且为  FIFO     返回 5
 *          且为  socket   返回 6
 *          且为  符号链接   返回 7 
 */
int check_path_st_mode( const char *path_ ){
    errno = 0; //-- 清空 errno
    struct stat st;
    if( lstat(path_, &st) == -1 ){
        return (-errno);  //-- errno 的负值
    }else{
        return check_st_mode( st.st_mode );
    }
}

/* ===========================================================
 *                  check_path_st_mode  [2]
 * -----------------------------------------------------------
 * -- 重载函数，参数改为 fd_
 */
int check_path_st_mode( int fd_ ){
    errno = 0; //-- 清空 errno
    struct stat st;
    if( fstat(fd_, &st) == -1 ){
        return (-errno);  //-- errno 的负值
    }else{
        return check_st_mode( st.st_mode );
    }
}


/* ===========================================================
 *                  check_path_st_mode  [3]
 * -----------------------------------------------------------
 * -- 重载函数，参数改为 (fd_, path_)
 */
int check_path_st_mode( int fd_, const char *path_ ){
    errno = 0; //-- 清空 errno
    struct stat st;
    if( fstatat(fd_, path_, &st, AT_SYMLINK_NOFOLLOW) == -1 ){
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
 * -- param: r_     -- check_path_st_mode 的返回值
 * -- param: fd_   -- 不需要时 写入 -1
 * -- param: path_ -- 不需要时 写入 NULL
 * 
 *  return:
 *      若 path_ 合法，    返回 true。
 *      若 _path格式不正确，返回 false
 *      若出现 系统级错误，直接 报错终止
 */
namespace{
bool _is_path_valid_result_check( int r_, int fd_, const char *path_ ){

    int r = r_;
    if( r > 0 ){
        return true; //-- path_ 确实指向某文件，为为有效路径名
    }else{
        r = -r; //-- 负值取反。
        switch( r ){
            case ENOENT:       //-  2 - path_ 中某一段不存在，或 _path是空字符串
            case EBADF:        //-  9 - fd 是坏的。（目前不可能）
            case EACCES:       //- 13 - path_ 中，某一级目录缺乏 执行／搜索权
            case ENOTDIR:      //- 20 - path_ 的前缀 不是 目录
            case ENAMETOOLONG: //- 36 - path_ 过长
            case ELOOP:        //- 40 - path_ 的 符号链接 级数过多
                return false;

            default: //-- 剩余的都是 系统级error，直接报错终止
                cout << "is_path_valid: lstat: ERROR. " 
                    << "errno = " << r << " " << strerror(r) 
                    << endl;
                if( fd_ != -1 ){
                    cout << "fd_ = " << fd_ << endl;
                }
                if( path_ != NULL ){
                    cout << "path_ = " << path_ << endl;
                }
                assert(0);
                return false; //- never reach
        }
    }
}
}//----- namespace -----//


/* ===========================================================
 *                  is_path_valid  [1]
 * -----------------------------------------------------------
 * -- 检查 path_ 是否 为有效路径名 
 *  return:
 *      若 path_ 合法，    返回 true。
 *      若 _path格式不正确，返回 false
 *      若出现 系统级错误，直接 报错终止
 */
bool is_path_valid( const char *path_ ){

    int r;
    r = check_path_st_mode( path_ );
    return _is_path_valid_result_check( r, -1, path_ );
}

/* ===========================================================
 *                  is_path_valid  [2]
 * -----------------------------------------------------------
 * -- 重载函数，参数改为 fd_
 */
bool is_path_valid( int fd_ ){

    int r;
    r = check_path_st_mode( fd_ );
    return _is_path_valid_result_check( r, fd_, NULL );
}

/* ===========================================================
 *                  is_path_valid  [3]
 * -----------------------------------------------------------
 * -- 重载函数，参数改为 (fd_, path_)
 */
bool is_path_valid( int fd_, const char *path_ ){

    int r;
    r = check_path_st_mode( fd_, path_ );
    return _is_path_valid_result_check( r, fd_, path_ );
}


/* ===========================================================
 *                  Is_path_valid  [1]
 * -----------------------------------------------------------
 * -- is_path_valid 的 包裹函数，对于 无效的 path_, 直接报错终止
 * -- 此函数的 使用频率很高
 */
void Is_path_valid( const char *path_, const std::string &err_info_ ){

    if( is_path_valid(path_) == false ){
        cout << "Is_path_valid: ERROR. "
            << "errno = " << errno << " " << strerror(errno) << "\n"
            << "path_: " << path_ << "\n" 
            << err_info_
            << endl;
        assert(0);
    }
}

/* ===========================================================
 *                  Is_path_valid   [2]
 * -----------------------------------------------------------
 * -- 重载函数，参数改为 fd_
 */
void Is_path_valid( int fd_, const std::string &err_info_ ){

    if( is_path_valid(fd_) == false ){
        cout << "Is_path_valid: ERROR. "
            << "errno = " << errno << " " << strerror(errno) << "\n"
            << "fd_: " << fd_ << "\n" 
            << err_info_
            << endl;
        assert(0);
    }
}

/* ===========================================================
 *                  Is_path_valid   [3]
 * -----------------------------------------------------------
 * -- 重载函数，参数改为 fd_
 */
void Is_path_valid( int fd_, const char *path_, const std::string &err_info_ ){

    if( is_path_valid(fd_, path_) == false ){
        cout << "Is_path_valid: ERROR. "
            << "errno = " << errno << " " << strerror(errno) << "\n"
            << "fd_   = " << fd_ << "\n" 
            << "path_ = " << path_
            << err_info_
            << endl;
        assert(0);
    }
}


/* ===========================================================
 *                _is_path_a_dir_result_check   [static]
 * -----------------------------------------------------------
 * -- 为 所有 is_path_a_dir 函数 服务的函数，
 * -- 检测 check_path_st_mode 函数的 返回值，并提供对应的 结果
 * -- param: r_     -- check_path_st_mode 的返回值
 * -- param: fd_   -- 不需要时 写入 -1
 * -- param: path_ -- 不需要时 写入 NULL
 */
namespace{
int _is_path_a_dir_result_check( int r_, int fd_, const char *path_ ){

    int r = r_;
    if( r == 2 ){        
        return 1;  //--  path_ 确实指向一个 目录

    }else if( r > 0 ){   
        return 0;  //--  path_ 有效，但指向的文件 不是目录

    }else{
        r = -r; //-- 负值取反。
        switch( r ){
            case ENOENT:       //-  2 - path_ 中某一段不存在，或 _path是空字符串
                return (-1);

            case EBADF:        //-  9 - fd 是坏的。（目前不可能）
            case EACCES:       //- 13 - path_ 中，某一级目录缺乏 执行／搜索权
            case ENOTDIR:      //- 20 - path_ 的前缀 不是 目录
            case ENAMETOOLONG: //- 36 - path_ 过长
            case ELOOP:        //- 40 - path_ 的 符号链接 级数过多
                return (-2);

            default: //-- 剩余的都是 系统级error，直接报错终止
                cout << "is_path_a_dir: lstat: ERROR. " 
                    << "errno = " << r << " " << strerror(r) << endl;
                if( fd_ != -1 ){
                    cout << "fd_ = " << fd_ << endl;
                }
                if( path_ != NULL ){
                    cout << "path_ = " << path_ << endl;
                }
                assert(0);
                return 0; //- never reach
        }
    }
}
}//----- namespace -----//

/* ===========================================================
 *                  is_path_a_dir  [1]
 * -----------------------------------------------------------
 * -- 检查 path_ 是否为 目录
 * -- return:
 *      若 path_ 确实指向一个 目录，                 返回 1
 *      若 path_ 有效，但指向的不是目录（其它文件类型） 返回 0
 *      若 path_ 指向的地址没有文件／目录，ENOENT， 返回 -1. (当检查一个 尚未新建的 路径时，这种情况可能发送)
 *      若 path_ 格式错误。                      返回 -2
 *      若 path_ 格式没问题，但 调用lstat 发生系统级错误，直接报错终止
 * 
 */
int is_path_a_dir( const char *path_ ){

    int r;
    r = check_path_st_mode( path_ );
    return _is_path_a_dir_result_check( r, -1, path_ );
}

/* ===========================================================
 *                  is_path_a_dir  [2]
 * -----------------------------------------------------------
 * -- 重载函数，参数改为 fd_
 */
int is_path_a_dir( int fd_ ){

    int r;
    r = check_path_st_mode( fd_ );
    return _is_path_a_dir_result_check( r, fd_, NULL );
}

/* ===========================================================
 *                  is_path_a_dir  [3]
 * -----------------------------------------------------------
 * -- 重载函数，参数改为 (fd_, path_)
 */
int is_path_a_dir( int fd_, const char *path_ ){

    int r;
    r = check_path_st_mode( fd_, path_ );
    return _is_path_a_dir_result_check( r, fd_, path_ );
}


/* ===========================================================
 *                  Is_path_a_dir  [1]
 * -----------------------------------------------------------
 * -- is_path_a_dir 的 包裹函数，对于 不是目录的 path_, 直接报错终止
 */
void Is_path_a_dir( const char *path_, const std::string &err_info_ ){

    if( is_path_a_dir(path_) != 1 ){
        cout << "Is_path_a_dir: ERROR. "
            << "errno = " << errno << " " << strerror(errno) << "\n"
            << "path_ = " << path_ << "\n" 
            << err_info_
            << endl;
        assert(0);
    }
}

/* ===========================================================
 *                  Is_path_a_dir  [2]
 * -----------------------------------------------------------
 * -- 重载函数，参数改为 fd_
 */
void Is_path_a_dir( int fd_, const std::string &err_info_ ){

    if( is_path_a_dir(fd_) != 1 ){
        cout << "Is_path_a_dir: ERROR. "
            << "errno = " << errno << " " << strerror(errno) << "\n"
            << "fd_ = " << fd_ << "\n" 
            << err_info_
            << endl;
        assert(0);
    }
}

/* ===========================================================
 *                  Is_path_a_dir  [3]
 * -----------------------------------------------------------
 * -- 重载函数，参数改为 (fd_, path_)
 */
void Is_path_a_dir( int fd_, const char *path_, const std::string &err_info_ ){

    if( is_path_a_dir(fd_, path_) != 1 ){
        cout << "Is_path_a_dir: ERROR. "
            << "errno = " << errno << " " << strerror(errno) << "\n"
            << "fd_   = " << fd_ << "\n" 
            << "path_ = " << path_ << "\n" 
            << err_info_
            << endl;
        assert(0);
    }
}


/* ===========================================================
 *                       mk_dir  [1]
 * -----------------------------------------------------------
 * -- param: path_  -- 新目录的 绝对路径名
 * -- return:
 *       若 path_ 尚未存在，执行 Mkdir，创新 新目录
 *       若 path_ 指向出，已经存在一个文件，且是目录，说明目录已存在，直接返回
 *       若 path_ 指向出，已经存在一个文件，但不是目录，报错终止
 *       若 path_ 存在格式错误，或系统错误，报错终止
 */
void mk_dir( const char *path_, mode_t mode_,
            const std::string &err_info_ ){
    
    std::string err_info = err_info_ + "mk_dir(): ";
    //--------------------------
    int r;
    r = is_path_a_dir( path_ );
    switch( r ){
        case  1:  //-- 目标目录已经创建了，直接跳出返回
            break;

        case  0:  //-- 存在一个同名的文件，但它不是目录
            cout << err_info << "ERROR. "
                << "find a file with same name, but it's not a dir. \n" 
                << "path_   = " << path_ << " \n"
                << endl;
            assert(0);

        case -1:  //-- 可以执行 新目录 的创建工作
            Mkdir( path_, mode_, err_info );
            break;

        case -2:  //-- path 存在格式错误。（但这几乎不可能）
        default:  //-- is_path_a_dir() 不可能存在 其它返回值
            cout << err_info << "ERROR. "
                << "errno = " << errno << " " << strerror(errno) << "\n"
                << "path_   = " << path_ << " \n"
                << endl;
            assert(0);
    }//--------- switch end ----------//
}

/* ===========================================================
 *                       mk_dir  [2]
 * -----------------------------------------------------------
 * -- 重载版，通过两个 路径名 合成 新目录的 绝对路径名
 * -- param: _path_dir  -- 父目录，在此目录下 创建新目录
 * -- param: name_      -- 新目录的 名字（不是路径名）
 */
const std::string mk_dir(   const std::string &_path_dir, 
                            const std::string &name_,  
                            mode_t mode_,
                            const std::string &err_info_ ){
    
    std::string err_info = err_info_ + "mk_dir(): ";
    std::string err_dir = err_info + "the parent dir is error. ";

    //--------------------------//
    //  确保 _path_dir 是有效的目录
    //--------------------------//
    Is_path_a_dir( _path_dir.c_str(), err_dir );

    //--------------------------//
    //  合成 新目录的 绝对路径名 
    //--------------------------//
    std::string path; //-- 新目录的 绝对路径名
    std::string dir = _path_dir;
    std::string name = name_;
    path = tprGeneral::path_combine( dir, name );
    //--------------------------//
    //  检测 新的目录 是否已存在 
    //--------------------------//
    mk_dir( path.c_str(), mode_, err_info ); //-- 调用 mk_dir-[1] 
    return path;
}


/* ===========================================================
 *                       mk_dir  [3]
 * -----------------------------------------------------------
 * -- 重载函数，参数改为 fd_
 */
void mk_dir( int fd_, const char * name_, mode_t mode_,
            const std::string &err_info_ ){

    std::string err_info = err_info_ + "mk_dir(): ";
    std::string err_dir = err_info + "the parent dir is error. ";

    //--------------------------//
    //  确保 _path_dir 是有效的目录
    //--------------------------//
    Is_path_a_dir( fd_, err_dir );

    //--------------------------//
    //  此重载 不需要 合成 绝对路径名
    //--------------------------//
    //  测试表明，参数 name_ 写成 "/name", "name", "//name"
    //      都能正确运行。所以也不需要做 "/" 的检测
    //--------------------------//
    int r;
    r = is_path_a_dir( fd_, name_);
    switch( r ){
        case  1:  //-- 目标目录已经创建了，直接跳出返回
            break;

        case  0:  //-- 存在一个同名的文件，但它不是目录
            cout << err_info << "ERROR. "
                << "find a file with same name, but it's not a dir. \n" 
                << "fd_   = " << fd_ << " \n"
                << "name_ = " << name_ << " \n"
                << endl;
            assert(0);

        case -1:  //-- 可以执行 新目录 的创建工作
            Mkdirat( fd_, name_, mode_, err_info );
            break;

        case -2:  //-- path 存在格式错误。（但这几乎不可能）
        default:  //-- is_path_a_dir() 不可能存在 其它返回值
            cout << err_info << "ERROR. "
                << "errno = " << errno << " " << strerror(errno) << "\n"
                << "fd_   = " << fd_ << "\n" 
                << "name_ = " << name_ << "\n"
                << endl;
            assert(0);
    }//--------- switch end ----------//

    //-- 没想好 返回值类型，暂设为 void
}





/* ===========================================================
 *                   is_path_not_too_long
 * -----------------------------------------------------------
 * -- 返回 true    表示 长度允许
 * -- 返回 false， 表示 太长了
 */
bool is_path_not_too_long( const std::string &path_ ){

    //-- TPR_PATH_MAX 里包含了 尾后0 的字节
    if( (path_.size()+1) <= TPR_PATH_MAX ){ 
        return true;
    }else{
        return false;
    }
}

/* ===========================================================
 *                   Is_path_not_too_long
 * -----------------------------------------------------------
 * -- 检测 path_ 长度，过长的 直接报错终止
 */
void Is_path_not_too_long( const std::string &path_,
                            const std::string &err_info_ ){

    //-- TPR_PATH_MAX 里包含了 尾后0 的字节
    if( (path_.size()+1) > TPR_PATH_MAX ){ 
        cout << "Is_path_not_too_long: ERROR. "
            << err_info_
            << "path_ =" << path_  
            << endl;
        assert(0);
    }
}


/* ===========================================================
 *                   get_file_size  [1]
 * -----------------------------------------------------------
 * -- 获得 文件的 字节数。fd版
 */
off_t get_file_size( int fd_, const std::string &err_info_ ){

    std::string err_info = err_info_ + "get_file_size(1): ";
    //-----------------------------
    //-- 暂存 旧的 “当前文件偏移量／current file offset”
    off_t cfo = Lseek( fd_, 0, SEEK_CUR, err_info );
    //-- 将 当前偏移量 指向 文件最后一字节, 同时获得 此字节的 偏移量数值（我们要的）
    off_t r = Lseek( fd_, 0, SEEK_END, err_info );
    //-- 将 当前偏移量 设置回 原来的值。
    Lseek( fd_, cfo, SEEK_SET, err_info );
    return r;
}

/* ===========================================================
 *                   get_file_size  [2]
 * -----------------------------------------------------------
 * -- 重载2，FILE版
 */
/*
off_t get_file_size( FILE *_fp, const std::string &err_info_ ){

    string err_info = err_info_ + "get_file_size(2): ";
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
off_t get_file_size( const char *path_, const std::string &err_info_ ){

    std::string err_info = err_info_ + "get_file_size(3): ";
    //-----------------------------
    Is_path_valid( path_, err_info ); //-- 确保 path_ 指向 已存在文件
    int fd = Open( path_, O_RDONLY, 0, err_info ); //-- 获得 fd

    off_t r = get_file_size( fd, err_info_ );
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
void file_load( const std::string &path_, std::string &_buf ){

    std::string err_info = "file_load(): ";
    //-------
    int fd;
    off_t flen;   //- 文件 字节数
    ssize_t rlen; //- 实际读取 字节数
    
    flen = get_file_size( path_.c_str(), err_info );
    _buf.resize( flen );
    fd = Open( path_.c_str(), O_RDONLY, 0, err_info );
    rlen = Read( fd, (void*)_buf.c_str(), flen, err_info );
    assert( rlen == (ssize_t)flen );

    Close( fd, err_info );
}





}//------------------- namespace: tprUnix ------------------------//

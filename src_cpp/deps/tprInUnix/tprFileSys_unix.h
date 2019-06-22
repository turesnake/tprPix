/*
 * ========================= tprFileSys_unix.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2018.10.14
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *    专门存放 文件系统 相关的 函数
 * ----------------------------
 */
#ifndef TPR_FILESYS_UNIX_H_
#define TPR_FILESYS_UNIX_H_

#include <string>

namespace tprUnix {//--------------- namespace: tprUnix -------------------//


int check_st_mode( mode_t _mode );
int check_path_st_mode( const char *_path );
int check_path_st_mode( int _fd );

bool is_path_valid( const char *_path );
bool is_path_valid( int _fd );

void Is_path_valid( const char *_path, const std::string &_err_info );
void Is_path_valid( int _fd, const std::string &_err_info );

int is_path_a_dir( const char *_path );
int is_path_a_dir( int _fd );

void Is_path_a_dir( const char *_path, const std::string &_err_info );
void Is_path_a_dir( int _fd, const std::string &_err_info );

void mk_dir( const char *_path, mode_t _mode,
            const std::string &_err_info );
const std::string mk_dir(   const std::string &_path_dir, 
                            const std::string &_name, 
                            mode_t _mode,
                            const std::string &_err_info );
void mk_dir( int _fd, const char * _name, mode_t _mode,
            const std::string &_err_info );

/*      
const std::string path_combine( const std::string &_pa, const std::string &_pb );
const std::string path_combine( const char *_pa, const char *_pb );
*/

/*
const std::string nameString_combine(   const std::string &_prefix,
                                        size_t _idx,
                                        const std::string &_suffix );
*/

bool is_path_not_too_long( const std::string &_path );
void Is_path_not_too_long( const std::string &_path,
                            const std::string &_err_info );

off_t get_file_size( int _fd, const std::string &_err_info );
//off_t get_file_size( FILE *_fp, const std::string &_err_info );
off_t get_file_size( const char *_path, const std::string &_err_info );

void file_load( const std::string &_path, std::string &_buf );



}//------------------- namespace: tprUnix ------------------------//
#endif


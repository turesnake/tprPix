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


int check_st_mode( mode_t mode_ );
int check_path_st_mode( const char *path_ );
int check_path_st_mode( int fd_ );

bool is_path_valid( const char *path_ );
bool is_path_valid( int fd_ );

void Is_path_valid( const char *path_, const std::string &err_info_ );
void Is_path_valid( int fd_, const std::string &err_info_ );

int is_path_a_dir( const char *path_ );
int is_path_a_dir( int fd_ );

void Is_path_a_dir( const char *path_, const std::string &err_info_ );
void Is_path_a_dir( int fd_, const std::string &err_info_ );

void mk_dir( const char *path_, mode_t mode_,
            const std::string &err_info_ );
const std::string mk_dir(   const std::string &_path_dir, 
                            const std::string &name_, 
                            mode_t mode_,
                            const std::string &err_info_ );
void mk_dir( int fd_, const char * name_, mode_t mode_,
            const std::string &err_info_ );

/*      
const std::string path_combine( const std::string &_pa, const std::string &_pb );
const std::string path_combine( const char *_pa, const char *_pb );
*/

/*
const std::string nameString_combine(   const std::string &_prefix,
                                        size_t _idx,
                                        const std::string &_suffix );
*/

bool is_path_not_too_long( const std::string &path_ );
void Is_path_not_too_long( const std::string &path_,
                            const std::string &err_info_ );

off_t get_file_size( int fd_, const std::string &err_info_ );
//off_t get_file_size( FILE *_fp, const std::string &err_info_ );
off_t get_file_size( const char *path_, const std::string &err_info_ );

void file_load( const std::string &path_, std::string &_buf );



}//------------------- namespace: tprUnix ------------------------//
#endif


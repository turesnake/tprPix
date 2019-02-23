/*
 * =================== readn_writen.cpp ========================
 *                          -- tpr --
 *                                        创建 -- 2018.09.26
 *                                        修改 -- 2018.09.26
 * ----------------------------------------------------------
 *    readn writen
 * ----------------------------
 */

//------------------- C -----------------------//
#include <unistd.h> //- ssize_t, read

#include <cassert> //-- assert
#include <errno.h> //- errno

//------------------- CPP ---------------------//
#include <iostream> //-- cout
#include <string>

//------------------- SELF ---------------------//
#include "tprDataType.h"


namespace tpr { //--------------- namespace: tpr -------------------//



//------------------- 提供给外部的 函数 ----------------
ssize_t readn( int _fd, void *_buf, size_t _nbytes, 
                int *_is_EOF, int *_fst_errno );

ssize_t writen( int _fd, const void *_buf, size_t _nbytes,
                int *_fst_errno );


/* ===========================================================
 *                        readn
 * -----------------------------------------------------------
 * -- 读取 管道／网络／终端 等慢速设备的 专用函数。
 *    本函数并不处理 各种异常，留给 调用者 来实现
 * 
 * -- param[out]: _is_EOF -- 若为  0，表示 没读到 EOF
 *                           若为  1，表示 读到了 EOF
 * -- param[out]: _fst_errno -- 存入 第一次捕捉的 errno
 *                              看起来有点多余，
 * -- return:
 *    本函数的 返回 是两个 独立的值：
 *      -- 返回值 
 *      -- _is_EOF
 *   （应当先 检查_is_EOF，再检查 返回值 ）
 * 
 *    返回值 就是 已经读取的 字节数（不管是否读满 要求的 长度）
 *    若返回值 == _nbytes。 说明读取成功。
 *    若返回值 < _nbytes,  说明读到一半 出错了，返回已经读取的 字节数
 *                        若读到一半 读出了 EOF，也会返回 已读取字节数
 *                        所以当遇到这种情况时，需要 额外检查 参数 _is_EOF 的值
 *    若返回  -1.          说明第一次调用 read 就出错了
 */
ssize_t readn( int _fd, void *_buf, size_t _nbytes, 
                int *_is_EOF, int *_fst_errno ){

    size_t nleft; //-- 剩余未读 字节数 
    ssize_t nread; //-- 每次调用 read 读到的 字节数
    u8_t *ptr = (u8_t*)_buf; //-- 读指针( 以 二进制格式 )

    *_is_EOF = 0; //-- 默认未读到 EOF
    *_fst_errno = 0;
    errno = 0; //-- 系统调用前 清空 errno

    nleft = _nbytes;
    while( nleft > 0 ){

        nread = read( _fd, (void*)ptr, nleft );

        //--- 若 某次 read 出错 ----
        //-- 在此不做 errno 判断，留给上层。
        if( nread < 0 ){
            *_fst_errno = errno; //-- 捕捉 errno
            if( nleft == _nbytes ){ //-- 第一次 read 就出错，
                return (-1); 
            }else{
                break; //-- 跳出 while 循环
            }
        }else 
        //---- 若 某次 read 读到 EOF -----
        if( nread == 0 ){ 
            *_is_EOF = 1; //-- 确认 读到 EOF
            break;        //-- 跳出 while 循环
        }

        nleft -= nread;
        ptr   += nread;
    }//--------- end of while -----------//

    return (_nbytes - nleft); 
}


/* ===========================================================
 *                        writen
 * -----------------------------------------------------------
 * -- 写入 管道／网络／终端 等慢速设备的 专用函数。
 *    本函数并不处理 各种异常，留给 调用者 来实现
 * 
 * -- param[out]: _fst_errno -- 存入 第一次捕捉的 errno
 *                              看起来有点多余，
 * 
 * -- return:
 *    若返回值 == _nbytes。 说明写入成功。
 *    若返回值 < _nbytes,  说明写到一半 出错了，返回已经写入的 字节数
 *                        
 *    若返回  -1.          说明第一次调用 write 就出错了
 */
ssize_t writen( int _fd, const void *_buf, size_t _nbytes,
                int *_fst_errno ){

    size_t nleft; //-- 剩余未写 字节数 
    ssize_t nwrite; //-- 每次调用 write 写入的 字节数
    u8_t *ptr = (u8_t*)_buf; //-- 读指针( 以 二进制格式 )

    *_fst_errno = 0;
    errno = 0; //-- 系统调用前 清空 errno

    nleft = _nbytes;
    while( nleft > 0 ){

        nwrite = write( _fd, ptr, nleft );

        //-- 若 某次调用 write 出错 ----
        if( nwrite < 0 ){
            *_fst_errno = errno; //-- 捕捉 errno
            if( nleft == _nbytes ){ //-- 第一次 write 就出错，
                return (-1);
            }else{
                break; //-- 跳出 while 循环
            }
        }else
        //-- ??? 什么时候，write 会返回 0 ???
        if( nwrite == 0 ){
            break; //-- 跳出 while 循环
        }

        nleft -= nwrite;
        ptr   += nwrite;
    }//--------- end of while -----------//

    return (_nbytes - nleft);
}


}//------------------- namespace: tpr ------------------------//


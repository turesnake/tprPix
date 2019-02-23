/*
 * ========================= xxd.cpp =========================
 *                          -- tpr --
 *                                        创建 -- 2018.10.18
 *                                        修改 -- 2018.10.18
 * ----------------------------------------------------------
 * 
 *   一个 类似 xxd 的小工具, (以函数的形式)。
 *   可以显示 一段 二进制数据块 的综合信息，
 *   包括：
 *       十六进制 数值
 *       ascii 码
 * 
 *  ***  此工具有个小问题，当访问的是 段保护的 内存时，  ***
 *       会遭遇 Segmentation fault: 11 错误。
 * 
 * ----------------------------
 * 依赖的其它库：
 *        tprDataType
 * ----------------------------
 */
#include "tprBinary.h" 

//------------ C -------------//

#include <errno.h>  //- errno
#include <cassert>  //- assert
#include <string.h> //- memcpy
//------------ CPP -------------//
#include <sstream> //- stringstream
#include <iomanip> //- setw, setfill

//------------ self -------------//

//using std::cout;
//using std::endl;
using std::string;
using std::vector;
using std::stringstream;
using std::setw;

namespace tpr { //--------------- namespace: tpr -------------------//


//------------------- 提供给外部的 函数 ----------------
const std::string xxd( void *_buf, size_t _len, 
                        bool _is_memaddr,
                        bool _is_cap, 
                        bool _is_line_bytes_16 
                        );

//---------------------- 局部 变量 ---------------------
namespace{
    //------ vals -------
    vector<u8_t> lines; //-- 容纳 一行数据。每一行都重新装载

    //------ funcs ------
    const std::string mk_line_head( size_t _num, size_t _max );
    const std::string mk_line_hex( bool _is_cap );
    const std::string mk_line_head( u64_t _memaddr, bool _is_cap );
    const std::string mk_line_ascii();
    u64_t find_align_byte( u64_t _memaddr, size_t _align_num );
}


/* ===========================================================
 *                          xxd
 * -----------------------------------------------------------
 * -- 将一段 二进制数据块。 逐个 字节，显示为对应的 ascii-char
 * -- ascii-char 中的功能键部分，会被 省略为 '.' 这种形式。
 * -- 此函数 常为 辅助函数，对性能要求不高
 * -- 如果 某 一行 全是0，则会省略。
 * -- 每一行都有 行号 [00876] 这种。（有 内存地址 和 序号，两种选择）
 * 
 * -- param: _buf
 * -- param: _len
 * -- param: _is_cap -- 默认: true
 * -- param: _line_bytes -- 一行显示 多少字节。默认: 16
 * -- param: _is_memaddr -- 行号是使用 内存线性地址，还是使用 序号，默认：false（序号）
 *                                                 (暂时 不实现 内存地址版)
 */
const std::string xxd( void *_buf, size_t _len, 
                        bool _is_memaddr,
                        bool _is_cap, 
                        bool _is_line_bytes_16 
                        ){

    string err_info = "xxd(): ";
    //-------------------------------//
    //           参数检测
    //-------------------------------//
    if( (_buf==NULL) || (_len==0) ){
        //cout << err_info << "ERROR. "
        //    << "(_buf==NULL) || (_len==0)" 
        //    << endl;
        assert(0);
    }
    //--- 目前只支持，8/16 两种 行字节数 ---//
    size_t line_bytes; //-- 一行 的字节数
    if( _is_line_bytes_16 == true ){
        line_bytes = 16;
    }else{
        line_bytes = 8;
    }
    //---------------------
    stringstream ss; //-- 我们最终要的 字符串 的容器

    u8_t  *p  = (u8_t*)_buf;  //- 每次指向 行首字节     
    u8_t  byte;             //-- 遍历 行内字节用
    ssize_t len = (ssize_t)_len; //-- 必须要改为 有符号类型，不然判断语句会 出问题
                                 //-- 容量上完全足够，可放心使用
    bool   is_line_all_zero;        //-- 这一行的数据是不是都是 0
    bool   is_starChar_set = false; //-- “*” 是否被写入 ss 中。（当连续数行都是0，ss中只会写入一次 "*\n"）
    //-------------------------------//
    //    为 行容器 预分配空间扩容
    //-------------------------------//
    lines.reserve( line_bytes ); //-- 预分配空间扩容，一种优化习惯

    //----------------------------//
    //     如果需要 地址版头部，
    //     需要预先 写入第一行
    //----------------------------//
    size_t line_len_L; //-- 第一行的 左侧部分，全写0
    size_t line_len_R; //-- 第一行的 右侧部分，读取buf 中值
    u64_t addr_align;    //-- 对齐的 首字节地址 (等于buf，或buf之前的某字节)
    if( _is_memaddr == true ){

        //--- 找到 对齐的 首字节 ---
        addr_align = find_align_byte( (u64_t)p, line_bytes );

        line_len_L = (u64_t)p - addr_align;
        line_len_R = line_bytes - line_len_L;

        //---- 制作／读取 第一行 ------//
        lines.insert( lines.end(), line_len_L, 0 );
        for( size_t i=0; i<line_len_R; i++ ){
            byte = *(p+i);
            lines.push_back( byte );
        }
        //------------------------------//
        // 第一行 就算全为0，也不会被省略缩写
        //------------------------------//
        ss << mk_line_head( addr_align, _is_cap ); //-- 地址版头部
        ss << mk_line_hex( _is_cap );
        ss << mk_line_ascii();
        ss << "\n"; 

        p   += line_len_R;
        len -= line_len_R;
    }

    //-----------------------//
    //    正式的 数据段 处理
    //    读取一行，处理一行
    //   （加快运行速度的 做法）
    //-----------------------//
    while( len > 0 ){

        lines.clear(); //-- 清空
        is_line_all_zero = true; //-- 先假定，整行都是 0

        //------- 读取一行数据 -------//
        for( size_t i=0; i<line_bytes; i++ ){
            byte = *(p+i);
            lines.push_back( byte );
            if( byte != 0 ){
                is_line_all_zero = false;
            }
        }
        //------------------------------------//
        // 如果这一行都是零，或者接下来的数行也是零
        // 这些行的显示会被省略为 一个 "*" 
        //------------------------------------//
        if( is_line_all_zero == true ){

            //----- 若此行 是第一行 全0 行 ------//
            if( is_starChar_set == false ){
                is_starChar_set = true;
                //--- 只写入 一个 "*" 号 ---//
                ss << "*\n";

            }else{
            //----- 若此行 不是第一行 全0行 ------//
            //--- 这说明 至少上一行 也是全0 ----
            //--- 什么也不做，去处理下一行
            }

        }else{
            //---- 清空 is_starChar_set ----//
            if( is_starChar_set != false ){
                is_starChar_set = false;
            }
            //----- 正式 制作 字符串 ----// 
            if( _is_memaddr == true ){
                ss << mk_line_head( (u64_t)p, _is_cap );     //-- 地址版头部
            }else{
                ss << mk_line_head( (size_t)(p-(u8_t*)_buf), _len ); //-- 序号版头部
            }

            ss << mk_line_hex( _is_cap );
            ss << mk_line_ascii();
            ss << "\n"; 
        }
        //------ 为下一回合做准备 -------//
        p   += line_bytes;
        len -= line_bytes;
    }//------ while end ------//

    return ss.str();
}



namespace{ //--------------- 局部函数 -----------------//


/* ===========================================================
 *                   mk_line_head  [序号版]
 * -----------------------------------------------------------
 * -- 制作 一行的头部  [序号版]
 * -- param: _num -- 要在头部打印的数字，每一行首字节的序号
 * -- param: _max -- _num 的最大值，也就是 数据段 的 最大序号（以此来计算头部 空白处的字节数）
 */
const std::string mk_line_head( size_t _num, size_t _max ){

    stringstream ss;
    //--------- 计算出 头部位数 ---------//
    size_t digit = get_num_digit( _max, false );
    digit++; //-- 左侧留一个 空格
    //------ 正式制作 头部 string ------//
    ss << std::right 
        << "[" << std::setw(digit) << _num << "]: ";
    
    return ss.str();
}

/* ===========================================================
 *                   mk_line_head  [内存地址版]
 * -----------------------------------------------------------
 * -- 制作 一行的头部  [内存地址版]
 * -- param: _memaddr -- 要在头部打印的数字，每一行首字节的 内存地址
 */
const std::string mk_line_head( u64_t _memaddr, bool _is_cap ){

    stringstream ss;
    //--------- 计算出 头部位数 ---------//
    size_t digit = 8 * 2; //-- 8-bytes, 1-bytes占用 2位（hex表达法） 

    //------ 正式制作 头部 string ------//
    ss << std::right 
        << "[" 
        << "0x" //-- 十六进制头部
        << std::setw(digit) 
        << memaddr_2_hexStr( _memaddr, _is_cap )
        << "]: ";
    
    return ss.str();
}




/* ===========================================================
 *                    mk_line_hex
 * -----------------------------------------------------------
 * -- 制作 一行的中部，"00 AE 7D FF 0B " 这样的 由空格相隔的 一段 hex
 */
const std::string mk_line_hex( bool _is_cap ){
    stringstream ss;
    for( auto c : lines ){
        ss << u8_2_hexStr( c, _is_cap ) << " ";
    }
    ss << "| "; //-- 额外补上一个 纵向分割线。 
    return ss.str();
}

/* ===========================================================
 *                    mk_line_hex
 * -----------------------------------------------------------
 * -- 制作 一行的尾部，尽可能打印出 每个 字节的 ascii 码
 * -- 当 一个u8 值为 0， 显示 "."
 * -- 当 一个u8 值为 ascii可显示字符，显示之
 * -- 当 一个u8 值 不为可显示字符，显示 "?"
 */
const std::string mk_line_ascii(){

    stringstream ss;
    for( auto c : lines ){

        if( c == 0 ){
            ss << ".";

        }else if( (c>=32) && (c<=126)){
            ss << (char)c;

        }else{
            ss << "?";

        }
    }//----- for end ------
    return ss.str();
}


/* ===========================================================
 *                    find_align_byte
 * -----------------------------------------------------------
 * -- 检测 目标地址 是否对齐。若不对齐，向前遍历，直到找到第一个对齐的 字节地址
 * -- param: _memaddr   -- 
 * -- param: _align_num --
 */
u64_t find_align_byte( u64_t _memaddr, size_t _align_num ){
    
    u64_t addr = _memaddr;

    while( (addr%_align_num) != 0 ){
        addr--;
    }
    return addr;
}




    

}//------ namespace -------//



}//------------------- namespace: tpr ------------------------//

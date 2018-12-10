/*
 * ========================= check_sum.cpp ==========================
 *                            -- tpr --
 *                                        创建 -- 2018.10.12
 *                                        修改 -- 2018.10.18
 * ----------------------------------------------------------
 * 
 *   check_sum / 检验和
 * 
 * ----------------------------
 */
#include <tprCrypto.h>

//------------ C -------------//
#include <string.h> //-- memcpy

//------------ CPP -------------//
#include <sstream> //- stringstream

//------------ self -------------//
#include "tprBinary.h" 

using std::cout;
using std::endl;
using std::string;
using std::vector;
using std::stringstream;

namespace tpr { //--------------- namespace: tpr -------------------//




//------------------- 提供给外部的 函数 ----------------
CheckSum64 mk_checksum64_old( u8 *_buf, size_t _len );
CheckSum64 mk_checksum64( u8 *_buf, size_t _len );
const std::string checksum64_2_str( const CheckSum64 &_cs,
                                    bool _is_hex, bool _is_cap );
bool cmp_checksum64( const CheckSum64 &_a, const CheckSum64 &_b );



CheckSum128 mk_checksum128_old( u8 *_buf, size_t _len );
CheckSum128 mk_checksum128( u8 *_buf, size_t _len );
const std::string checksum128_2_str( const CheckSum128 &_cs,
                                    bool _is_hex, bool _is_cap );
bool cmp_checksum128( const CheckSum128 &_a, const CheckSum128 &_b );



//------------------- 局部函数／变量 ----------------
namespace{

    //-- 以下 2 容器 只在 old版 mk_checksum 中被使用。
    //-- 新版 更改了实现策略，这两个容器不再被使用。
    vector<u32> block32_ary; //-- 将数据切割成 无数个 block32，再合成一个 数组  [old]
    vector<u64> block64_ary; //-- 将数据切割成 无数个 block64，再合成一个 数组  [old]

}


/* ===========================================================
 *                 mk_checksum64_old  [已被新版替代]
 * -----------------------------------------------------------
 * -- 一种 类似 Adler检验和 的实现。
 *    最终产生的 checksum 为 64-bits，实际分为两段，L（高32位） 和 R（低32位）
 *    先将 所有数据 分割为 32-bits 的block。然后逐个处理每一个 block
 *    每一回合，R 累加新的 block。 L 则 累加 R（从而让 L 实现加权）。
 *    在每一次加法中，都使用 “二进制反码”技术，即让 溢出的高位 回绕到 低位。
 * 
 * -- param: _buf -- 制作 checksum 的原始数据，以 二进制 看待它
 * -- param: _len -- 原始数据的 字节数
 */
CheckSum64 mk_checksum64_old( u8 *_buf, size_t _len ){

    u8     *p  = _buf;
    ssize_t len = (ssize_t)_len; 
                //-- size_t 的容量是 1844万亿亿 字节。 不存在文件能 到达这个值的一半（其负数域）。
                //-- 所以，可以安全地将 size_t 转换为 ssize_t

    u32    block32;                       //-- 临时变量, 块，一次存 32-bits 数据
    size_t block32_len = sizeof( u32 ); //-- 单个 block32 的字节数(4字节)
    //-------------------------------//
    // 将原始数据，分段 (4-bytes) 存入block32_ary
    //-------------------------------//
    block32_ary.clear(); //-- 清空
    while( len > 0 ){

        block32 = 0; //-- 清空
        if( len >= block32_len ){
            //-- 整取，一次取 4-bytes --
            block32 = *((u32*)p);
        }else{
            //-- 散取,最后一趟，小于 4-bytes--
            memcpy( (void*)&block32,
                    (void*)p,
                    len
                    );
        }

        block32_ary.push_back( block32 ); //-- 存入

        p += block32_len;
        len -= block32_len;
    }

    //-------------------------------//
    //        计算出 L 和 R
    //-------------------------------//
    u32 L = 0; //-- checksum 高 32-bits
    u32 R = 0; //-- checksum 低 32-bits

        cout << "\nblock32_ary.size() = " << block32_ary.size() << endl;

    for( auto b : block32_ary ){
        R = sum_wrapping( R, b ); //-- 线性累加 
        L = sum_wrapping( L, R ); //-- 加权累加
    }
    
        cout << "    L = " << L << endl;
        cout << "    R = " << R << endl;

    //-------------------------------//
    //    将 L 和 R 存入 最后的 checksum
    //-------------------------------//
    CheckSum64 cs;
    memcpy( (void*)&(cs.data[0]), //-- 拷贝 L
            (void*)&L,
            block32_len
            );
    memcpy( (void*)&(cs.data[block32_len]), //-- 拷贝 R
            (void*)&R,
            block32_len
            );
    return cs;
}


/* ===========================================================
 *                    mk_checksum64
 * -----------------------------------------------------------
 * -- 全新版本，不再将 buf 中所有数据 复制到本地。
 * -- 而是 读取 1 block，处理 1 block。 减少了 I/O
 * -- 64-bits版的 生成速度，会比 128-bits版的 慢一倍。
 */
CheckSum64 mk_checksum64( u8 *_buf, size_t _len ){

    u8     *p  = _buf;
    ssize_t len = (ssize_t)_len; //-- 必须要改为 有符号类型，不然判断语句会 出问题
                //-- size_t 的容量是 1844万亿亿 字节。 不存在文件能 到达这个值的一半（其负数域）。
                //-- 所以，可以安全地将 size_t 转换为 ssize_t

    u32    block32;                     //-- 临时变量, 块，一次存 32-bits 数据
    size_t block32_len = sizeof( u32 ); //-- 单个 block32 的字节数(4字节)

    u32 L = 0; //-- checksum 高 32-bits
    u32 R = 0; //-- checksum 低 32-bits

    //-------------------------------//
    //   读取 1 block，处理 1 block
    //-------------------------------//
    while( len > 0 ){
        //------- 读取 1 block --------//
        block32 = 0; //-- 清空
        if( len >= block32_len ){
            //-- 整取，一次取 4-bytes --
            block32 = *((u32*)p);
        }else{
            //-- 散取,最后一趟，小于 4-bytes--
            memcpy( (void*)&block32,
                    (void*)p,
                    len
                    );
        }
        //----- 更新 L/R 的值 -----------//
        R = sum_wrapping( R, block32 ); //-- 线性累加 
        L = sum_wrapping( L, R ); //-- 加权累加

        //------ 为下一回合做准备 -------//
        p += block32_len;
        len -= block32_len;
    }//------ while end ------//

        cout << "    L = " << L << endl;
        cout << "    R = " << R << endl;

    //-------------------------------//
    //    将 L 和 R 存入 最后的 checksum
    //-------------------------------//
    CheckSum64 cs;
    memcpy( (void*)&(cs.data[0]), //-- 拷贝 L
            (void*)&L,
            block32_len
            );
    memcpy( (void*)&(cs.data[block32_len]), //-- 拷贝 R
            (void*)&R,
            block32_len
            );
    return cs;
}


/* ===========================================================
 *                mk_checksum128_old   [已被新版替代]
 * -----------------------------------------------------------
 * -- 一种 类似 Adler检验和 的实现。
 *    最终产生的 checksum 为 128-bits，实际分为两段，L（高64位） 和 R（低64位）
 *    先将 所有数据 分割为 64-bits 的block。然后逐个处理每一个 block
 *    每一回合，R 累加新的 block。 L 则 累加 R（从而让 L 实现加权）。
 *    在每一次加法中，都使用 “二进制反码”技术，即让 溢出的高位 回绕到 低位。
 * 
 * -- param: _buf -- 制作 checksum 的原始数据，以 二进制 看待它
 * -- param: _len -- 原始数据的 字节数
 */
CheckSum128 mk_checksum128_old( u8 *_buf, size_t _len ){

    u8     *p  = _buf;
    ssize_t len = (ssize_t)_len; 
                //-- size_t 的容量是 1844万亿亿 字节。 不存在文件能 到达这个值的一半（其负数域）。
                //-- 所以，可以安全地将 size_t 转换为 ssize_t

    u64    block64;                       //-- 临时变量, 块，一次存 64-bits 数据
    size_t block64_len = sizeof( u64 ); //-- 单个 block64 的字节数(8字节)
    //-------------------------------//
    // 将原始数据，分段 (8-bytes) 存入block64_ary
    //-------------------------------//
    block64_ary.clear(); //-- 清空
    while( len > 0 ){

        block64 = 0; //-- 清空
        if( len >= block64_len ){
            //-- 整取，一次取 4-bytes --
            block64 = *((u64*)p);
        }else{
            //-- 散取,最后一趟，小于 4-bytes--
            memcpy( (void*)&block64,
                    (void*)p,
                    len
                    );
        }

        block64_ary.push_back( block64 ); //-- 存入

        p += block64_len;
        len -= block64_len;
    }

    //-------------------------------//
    //        计算出 L 和 R
    //-------------------------------//
    u64 L = 0; //-- checksum 高 64-bits
    u64 R = 0; //-- checksum 低 64-bits

        cout << "\nblock64_ary.size() = " << block64_ary.size() << endl;

    for( auto b : block64_ary ){
        R = sum_wrapping( R, b ); //-- 线性累加 
        L = sum_wrapping( L, R ); //-- 加权累加
    }
    
        cout << "    L = " << L << endl;
        cout << "    R = " << R << endl;

    //-------------------------------//
    //    将 L 和 R 存入 最后的 checksum
    //-------------------------------//
    CheckSum128 cs;
    memcpy( (void*)&(cs.data[0]), //-- 拷贝 L
            (void*)&L,
            block64_len
            );
    memcpy( (void*)&(cs.data[block64_len]), //-- 拷贝 R
            (void*)&R,
            block64_len
            );
    return cs;
}


/* ===========================================================
 *                   mk_checksum128
 * -----------------------------------------------------------
 * -- 全新版本，不再将 buf 中所有数据 复制到本地。
 * -- 而是 读取 1 block，处理 1 block。 减少了 IO
 */
CheckSum128 mk_checksum128( u8 *_buf, size_t _len ){

    u8     *p  = _buf;
    ssize_t len = (ssize_t)_len; //-- 必须要改为 有符号类型，不然判断语句会 出问题
                //-- size_t 的容量是 1844万亿亿 字节。 不存在文件能 到达这个值的一半（其负数域）。
                //-- 所以，可以安全地将 size_t 转换为 ssize_t

    u64    block64;                       //-- 临时变量, 块，一次存 64-bits 数据
    size_t block64_len = sizeof( u64 ); //-- 单个 block64 的字节数(8字节)

    u64 L = 0; //-- checksum 高 64-bits
    u64 R = 0; //-- checksum 低 64-bits

    //-------------------------------//
    //   读取 1 block，处理 1 block
    //-------------------------------//
    while( len > 0 ){

        //------- 读取 1 block --------//
        block64 = 0; //-- 清空
        if( len >= block64_len ){
            //-- 整取，一次取 4-bytes --
            block64 = *((u64*)p);
        }else{
            //-- 散取,最后一趟，小于 4-bytes--
            memcpy( (void*)&block64,
                    (void*)p,
                    len
                    );
        }
        //----- 更新 L/R 的值 -----------//
        R = sum_wrapping( R, block64 ); //-- 线性累加 
        L = sum_wrapping( L, R ); //-- 加权累加

        //------ 为下一回合做准备 -------//
        p += block64_len;
        len -= block64_len;
    }//------ while end ------//
    
        //cout << "    L = " << L << endl;
        //cout << "    R = " << R << endl;

    //-------------------------------//
    //    将 L 和 R 存入 最后的 checksum
    //-------------------------------//
    CheckSum128 cs;
    memcpy( (void*)&(cs.data[0]), //-- 拷贝 L
            (void*)&L,
            block64_len
            );
    memcpy( (void*)&(cs.data[block64_len]), //-- 拷贝 R
            (void*)&R,
            block64_len
            );
    return cs;
}






/* ===========================================================
 *                    checksum64_2_str
 * -----------------------------------------------------------
 * -- 将 checksum64 转换为 字符串
 * -- param: _is_hex --
 * -- param: _is_cap -- 当输出为 hex 时，设置是否大写。 未设为hex时，设置任何值都无效
 */
const std::string checksum64_2_str( const CheckSum64 &_cs,
                                    bool _is_hex, bool _is_cap ){

    stringstream ss;
    for( int i=0; i<CHECKSUM64_LEN; i++ ){
        if( _is_hex == true ){
            //-- 输出 hex --//
            ss << u8_2_hexStr(_cs.data[i], _is_cap) << ", ";
        }else{
            //-- 输出 十进制 --//
            ss << (int)(_cs.data[i]) << ", ";
        }
    }
    return ss.str();
}


/* ===========================================================
 *                    checksum128_2_str
 * -----------------------------------------------------------
 * -- 将 checksum128 转换为 字符串
 * -- param: _is_hex --
 * -- param: _is_cap -- 当输出为 hex 时，设置是否大写。 未设为hex时，设置任何值都无效
 */
const std::string checksum128_2_str( const CheckSum128 &_cs,
                                    bool _is_hex, bool _is_cap ){

    stringstream ss;
    for( int i=0; i<CHECKSUM128_LEN; i++ ){
        if( _is_hex == true ){
            //-- 输出 hex --//
            ss << u8_2_hexStr(_cs.data[i], _is_cap) << ", ";
        }else{
            //-- 输出 十进制 --//
            ss << (int)(_cs.data[i]) << ", ";
        }
    }
    return ss.str();
}


/* ===========================================================
 *                    cmp_checksum64
 * -----------------------------------------------------------
 * -- 对比两个 checksum64，看是否相同
 */
bool cmp_checksum64( const CheckSum64 &_a, const CheckSum64 &_b ){

    for( int i=0; i<CHECKSUM64_LEN; i++ ){
        if( _a.data[i] != _b.data[i] ){
            return false;
        }
    }
    return true;
}

/* ===========================================================
 *                    cmp_checksum128
 * -----------------------------------------------------------
 * -- 对比两个 checksum128，看是否相同
 */
bool cmp_checksum128( const CheckSum128 &_a, const CheckSum128 &_b ){

    for( int i=0; i<CHECKSUM128_LEN; i++ ){
        if( _a.data[i] != _b.data[i] ){
            return false;
        }
    }
    return true;
}








}//------------------- namespace: tpr ------------------------//


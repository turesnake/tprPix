/*
 * ========================= tprBinary.cpp ==========================
 *                          -- tpr --
 *                                        创建 -- 2018.10.17
 *                                        修改 -- 2018.10.17
 * ----------------------------------------------------------
 * 
 *   常用的 二进制 处理函数
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
 

//------------ self -------------//

//using std::cout;
//using std::endl;
using std::string;
using std::vector;
using std::stringstream;

namespace tpr { //--------------- namespace: tpr -------------------//


//------------------- 提供给外部的 函数 ----------------
const std::vector<u8> sum( const std::vector<u8> &_a, const std::vector<u8> &_b );
const std::vector<u8> sum( const u64 _a, const u64 _b );

const u32 sum_wrapping( const u32 _a, const u32 _b );
const u64 sum_wrapping( const u64 _a, const u64 _b );


const std::string u8_2_hexStr( u8 _v, bool _is_cap );

size_t get_num_digit( size_t _num, bool _is_hex );

const std::string memaddr_2_hexStr( u64 _memaddr, bool _is_cap );



//---------------------- 局部 变量 ---------------------
namespace{
    //------ val -------


    //------ funcs ------
    char _get_hexChar( u8 _v, bool _is_cap );

}


/* ===========================================================
 *                        sum   [1]
 * -----------------------------------------------------------
 * -- 进位加法器 [小端]
 * -- 支持 长度不同的 二进制数据 相加。
 * -- 返回 相加后的和，字节数 可能和 参数不一样（变长了）
 */
const std::vector<u8> sum( const std::vector<u8> &_a, const std::vector<u8> &_b ){

    vector<u8> a = _a;
    vector<u8> b = _b;
    int len; //-- 两参数中，长度 大的那个值
    int off = (int)(_a.size()) - (int)(_b.size());

        //cout << "off = " << off << endl;

    //--- 补齐 短的那个数据，使两数据 一样长（方便后面的相加）---//
    if( off == 0 ){
        len = (int)_a.size();

    }else if( off > 0 ){
        len = (int)_a.size();
        b.insert( b.end(), off, 0 );

    }else{
        len = (int)_b.size();
        a.insert( a.end(), (-off), 0 );
    }


    u8  unit_carry = 0; //-- 单元求和 的 进位值
    u8  unit_sum;   //-- 单位求和 的 单位和（已经把溢出值 割掉了）
    u32 sum;        //-- 单元求和 的 真和（会溢出，所以要用大容器来装）

    vector<u8> r; //-- 容纳 求和的返回值。
    r.reserve( len ); //-- 预分配空间扩容，一种优化习惯

    for( int i=0; i<len; i++ ){
        //-- 这个阶段，相加的两个原始值 都有数据 --
        sum = (u32)_a[i] + (u32)_b[i] + (u32)unit_carry;

        unit_carry = (u8)(sum / 256); //- 地板除
        unit_sum =   (u8)(sum % 256); //- 取余

        r.push_back( unit_sum );
    }

    if( unit_carry > 0 ){
        r.push_back( unit_carry );
    }
    return r;
}


/* ===========================================================
 *                        sum  [2]
 * -----------------------------------------------------------
 * -- 重载2，速度更快的， 64-bits 加法
 * -- 由于存在溢出，所以返回值 仍然是 vector<u8> 类型
 * -- 实现原理：
 *       1.两个 64-bits 值相加，之和过大时，会自动溢出，而且不记录在 errno 中
 *       2.溢出值 一定比 两个 被加数 小。-- 以此来判断是否发生溢出。
 */
const std::vector<u8> sum( const u64 _a, const u64 _b ){

    vector<u8> r;  //-- 容纳 求和的返回值。
    r.resize( sizeof(u64) ); //-- 不然 memcpy 会出错

    u64 half_sum = _a + _b; //-- 半加

    //-- 把 和 的低 8 字节复制到 容器中。
    memcpy( (void*)&(r[0]),
            (void*)&half_sum,
            8
            );

    if( (half_sum < _a) || (half_sum < _a) ){ //-- 发生溢出
        r.push_back( 1 );
    } 
    return r;
}


/* ===========================================================
 *                sum_wrapping  [1]
 * -----------------------------------------------------------
 * -- 伪 二进制反码 加法 , 32-bits
 * -- 当出现溢出时，高位的溢出值 会被 “绕回 累加” 到低位。
 */
const u32 sum_wrapping( const u32 _a, const u32 _b ){

    u32 r; //-- 返回值 
    u32 half_sum = _a + _b; //-- 半加

    r = half_sum;
    if( (half_sum < _a) || (half_sum < _b) ){ //-- 发生溢出
        r += 1; //-- 回绕，累加
    } 
    return r;
}


/* ===========================================================
 *                sum_wrapping  [2]
 * -----------------------------------------------------------
 * -- 重载2, 64-bits
 */
const u64 sum_wrapping( const u64 _a, const u64 _b ){

    u64 r; //-- 返回值 
    u64 half_sum = _a + _b; //-- 半加

    r = half_sum;
    if( (half_sum < _a) || (half_sum < _b) ){ //-- 发生溢出
        r += 1; //-- 回绕，累加
    } 
    return r;
}




/* ===========================================================
 *                    checksum_2_str
 * -----------------------------------------------------------
 * -- 将 checksum 转换为 字符串
 * -- 参数 _is_cap 控制 大小写
 */
const std::string u8_2_hexStr( u8 _v, bool _is_cap ){

    u8 quot = _v / 16; //-- 商   高位
    u8 mod  = _v % 16; //-- 余数 低位

    stringstream ss;
    ss << _get_hexChar(quot, _is_cap)
       << _get_hexChar(mod, _is_cap);
    return ss.str();
}


/* ===========================================================
 *                   _get_hexChar  [static]
 * -----------------------------------------------------------
 * -- 传入一个 0~15 的数，转换成 十六进制值。 
 * -- 参数 _is_cap 控制 大小写
 */
namespace{
char _get_hexChar( u8 _v, bool _is_cap ){

    char r; //-- 返回值
    //----- 参数检测 -------//
    if( _v >= 16 ){
        //cout << "_get_hexChar(): ERROR. "
        //    << "_v >= 16. _v = " << _v    
        //    << endl;
        assert(0); //-- 调用者代码错误
    }

    if( _v < 10 ){ 
        //------- 数字: 0,1... ------//
        r = 48 + _v;
    }else{
        if( _is_cap == true ){
            //------ 大写: A... -------//
            r = 65 + (_v - 10);

        }else{
            //------ 小写: a... -------//
            r = 97 + (_v - 10);
        }
    }
    return r;
}
}//----- namespace -------//



/* ===========================================================
 *                   get_num_digit
 * -----------------------------------------------------------
 * -- 获得一个数字的 位数。（比如，349 在 十进制中，位数 == 3）
 * -- param: _num    -- 本函数的 计算对象
 * -- param: _is_hex -- 选择 十进制，或 十六进制。（目前只支持两种） 
 */
size_t get_num_digit( size_t _num, bool _is_hex ){

    if( _num == 0 ){
        return 1;
    }
    //------- 十进制 还是 十六进制 ----------
    size_t base;
    if( _is_hex == true ){
        base = 16;
    }else{
        base = 10;
    }
    //-----------------
    size_t num = _num;
    size_t digit = 0; //-- 位数
    while( num > 0 ){
        num = num / base;
        digit++;
    }
    return digit;
}


/* ===========================================================
 *                memaddr_2_hexStr   [64-bits]
 * -----------------------------------------------------------
 * -- 将内存地址，转换为 十六进制 字符串
 * -- 默认 字节序是 小端
 */
const std::string memaddr_2_hexStr( u64 _memaddr, bool _is_cap ){

    stringstream ss;
    u8 *p = (u8*)&_memaddr;
    vector<u8> bytes; //-- 存放 地址的 每个字节
    bytes.reserve( sizeof(u64) ); //-- 预分配空间扩容，一种优化习惯

    //-- 将 地址中 每个字节依次存入 容器
    //-- 在 小段字节序中，是倒着的，所以后面要 正过来
    for( size_t i=0; i<sizeof(u64); i++ ){
        bytes.push_back( *(p+i) );
    }

    //-- 倒序遍历容器 --//
    auto rit = bytes.rbegin();
    for( ; rit != bytes.rend(); rit++ ){ //-- 反向迭代器的 ++ 就是 真实含义中的 --
        ss << u8_2_hexStr( *rit, _is_cap );
    }
    return ss.str();
}






}//------------------- namespace: tpr ------------------------//



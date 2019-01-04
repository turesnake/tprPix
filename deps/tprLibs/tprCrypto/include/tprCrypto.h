/*
 * ========================= tprCrypto.h ==========================
 *                            -- tpr --
 *                                        创建 -- 2018.10.12
 *                                        修改 -- 2018.10.12
 * ----------------------------------------------------------
 * 
 *   一些 简易的 密码学 库函数
 * 
 * ----------------------------
 * 依赖的其它库：
 *       tprDataType
 * ----------------------------
 */

#ifndef _TPR_CRYPTO_H_
#define _TPR_CRYPTO_H_

#include "tprDataType.h"

//------------ C -------------//

//------------ CPP -------------//
//#include <iostream>
#include <string>
#include <vector> 

//------------ self -------------//


namespace tpr { //--------------- namespace: tpr -------------------//


//--- checksum64 的类型，实体是个 固定的 二进制数据串。 64-bits ／ 8-bytes
#define CHECKSUM64_LEN 8 
struct CheckSum64{
    u8 data[ CHECKSUM64_LEN ] = {0};  //-- 显式初始化，很有效
};

//--- checksum128 的类型，实体是个 固定的 二进制数据串。 128-bits ／ 16-bytes
#define CHECKSUM128_LEN 16 
struct CheckSum128{
    u8 data[ CHECKSUM128_LEN ] = {0};  //-- 显式初始化，很有效
};


//----------- check_sum.cpp ------------//
//--- 64 --
CheckSum64 mk_checksum64_old( u8 *_buf, size_t _len ); //-- 已被新版替代，不推荐使用
CheckSum64 mk_checksum64( u8 *_buf, size_t _len );
const std::string checksum64_2_str( const CheckSum64 &_cs,
                                    bool _is_hex = true, bool _is_cap = true );

bool cmp_checksum64( const CheckSum64 &_a, const CheckSum64 &_b );

//--- 128 --
CheckSum128 mk_checksum128_old( u8 *_buf, size_t _len ); //-- 已被新版替代，不推荐使用
CheckSum128 mk_checksum128( u8 *_buf, size_t _len );
const std::string checksum128_2_str( const CheckSum128 &_cs,
                                    bool _is_hex = true, bool _is_cap = true );
bool cmp_checksum128( const CheckSum128 &_a, const CheckSum128 &_b );






}//------------------- namespace: tpr ------------------------//


#endif


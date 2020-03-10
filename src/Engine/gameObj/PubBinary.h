/*
 * ==================== PubBinary.h =====================
 *                          -- tpr --
 *                                        CREATE -- 2019.02.03
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#ifndef TPR_PUB_BINARY_H
#define TPR_PUB_BINARY_H

//-------------------- CPP --------------------//
#include <vector>
#include <unordered_map>

//-------------------- Engine --------------------//
#include "tprAssert.h"
#include "PubBinaryValType.h"

//-- 维护一个 二进制块，可以动态存储 各种 pubBinary变量（一种一个）
class PubBinary{
    using idx_t     = uint32_t; //- pubBinaryValTyprIdx
    using byteoff_t = uint32_t; //- byte_off in binary
public:
    PubBinary() = default;

    //- 通过一个 变量类型表，一次性注册所有变量
    inline void init( const std::vector<PubBinaryValType> &types_ ){
        idx_t      idx {}; //- tmp
        byteoff_t  off {0}; 
        for( const auto &i : types_ ){
            idx = static_cast<idx_t>(i);
            auto [insertIt, insertBool] = valOffs.emplace( idx, off );
            tprAssert( insertBool );
            off += (byteoff_t)(get_PubBinaryValSizes().at(idx));
        }
        binary.resize( off );
    }

    inline bool check( PubBinaryValType type_ ) const {
        return (valOffs.find((idx_t)type_)!=valOffs.end());
    }

    //- 仅返回 void*, 需要调用者自行转换为 对应的 指针类型...
    //  此指针同时提供 读写权限
    //  使用前 应主动调用 check() 
    inline void *get_valPtr( PubBinaryValType type_ ) const {
        const byteoff_t &off = valOffs.at((idx_t)type_);
        return reinterpret_cast<void*>(  const_cast<uint8_t*>(&(binary.at(off))) ); 
                        //- first remove const，then reinterpret_cast
    }

private:
    std::unordered_map<idx_t, byteoff_t>  valOffs {};
                    //- 记载 某个变量 是否被注册，以及它在 binary中的 地址偏移
    std::vector<uint8_t>  binary {}; //- 所有变量 真实存储区
};


#endif 


/*
 * ======================= BoolBitMap.h =======================
 *                          -- tpr --
 *                                        CREATE -- 2019.02.03
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *   each bit mean a bool_flag 
 * ----------------------------
 */
#ifndef TPR_BOOL_BIT_MAP_H
#define TPR_BOOL_BIT_MAP_H

//-------------------- CPP --------------------//
#include <cmath>
#include <vector>

//-------------------- Engine --------------------//
#include "tprAssert.h"
#include "tprCast.h"



//- each bit:
//  0: false
//  1: true
class BoolBitMap{
public:
    BoolBitMap() = default;

    inline void resize( size_t w_, size_t h_=1 )noexcept{
        this->wLen = w_;
        this->hLen = h_;
        this->totalNum = w_ * h_;
        //--
        size_t bytes = cast_2_size_t( ceil( static_cast<double>(this->totalNum) / 
                            static_cast<double>(BoolBitMap::BITS_PER_BYTE) ) );
        bitMap.resize( bytes );
    }

    inline void clear_all()noexcept{
        for( auto &i : bitMap ){
            i = 0; //- all false 
        }
    }

    inline void signUp( size_t w_, size_t h_ )noexcept{
        tprAssert( (w_<this->wLen) && (h_<this->hLen) );
        size_t idx = h_ * this->wLen + w_;
        //---
        tprAssert( (idx/BoolBitMap::BITS_PER_BYTE) < bitMap.size() );
        uint8_t &bitRef = bitMap.at( idx/BoolBitMap::BITS_PER_BYTE );
        bitRef = bitRef | static_cast<uint8_t>(1 << (idx%BoolBitMap::BITS_PER_BYTE));
    }
    inline void signUp( size_t idx_ )noexcept{
        tprAssert( idx_ < this->totalNum );
        //---
        tprAssert( (idx_/BoolBitMap::BITS_PER_BYTE) < bitMap.size() );
        uint8_t &bitRef = bitMap.at( idx_/BoolBitMap::BITS_PER_BYTE );
        bitRef = bitRef | static_cast<uint8_t>(1 << (idx_%BoolBitMap::BITS_PER_BYTE));
    }

    inline bool check( size_t w_, size_t h_ )noexcept{
        tprAssert( (w_<this->wLen) && (h_<this->hLen) );
        size_t idx = h_ * this->wLen + w_;
        //---
        tprAssert( (idx/BoolBitMap::BITS_PER_BYTE) < bitMap.size() );
        const uint8_t &bitRef = bitMap.at( idx/BoolBitMap::BITS_PER_BYTE ); 
        return  ( ((bitRef>>(idx%BoolBitMap::BITS_PER_BYTE)) & 1)==1 );
    }
    inline bool check( size_t idx_ )noexcept{
        tprAssert( idx_ < this->totalNum );
        //---
        tprAssert( (idx_/BoolBitMap::BITS_PER_BYTE) < bitMap.size() );
        const uint8_t &bitRef = bitMap.at( idx_/BoolBitMap::BITS_PER_BYTE ); 
        return  ( ((bitRef>>(idx_%BoolBitMap::BITS_PER_BYTE)) & 1)==1 );
    }

private:
    std::vector<uint8_t> bitMap {}; 

    size_t  wLen {};
    size_t  hLen {};
    size_t  totalNum {};

    //======== static ========//
    static size_t BITS_PER_BYTE;
};


#endif 


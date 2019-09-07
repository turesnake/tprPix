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
#include <vector>

//-------------------- Engine --------------------//
#include "tprAssert.h"


//- each bit:
//  0: false
//  1: true
class BoolBitMap{
public:
    BoolBitMap() = default;

    inline void init( size_t bytes_ )noexcept{
        bitMap.resize( bytes_ );
    }

    inline void clear_all()noexcept{
        for( auto &i : bitMap ){
            i = 0; //- all false 
        }
    }

    inline void signUp( u32_t idx_ )noexcept{
        tprAssert( (idx_/bitsPerByte) < bitMap.size() );
        u8_t &bitRef = bitMap.at( idx_/bitsPerByte );
        bitRef = bitRef | static_cast<u8_t>(1 << (idx_%bitsPerByte));
    }

    inline bool check( u32_t idx_ )noexcept{
        tprAssert( (idx_/bitsPerByte) < bitMap.size() );
        const u8_t &bitRef = bitMap.at( idx_/bitsPerByte ); 
        return  ( ((bitRef>>(idx_%bitsPerByte)) & 1)==1 );
    }

private:
    std::vector<u8_t> bitMap {}; 

    //======== static ========//
    static u32_t bitsPerByte;
};

//-------- static --------//
inline u32_t BoolBitMap::bitsPerByte {8};

#endif 


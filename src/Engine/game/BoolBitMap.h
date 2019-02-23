/*
 * ======================= BoolBitMap.h =======================
 *                          -- tpr --
 *                                        CREATE -- 2019.02.03
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *   each bit mean a bool_flag 
 * ----------------------------
 */
#ifndef _TPR_BOOL_BIT_MAP_H_
#define _TPR_BOOL_BIT_MAP_H_

//-------------------- C --------------------//
#include <cassert>

//-------------------- CPP --------------------//
#include <vector>

//-------------------- Engine --------------------//
#include "PubBinaryValType.h"


//- each bit:
//  0: false
//  1: true
class BoolBitMap{
public:
    BoolBitMap() = default;

    inline void init( size_t _bytes ){
        bitMap.resize( _bytes );
    }

    inline void clear_all(){
        for( auto &i : bitMap ){
            i = 0; //- all false 
        }
    }

    inline void signUp( u32_t _idx ){
        assert( (_idx/bitsPerByte) < bitMap.size() );
        u8_t &bitRef = bitMap.at( _idx/bitsPerByte );
        bitRef = bitRef | (1 << (_idx%bitsPerByte));
    }

    inline bool check( u32_t _idx ){
        assert( (_idx/bitsPerByte) < bitMap.size() );
        const u8_t &bitRef = bitMap.at( _idx/bitsPerByte ); 
        return  ( ((bitRef>>(_idx%bitsPerByte)) & 1)==1 );
    }

private:
    std::vector<u8_t> bitMap {}; 

    //======== static ========//
    static u32_t bitsPerByte;
};

//-------- static --------//
inline u32_t BoolBitMap::bitsPerByte {8};

#endif 


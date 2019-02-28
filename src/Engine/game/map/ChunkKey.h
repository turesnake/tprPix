/*
 * ====================== ChunkKey.h =======================
 *                          -- tpr --
 *                                        CREATE -- 2019.01.07
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *    Chunk "id":  (int)w + (int)h
 * ----------------------------
 */
#ifndef _TPR_CHUNK_KEY_H_
#define _TPR_CHUNK_KEY_H_

//-------------------- C --------------------//
#include <cassert>

//-------------------- CPP --------------------//
#include <vector>

//------------------- Libs --------------------//
#include "tprDataType.h" 

//-------------------- Engine --------------------//
#include "config.h"
#include "IntVec.h" 
#include "MapCoord.h"

 

/* ===========================================================
 *                chunkKey_2_mcpos
 * -----------------------------------------------------------
 * -- 传入某个key，生成其 chunk 的 mcpos
 */
inline MapCoord chunkKey_2_mcpos( u64_t _key ){
    IntVec2  mpos {};
    int *ptr = (int*)&_key;
    //---
    mpos.x = *ptr;
    ptr++;
    mpos.y = *ptr;
    //---
    return MapCoord{ mpos };
} 


/* ===========================================================
 *             just_creat_chunkKey_by_mpos
 * -----------------------------------------------------------
 * -- 传入 chunk左下角mpos，获得 chunk key（u64）
 */
inline u64_t chunkMPos_2_key( const IntVec2 &_mpos ){
    u64_t key;
    int *ptr = (int*)&key;
    *ptr = _mpos.x;
    ptr++;
    *ptr = _mpos.y; 
    //--------
    return key;
}

/* ===========================================================
 *             get_chunk_mpos
 * -----------------------------------------------------------
 * -- 传入 任意 mapent 的 mpos，获得其 所在 chunk 的 mpos（chunk左下角）
 */
inline IntVec2 get_chunk_mpos( const IntVec2 &_mpos ){
    return ( floorDiv(_mpos,ENTS_PER_CHUNK) * ENTS_PER_CHUNK );
}


/* ===========================================================
 *                get_chunk_lMPosOff
 * -----------------------------------------------------------
 * -- 获得 目标mapent.mpos 在其 chunk 中的 相对mpos偏移
 */
inline IntVec2 get_chunk_lMPosOff( const IntVec2 &_mpos ){
    return ( _mpos - get_chunk_mpos(_mpos) );
}



//-- based on mapEntPos 
//  此实例通常为静态数据，一旦被设置，就永不改写。
class ChunkKey{
public:
    ChunkKey() = default;

    inline void init_by_key( const u64_t _key ){
        key = _key;
        //-------
        int *ptr = (int*)&key;
        mpos.x = *ptr;
        ptr++;
        mpos.y = *ptr;
        //--------
        build_nearbyKeys(); 
    }

    //-- 使用此函数来 根据 “任意mapent mpos” 初始化其对应的 key
    inline void init_by_mapEnt_mpos( const IntVec2 &_mpos ){
        //-- “地板除法，向低取节点值”, 在乘回 节点间距。
        //   获得 所在chunk 左下ent mpos
        mpos = get_chunk_mpos( _mpos );
        //--------
        key = chunkMPos_2_key( mpos );
        //--------
        build_nearbyKeys();   
    }


    //-- 获得本 chunk 周围 8个chunk (以及自己) 的 chunkKeys 集合 的访问权 --
    inline const std::vector<u64_t> &get_near_9_chunkKeys() const {
        return nearbyKeys;
    }


    inline const u64_t get_key() const {
        return key;
    }
    inline const IntVec2& get_mpos() const {
        return mpos;
    }

private:

    //-- 填充 nearbyKeys --
    //  共9个元素，与 NineBox 次序一致
    inline void build_nearbyKeys(){
        IntVec2 tmpMPos;
        u64_t   tmpKey;
        //-- 遍历九宫格 --
        for( int h=-1; h<=1; h++  ){
            for( int w=-1; w<=1; w++ ){
                //-- 自己 chunk --
                if( (h==0) && (w==0) ){
                    nearbyKeys.push_back( key );
                    continue;
                }
                tmpMPos.x = mpos.x + w*ENTS_PER_CHUNK;
                tmpMPos.y = mpos.y + h*ENTS_PER_CHUNK;
                tmpKey = chunkMPos_2_key( tmpMPos );
                //---
                nearbyKeys.push_back( tmpKey );//- copy
            }
        }
        assert( nearbyKeys.size() == 9 );
    }


    //========= vals ========//
    u64_t      key   {0};
    IntVec2    mpos  {0,0}; //- chunk 左下ent.mpos 
    //-----
    std::vector<u64_t>  nearbyKeys; //- 周围8个chunk 的 keys
};



/* ===========================================================
 *             just_creat_chunkKey_by_mpos
 * -----------------------------------------------------------
 * -- 当需要通过 mpos 计算出它的 key，又不需要 正式制作一个 ChunkKey实例时，
 *    推荐使用本函数。
 * ------
 * param: _mpos -- 任意 mapent 的 mpos
 */
inline u64_t just_creat_chunkKey_by_mpos( const IntVec2 &_mpos ){
    //-- “地板除法，向低取节点值”, 再乘回 节点间距。
    //   获得 所在chunk 左下ent mpos
    IntVec2 mpos = get_chunk_mpos( _mpos );
    //--------
    return chunkMPos_2_key( mpos );
}






#endif 


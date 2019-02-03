/*
 * ====================== SectionKey.h =======================
 *                          -- tpr --
 *                                        CREATE -- 2019.01.07
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *    mapSection "id":  (int)w + (int)h
 * ----------------------------
 */
#ifndef _TPR_SECTION_KEY_H_
#define _TPR_SECTION_KEY_H_

//------------------- Libs --------------------//
#include "tprDataType.h" 

//-------------------- Engine --------------------//
#include "config.h"
#include "IntVec.h" 


//-- based on mapEntPos 
class SectionKey{
public:
    SectionKey() = default;

    inline void init_by_key( const u64 _key ){
        key = _key;
        //-------
        int *ptr = (int*)&key;
        mpos.x = *ptr;
        ptr++;
        mpos.y = *ptr;
    }

    //-- 此函数不能用于 碰撞检测 --
    /*
    inline void init_by_mpos( const IntVec2 &_mpos ){
        mpos = _mpos;
        //--------
        int *ptr = (int*)&key;
        *ptr = mpos.x;
        ptr++;
        *ptr = mpos.y;
    }
    */

    //-- 使用此函数来 根据 “任意mapent mpos” 初始化其对应的 key
    inline void init_by_mapEnt_mpos( const IntVec2 &_mpos ){

        mpos.set(   _mpos.x/SECTION_W_ENTS,
                    _mpos.y/SECTION_H_ENTS );
        //--------
        int *ptr = (int*)&key;
        *ptr = mpos.x;
        ptr++;
        *ptr = mpos.y;     
    }


    inline const u64 get_key() const {
        return key;
    }
    inline const IntVec2& get_pos() const {
        return mpos;
    }

private:
    u64        key   {0};
    IntVec2    mpos  {0,0};
};


#endif 


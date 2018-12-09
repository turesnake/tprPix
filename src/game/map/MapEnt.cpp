/*
 * ====================== MapEnt.cpp =======================
 *                          -- tpr --
 *                                        创建 -- 2018.12.09
 *                                        修改 -- 2018.12.09
 * ----------------------------------------------------------
 *    map 地面单位 
 *    ----------
 *    
 * ----------------------------
 */
#include "MapEnt.h"


/* ===========================================================
 *                       d2m      
 * -----------------------------------------------------------
 * -- 从 Fst_diskMapEnt 实例中 解析数据，转存到 Fst_memMapEnt 实例中
 */
void Fst_memMapEnt::d2m( Fst_diskMapEnt *_me ){

    //--------- is_land -----------------
    ( (((_me->mask_id)>>7) & 1) == 1 ) ?
        is_land = true : is_land = false;
    
    //---------- mask_id ----------------
    mask_id = _me->mask_id & 0x7F; //- 抹掉高位 1-bit
    
    //---------- altitude ----------------
    altitude = (_me->fst_data)>>4;

    //---------- is_reserved ----------------
    ( (((_me->fst_data)>>3) & 1) == 1 ) ?
        is_reserved = true : is_reserved = false;

    //---------- is_covered ----------------
    ( (((_me->fst_data)>>2) & 1) == 1 ) ?
        is_covered = true : is_covered = false;

    //---------- is_cover_go_head ----------------
    ( (((_me->fst_data)>>1) & 1) == 1 ) ?
        is_cover_go_head = true : is_cover_go_head = false;

    //---------- tex_id ----------------
    tex_id = _me->tex_id;

    //---------- sec_data_id ----------------
    sec_data_id = _me->sec_data_id;

}


/* ===========================================================
 *                       m2d   
 * -----------------------------------------------------------
 * -- 从 Fst_memMapEnt 实例中 解析数据，转存到 Fst_diskMapEnt 实例中
 */
Fst_diskMapEnt Fst_memMapEnt::m2d(){

    Fst_diskMapEnt me; //- return

    //--------- is_land -----------------
    (is_land == true) ? me.mask_id = 0x80 : me.mask_id = 0;

    //--------- mask_id -----------------
    me.mask_id += mask_id & 0x7F;

    //--------- altitude -----------------
    me.fst_data = 0;
    me.fst_data = (altitude & 0xF) << 4; //- 高4-bit：altitude

    //--------- is_reserved -----------------
    if( is_reserved == true ){
        me.fst_data += 1<<3;
    }

    //--------- is_covered -----------------
    if( is_covered == true ){
        me.fst_data += 1<<2;
    }

    //--------- is_cover_go_head -----------------
    if( is_cover_go_head == true ){
        me.fst_data += 1<<1;
    }

    //--------- tex_id -----------------
    me.tex_id = tex_id;

    //--------- sec_data_id -----------------
    me.sec_data_id = sec_data_id;

    return me;
}



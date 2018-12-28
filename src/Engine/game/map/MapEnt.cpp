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
#include "Engine/game/map/MapEnt.h"


/* ===========================================================
 *                       fst_d2m      
 * -----------------------------------------------------------
 * -- 从 Fst_diskMapEnt 实例中 解析数据，转存到 MemMapEnt 实例中
 */
void MemMapEnt::fst_d2m( Fst_diskMapEnt *_dme ){
    
    //---------- mask_id ----------------
    mask_id = _dme->mask_id & 0x7F; //- 抹掉高位 1-bit
    
    //---------- altitude ----------------
    altitude = (_dme->fst_data)>>4;

    //---------- is_XXX ----------------
    is_land =          ((((_dme->mask_id)>>7) & 1) == 1 );
    is_reserved =      ((((_dme->fst_data)>>3) & 1) == 1 );
    is_covered =       ((((_dme->fst_data)>>2) & 1) == 1 );    
    is_cover_go_head = is_covered;

            //- 此时 的 is_covered 并未初始化完全：
            //  在 硬盘态，只有 head ent 被记录。而此 go 的剩余 ent，没有记录 
            //  需要在 go 的恢复过程中，在此 “检查-改写” 那些 非head ent。
            //  补上所有的 is_covered 设置

    //---------- tex_id ----------------
    tex_id = _dme->tex_id;

    //---------- sec_data_id ----------------
    sec_data_id = _dme->sec_data_id;

    //--------- is_XXX_go_default -------
    is_major_go_default   = ((((_dme->sec_data_info)>>7) & 1) == 1);
}


/* ===========================================================
 *                       fst_m2d   
 * -----------------------------------------------------------
 * -- 从 MemMapEnt 实例中 解析数据，转存到 Fst_diskMapEnt 实例中
 */
Fst_diskMapEnt MemMapEnt::fst_m2d(){

    Fst_diskMapEnt me; //- return

    //--------- is_land -----------------
    (is_land == true) ? me.mask_id = 0x80 : me.mask_id = 0;

    //--------- mask_id -----------------
    me.mask_id += mask_id & 0x7F;

    //--------- altitude -----------------
    me.fst_data = (altitude & 0xF) << 4; //- 高4-bit：altitude

    //--------- is_reserved -----------------
    if( is_reserved == true )     { me.fst_data += 1<<3; }
    if( is_covered == true )      { me.fst_data += 1<<2; }

            //- 硬盘态 并不保留 is_cover_go_head 信息

    //--------- tex_id -----------------
    me.tex_id = tex_id;

    //--------- sec_data_id -----------------
    me.sec_data_id = sec_data_id;

    //--------- is_XXX_go_default -------
    if( is_major_go_default == true )  { me.sec_data_info += 1<<7; }
    
    return me;
}



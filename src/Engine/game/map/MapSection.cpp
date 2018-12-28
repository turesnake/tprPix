/*
 * ====================== MapSection.cpp =======================
 *                          -- tpr --
 *                                        创建 -- 2018.12.09
 *                                        修改 -- 2018.12.09
 * ----------------------------------------------------------
 *    map section 一个地图区域。 左下坐标系
 *    ----------
 *    
 * ----------------------------
 */
#include "Engine/game/map/MapSection.h"








/* ===========================================================
 *                  get_mapSection_key
 * -----------------------------------------------------------
 * -- 根据 一个 mapent 的 pos，计算出 它所属的 section 的 key
 */
u64 get_mapSection_key( PixVec2 _pos ){

    int w = SECTION_W * MAPENT_PIX_STEP;
    int h = SECTION_H * MAPENT_PIX_STEP;

    u64 x = (u64)(_pos.x/w) * w;
    u64 y = (u64)(_pos.y/h) * h;

    return ( (x<<32) + y );
}





/*
 * ====================== MapSection.h =======================
 *                          -- tpr --
 *                                        创建 -- 2018.12.09
 *                                        修改 -- 2018.12.09
 * ----------------------------------------------------------
 *    map section 一个地图区域。 左下坐标系
 *    ----------
 *    
 * ----------------------------
 */
#ifndef _TPR_MAP_SECTION_H_
#define _TPR_MAP_SECTION_H_

//-------------------- CPP --------------------//
#include <vector>

//------------------- SELF --------------------//
#include "Engine/game/PixVec.h" 
#include "Engine/game/gameObj/GameObj.h" 
#include "Engine/game/map/MapEnt.h"

//-- 1个 mapent 占用 3*3 个像素
inline constexpr int MAPENT_PIX_STEP = 3;
//-- 一个 section。长宽各多少个 mapEnt --
inline constexpr int SECTION_W = 256; 
inline constexpr int SECTION_H = 256; 


//-- 256*256 个 Fst_diskMapEnt 元素.[硬盘态] --
struct Fst_diskMapSection{

    Fst_diskMapEnt data[ SECTION_W * SECTION_H ]; //- 512KB
};



//-- 256*256 个 mapEnt, 组成一张 section --
//  section 作为一个整体被存储到硬盘，就像 mc 中的 chunk
class MapSection{
public:
    explicit MapSection(){

    }



    std::vector<MemMapEnt> memMapEnts; 

    PixVec2  pos; //- 本 section 左下角 mapEnt pos.
    u64      key; //- 本 section 左下角 mapEnt，的 宽长坐标值（int）:w+h


private:


};


//-- 根据 一个 mapent 的 pos，计算出 它所属的 section 的 key --
u64 get_mapSection_key( PixVec2 _pos );







#endif


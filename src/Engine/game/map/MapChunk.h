/*
 * ===================== MapChunk.h =======================
 *                          -- tpr --
 *                                        CREATE -- 2019.02.27
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *   4*4mapent 构成一个 chunk
 * ----------------------------
 */
#ifndef _TPR_MAP_CHUNK_H
#define _TPR_MAP_CHUNK_H

//-------------------- CPP --------------------//
#include <vector>

//-------------------- Engine --------------------//
#include "config.h"
#include "IntVec.h"
#include "EcoSysType.h"
#include "MapCoord.h"


//-- 4*4mapent 构成一个 chunk -- [mem]
//  另一个身份是 “距离场” 
//  每一个 section 都要存储 64*64个 MapChunk数据。
//  而且每个 section的这组数据，都是在 section被正式生成前 就已经创建了
// （由相邻的section）。这组数据会 始终存储在 mem/disk 。需要考虑其 空间开销
//  ------
//  为了节约空间，将一部分 build阶段需要处理的数据，存储在 MapChunkInBuild 中。
class MapChunk{
public:

    //----- 第一阶段生成的数据 ------//
    IntVec2     nodePPos {};    //- 距离场点 ppos (20*20pix 中的一个点) （均匀距离场）
                                //- 绝对 ppos 坐标。
    IntVec2     lNodePPosOff {};//- 基于 本section 左下角 的 相对 ppos偏移
    MapCoord    mcpos    {};    //- field左下角mcpos
                                 // 这么存储很奢侈，也许会在未来被取消...

    //----- 第二阶段生成的数据 ------//
    u8_t        isLand      {1}; //- 1:land, 0:waters
    EcoSysType  ecoSysType  { EcoSysType::Forst }; 
    

    //====== static =======//
    static  int  entSideLen; //- 4*4   个 mapent (只记录边长)
    static  int  pixSideLen; //- 20*20 个 pixel  (只记录边长)
};
//====== static =======//
inline int  MapChunk::entSideLen  { 4 };
inline int  MapChunk::pixSideLen  { 4*PIXES_PER_MAPENT }; //- 20


#endif 


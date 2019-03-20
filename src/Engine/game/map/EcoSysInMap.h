/*
 * ========================== EcoSysInMap.h =======================
 *                          -- tpr --
 *                                        CREATE -- 2019.03.02
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 * ----------------------------
 */
#ifndef _TPR_ECOSYS_IN_MAP_H_
#define _TPR_ECOSYS_IN_MAP_H_

//-------------------- CPP --------------------//
#include <vector>

//-------------------- Engine --------------------//
#include "sectionKey.h"
#include "EcoSysType.h"
#include "Quad.h"


//- 一个在游戏地图上存在的 实实在在的区域。
//- 在游戏世界中，每个 section左下角，都放置一个 EcoSys数据集 
//  存储了这个 EcoSys实例 的各种数据 （用来生成地图）
//  ----
//  这组数据会暂时 长期存储在 mem/disk
class EcoSysInMap{
public:
    EcoSysInMap() = default;

    void init();

    void plan(); //- 核心函数

    //-- param: _sectionMPos - 本ecoSysInMap.key 所在 section 的 左下角坐标。
    inline void set_by_sectionMPos( const IntVec2 &_sectionMPos ){
        this->sectionKey = sectionMPos_2_sectionKey( _sectionMPos );
        mcpos.set_by_mpos( sectionKey_2_mpos(this->sectionKey) );        
    }
    inline void set_by_sectionKey( sectionKey_t _sectionKey ){
        this->sectionKey = _sectionKey;
        mcpos.set_by_mpos( sectionKey_2_mpos(this->sectionKey) );        
    }

    inline void accum_chunkNum( QuadType _type ){
        chunkNums_per_quad.at(QuadType_2_Idx(_type))++;
    }


    //======== vals ========//
    sectionKey_t  sectionKey {};
    MapCoord      mcpos  {}; //- [left-bottom]

    EcoSysType    ecoSysType  {EcoSysType::Forst};


    std::vector<sectionKey_t>  quadSectionKeys; //- 本 ecosy 含有的四个 section的 keys


    std::vector<int> chunkNums_per_quad { 0, 0, 0, 0, }; //- 每个象限含有的 chunk数量
    int   chunkTotalNum  {0}; //- 本 ecosys 总共包含几个chunk
    
    //======== flags ========//
    bool       is_quadSectionKeys_set {false};
    QuadFlag   is_all_sections_done  {false}; //- 本 ecosys包含的 四个象限section 是否都生成完毕 
                    
    bool       is_plan_done  {false};


    void init_quadSectionKeys();

};









#endif 


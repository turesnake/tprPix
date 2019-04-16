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
#include "occupyWeight.h"


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

    inline const IntVec2& get_mpos() const {
        return this->mcpos.get_mpos();
    }

    //======== vals ========//
    sectionKey_t  sectionKey {};
    MapCoord      mcpos  {}; //- [left-bottom]

    EcoSysType    ecoSysType  {EcoSysType::Forest};

    int   chunkTotalNum  {0}; //- 本 ecosys 总共包含几个chunk

    float weight {};  //- 根据 simplexNoise 生成的 权重值。[-100.0, 100.0]
                      // [just mem] 

    occupyWeight_t  occupyWeight {0}; //- 抢占权重。 [0,15]
                            //- 数值越高，此 ecosys 越强势，能占据更多fields
                            //- [just mem] 
    
    //======== flags ========//                    
    bool       is_plan_done  {false};


private:

    void init_occupyWeight();

};









#endif 


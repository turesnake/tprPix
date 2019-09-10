/*
 * ==================== MapSurfaceRandSet.h =======================
 *                          -- tpr --
 *                                        CREATE -- 2019.09.08
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#ifndef TPR_MAP_SURFACE_RAND_SET_H
#define TPR_MAP_SURFACE_RAND_SET_H

//-------------------- CPP --------------------//
#include <cmath>
#include <vector>
#include <unordered_map>

//-------------------- Engine --------------------//
#include "fieldKey.h"
#include "ID_Manager.h" 
#include "mapSurfaceRandEntId_t.h"


enum class MapSurfaceRandLvl{
    Nil, //- none
    Sml,
    Mid,
    Big
};


//-- 管理 一个chunk 的 所有 fields，是否种植 mapsurface, 种的类型（lvl）--
class MapSurfaceRandEnt{
public:
    MapSurfaceRandEnt() = default;
    size_t smlPointNum {};
    size_t midPointNum {};
    size_t bigPointNum {};
    std::unordered_map<size_t, MapSurfaceRandLvl> datas {};
                        //- key: 目标field，在 本chunk 中的 idx
    
};



class MapSurfaceRandSet{
public: 
    MapSurfaceRandSet()=default;

    void load_mapSurfaceRandSets();



    inline mapSurfaceRandEntId_t apply_a_mapSurfaceRandEntId( double randVal_ )noexcept{
        size_t idx = static_cast<size_t>(floor(randVal_)) % this->ids.size();
        return this->ids.at(idx);
    }


private:
    std::unordered_map<mapSurfaceRandEntId_t, MapSurfaceRandEnt> ents {};
    std::vector<mapSurfaceRandEntId_t>                           ids  {};


    //======== static ========//
    static ID_Manager  id_manager; //- 负责生产 id 


};
//======== static ========//
inline ID_Manager  MapSurfaceRandSet::id_manager { ID_TYPE::U32, 0};




#endif 



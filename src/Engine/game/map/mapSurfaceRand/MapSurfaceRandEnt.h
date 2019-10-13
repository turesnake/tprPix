/*
 * ==================== MapSurfaceRandEnt.h =======================
 *                          -- tpr --
 *                                        CREATE -- 2019.09.08
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#ifndef TPR_MAP_SURFACE_RAND_SET_H
#define TPR_MAP_SURFACE_RAND_SET_H
//--- glm - 0.9.9.5 ---
#include "glm_no_warnings.h"

//-------------------- CPP --------------------//
#include <cmath>
#include <vector>
#include <unordered_map>
#include <utility>
#include <memory>

//-------------------- Engine --------------------//
#include "fieldKey.h"
#include "ID_Manager.h" 
#include "mapSurfaceRandEntId_t.h"
#include "MapSurfaceRandLvl.h"
#include "tprCast.h"

namespace mapSurface {//------------------ namespace: mapSurface -------------------------//


//-- 管理 一个chunk 的 所有 fields，是否种植 mapsurface, 种的类型（lvl）--
class RandEnt{
public:
    RandEnt() = default;
    size_t smlPointNum {}; // not used yet...
    size_t midPointNum {}; // not used yet...
    size_t bigPointNum {}; // not used yet...
    std::unordered_map<size_t, std::pair<RandEntLvl,glm::dvec2>> datas {};
                        //- key:            field_idx in chunk
                        //  pair.first:     randEntLvl
                        //  pair.second:    ent_dposOff in field [left-bottom]
    //======== static ========//
    static ID_Manager  id_manager;
    static std::pair<RandEntLvl,glm::dvec2> nilEntData; // retVal in RandEntSet::get_mapSurfaceRandEntData()
};


class RandEntSet{
public: 
    RandEntSet()=default;

    void load_mapSurfaceRandSets();

    inline randEntId_t apply_a_randEntId( size_t randUVal_ )const noexcept{
        
        size_t idx = (randUVal_ + 677601) % this->ids.size();
        return this->ids.at(idx);
    }

    inline const std::pair<RandEntLvl,glm::dvec2> &
    get_randEntData( randEntId_t entId_, size_t fieldIdx_in_chunk_  )const noexcept{
        tprAssert( this->entUPtrs.find(entId_) != this->entUPtrs.end() );
        auto &entRef = *(this->entUPtrs.at(entId_));
        if( entRef.datas.find(fieldIdx_in_chunk_) == entRef.datas.end() ){
            return RandEnt::nilEntData;
        }else{
            return entRef.datas.at(fieldIdx_in_chunk_);
        }
    }

private:
    std::unordered_map<randEntId_t, std::unique_ptr<RandEnt>> entUPtrs {};
    std::vector<randEntId_t>                 ids  {};

};




}//---------------------- namespace: mapSurface -------------------------//
#endif 



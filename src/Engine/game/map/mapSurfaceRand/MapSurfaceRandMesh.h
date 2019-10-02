/*
 * ==================== MapSurfaceRandMesh.h =======================
 *                          -- tpr --
 *                                        CREATE -- 2019.10.02
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#ifndef TPR_MAP_SURFACE_RAND_MESH_H
#define TPR_MAP_SURFACE_RAND_MESH_H
//--- glm - 0.9.9.5 ---
#include "glm_no_warnings.h"

//-------------------- CPP --------------------//
#include <cmath>
#include <vector>
#include <unordered_map>
#include <map>
#include <utility>
#include <memory>
  

//-------------------- Engine --------------------//
#include "fieldKey.h"
#include "ID_Manager.h" 
#include "mapSurfaceRandEntId_t.h"
#include "MapSurfaceRandLvl.h"
#include "tprCast.h"



namespace mapSurface {//------------------ namespace: mapSurface -------------------------//



// single mesh data
struct RandMeshData{
    RandMeshLvl meshLvl {}; 
    glm::dvec2  dposOff {}; // offset from field MidDPos
    IntVec2     mposOff {}; // offset from field LeftBottom
};


// one instance, map to one rendEntGo 
class RandMesh{
public:
    RandMesh()=default;

    inline void push_back( RandMeshLvl lvl_, const glm::dvec2 &dposOff_, IntVec2 mposOff_ )noexcept{
        this->data.push_back( RandMeshData{ lvl_, dposOff_, mposOff_ } );
    }

    inline const std::vector<RandMeshData> &get_data()const noexcept{
        return this->data;
    }

private:
    std::vector<RandMeshData> data {};
};



class RandMeshSet{
public:
    RandMeshSet()
        {
            this->dataUPtrs.insert({ RandEntLvl::Field_1f1,  std::vector<std::unique_ptr<RandMesh>>{} });
            this->dataUPtrs.insert({ RandEntLvl::Field_2f2,  std::vector<std::unique_ptr<RandMesh>>{} });
            this->dataUPtrs.insert({ RandEntLvl::Field_3f3,  std::vector<std::unique_ptr<RandMesh>>{} });
        }


    inline RandMesh &apply_new_randMeshData( RandEntLvl lvl_ )noexcept{
        tprAssert( this->dataUPtrs.find(lvl_) != this->dataUPtrs.end() );
        auto &vecRef = this->dataUPtrs.at(lvl_);
        vecRef.push_back( std::make_unique<RandMesh>() );
        return *(vecRef.back().get());
    }


    inline const RandMesh &get_a_randMeshData( RandEntLvl lvl_, double randVal_ )const noexcept{
        tprAssert( this->dataUPtrs.find(lvl_) != this->dataUPtrs.end() );
        auto &vecRef = this->dataUPtrs.at(lvl_);
        //---
        double absVal = std::abs(randVal_ * 3.17 + 160.71);
        size_t idx = cast_2_size_t(floor(absVal)) % vecRef.size();
        return *(vecRef.at(idx).get());
    }


private:
    std::unordered_map<RandEntLvl, std::vector< std::unique_ptr<RandMesh> >> dataUPtrs {};
        // Field_1f1: vector<>: apply a randMeshData
        // Field_2f2: vector<>: apply a randMeshData
        // Field_3f3: vector<>: apply a randMeshData
};




}//---------------------- namespace: mapSurface -------------------------//
#endif 


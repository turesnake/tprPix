/*
 * ================ calc_groundGoMeshName.cpp =======================
 *                          -- tpr --
 *                                        CREATE -- 2020.02.27
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */

//-------------------- CPP --------------------//
#include <string>
#include <unordered_map>

//-------------------- Engine --------------------//
#include "tprAssert.h"
#include "ColorTable.h"
#include "groundGoEntType.h"

#include "esrc_colorTableSet.h"



namespace cggmn_inn {//-------- namespace: cggmn_inn --------------//


    const std::unordered_map<GroundGoEntType, std::string> forest_umap{
        { GroundGoEntType::MapEnt,      "forest_sml" },
        { GroundGoEntType::HalfField,   "forest_mid" },
        { GroundGoEntType::Field,       "forest_big" },
        { GroundGoEntType::SimpleField, "forest_big" }
    };

    const std::unordered_map<GroundGoEntType, std::string> darkForest_umap{
        { GroundGoEntType::MapEnt,      "darkForest_sml" },
        { GroundGoEntType::HalfField,   "darkForest_mid" },
        { GroundGoEntType::Field,       "darkForest_big" },
        { GroundGoEntType::SimpleField, "darkForest_big" }
    };

    const std::unordered_map<GroundGoEntType, std::string> desert_umap{
        { GroundGoEntType::MapEnt,      "desert_sml" },
        { GroundGoEntType::HalfField,   "desert_mid" },
        { GroundGoEntType::Field,       "desert_big" },
        { GroundGoEntType::SimpleField, "desert_big" }
    };

    const std::unordered_map<GroundGoEntType, std::string> savannah_umap{
        { GroundGoEntType::MapEnt,      "savannah_sml" },
        { GroundGoEntType::HalfField,   "savannah_mid" },
        { GroundGoEntType::Field,       "savannah_big" },
        { GroundGoEntType::SimpleField, "savannah_big" }
    };

    const std::unordered_map<EcoSysPlanType, const std::unordered_map<GroundGoEntType, std::string>*> goMeshNames{
        { EcoSysPlanType::Forest,       &forest_umap },
        { EcoSysPlanType::DarkForest,   &darkForest_umap },
        { EcoSysPlanType::Desert,       &desert_umap },
        { EcoSysPlanType::Savannah,     &savannah_umap }
    };

}//------------- namespace: cggmn_inn end --------------//



const std::string &calc_groundGoMeshName( GroundGoEntType groundType_, colorTableId_t colorTableId_ )noexcept{

    EcoSysPlanType ecoType = esrc::get_colorTabelSet().get_ecoSysPlanType(colorTableId_);
    //---
    tprAssert( cggmn_inn::goMeshNames.find(ecoType) != cggmn_inn::goMeshNames.end() );
    const auto &umap = cggmn_inn::goMeshNames.at(ecoType);
    //---
    tprAssert( umap->find(groundType_) != umap->end() );
    return umap->at(groundType_);
}



/*
 * ================ calc_groundGoMeshName.cpp =======================
 *                          -- tpr --
 *                                        CREATE -- 2020.02.27
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#include "pch.h"

//-------------------- Engine --------------------//
#include "ColorTable.h"
#include "fieldFractType.h"

#include "esrc_colorTableSet.h"



namespace cggmn_inn {//-------- namespace: cggmn_inn --------------//


    const std::unordered_map<FieldFractType, std::string> forest_umap{
        { FieldFractType::MapEnt,      "forest_sml" },
        { FieldFractType::HalfField,   "forest_mid" },
        { FieldFractType::Field,       "forest_big" }
    };

    const std::unordered_map<FieldFractType, std::string> darkForest_umap{
        { FieldFractType::MapEnt,      "darkForest_sml" },
        { FieldFractType::HalfField,   "darkForest_mid" },
        { FieldFractType::Field,       "darkForest_big" }
    };

    const std::unordered_map<FieldFractType, std::string> desert_umap{
        { FieldFractType::MapEnt,      "desert_sml" },
        { FieldFractType::HalfField,   "desert_mid" },
        { FieldFractType::Field,       "desert_big" }
    };

    const std::unordered_map<FieldFractType, std::string> savannah_umap{
        { FieldFractType::MapEnt,      "savannah_sml" },
        { FieldFractType::HalfField,   "savannah_mid" },
        { FieldFractType::Field,       "savannah_big" }
    };

    const std::unordered_map<EcoSysPlanType, const std::unordered_map<FieldFractType, std::string>*> goMeshNames{
        { EcoSysPlanType::Forest,       &forest_umap },
        { EcoSysPlanType::DarkForest,   &darkForest_umap },
        { EcoSysPlanType::Desert,       &desert_umap },
        { EcoSysPlanType::Savannah,     &savannah_umap }
    };

}//------------- namespace: cggmn_inn end --------------//



const std::string &calc_groundGoMeshName( FieldFractType fieldFractType_, colorTableId_t colorTableId_ )noexcept{

    EcoSysPlanType ecoType = esrc::get_colorTabelSet().get_ecoSysPlanType(colorTableId_);
    //---
    tprAssert( cggmn_inn::goMeshNames.find(ecoType) != cggmn_inn::goMeshNames.end() );
    const auto &umap = cggmn_inn::goMeshNames.at(ecoType);
    //---
    tprAssert( umap->find(fieldFractType_) != umap->end() );
    return umap->at(fieldFractType_);
}



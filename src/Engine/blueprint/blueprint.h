/*
 * ======================= blueprint.h =======================
 *                          -- tpr --
 *                                        CREATE -- 2019.12.04
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *  interface
 */
#ifndef TPR_BLUE_PRINT_H
#define TPR_BLUE_PRINT_H

//-------------------- CPP --------------------//
#include <vector>
#include <unordered_map>

//-------------------- Engine --------------------//
#include "blueprintId.h"
#include "mapEntKey.h"
#include "GoDataForCreate.h"
#include "IntVec.h"


namespace blueprint {//------------------ namespace: blueprint start ---------------------//


void init_blueprint()noexcept;

villageBlueprintId_t str_2_villageBlueprintId( const std::string &name_ )noexcept;


void build_ecoObj_goDatasForCreate( villageBlueprintId_t villageId_,
                                    IntVec2  ecoObjMPos_,
                                    size_t   ecoObjUWeight_,
                                    std::unordered_map<mapEntKey_t, std::unique_ptr<GoDataForCreate>> &majorGoDatasForCreate_,
                                    std::unordered_map<mapEntKey_t, std::unique_ptr<GoDataForCreate>> &floorGoDatasForCreate_ );


}//--------------------- namespace: blueprint end ------------------------//
#endif 


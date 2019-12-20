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
#include <unordered_set>

//-------------------- Engine --------------------//
#include "blueprintId.h"
#include "mapEntKey.h"
#include "GoDataForCreate.h"
#include "IntVec.h"
#include "fieldKey.h"

namespace blueprint {//------------------ namespace: blueprint start ---------------------//


void init_blueprint()noexcept;

villageBlueprintId_t str_2_villageBlueprintId( const std::string &name_ )noexcept;


void build_ecoObj_goDatasForCreate( villageBlueprintId_t villageId_,
                                    IntVec2  ecoObjMPos_,
                                    size_t   ecoObjUWeight_,
                                    std::unordered_map<mapEntKey_t, std::unique_ptr<GoDataForCreate>> &majorGoDatasForCreate_,
                                    std::unordered_map<mapEntKey_t, std::unique_ptr<GoDataForCreate>> &floorGoDatasForCreate_,
                                    std::unordered_set<fieldKey_t> &artifactFieldKeys );


void build_yard_goDatasForCreate(   std::unordered_map<mapEntKey_t, std::unique_ptr<GoDataForCreate>> &majorGoDatasForCreate_,
                                    std::unordered_map<mapEntKey_t, std::unique_ptr<GoDataForCreate>> &floorGoDatasForCreate_,
                                    yardBlueprintId_t yardId_,
                                    IntVec2 yardMPos_,
                                    size_t yard_uWeight_ );
                                        

}//--------------------- namespace: blueprint end ------------------------//
#endif 


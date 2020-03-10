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

#include "pch.h"

//-------------------- Engine --------------------//
#include "blueprintId.h"
#include "mapEntKey.h"
#include "GoDataForCreate.h"
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


void build_natureYard_majorGoDatasForCreate(   
                                        std::unordered_map<mapEntKey_t, std::unique_ptr<GoDataForCreate>> &majorGoDatasForCreate_,
                                        yardBlueprintId_t natureMajorYardId_,
                                        IntVec2 yardMPos_,
                                        size_t yard_uWeight_,
                                        std::function<bool(IntVec2)> f_is_mapent_in_bioSoup_
                                        );

void build_natureYard_floorGoDatasForCreate(
                                        std::unordered_map<mapEntKey_t, std::unique_ptr<GoDataForCreate>> &floorGoDatasForCreate_,
                                        yardBlueprintId_t natureFloorYardId_,
                                        IntVec2 yardMPos_,
                                        size_t yard_uWeight_,
                                        std::function<bool(IntVec2)> f_is_correct_density_
                                        );
                                        

}//--------------------- namespace: blueprint end ------------------------//
#endif 


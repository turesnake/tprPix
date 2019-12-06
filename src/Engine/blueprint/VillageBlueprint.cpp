/*
 * =================== VillageBlueprint.cpp =======================
 *                          -- tpr --
 *                                        CREATE -- 2019.12.02
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#include "VillageBlueprint.h"


namespace blueprint {//------------------ namespace: blueprint start ---------------------//

//============== static ===============//
ID_Manager                                              VillageBlueprint::id_manager { ID_TYPE::U32, 0};
std::unordered_map<std::string, villageBlueprintId_t>   VillageBlueprint::name_2_ids {};
std::unordered_map<villageBlueprintId_t, std::unique_ptr<VillageBlueprint>> VillageBlueprint::villageUPtrs {};


void VillageBlueprint::init_for_static()noexcept{
    VillageBlueprint::name_2_ids.reserve(1000);
    VillageBlueprint::villageUPtrs.reserve(1000);
}


/* [static]
 * 外部禁止 自行创建 Yard 实例，必须通过此函数
 */
villageBlueprintId_t VillageBlueprint::init_new_village( const std::string &name_ ){
    //-- name_2_ids
    villageBlueprintId_t id = VillageBlueprint::id_manager.apply_a_u32_id();
    auto outPair1 = VillageBlueprint::name_2_ids.insert({ name_, id });
    tprAssert( outPair1.second );
    //-- villageUPtrs 
    auto outPair2 = VillageBlueprint::villageUPtrs.insert({ id, std::make_unique<VillageBlueprint>() });
    tprAssert( outPair2.second );
    //--
    return id;
}




}//--------------------- namespace: blueprint end ------------------------//


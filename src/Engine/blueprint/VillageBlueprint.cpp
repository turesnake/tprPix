/*
 * =================== VillageBlueprint.cpp =======================
 *                          -- tpr --
 *                                        CREATE -- 2019.12.02
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#include "VillageBlueprint.h"

//-------------------- Engine --------------------//
#include "esrc_gameSeed.h"


namespace blueprint {//------------------ namespace: blueprint start ---------------------//

//============== static ===============//
std::unordered_set<villageBlueprintId_t>    VillageBlueprint::villageIds {};
std::hash<std::string>                      VillageBlueprint::hasher {};

std::unordered_map<villageBlueprintId_t, std::unique_ptr<VillageBlueprint>> VillageBlueprint::villageUPtrs {};


void VillageBlueprint::init_for_static()noexcept{
    VillageBlueprint::villageUPtrs.reserve(1000);
}



void VarTypeDatas_Village::init_check()noexcept{
    tprAssert( !this->getYardId_functors.empty() );
    //-- shuffle --//
    auto &shuffleEngine = esrc::get_gameSeed().getnc_shuffleEngine(); 
    std::shuffle( this->getYardId_functors.begin(), this->getYardId_functors.end(), shuffleEngine );
}


/* [static]
 * 外部禁止 自行创建 Yard 实例，必须通过此函数
 */
villageBlueprintId_t VillageBlueprint::init_new_village( const std::string &name_ ){
    //-- name_2_ids
    villageBlueprintId_t id = VillageBlueprint::hasher(name_);
    auto outPair1 = VillageBlueprint::villageIds.insert( id );
    tprAssert( outPair1.second );

    //-- villageUPtrs 
    auto outPair2 = VillageBlueprint::villageUPtrs.insert({ id, std::make_unique<VillageBlueprint>() });
    tprAssert( outPair2.second );
    //--
    return id;
}




}//--------------------- namespace: blueprint end ------------------------//


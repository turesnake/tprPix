/*
 * ======================= YardBlueprint.cpp =======================
 *                          -- tpr --
 *                                        CREATE -- 2019.12.04
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#include "YardBlueprint.h"

#include "tprDebug.h"



namespace blueprint {//------------------ namespace: blueprint start ---------------------//


//============== static ===============//
ID_Manager                                          YardBlueprint::id_manager { ID_TYPE::U32, 0 };
std::unordered_map<std::string, yardBlueprintId_t>  YardBlueprint::name_2_ids {};
std::unordered_map<yardBlueprintId_t, std::unique_ptr<YardBlueprint>> YardBlueprint::yardUPtrs {};



void YardBlueprint::init_for_static()noexcept{
    YardBlueprint::name_2_ids.reserve(1000);
    YardBlueprint::yardUPtrs.reserve(1000);
}


/* [static]
 * 外部禁止 自行创建 Yard 实例，必须通过此函数
 */
yardBlueprintId_t YardBlueprint::init_new_yard( const std::string &name_ ){
    //-- name_2_ids
    yardBlueprintId_t id = YardBlueprint::id_manager.apply_a_u32_id();
    auto outPair1 = YardBlueprint::name_2_ids.insert({ name_, id });
    tprAssert( outPair1.second );
    //-- yardUPtrs 
    auto outPair2 = YardBlueprint::yardUPtrs.insert({ id, std::make_unique<YardBlueprint>() });
    tprAssert( outPair2.second );
    //--
    return id;
}



YardBlueprint &YardBlueprint::get_yardBlueprintRef( yardBlueprintId_t id_ )noexcept{

    

    if( YardBlueprint::yardUPtrs.find(id_) == YardBlueprint::yardUPtrs.end() ){
        cout << "yardId = " << id_
            << "; YardBlueprint::yardUPtrs.size() = " << YardBlueprint::yardUPtrs.size()
            << endl;
    }


    tprAssert( YardBlueprint::yardUPtrs.find(id_) != YardBlueprint::yardUPtrs.end() );
    return *(YardBlueprint::yardUPtrs.at(id_));


}





}//--------------------- namespace: blueprint end ------------------------//


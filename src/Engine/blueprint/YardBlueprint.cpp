/*
 * ======================= YardBlueprint.cpp =======================
 *                          -- tpr --
 *                                        CREATE -- 2019.12.04
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#include "pch.h"
#include "YardBlueprint.h"

//-------------------- Engine --------------------//
#include "esrc_gameSeed.h"



namespace blueprint {//------------------ namespace: blueprint start ---------------------//


//============== static ===============//
ID_Manager                                              YardBlueprint::mapDataId_manager { ID_TYPE::U32, 0 };
//---
ID_Manager                                              YardBlueprintSet::yardId_manager { ID_TYPE::U32, 0 };
std::unordered_map<YardBlueprintSet::yardBlueprintSetId_t, std::unique_ptr<YardBlueprintSet>> YardBlueprintSet::setUPtrs {};
std::unordered_map<yardBlueprintId_t, std::unique_ptr<YardBlueprint>>       YardBlueprintSet::yardUPtrs {}; // 真实资源
//---
ID_Manager                                              VarTypeDatas_Yard_MajorGo::id_manager { ID_TYPE::U32, 0 };
ID_Manager                                              VarTypeDatas_Yard_FloorGo::id_manager { ID_TYPE::U32, 0 };


// [*main-thread*]
void YardBlueprintSet::init_for_static()noexcept{
    YardBlueprintSet::setUPtrs.reserve(1000);
    YardBlueprintSet::yardUPtrs.reserve(1000);
}




void YardBlueprint::init_check()noexcept{

    auto &shuffleEngine = esrc::get_gameSeed().getnc_shuffleEngine(); 
    if( this->isHaveMajorGos ){
        tprAssert(  !this->majorGo_mapDataIds.empty() &&
                    !this->majorGo_mapDatas.empty() &&
                    !this->majorGo_varTypes.empty() &&
                    !this->majorGo_varTypeDatas.empty() );
        //-- shuffle --//
        std::shuffle( this->majorGo_mapDataIds.begin(), this->majorGo_mapDataIds.end(), shuffleEngine );
    }
    if( this->isHaveFloorGos ){
        tprAssert(  !this->floorGo_mapDataIds.empty() &&
                    !this->floorGo_mapDatas.empty() &&
                    !this->floorGo_varTypes.empty() &&
                    !this->floorGo_varTypeDatas.empty() );
        //-- shuffle --//
        std::shuffle( this->floorGo_mapDataIds.begin(), this->floorGo_mapDataIds.end(), shuffleEngine );
    }


    if( this->isHaveMajorGos && this->isHaveFloorGos ){
        size_t majorNum = this->majorGo_mapDatas.size();
        size_t floorNum = this->floorGo_mapDatas.size();
        size_t bindNum  = this->majorDataId_2_floorDataIds.size();
        //--
        tprAssert( majorNum == floorNum );
        tprAssert( majorNum == bindNum );
    }

}


void VarTypeDatas_Yard_MajorGo::init_check()noexcept{
    auto &shuffleEngine = esrc::get_gameSeed().getnc_shuffleEngine(); 
    if( this->isPlotBlueprint ){
        tprAssert( !this->plotIds.empty() );
        //-- shuffle --//
        std::shuffle( this->plotIds.begin(), this->plotIds.end(), shuffleEngine );

    }else{
        tprAssert( !this->goSpecPool.empty() );
        tprAssert( !this->goSpecRandPool.empty() );
        //-- shuffle --//
        std::shuffle( this->goSpecRandPool.begin(), this->goSpecRandPool.end(), shuffleEngine );
    }
}



void VarTypeDatas_Yard_FloorGo::init_check()noexcept{
    tprAssert( !this->goSpecPool.empty() );
    tprAssert( !this->goSpecRandPool.empty() );
    //-- shuffle --//
    auto &shuffleEngine = esrc::get_gameSeed().getnc_shuffleEngine(); 
    std::shuffle( this->goSpecRandPool.begin(), this->goSpecRandPool.end(), shuffleEngine );
}


/* [static]
 * 外部禁止 自行创建 Yard 实例，必须通过此函数
 * 允许多次向同一个  yard 实例 添加数据（宽松版）
 */
yardBlueprintId_t YardBlueprintSet::init_new_yard(  const std::string &yardName_, 
                                                    const std::string &yardLabel_,
                                                    NineDirection      yardDir_ ){

    //------------//
    //  yardName
    //------------//
    yardBlueprintSetId_t yardSetId = std::hash<std::string>{}( yardName_ );
    // find or create
    std::unique_ptr<YardBlueprintSet> setUPtr ( new YardBlueprintSet() ); // can't use std::make_unique
    auto [insertIt1, insertBool1] = YardBlueprintSet::setUPtrs.insert({ yardSetId, std::move(setUPtr) });
    YardBlueprintSet *setPtr = insertIt1->second.get();

    //------------//
    //  yardLabel
    //------------//
    std::string yardLabel = check_and_unify_default_labels(yardLabel_); // "_DEFAULT_"
    yardLabelId_t yardLabelId = std::hash<std::string>{}( yardLabel );
    yardBlueprintId_t   yardId {};

    if( setPtr->yardIDs.find(yardLabelId) == setPtr->yardIDs.end() ){ // not find
        auto [insertIt2, insertBool2] = setPtr->yardIDs.insert({ yardLabelId, std::unordered_map<NineDirection, yardBlueprintId_t>{} });
        tprAssert( insertBool2 );
        auto &innUMap = insertIt2->second;
        //---
        yardId = YardBlueprintSet::yardId_manager.apply_a_u32_id();
        
        auto [insertIt3, insertBool3] = innUMap.insert({ yardDir_, yardId });
        tprAssert( insertBool3 ); // Must success

        //---
        auto [insertIt4, insertBool4] = YardBlueprintSet::yardUPtrs.insert({ yardId, YardBlueprint::create_new_uptr() });
        tprAssert( insertBool4 );

    }else{ // find
        tprAssert( setPtr->yardIDs.find(yardLabelId) != setPtr->yardIDs.end() );
        auto &innUMap = setPtr->yardIDs.at( yardLabelId ); // Must Exist

        if( innUMap.find(yardDir_) == innUMap.end() ){ // not find

            yardId = YardBlueprintSet::yardId_manager.apply_a_u32_id();

            auto [insertIt5, insertBool5] = innUMap.insert({ yardDir_, yardId });
            tprAssert( insertBool5 );

            //---
            auto [insertIt6, insertBool6] = YardBlueprintSet::yardUPtrs.insert({ yardId, YardBlueprint::create_new_uptr() });
            tprAssert( insertBool6 ); 

        }else{ // find
            yardId = innUMap.at( yardDir_ ); // Must Exist
            tprAssert( YardBlueprintSet::yardUPtrs.find(yardId) != YardBlueprintSet::yardUPtrs.end() ); // Must Exist
        }
    }
    return yardId;
}








YardBlueprint &YardBlueprintSet::get_yardBlueprintRef( yardBlueprintId_t id_ )noexcept{
    // debug
    if( YardBlueprintSet::yardUPtrs.find(id_) == YardBlueprintSet::yardUPtrs.end() ){
        tprDebug::console(
            "yardId = {0}; YardBlueprint::yardUPtrs.size() = {1}",
            id_, YardBlueprintSet::yardUPtrs.size()
        );
    }
    tprAssert( YardBlueprintSet::yardUPtrs.find(id_) != YardBlueprintSet::yardUPtrs.end() );
    return *(YardBlueprintSet::yardUPtrs.at(id_));
}




// 收集 yard 占据的所有 fieldKeys 
void calc_yard_fieldKeys( std::unordered_set<fieldKey_t> &outContainer_,
                            IntVec2 yardMPos_,
                            YardSize sizeByFild_ )noexcept{

    int sideLen = static_cast<int>(sizeByFild_);

    IntVec2 yardFieldMPos = anyMPos_2_fieldMPos(yardMPos_);
    IntVec2 fieldMPos {};

    for( int j=0; j<sideLen; j++ ){
        for( int i=0; i<sideLen; i++ ){
            fieldMPos.x = yardFieldMPos.x + i*ENTS_PER_FIELD<>;
            fieldMPos.y = yardFieldMPos.y + j*ENTS_PER_FIELD<>;
            outContainer_.insert( fieldMPos_2_fieldKey(fieldMPos) ); // maybe
        }
    }
}






}//--------------------- namespace: blueprint end ------------------------//


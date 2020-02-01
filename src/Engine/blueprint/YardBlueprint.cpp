/*
 * ======================= YardBlueprint.cpp =======================
 *                          -- tpr --
 *                                        CREATE -- 2019.12.04
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#include "YardBlueprint.h"


//-------------------- Engine --------------------//
#include "config.h"
#include "esrc_gameSeed.h"


#include "tprDebug.h"


namespace blueprint {//------------------ namespace: blueprint start ---------------------//


//============== static ===============//
ID_Manager                                              YardBlueprint::mapDataId_manager { ID_TYPE::U32, 0 };
//---
ID_Manager                                              YardBlueprintSet::yardSetId_manager { ID_TYPE::U32, 0 };
ID_Manager                                              YardBlueprintSet::labelId_manager { ID_TYPE::U32, 0 };
ID_Manager                                              YardBlueprintSet::yardId_manager { ID_TYPE::U32, 0 };
std::unordered_map<std::string, yardBlueprintSetId_t>   YardBlueprintSet::name_2_yardSetIds {};
std::unordered_map<yardBlueprintSetId_t, std::unique_ptr<YardBlueprintSet>> YardBlueprintSet::setUPtrs {};
std::unordered_map<yardBlueprintId_t, std::unique_ptr<YardBlueprint>>       YardBlueprintSet::yardUPtrs {}; // 真实资源
//---
ID_Manager                                              VarTypeDatas_Yard_MajorGo::id_manager { ID_TYPE::U32, 0 };
ID_Manager                                              VarTypeDatas_Yard_FloorGo::id_manager { ID_TYPE::U32, 0 };



void YardBlueprintSet::init_for_static()noexcept{
    YardBlueprintSet::name_2_yardSetIds.reserve(1000);
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
    yardBlueprintSetId_t yardSetId {};
    YardBlueprintSet *setPtr {nullptr};

    // find or create
    if( YardBlueprintSet::name_2_yardSetIds.find(yardName_) == YardBlueprintSet::name_2_yardSetIds.end() ){ // not find
        yardSetId = YardBlueprintSet::yardSetId_manager.apply_a_u32_id();
        auto outPair1 = YardBlueprintSet::name_2_yardSetIds.insert({ yardName_, yardSetId });
        tprAssert( outPair1.second );
        //---
        auto outPair2 = YardBlueprintSet::setUPtrs.insert({ yardSetId, std::make_unique<YardBlueprintSet>() });
        tprAssert( outPair2.second );
        setPtr = outPair2.first->second.get();

    }else{ // find
        yardSetId = YardBlueprintSet::name_2_yardSetIds.at(yardName_); // Must Exist
        setPtr = YardBlueprintSet::setUPtrs.at(yardSetId).get(); // Must Exist
    }

    //------------//
    //  yardLabel
    //------------//
    yardLabelId_t       labelId {};
    yardBlueprintId_t   yardId {};

    std::string yardLabel = YardBlueprintSet::check_and_unify_default_labels(yardLabel_); // "_DEFAULT_"

    if( setPtr->name_2_labelIds.find(yardLabel) == setPtr->name_2_labelIds.end() ){ // not find
        labelId = YardBlueprintSet::labelId_manager.apply_a_u32_id();
        auto outPair1 = setPtr->name_2_labelIds.insert({ yardLabel, labelId });
        tprAssert( outPair1.second );
        //---
        yardId = YardBlueprintSet::yardId_manager.apply_a_u32_id();


        auto outPair2 = setPtr->yardIDs.insert({ labelId, std::unordered_map<NineDirection, yardBlueprintId_t>{} });
        tprAssert( outPair2.second ); // Must success
        auto &innUMap = outPair2.first->second;

        auto outPair3 = innUMap.insert({ yardDir_, yardId });
        tprAssert( outPair3.second ); // Must success

        //---
        auto outPair4 = YardBlueprintSet::yardUPtrs.insert({ yardId, std::make_unique<YardBlueprint>() });
        tprAssert( outPair4.second );

    }else{ // find
        labelId = setPtr->name_2_labelIds.at( yardLabel ); // Must Exist

        tprAssert( setPtr->yardIDs.find(labelId) != setPtr->yardIDs.end() );
        auto &innUMap = setPtr->yardIDs.at( labelId ); // Must Exist


        if( innUMap.find(yardDir_) == innUMap.end() ){ // not find

            yardId = YardBlueprintSet::yardId_manager.apply_a_u32_id();

            auto outPair1 = innUMap.insert({ yardDir_, yardId });
            tprAssert( outPair1.second );

            //---
            auto outPair2 = YardBlueprintSet::yardUPtrs.insert({ yardId, std::make_unique<YardBlueprint>() });
            tprAssert( outPair2.second ); 

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
        cout << "yardId = " << id_
            << "; YardBlueprint::yardUPtrs.size() = " << YardBlueprintSet::yardUPtrs.size()
            << endl;
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
            fieldMPos.x = yardFieldMPos.x + i*ENTS_PER_FIELD;
            fieldMPos.y = yardFieldMPos.y + j*ENTS_PER_FIELD;
            outContainer_.insert( fieldMPos_2_fieldKey(fieldMPos) ); // maybe
        }
    }
}






}//--------------------- namespace: blueprint end ------------------------//


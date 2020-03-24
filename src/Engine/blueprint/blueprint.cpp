/*
 * ======================= blueprint.cpp =======================
 *                          -- tpr --
 *                                        CREATE -- 2019.12.04
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#include "pch.h"
#include "blueprint.h"

//-------------------- Engine --------------------//
#include "PlotBlueprint.h"
#include "YardBlueprint.h"
#include "VillageBlueprint.h"
#include "random.h"
#include "blueprint_inn.h"
#include "WindClock.h"
#include "GoSpecFromJson.h"


#include "esrc_state.h"
#include "esrc_animFrameSet.h"



namespace blueprint {//------------------ namespace: blueprint start ---------------------//
namespace blueP_inn {//----------- namespace: blueP_inn ----------------//

    void create_new_goDataForCreate(std::unordered_map<mapEntKey_t, std::unique_ptr<GoDataForCreate>> &goDatasForCreate_,
                                    IntVec2 mpos_,
                                    const glm::dvec2  &dpos_,
                                    const GoSpec &goSpecRef_,
                                    const MapDataEnt &mapDataEntRef_ )noexcept;

    void create_new_floorGoDataForCreate(std::unordered_map<mapEntKey_t, std::unique_ptr<GoDataForCreate>> &goDatasForCreate_,
                                    IntVec2 mpos_,
                                    const GoSpec &goSpecRef_,
                                    const MapDataEnt &mapDataEntRef_,
                                    FloorGoSize floorGoSize_
                                    )noexcept;
    
}//-------------- namespace: blueP_inn end ----------------//


// [*main-thread*]
void init_blueprint()noexcept{
    PlotBlueprint::init_for_static();
    YardBlueprintSet::init_for_static(); // Not YardBlueprint
    VillageBlueprint::init_for_static();
    //---
    parse_plotJsonFiles();
    parse_yardJsonFiles();
    parse_villageJsonFiles();
    //---
    esrc::insertState("blueprint");
}



villageBlueprintId_t str_2_villageBlueprintId( const std::string &name_ )noexcept{
    return VillageBlueprint::str_2_villageBlueprintId( name_ );
}



// 被 job 线程调用 
void build_ecoObj_goDatasForCreate( villageBlueprintId_t villageId_,
                                    IntVec2  ecoObjMPos_,
                                    size_t   ecoObjUWeight_,
                                    std::unordered_map<mapEntKey_t, std::unique_ptr<GoDataForCreate>> &majorGoDatasForCreate_,
                                    std::unordered_map<mapEntKey_t, std::unique_ptr<GoDataForCreate>> &floorGoDatasForCreate_,
                                    std::unordered_set<fieldKey_t> &artifactFieldKeys ){

    // 使用的随机数，直接取自 相关 mpos 的 mapEntKey 值
    size_t  suWeight   {};
    IntVec2 yardMPos {};
    IntVec2 plotMPos {};
    IntVec2 entMPos  {};

    //=================//
    //    village
    //=================//
    VillageBlueprint &villageRef = VillageBlueprint::get_villageBlueprintRef( villageId_ );
    auto varType_village_managerUPtr = std::make_unique<blueP_inn::VarType_Village_Manager>( villageRef );

    const MapData &villageMapData = villageRef.apply_a_random_mapData( ecoObjUWeight_ );
    for( const auto &yEntUPtr : villageMapData.data ){ // each mapDataEnt uptr / yard

        
        //=================//
        //      yard
        //=================//
        yardMPos = ecoObjMPos_ - IntVec2{HALF_ENTS_PER_SECTION<>, HALF_ENTS_PER_SECTION<>} + yEntUPtr->mposOff;
                //-- 蓝图 以 ecoobj/section left-bottom 点为 中心点

        suWeight = calc_simple_mapent_uWeight( yardMPos );
        auto yardId = varType_village_managerUPtr->apply_a_yardBlueprintId( yEntUPtr->varTypeIdx, suWeight, yEntUPtr->direction );

        YardBlueprint &yardRef = YardBlueprintSet::get_yardBlueprintRef( yardId );
        
        auto varType_yard_managerUPtr = std::make_unique<blueP_inn::VarType_Yard_Manager>( yardRef );

        //--- 登记 yard 占据的所有 fieldKey ---
        calc_yard_fieldKeys( artifactFieldKeys, yardMPos, yardRef.get_yardSize() );

        //---------//
        // majorGos
        //---------//
        std::pair<YardBlueprint::mapDataId_t, const MapData*> floorDataPair {};

        if( yardRef.get_isHaveMajorGos() ){

            auto [yardMajorDataId, yardMajorMapDataPtr] = yardRef.apply_a_random_majorGo_mapData( suWeight );
            if( yardRef.get_isHaveFloorGos() ){
                floorDataPair = yardRef.get_binded_floorData( yardMajorDataId );
            }

            for( const auto &pEntUPtr : yardMajorMapDataPtr->data ){ // each mapDataEnt uptr / plot

                const auto *varTypeDatas_Yard_majorGoPtr = yardRef.get_majorGo_varTypeDataPtr_Yard( pEntUPtr->varTypeIdx );

                plotMPos = yardMPos + pEntUPtr->mposOff;
                suWeight = calc_simple_mapent_uWeight( plotMPos );

                if( !varTypeDatas_Yard_majorGoPtr->get_isPlotBlueprint() ){
                    //-- 直接可以获得 mp 位置的 goSpec 数据

                    const GoSpec &goSpecRef = varType_yard_managerUPtr->apply_a_majorGoSpec( pEntUPtr->varTypeIdx, suWeight );

                    //--- 正式在 ecoObj 中创建 major GoDataForCreate 实例 --
                    blueP_inn::create_new_goDataForCreate(majorGoDatasForCreate_,
                                                    plotMPos,
                                                    mpos_2_midDPos(plotMPos),
                                                    goSpecRef,
                                                    *(pEntUPtr) );

                }else{
                    //-- mp 位置存储的是 plot 数据 --

                    //=================//
                    //      plot
                    //=================//s
                    auto plotId = varType_yard_managerUPtr->apply_a_plotBlueprintId( pEntUPtr->varTypeIdx, suWeight );
                    if( PlotBlueprint::is_placeHolderId(plotId) ){
                        continue;
                    }
                    PlotBlueprint &plotRef = PlotBlueprint::get_plotBlueprintRef( plotId );

                    auto varType_Plot_ManagerUPtr = std::make_unique<blueP_inn::VarType_Plot_Manager>( plotRef );

                    const MapData &plotMapData = plotRef.apply_a_random_mapData( suWeight );
                    for( const auto &mpEntUPtr : plotMapData.data ){ // each mapDataEnt uptr / mapent

                        entMPos = plotMPos + mpEntUPtr->mposOff;
                        suWeight = calc_simple_mapent_uWeight( entMPos );

                        const GoSpec &goSpecRef = varType_Plot_ManagerUPtr->apply_a_goSpec( mpEntUPtr->varTypeIdx, suWeight );

                        //--- 正式在 ecoObj 中创建 major GoDataForCreate 实例 --
                        blueP_inn::create_new_goDataForCreate(majorGoDatasForCreate_,
                                                        entMPos,
                                                        mpos_2_midDPos(entMPos),
                                                        goSpecRef,
                                                        *(mpEntUPtr) );
                    }
                }
            }
        }

        //---------//
        // floorGos
        //---------//
        // floor go 不会再细分出 plot 变量
        if( yardRef.get_isHaveFloorGos() ){

            const MapData *floorDataPtr {};

            if( yardRef.get_isHaveMajorGos() ){
                floorDataPtr = floorDataPair.second;
            }else{
                auto pair = yardRef.apply_a_random_floorGo_mapData( suWeight );
                floorDataPtr = pair.second;
            }
            
            for( const auto &pEntUPtr : floorDataPtr->data ){ // each mapDataEnt uptr 

                const auto *varTypeDatas_Yard_floorGoPtr = yardRef.get_floorGo_varTypeDataPtr_Yard( pEntUPtr->varTypeIdx );
                entMPos = yardMPos + pEntUPtr->mposOff; // floorGo mid点所在 mpos
                suWeight = calc_simple_mapent_uWeight( entMPos );

                const GoSpec &goSpecRef = varType_yard_managerUPtr->apply_a_floorGoSpec( pEntUPtr->varTypeIdx, suWeight );
                //--- 正式在 ecoObj 中创建 floor GoDataForCreate 实例 --
                blueP_inn::create_new_floorGoDataForCreate(floorGoDatasForCreate_,
                                                    entMPos,
                                                    goSpecRef,
                                                    *(pEntUPtr),
                                                    varTypeDatas_Yard_floorGoPtr->get_floorGoSize()
                                                    );
            }

        }

    }
}



// 用于 chunk 生成阶段
// 专用于生成 nature majorGos 数据
void build_natureYard_majorGoDatasForCreate(   std::unordered_map<mapEntKey_t, std::unique_ptr<GoDataForCreate>> &majorGoDatasForCreate_,
                                        yardBlueprintId_t natureMajorYardId_,
                                        IntVec2 yardMPos_,
                                        size_t yard_uWeight_,
                                        std::function<bool(IntVec2)> f_is_mapent_in_bioSoup_
                                        ){

    size_t  suWeight  {};
    IntVec2 plotMPos {};
    IntVec2 entMPos  {};

    //---------//
    // majorGos
    //---------//
    YardBlueprint &natureMajoryardRef = YardBlueprintSet::get_yardBlueprintRef( natureMajorYardId_ );

    auto [yardMajorDataId, yardMajorMapDataPtr] = natureMajoryardRef.apply_a_random_majorGo_mapData( yard_uWeight_ );

    auto varType_majorYard_managerUPtr = std::make_unique<blueP_inn::VarType_Yard_Manager>( natureMajoryardRef );

    for( const auto &pEntUPtr : yardMajorMapDataPtr->data ){ // each mapDataEnt uptr / plot

        const auto *varTypeDatas_Yard_majorGoPtr = natureMajoryardRef.get_majorGo_varTypeDataPtr_Yard( pEntUPtr->varTypeIdx );

        plotMPos = yardMPos_ + pEntUPtr->mposOff;
        suWeight = calc_simple_mapent_uWeight( plotMPos );

        if( !varTypeDatas_Yard_majorGoPtr->get_isPlotBlueprint() ){
            //-- 直接可以获得 mp 位置的 goSpec 数据

            // skip mapent in biosoup
            if( f_is_mapent_in_bioSoup_(plotMPos) ){
                continue;
            }

            const GoSpec &goSpecRef = varType_majorYard_managerUPtr->apply_a_majorGoSpec( pEntUPtr->varTypeIdx, suWeight );
            //--- 正式在 ecoObj 中创建 major GoDataForCreate 实例 --
            blueP_inn::create_new_goDataForCreate(majorGoDatasForCreate_,
                                                plotMPos,
                                                mpos_2_midDPos(plotMPos),
                                                goSpecRef,
                                                *(pEntUPtr) );

        }else{
            //-- mp 位置存储的是 plot 数据 --
            //=================//
            //      plot
            //=================//s
            auto plotId = varType_majorYard_managerUPtr->apply_a_plotBlueprintId( pEntUPtr->varTypeIdx, suWeight );
            if( PlotBlueprint::is_placeHolderId(plotId) ){
                continue;
            }
            PlotBlueprint &plotRef = PlotBlueprint::get_plotBlueprintRef( plotId );

            auto varType_Plot_ManagerUPtr = std::make_unique<blueP_inn::VarType_Plot_Manager>( plotRef );

            const MapData &plotMapData = plotRef.apply_a_random_mapData( suWeight );
            for( const auto &mpEntUPtr : plotMapData.data ){ // each mapDataEnt uptr / mapent

                entMPos = plotMPos + mpEntUPtr->mposOff;
                suWeight = calc_simple_mapent_uWeight( entMPos );

                // skip mapent in biosoup
                if( f_is_mapent_in_bioSoup_(entMPos) ){
                    continue;
                }

                const GoSpec &goSpecRef = varType_Plot_ManagerUPtr->apply_a_goSpec( mpEntUPtr->varTypeIdx, suWeight );
                //--- 正式在 ecoObj 中创建 major GoDataForCreate 实例 --
                blueP_inn::create_new_goDataForCreate(majorGoDatasForCreate_,
                                                    entMPos,
                                                    mpos_2_midDPos(entMPos),
                                                    goSpecRef,
                                                    *(mpEntUPtr) );
            }
        }
    }

}



void build_natureYard_floorGoDatasForCreate(
                                        std::unordered_map<mapEntKey_t, std::unique_ptr<GoDataForCreate>> &floorGoDatasForCreate_,
                                        yardBlueprintId_t natureFloorYardId_,
                                        IntVec2 yardMPos_,
                                        size_t yard_uWeight_,
                                        std::function<bool(IntVec2)> f_is_correct_density_
                                        ){

    size_t  suWeight  {};
    IntVec2 plotMPos {};
    IntVec2 entMPos  {};

    //---------//
    // floorGos
    //---------//

    //- 从 ecoobj 中获取的 floorYard 数据    
    YardBlueprint &natureFloorYardRef = YardBlueprintSet::get_yardBlueprintRef( natureFloorYardId_ );

    auto [yardFloorDataId, yardFloorMapDataPtr] = natureFloorYardRef.apply_a_random_floorGo_mapData( yard_uWeight_ );

    auto varType_floorYard_managerUPtr = std::make_unique<blueP_inn::VarType_Yard_Manager>( natureFloorYardRef );


    for( const auto &pEntUPtr : yardFloorMapDataPtr->data ){ // each mapDataEnt uptr 
        const auto *varTypeDatas_Yard_floorGoPtr = natureFloorYardRef.get_floorGo_varTypeDataPtr_Yard( pEntUPtr->varTypeIdx );

        entMPos = yardMPos_ + pEntUPtr->mposOff; // floorGo mid点所在 mpos
        suWeight = calc_simple_mapent_uWeight( entMPos );

        // skip un correct density
        if( !f_is_correct_density_(entMPos) ){
            continue;
        }
        const GoSpec &goSpecRef = varType_floorYard_managerUPtr->apply_a_floorGoSpec( pEntUPtr->varTypeIdx, suWeight );
        //--- 正式在 ecoObj 中创建 floor GoDataForCreate 实例 --
        blueP_inn::create_new_floorGoDataForCreate(floorGoDatasForCreate_,
                                                entMPos,
                                                goSpecRef,
                                                *(pEntUPtr),
                                                varTypeDatas_Yard_floorGoPtr->get_floorGoSize()
                                                );
    
    }
}




namespace blueP_inn {//----------- namespace: blueP_inn ----------------//


// for majorGo
void create_new_goDataForCreate(std::unordered_map<mapEntKey_t, std::unique_ptr<GoDataForCreate>> &goDatasForCreate_,
                                IntVec2 mpos_,
                                const glm::dvec2  &dpos_, // 让外部计算好
                                const GoSpec &goSpecRef_,
                                const MapDataEnt &mapDataEntRef_ 
                                )noexcept{

    // IMPORTANT!!!
    if( goSpecRef_.isPlaceHolder ){
        return;
    }

    //--- 正式在 ecoObj 中创建 GoDataForCreate 实例 --
    auto goDUPtr = GoDataForCreate::create_new_goDataForCreate(  
                                                    mpos_,
                                                    dpos_,
                                                    goSpecRef_.goSpeciesId,
                                                    goSpecRef_.goLabelId,
                                                    mapDataEntRef_.direction,
                                                    mapDataEntRef_.brokenLvl
                                                );

    auto [insertIt, insertBool] = goDatasForCreate_.emplace( mpos_2_key(mpos_), std::move(goDUPtr) );
    tprAssert( insertBool );
}

// for floorGo
void create_new_floorGoDataForCreate(std::unordered_map<mapEntKey_t, std::unique_ptr<GoDataForCreate>> &goDatasForCreate_,
                                IntVec2 mpos_,
                                const GoSpec &goSpecRef_,
                                const MapDataEnt &mapDataEntRef_,
                                FloorGoSize floorGoSize_
                                )noexcept{

    // IMPORTANT!!!
    if( goSpecRef_.isPlaceHolder ){
        return;
    }

    //--- 正式在 ecoObj 中创建 GoDataForCreate 实例 --
    auto goDUPtr = GoDataForCreate::create_new_floorGoDataForCreate(  
                                                    mpos_,
                                                    mpos_2_dpos(mpos_) + calc_floorGo_mid_dposOff(floorGoSize_),
                                                    goSpecRef_.goSpeciesId,
                                                    goSpecRef_.goLabelId,
                                                    mapDataEntRef_.direction
                                                );

    auto [insertIt, insertBool] = goDatasForCreate_.emplace( mpos_2_key(mpos_), std::move(goDUPtr) );
    tprAssert( insertBool );
}




    
}//-------------- namespace: blueP_inn end ----------------//
}//--------------------- namespace: blueprint end ------------------------//


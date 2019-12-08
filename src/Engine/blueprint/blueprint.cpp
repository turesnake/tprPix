/*
 * ======================= blueprint.cpp =======================
 *                          -- tpr --
 *                                        CREATE -- 2019.12.04
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#include "blueprint.h"


//-------------------- CPP --------------------//


//-------------------- Engine --------------------//
#include "PlotBlueprint.h"
#include "YardBlueprint.h"
#include "VillageBlueprint.h"
#include "random.h"
#include "tprCast.h"

#include "blueprint_inn.h"

#include "esrc_state.h"
#include "esrc_animFrameSet.h"



//-------------------- Script --------------------//
#include "Script/resource/ssrc_gameObj.h" // tmp


#include "tprDebug.h"



namespace blueprint {//------------------ namespace: blueprint start ---------------------//
namespace blueP_inn {//----------- namespace: blueP_inn ----------------//

    void create_new_goDataForCreate(std::unordered_map<mapEntKey_t, std::unique_ptr<GoDataForCreate>> &goDatasForCreate_,
                                    mapEntKey_t key_,
                                    const glm::dvec2  &dpos_,
                                    const GoSpec &goSpecRef_,
                                    const MapDataEnt &mapDataEntRef_ )noexcept;
    
}//-------------- namespace: blueP_inn end ----------------//


void init_blueprint()noexcept{
    PlotBlueprint::init_for_static();
    YardBlueprint::init_for_static();
    VillageBlueprint::init_for_static();
    //---
    parse_from_plotJsonFiles();
    parse_from_yardJsonFiles();
    parse_from_villageJsonFiles();
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
                                    std::unordered_map<mapEntKey_t, std::unique_ptr<GoDataForCreate>> &floorGoDatasForCreate_ ){

    // 使用的随机数，直接取自 相关 mpos 的 mapEntKey 值
    size_t  uWeight   {};
    IntVec2 yardMPos {};
    IntVec2 plotMPos {};
    IntVec2 entMPos  {};

    //=================//
    //    village
    //=================//
    VillageBlueprint &villageRef = VillageBlueprint::get_villageBlueprintRef( villageId_ );
    std::unique_ptr<blueP_inn::VarType_Village_Manager> varType_village_managerUPtr 
                = std::make_unique<blueP_inn::VarType_Village_Manager>( villageRef );


    uWeight = cast_2_size_t( mpos_2_key(ecoObjMPos_) );
    const MapData &villageMapData = villageRef.apply_a_random_mapData( uWeight );
    for( const auto &yEntUPtr : villageMapData.data ){ // each mapDataEnt uptr / yard


        // isRoad
        // 此功能尚未完成 ...

        // 并未处理 yEntUPtr 的 brokenLvl / direction 数据
        
        //=================//
        //      yard
        //=================//
        int half_ents_per_section = ENTS_PER_SECTION / 2;
        yardMPos = ecoObjMPos_ - IntVec2{half_ents_per_section, half_ents_per_section} + yEntUPtr->mposOff;
                //-- 蓝图 以 ecoobj/section left-bottom 点为 中心点

        uWeight = cast_2_size_t( mpos_2_key( yardMPos ) );
        auto yardId = varType_village_managerUPtr->apply_a_yardBlueprintId( yEntUPtr->varTypeIdx, uWeight );

        YardBlueprint &yardRef = YardBlueprint::get_yardBlueprintRef( yardId );
        
        std::unique_ptr<blueP_inn::VarType_Yard_Manager> varType_yard_managerUPtr 
                    = std::make_unique<blueP_inn::VarType_Yard_Manager>( yardRef );

        //---------//
        // majorGos
        //---------//
        const MapData &yardMapData_majorGos = yardRef.apply_a_random_majorGo_mapData( uWeight );
        for( const auto &pEntUPtr : yardMapData_majorGos.data ){ // each mapDataEnt uptr / plot

            const auto *varTypeDatas_Yard_majorGoPtr = yardRef.get_majorGo_varTypeDataPtr_Yard( pEntUPtr->varTypeIdx );

            plotMPos = yardMPos + pEntUPtr->mposOff;
            uWeight = cast_2_size_t( mpos_2_key( plotMPos ) );

            if( !varTypeDatas_Yard_majorGoPtr->get_isPlotBlueprint() ){
                //-- 直接可以获得 mp 位置的 goSpec 数据

                const GoSpec &goSpecRef = varType_yard_managerUPtr->apply_a_majorGoSpec( pEntUPtr->varTypeIdx, uWeight );

                //--- 正式在 ecoObj 中创建 major GoDataForCreate 实例 --
                blueP_inn::create_new_goDataForCreate(majorGoDatasForCreate_,
                                                mpos_2_key(plotMPos),
                                                mpos_2_midDPos(plotMPos),
                                                goSpecRef,
                                                *(pEntUPtr) );

            }else{
                //-- mp 位置存储的是 plot 数据 --

                //=================//
                //      plot
                //=================//s
                auto plotId = varType_yard_managerUPtr->apply_a_plotBlueprintId( pEntUPtr->varTypeIdx, uWeight );
                PlotBlueprint &plotRef = PlotBlueprint::get_plotBlueprintRef( plotId );

                std::unique_ptr<blueP_inn::VarType_Plot_Manager> varType_Plot_ManagerUPtr 
                            = std::make_unique<blueP_inn::VarType_Plot_Manager>( plotRef );

                const MapData &plotMapData = plotRef.apply_a_random_mapData( uWeight );
                for( const auto &mpEntUPtr : plotMapData.data ){ // each mapDataEnt uptr / mapent

                    entMPos = plotMPos + mpEntUPtr->mposOff;
                    uWeight = cast_2_size_t( mpos_2_key( entMPos ) );

                    const GoSpec &goSpecRef = varType_Plot_ManagerUPtr->apply_a_goSpec( mpEntUPtr->varTypeIdx, uWeight );

                    //--- 正式在 ecoObj 中创建 major GoDataForCreate 实例 --
                    blueP_inn::create_new_goDataForCreate(majorGoDatasForCreate_,
                                                    mpos_2_key(entMPos),
                                                    mpos_2_midDPos(entMPos),
                                                    goSpecRef,
                                                    *(mpEntUPtr) );
                }
            }
        }

        //---------//
        // floorGos
        //---------//
        // floor go 不会再细分出 plot 变量
        glm::dvec2 floorGoDPos {};

        const MapData &yardMapData_floorGos = yardRef.apply_a_random_floorGo_mapData( uWeight );
        for( const auto &pEntUPtr : yardMapData_floorGos.data ){ // each mapDataEnt uptr 

            const auto *varTypeDatas_Yard_floorGoPtr = yardRef.get_floorGo_varTypeDataPtr_Yard( pEntUPtr->varTypeIdx );

            entMPos = yardMPos + pEntUPtr->mposOff; // floorGo mid点所在 mpos
            uWeight = cast_2_size_t( mpos_2_key(entMPos) );




            const GoSpec &goSpecRef = varType_yard_managerUPtr->apply_a_floorGoSpec( pEntUPtr->varTypeIdx, uWeight );

  
            floorGoDPos = mpos_2_dpos(entMPos) + calc_floorGo_mid_dposOff( varTypeDatas_Yard_floorGoPtr->get_floorGoSize() );
            

            //--- 正式在 ecoObj 中创建 floor GoDataForCreate 实例 --
            blueP_inn::create_new_goDataForCreate(floorGoDatasForCreate_,
                                                mpos_2_key(entMPos),
                                                floorGoDPos,
                                                goSpecRef,
                                                *(pEntUPtr) );
            

        }

    }
}


namespace blueP_inn {//----------- namespace: blueP_inn ----------------//








void create_new_goDataForCreate(std::unordered_map<mapEntKey_t, std::unique_ptr<GoDataForCreate>> &goDatasForCreate_,
                                mapEntKey_t key_,
                                const glm::dvec2  &dpos_, // 让外部计算好
                                const GoSpec &goSpecRef_,
                                const MapDataEnt &mapDataEntRef_ )noexcept{

    //--- 正式在 ecoObj 中创建 GoDataForCreate 实例 --
    auto outPair = goDatasForCreate_.insert({ key_, std::make_unique<GoDataForCreate>() });
    tprAssert( outPair.second );
    GoDataForCreate &goDRef = *(outPair.first->second);
    //--- 为 GoDataForCreate 实例 装填数据 --
    goDRef.direction = mapDataEntRef_.direction;
    goDRef.brokenLvl = mapDataEntRef_.brokenLvl;
    goDRef.goSpecId = goSpecRef_.goSpecId;

    //goDRef.mpos = mapEntKey_2_mpos( key_ );
    //goDRef.mpos = mpos_;
    goDRef.dpos = dpos_;

    const auto &goSpecRef = ssrc::get_goSpecRef( goDRef.goSpecId );
    goDRef.subspecId = esrc::apply_a_random_animSubspeciesId(   goSpecRef.animFrameSetName, // e.g. "mushroom"
                                                                goSpecRef_.animLabels,
                                                                cast_2_size_t(key_) );

}
    
}//-------------- namespace: blueP_inn end ----------------//
}//--------------------- namespace: blueprint end ------------------------//

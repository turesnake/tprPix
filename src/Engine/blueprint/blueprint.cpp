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

#include "esrc_state.h"
#include "esrc_animFrameSet.h"

//-------------------- Script --------------------//
#include "Script/resource/ssrc_gameObj.h" // tmp


#include "tprDebug.h"



namespace blueprint {//------------------ namespace: blueprint start ---------------------//
namespace inn {//----------- namespace: inn ----------------//

    void create_new_goDataForCreate(std::unordered_map<mapEntKey_t, std::unique_ptr<GoDataForCreate>> &outContainer_,
                                    mapEntKey_t key_,
                                    const GoSpec &goSpecRef_,
                                    const MapDataEnt &mapDataEntRef_ )noexcept;
    
}//-------------- namespace: inn end ----------------//


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
                                    std::unordered_map<mapEntKey_t, std::unique_ptr<GoDataForCreate>> &outContainer_ ){

    // 使用的随机数，直接取自 相关 mpos 的 mapEntKey 值
    size_t  uWeight   {};
    IntVec2 yardMPos {};
    IntVec2 plotMPos {};
    IntVec2 mpos     {};

    //---------------//
    //    village
    //---------------//
    VillageBlueprint &villageRef = VillageBlueprint::get_villageBlueprintRef( villageId_ );

            //cout << "--1--" << endl;

    uWeight = cast_2_size_t( mpos_2_key(ecoObjMPos_) );
    const MapData &villageMapData = villageRef.apply_a_random_mapData( uWeight );
    for( const auto &yEntUPtr : villageMapData.data ){ // each mapDataEnt uptr / yard

        auto &varTypeDatas_VillageRef = villageRef.get_varTypeDatas_Village( yEntUPtr->varTypeIdx );

            //cout << "--2--" << endl;

        // isRoad
        // 此功能尚未完成 ...

        // 并未处理 yEntUPtr 的 brokenLvl / direction 数据
        
        //---------------//
        //      yard
        //---------------//
        int half_ents_per_section = ENTS_PER_SECTION / 2;
        yardMPos = ecoObjMPos_ - IntVec2{half_ents_per_section, half_ents_per_section} + yEntUPtr->mposOff;
                //-- 蓝图 以 ecoobj/section left-bottom 点为 中心点

        uWeight = cast_2_size_t( mpos_2_key( yardMPos ) );
        auto yardId = varTypeDatas_VillageRef.apply_a_yardBlueprintId( uWeight );
        YardBlueprint &yardRef = YardBlueprint::get_yardBlueprintRef( yardId );

            //cout << "--3--" << endl;

        const MapData &yardMapData = yardRef.apply_a_random_mapData( uWeight );
        for( const auto &pEntUPtr : yardMapData.data ){ // each mapDataEnt uptr / plot

            auto &varTypeDatas_YardRef = yardRef.get_varTypeDatas_Yard( pEntUPtr->varTypeIdx );

            plotMPos = yardMPos + pEntUPtr->mposOff;
            uWeight = cast_2_size_t( mpos_2_key( plotMPos ) );

                //cout << "--4--" << endl;

            if( !varTypeDatas_YardRef.get_isPlotBlueprint() ){
                //-- 直接可以获得 mp 位置的 goSpec 数据

                const GoSpec &goSpecRef = varTypeDatas_YardRef.apply_a_goSpec( uWeight );

                    //cout << "--5--" << endl;

                //--- 正式在 ecoObj 中创建 GoDataForCreate 实例 --
                inn::create_new_goDataForCreate(outContainer_,
                                                mpos_2_key(plotMPos),
                                                goSpecRef,
                                                *(pEntUPtr) );

            }else{
                //-- mp 位置存储的是 plot 数据 --

                //cout << "    --6--" << endl;

                //---------------//
                //      plot
                //---------------//
                auto plotId = varTypeDatas_YardRef.apply_a_plotBlueprintId( uWeight );
                PlotBlueprint &plotRef = PlotBlueprint::get_plotBlueprintRef( plotId );

                const MapData &plotMapData = plotRef.apply_a_random_mapData( uWeight );
                for( const auto &mpEntUPtr : plotMapData.data ){ // each mapDataEnt uptr / mapent

                    auto &varTypeDatas_PlotRef = plotRef.get_varTypeDatas_Plot( mpEntUPtr->varTypeIdx );

                    mpos = plotMPos + mpEntUPtr->mposOff;
                    uWeight = cast_2_size_t( mpos_2_key( mpos ) );

                    const GoSpec &goSpecRef = varTypeDatas_PlotRef.apply_a_goSpec( uWeight );

                        //cout << "--7--" << endl;

                    //--- 正式在 ecoObj 中创建 GoDataForCreate 实例 --
                    inn::create_new_goDataForCreate(outContainer_,
                                                    mpos_2_key(mpos),
                                                    goSpecRef,
                                                    *(mpEntUPtr) );

                }
            }
        }
    }
}


namespace inn {//----------- namespace: inn ----------------//

void create_new_goDataForCreate(std::unordered_map<mapEntKey_t, std::unique_ptr<GoDataForCreate>> &outContainer_,
                                mapEntKey_t key_,
                                const GoSpec &goSpecRef_,
                                const MapDataEnt &mapDataEntRef_ )noexcept{

    //--- 正式在 ecoObj 中创建 GoDataForCreate 实例 --
    auto outPair = outContainer_.insert({ key_, std::make_unique<GoDataForCreate>() });
    tprAssert( outPair.second );
    GoDataForCreate &goDRef = *(outPair.first->second);
    //--- 为 GoDataForCreate 实例 装填数据 --
    goDRef.direction = mapDataEntRef_.direction;
    goDRef.brokenLvl = mapDataEntRef_.brokenLvl;
    goDRef.goSpecId = goSpecRef_.goSpecId;

    goDRef.mpos = mapEntKey_2_mpos( key_ );

    const auto &goSpecRef = ssrc::get_goSpecRef( goDRef.goSpecId );
    goDRef.subspecId = esrc::apply_a_random_animSubspeciesId(   goSpecRef.animFrameSetName, // e.g. "mushroom"
                                                                goSpecRef_.animLabels,
                                                                cast_2_size_t(key_) );

}
    
}//-------------- namespace: inn end ----------------//
}//--------------------- namespace: blueprint end ------------------------//

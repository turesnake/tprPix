/*
 * ================== json_ecoSysPlan.cpp ===================
 *                          -- tpr --
 *                                        创建 -- 2019.07.07
 *                                        修改 -- 
 * ----------------------------------------------------------
 */
#include "pch.h"


//--------------- Libs ------------------//
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

//-------------------- Engine --------------------//
#include "global.h"
#include "fileIO.h"
#include "EcoSysPlan.h"
#include "json_oth.h"
#include "blueprint.h"
#include "GoSpecData.h"

#include "GoSpecFromJson.h"

#include "YardBlueprint.h"


#include "esrc_ecoSysPlan.h"
#include "esrc_colorTableSet.h"
#include "esrc_state.h"

//--------------- Script ------------------//
#include "Script/json/json_all.h"


using namespace rapidjson;


namespace json{//------------- namespace json ----------------
namespace espJson_inn {//-------- namespace: espJson_inn --------------//
   
   
    //--- densityDivideVals ---
    const std::vector<double> densityDivideVals_default 
        { -65.0, -40.0, -15.0, 15.0, 40.0, 65.0 }; //- 较为均匀的分布

    const std::vector<double> densityDivideVals_50_20_50
        { -50.0, -30.0, -10.0, 10.0, 30.0, 50.0 }; //- 两极各占:50，中间lvl各占:20

    const std::vector<double> *str_2_densityDivideValsPtr( const std::string &str_ )noexcept{
        if( str_ == std::string{"default"} ){
            return &densityDivideVals_default;
        }else if( str_ == std::string{"50_20_50"} ){
            return &densityDivideVals_50_20_50;
        }else{
            tprAssert(0);
            return &densityDivideVals_default; //- never reach
        }
    }

    //----- funcs -----//
    void parse_single_ecoSysPlansJsonFile( const std::string &path_file_ );
    void parse_naturePool( const Value &densityPoolVal_, EcoSysPlan &ecoPlanREf_ );


}//------------- namespace: espJson_inn end --------------//


/* Do Not Worry About Performance !!!
 */
void parse_ecoSysPlansJsonFile(){

    tprDebug::console( "   ----- parse_ecoSysPlansJsonFile: start ----- " );

    esrc::is_setState("json_gameObj");
    esrc::is_setState("blueprint");

    std::vector<std::string> path_files {};
    collect_fileNames( path_jsons, "ecoSysPlans", "_files.json", path_files );
    //---
    for( const auto &i : path_files ){
        espJson_inn::parse_single_ecoSysPlansJsonFile(i);
    }

    esrc::insertState("json_ecoSysPlan");
    tprDebug::console( "   ----- parse_ecoSysPlansJsonFile: end ----- " );
}

namespace espJson_inn {//-------- namespace: espJson_inn --------------//



void parse_single_ecoSysPlansJsonFile( const std::string &path_file_ ){
    //-----------------------------//
    //         load file
    //-----------------------------//
    auto jsonBufUPtr = read_a_file( path_file_ );

    //-----------------------------//
    //      parce JSON data
    //-----------------------------//
    Document doc;
    doc.Parse( jsonBufUPtr->c_str() );

    //--- tmp-vals ---
    EcoSysPlanType              ecoPlanType {};
    std::string                 colorTableName {};
    double                      density_SeaLvlOff {};
    const std::vector<double>   *density_DivValsPtr {};

    tprAssert( doc.IsArray() );
    for( const auto &eco : doc.GetArray() ){

        {//--- EcoSysPlanType ---//
            const auto &a = check_and_get_value( eco, "EcoSysPlanType", JsonValType::String );
            ecoPlanType = str_2_ecoSysPlanType( a.GetString() );
        }
        {//--- colorTableName ---//
            const auto &a = check_and_get_value( eco, "colorTableName", JsonValType::String );
            colorTableName = a.GetString();
        }
        {//--- density.SeaLvlOff ---//
            const auto &a = check_and_get_value( eco, "density.SeaLvlOff", JsonValType::Double );
            density_SeaLvlOff = a.GetDouble();
        }
        {//--- density.DivideType ---//
            const auto &a = check_and_get_value( eco, "density.DivideType", JsonValType::String );
            density_DivValsPtr = espJson_inn::str_2_densityDivideValsPtr( a.GetString() );
        }

        auto &ecoPlanRef = esrc::insert_new_ecoSysPlan( ecoPlanType );

        ecoPlanRef.set_colorTableId( esrc::get_colorTabelSet().get_colorTableId(colorTableName) );
        ecoPlanRef.init_densityDatas( density_SeaLvlOff, *density_DivValsPtr );

        {//--- natureMajorYardPools ---//
            const auto &a = check_and_get_value( eco, "natureMajorYardPools", JsonValType::Array );
            for( const auto &pool : a.GetArray() ){
                espJson_inn::parse_naturePool( pool, ecoPlanRef );
            }
        }

        {//--- villageBlueprintPool ---//
            std::string villageName {};
            size_t      num {};

            const auto &villageBlueprintPool = check_and_get_value( eco, "villageBlueprintPool", JsonValType::Array );
            for( const auto &village : villageBlueprintPool.GetArray() ){
                tprAssert( village.IsObject() );
                {//--- villageName ---//
                    const auto &a = check_and_get_value( village, "villageName", JsonValType::String );
                    villageName = a.GetString();
                }
                {//--- num ---//
                    const auto &a = check_and_get_value( village, "num", JsonValType::Uint64 );
                    num = cast_2_size_t( a.GetUint64() );
                }

                // 允许写入 0
                if( num == 0 ){
                    continue;
                }

                auto id = blueprint::str_2_villageBlueprintId( villageName );
                ecoPlanRef.insert_2_villageIdRandPool( id, num );
            }

        }

        {//--- natureFloorYards ---//
            Density     density {};
            std::string yardName {};
            std::string yardLabel {};
            size_t      num {};

            const auto &natureFloorYards = check_and_get_value( eco, "natureFloorYards", JsonValType::Object );
            {//--- yardPool ---//
                const auto &yardPool = check_and_get_value( natureFloorYards, "yardPool", JsonValType::Array );
                for( const auto &yardEnt : yardPool.GetArray() ){
                    tprAssert( yardEnt.IsObject() );
                    {//--- yardName ---//
                        const auto &a = check_and_get_value( yardEnt, "yardName", JsonValType::String );
                        yardName = a.GetString();
                    }
                    {//--- yardLabel ---//
                        const auto &a = check_and_get_value( yardEnt, "yardLabel", JsonValType::String );
                        yardLabel = a.GetString();
                    }
                    {//--- num ---//
                        const auto &a = check_and_get_value( yardEnt, "num", JsonValType::Uint64 );
                        num = cast_2_size_t( a.GetUint64() );
                    }

                    // 允许写入 0
                    if( num == 0 ){
                        continue;
                    }

                    tprAssert( blueprint::YardBlueprintSet::is_find_name(yardName, yardLabel) ); // Must Exist!!!
                    auto yardId = blueprint::YardBlueprintSet::get_yardBlueprintId(yardName, yardLabel, NineDirection::Center);
                    auto &yardRef = blueprint::YardBlueprintSet::get_yardBlueprintRef( yardId );
                    tprAssert( yardRef.get_yardSize() == blueprint::YardSize::_2f2 ); // Must 1f1 ( nature floor yard  )
                    //---
                    ecoPlanRef.insert_2_natureFlooryardIdRandPool( yardId, num );
                }
            }
            
            {//--- density.lvls ---//
                const auto &lvls = check_and_get_value( natureFloorYards, "density.lvls", JsonValType::Array );
                for( const auto &lvl : lvls.GetArray() ){
                    tprAssert( lvl.IsInt() );
                    density = Density{ lvl.GetInt() };
                    ecoPlanRef.insert_2_natureFloorDensitys( density );
                }
            }
        }

        ecoPlanRef.init_check();
    }
}




// natureYardPools == densityPools
void parse_naturePool( const Value &densityPoolVal_, EcoSysPlan &ecoPlanREf_ ){

    Density             density {};

    {//--- density.lvl ---//
        const auto &a = check_and_get_value( densityPoolVal_, "density.lvl", JsonValType::Int );
        density = Density{ a.GetInt() };
    }

    DensityPool &densityPoolRef = ecoPlanREf_.create_new_densityPool( density );


    {//--- applyPercent ---//
        const auto &a = check_and_get_value( densityPoolVal_, "applyPercent", JsonValType::Double );
        densityPoolRef.set_applyPercent( a.GetDouble() );
    }


    {//--- yardPool ---//
        std::string                     yardName {};
        std::string                     yardLabel {};
        blueprint::yardBlueprintId_t    yardId {};
        size_t                          num      {};

        const auto &yardPool = check_and_get_value( densityPoolVal_, "yardPool", JsonValType::Array );
        for( const auto &yardEnt : yardPool.GetArray() ){
            tprAssert( yardEnt.IsObject() );

            {//--- yardName ---//
                const auto &a = check_and_get_value( yardEnt, "yardName", JsonValType::String );
                yardName = a.GetString();
            }
            {//--- yardLabel ---//
                const auto &a = check_and_get_value( yardEnt, "yardLabel", JsonValType::String );
                yardLabel = a.GetString();
            }
            {//--- num ---//
                const auto &a = check_and_get_value( yardEnt, "num", JsonValType::Uint64 );
                num = cast_2_size_t( a.GetUint64() );
            }

            // 允许写入 0
            if( num == 0 ){
                continue;
            }

            //--- 检查 name / yardSize ---
            tprAssert( blueprint::YardBlueprintSet::is_find_name(yardName, yardLabel) ); // Must Exist!!!
            yardId = blueprint::YardBlueprintSet::get_yardBlueprintId( yardName, yardLabel, NineDirection::Center );
            auto &yardRef = blueprint::YardBlueprintSet::get_yardBlueprintRef( yardId );
            tprAssert( yardRef.get_yardSize() == blueprint::YardSize::_1f1 ); // Must 1f1 ( nature major yard  )
            //---
            densityPoolRef.insert_2_yardBlueprintIds( yardId, num );
        }
    }

}


}//------------- namespace: espJson_inn end --------------//
}//------------- namespace json: end ----------------

/*
 * ================== json_ecoSysPlan.cpp ===================
 *                          -- tpr --
 *                                        创建 -- 2019.07.07
 *                                        修改 -- 
 * ----------------------------------------------------------
 */
//--------------- CPP ------------------//
#include <string>
#include <vector>
#include <memory>
#include <unordered_set>

//--------------- Libs ------------------//
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

#include "tprGeneral.h"

//-------------------- Engine --------------------//
#include "tprAssert.h"
#include "tprCast.h"
#include "global.h"
#include "fileIO.h"
#include "EcoSysPlan.h"
#include "AnimLabel.h"
#include "BodySize.h"
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

#include <iostream>
using std::cout;
using std::endl;

namespace json{//------------- namespace json ----------------
namespace espJson_inn {//-------- namespace: espJson_inn --------------//
   
   
    //--- densityDivideVals ---
    const std::vector<double> densityDivideVals_default 
        { -65.0, -40.0, -15.0, 15.0, 40.0, 65.0 }; //- 较为均匀的分布

    const std::vector<double> densityDivideVals_50_20_50
        { -50.0, -30.0, -10.0, 10.0, 30.0, 50.0 }; //- 两极各占:50，中间lvl各占:20

    const std::vector<double> *str_2_DensityDivideValsPtr( const std::string &str_ )noexcept{
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

    cout << "   ----- parse_ecoSysPlansJsonFile: start ----- " << endl;

    esrc::is_setState("json_gameObj");
    esrc::is_setState("blueprint");

    std::vector<std::string> path_files {};
    collect_fileNames( path_jsons, "ecoSysPlans", "_files.json", path_files );
    //---
    for( const auto &i : path_files ){
        espJson_inn::parse_single_ecoSysPlansJsonFile(i);
    }

    esrc::insertState("json_ecoSysPlan");
    cout << "   ----- parse_ecoSysPlansJsonFile: end ----- " << endl;
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
    u32_t                       fixedSeed {};

    tprAssert( doc.IsArray() );
    for( const auto &eco : doc.GetArray() ){

        {//--- EcoSysPlanType ---//
            const auto &a = check_and_get_value( eco, "EcoSysPlanType", JsonValType::String );
            ecoPlanType = str_2_EcoSysPlanType( a.GetString() );
        }
        {//--- colorTableName ---//
            const auto &a = check_and_get_value( eco, "colorTableName", JsonValType::String );
            colorTableName = a.GetString();
        }
        {//--- fixedSeed ---//
            const auto &a = check_and_get_value( eco, "fixedSeed", JsonValType::Uint );
            fixedSeed = a.GetUint();
        }
        {//--- density.SeaLvlOff ---//
            const auto &a = check_and_get_value( eco, "density.SeaLvlOff", JsonValType::Double );
            density_SeaLvlOff = a.GetDouble();
        }
        {//--- density.DivideType ---//
            const auto &a = check_and_get_value( eco, "density.DivideType", JsonValType::String );
            density_DivValsPtr = espJson_inn::str_2_DensityDivideValsPtr( a.GetString() );
        }

        auto &ecoPlanRef = esrc::insert_new_ecoSysPlan( ecoPlanType );

        ecoPlanRef.set_colorTableId( esrc::get_colorTabelSet().get_colorTableId(colorTableName) );
        ecoPlanRef.init_densityDatas( density_SeaLvlOff, *density_DivValsPtr );

        {//--- naturePools ---//
            const auto &a = check_and_get_value( eco, "naturePools", JsonValType::Array );
            for( const auto &densityPool : a.GetArray() ){
                espJson_inn::parse_naturePool( densityPool, ecoPlanRef );
            }
        }

        {//--- villageBlueprintPool ---//
            const auto &a = check_and_get_value( eco, "villageBlueprintPool", JsonValType::Array );
            for( const auto &villageName : a.GetArray() ){
                tprAssert( villageName.IsString() );
                auto id = blueprint::str_2_villageBlueprintId( villageName.GetString() );
                ecoPlanRef.pushBack_new_villageBlueprintId( id );
            }
        }

        ecoPlanRef.shuffle_goSpecDataPools( fixedSeed );
        ecoPlanRef.chueck_end();
    }
}




// natureYardPools == densityPools
void parse_naturePool( const Value &densityPoolVal_, EcoSysPlan &ecoPlanREf_ ){

    Density             density {};
    std::unordered_set<BodySize> bodySizes {};

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

            //--- 检查 name / yardSize ---
            tprAssert( blueprint::YardBlueprintSet::is_find_name(yardName, yardLabel) ); // Must Exist!!!
            yardId = blueprint::YardBlueprintSet::get_yardBlueprintId( yardName, yardLabel );

            blueprint::YardBlueprint &yardRef = blueprint::YardBlueprintSet::get_yardBlueprintRef( yardId );

            tprAssert( yardRef.get_yardSize() == blueprint::YardSize::_1f1 ); // Must

            //---
            densityPoolRef.insert_2_yardBlueprintIds( yardId, num );
        }
    }

}


}//------------- namespace: espJson_inn end --------------//
}//------------- namespace json: end ----------------

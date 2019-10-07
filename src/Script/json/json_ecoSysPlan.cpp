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

#include "esrc_ecoSysPlan.h"
#include "esrc_colorTableSet.h"
#include "esrc_state.h"

//--------------- Script ------------------//
#include "Script/json/json_all.h"
#include "Script/json/json_oth.h"
#include "Script/resource/ssrc.h" 


using namespace rapidjson;

#include <iostream>
using std::cout;
using std::endl;

namespace json{//------------- namespace json ----------------
namespace espJson_inn {//-------- namespace: espJson_inn --------------//
   
   
    //--- densityDivideVals ---
    std::vector<double> densityDivideVals_default 
        { -65.0, -40.0, -15.0, 15.0, 40.0, 65.0 }; //- 较为均匀的分布

    std::vector<double> densityDivideVals_50_20_50
        { -50.0, -30.0, -10.0, 10.0, 30.0, 50.0 }; //- 两极各占:50，中间lvl各占:20

    const std::vector<double> *str_2_DensityDivideValsPtr( const std::string &str_ ){
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
    void parse_from_single_ecoSysPlansJsonFile( const std::string &path_file_ );
    void parse_pool( const Value &cpool_, EcoSysPlan &ecoPlanREf_ );


}//------------- namespace: espJson_inn end --------------//


/* ===========================================================
 *             parse_from_ecoSysPlansJsonFile
 * -----------------------------------------------------------
 * Do Not Worry About Performance !!!
 */
void parse_from_ecoSysPlansJsonFile(){

    cout << "   ----- parse_from_ecoSysPlansJsonFile: start ----- " << endl;

    std::vector<std::string> path_files {};
    collect_fileNames( "ecoSysPlans", "files.json", path_files );
    //---
    for( const auto &i : path_files ){
        espJson_inn::parse_from_single_ecoSysPlansJsonFile(i);
    }

    esrc::insertState("json_ecoSysPlan");
    cout << "   ----- parse_from_ecoSysPlansJsonFile: end ----- " << endl;
}

namespace espJson_inn {//-------- namespace: espJson_inn --------------//



void parse_from_single_ecoSysPlansJsonFile( const std::string &path_file_ ){
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
    EcoSysPlanType    ecoPlanType {};
    std::string       colorTableName {};
    double            density_SeaLvlOff {};
    const std::vector<double> *density_DivValsPtr {};
    u32_t             fixedSeed {};

    tprAssert( doc.IsArray() );
    for( auto &ent : doc.GetArray() ){

        {//--- EcoSysPlanType ---//
            const auto &a = check_and_get_value( ent, "EcoSysPlanType", JsonValType::String );
            ecoPlanType = str_2_EcoSysPlanType( a.GetString() );
        }
        {//--- colorTableName ---//
            const auto &a = check_and_get_value( ent, "colorTableName", JsonValType::String );
            colorTableName = a.GetString();
        }
        {//--- fixedSeed ---//
            const auto &a = check_and_get_value( ent, "fixedSeed", JsonValType::Uint );
            fixedSeed = a.GetUint();
        }
        {//--- density.SeaLvlOff ---//
            const auto &a = check_and_get_value( ent, "density.SeaLvlOff", JsonValType::Double );
            density_SeaLvlOff = a.GetDouble();
        }
        {//--- density.DivideType ---//
            const auto &a = check_and_get_value( ent, "density.DivideType", JsonValType::String );
            density_DivValsPtr = espJson_inn::str_2_DensityDivideValsPtr( a.GetString() );
        }

        auto &ecoPlanRef = esrc::insert_new_ecoSysPlan( ecoPlanType );

        ecoPlanRef.set_colorTableId( esrc::get_colorTabelSet().get_colorTableId(colorTableName) );
        ecoPlanRef.init_densityDatas( density_SeaLvlOff, *density_DivValsPtr );
        ecoPlanRef.init_goSpecDataPools_and_applyPercents();

        {//--- pools ---//
            const auto &a = check_and_get_value( ent, "pools", JsonValType::Array );
            for( auto &pool : a.GetArray() ){
                espJson_inn::parse_pool( pool, ecoPlanRef );
            }
        }

        ecoPlanRef.shuffle_goSpecDataPools( fixedSeed );
        ecoPlanRef.chueck_end();
    }
}


void parse_pool(   const Value &pool_,
                    EcoSysPlan &ecoPlanREf_ ){

    int    densityLvl {};
    double applyPercent {};
    std::vector<std::unique_ptr<EcoEnt>> ecoEnts {};
    //---
    std::string specName {};
    size_t      num      {};
    std::vector<AnimLabel> labels {}; //- 允许是空的

    {//--- density.lvl ---//
        const auto &a = check_and_get_value( pool_, "density.lvl", JsonValType::Int );
        densityLvl = a.GetInt();
    }
    {//--- applyPercent ---//
        const auto &a = check_and_get_value( pool_, "applyPercent", JsonValType::Double );
        applyPercent = a.GetDouble();
    }
    {//--- ecoEnts ---//
        const auto &a = check_and_get_value( pool_, "ecoEnts", JsonValType::Array );
        for( auto &ecoEnt : a.GetArray() ){

            {//--- specName ---//
                const auto &inna = check_and_get_value( ecoEnt, "specName", JsonValType::String );
                specName = inna.GetString();
            }
            {//--- animLabels ---//
                labels.clear();
                const auto &inna = check_and_get_value( ecoEnt, "animLabels", JsonValType::Array );
                if( inna.Size() > 0 ){
                    for( auto &i : inna.GetArray() ){
                        labels.push_back( str_2_AnimLabel(i.GetString()) );
                    }
                }
            }
            {//--- num ---//
                const auto &inna = check_and_get_value( ecoEnt, "num", JsonValType::Uint64 );
                num = cast_2_size_t(inna.GetUint64());
            }
            ecoEnts.push_back( std::make_unique<EcoEnt>(specName, labels, num) );
        }
    }
    ecoPlanREf_.insert( densityLvl, 
                        applyPercent,
                        ecoEnts );
}




}//------------- namespace: espJson_inn end --------------//
}//------------- namespace json: end ----------------

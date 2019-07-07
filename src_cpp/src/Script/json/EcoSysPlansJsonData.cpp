/*
 * ================== EcoSysPlansJsonData.cpp ===================
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
#include "global.h"
#include "fileIO.h"
#include "EcoSysPlan.h"
#include "esrc_ecoSysPlan.h"


//--------------- Script ------------------//
#include "Script/json/EcoSysPlansJsonData.h"
#include "Script/json/json_oth.h"
#include "Script/resource/ssrc.h" 


using namespace rapidjson;

#include <iostream>
using std::cout;
using std::endl;

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

    //--- landColor ---
    RGBA  color_Forest     { 130, 150, 109, 255 };
    RGBA  color_DarkForest { 106, 130, 113, 255 };
    RGBA  color_Plain      { 155, 159, 134, 255 };
    RGBA  color_Swamp      { 138, 139, 124, 255 };
    RGBA  color_Desert     { 210, 195, 142, 255 };

    RGBA *str_2_LandColorPtr( const std::string &str_ ){
        if( str_ == std::string{"color_Forest"} ){
            return &color_Forest;
        }else if( str_ == std::string{"color_DarkForest"} ){
            return &color_DarkForest;
        }else if( str_ == std::string{"color_Plain"} ){
            return &color_Plain;
        }else if( str_ == std::string{"color_Swamp"} ){
            return &color_Swamp;
        }else if( str_ == std::string{"color_Desert"} ){
            return &color_Desert;
        }else{
            tprAssert(0);
            return &color_Forest; //- never reach
        }
    }

    //----- funcs -----//
    void parse_landColor( const Value &color_,
                      EcoSysPlan &ecoPlanREf_ );

    void parse_pool(   const Value &cpool_,
                    EcoSysPlan &ecoPlanREf_ );

}//------------- namespace: espJson_inn end --------------//


/* ===========================================================
 *             parse_from_ecoSysPlansJsonFile
 * -----------------------------------------------------------
 * Do Not Worry About Performance !!!
 */
void parse_from_ecoSysPlansJsonFile(){

    cout << "   ----- parse_from_ecoSysPlansJsonFile: start ----- " << endl;

    //-----------------------------//
    //         load file
    //-----------------------------//
    std::string path_file = tprGeneral::path_combine(path_jsons, "ecoSysPlans.json");
    auto jsonBufUPtr = read_a_file( path_file );

    //-----------------------------//
    //      parce JSON data
    //-----------------------------//
    Document doc;
    doc.Parse( jsonBufUPtr->c_str() );

    //--- tmp-vals ---
    EcoSysPlanType    ecoPlanType {};
    double            density_SeaLvlOff {};
    const std::vector<double> *density_DivValsPtr {};
    u32_t             fixedSeed {};

    tprAssert( doc.IsArray() );
    for( auto &ent : doc.GetArray() ){

        {//--- EcoSysPlanType ---//
            const auto &a = json_inn::check_and_get_value( ent, "EcoSysPlanType", json_inn::JsonValType::String );
            ecoPlanType = str_2_EcoSysPlanType( a.GetString() );
        }
        {//--- fixedSeed ---//
            const auto &a = json_inn::check_and_get_value( ent, "fixedSeed", json_inn::JsonValType::Uint );
            fixedSeed = a.GetUint();
        }
        {//--- density.SeaLvlOff ---//
            const auto &a = json_inn::check_and_get_value( ent, "density.SeaLvlOff", json_inn::JsonValType::Double );
            density_SeaLvlOff = a.GetDouble();
        }
        {//--- density.DivideType ---//
            const auto &a = json_inn::check_and_get_value( ent, "density.DivideType", json_inn::JsonValType::String );
            density_DivValsPtr = espJson_inn::str_2_DensityDivideValsPtr( a.GetString() );
        }

        auto &ecoPlanRef = esrc::insert_new_ecoSysPlan( ecoPlanType );

        {//--- color ---//
            const auto &a = json_inn::check_and_get_value( ent, "color", json_inn::JsonValType::Object );
            espJson_inn::parse_landColor( a, ecoPlanRef );
        }

        ecoPlanRef.init_densityDatas( density_SeaLvlOff, *density_DivValsPtr );
        ecoPlanRef.init_goSpecIdPools_and_applyPercents();

        {//--- pools ---//
            const auto &a = json_inn::check_and_get_value( ent, "pools", json_inn::JsonValType::Array );
            for( auto &pool : a.GetArray() ){
                espJson_inn::parse_pool( pool, ecoPlanRef );
            }
        }

        ecoPlanRef.shuffle_goSpecIdPools( fixedSeed );
        ecoPlanRef.chueck_end();
    }

    cout << "   ----- parse_from_ecoSysPlansJsonFile: end ----- " << endl;
}


namespace espJson_inn {//-------- namespace: espJson_inn --------------//

void parse_landColor( const Value &color_,
                      EcoSysPlan &ecoPlanREf_ ){

    std::string  landColorType {};
    {//--- colorType ---//
        const auto &a = json_inn::check_and_get_value( color_, "colorType", json_inn::JsonValType::String );
        landColorType = a.GetString();
    }

    if( landColorType == std::string{"onlyHighLand"} ){

        RGBA *landColorPtr {};
        {//--- color ---//
            const auto &a = json_inn::check_and_get_value( color_, "color", json_inn::JsonValType::String );
            landColorPtr = str_2_LandColorPtr( a.GetString() );
        }
        ecoPlanREf_.init_landColor_onlyHighLand( *landColorPtr );

    }else if( landColorType == std::string{"doubleDeep"} ){

        RGBA *landColorPtr {};
        {//--- color ---//
            const auto &a = json_inn::check_and_get_value( color_, "color", json_inn::JsonValType::String );
            landColorPtr = str_2_LandColorPtr( a.GetString() );
        }
        ecoPlanREf_.init_landColor_doubleDeep( *landColorPtr );

    }else if( landColorType == std::string{"twoPattern"} ){
        
        int   densityHighLvl {};
        RGBA *landColorHighPtr {};
        RGBA *landColorLowPtr {};
        bool  is_goDeep_high {};
        bool  is_goDeep_low  {};
        {//--- density.highLvl ---//
            const auto &a = json_inn::check_and_get_value( color_, "density.highLvl", json_inn::JsonValType::Int );
            densityHighLvl = a.GetInt();
        }
        {//--- color.high ---//
            const auto &a = json_inn::check_and_get_value( color_, "color.high", json_inn::JsonValType::String );
            landColorHighPtr = str_2_LandColorPtr( a.GetString() );
        }
        {//--- color.low ---//
            const auto &a = json_inn::check_and_get_value( color_, "color.low", json_inn::JsonValType::String );
            landColorLowPtr = str_2_LandColorPtr( a.GetString() );
        }
        {//--- is_goDeep_high ---//
            const auto &a = json_inn::check_and_get_value( color_, "is_goDeep_high", json_inn::JsonValType::Bool );
            is_goDeep_high = a.GetBool();
        }
        {//--- is_goDeep_low ---//
            const auto &a = json_inn::check_and_get_value( color_, "is_goDeep_low", json_inn::JsonValType::Bool );
            is_goDeep_low = a.GetBool();
        }
        ecoPlanREf_.init_landColor_twoPattern( densityHighLvl,
                                                *landColorHighPtr,
                                                *landColorLowPtr,
                                                is_goDeep_high,
                                                is_goDeep_low );
    }else{
        tprAssert(0);
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

    {//--- density.lvl ---//
        const auto &a = json_inn::check_and_get_value( pool_, "density.lvl", json_inn::JsonValType::Int );
        densityLvl = a.GetInt();
    }
    {//--- applyPercent ---//
        const auto &a = json_inn::check_and_get_value( pool_, "applyPercent", json_inn::JsonValType::Double );
        applyPercent = a.GetDouble();
    }
    {//--- ecoEnts ---//
        const auto &a = json_inn::check_and_get_value( pool_, "ecoEnts", json_inn::JsonValType::Array );
        for( auto &ecoEnt : a.GetArray() ){

            {//--- specName ---//
                const auto &inna = json_inn::check_and_get_value( ecoEnt, "specName", json_inn::JsonValType::String );
                specName = inna.GetString();
            }
            {//--- num ---//
                const auto &inna = json_inn::check_and_get_value( ecoEnt, "num", json_inn::JsonValType::Uint64 );
                num = static_cast<size_t>(inna.GetUint64());
            }
            ecoEnts.push_back( std::make_unique<EcoEnt>(specName, num) );
        }
    }
    ecoPlanREf_.insert( densityLvl, 
                        applyPercent,
                        ecoEnts );
}




}//------------- namespace: espJson_inn end --------------//

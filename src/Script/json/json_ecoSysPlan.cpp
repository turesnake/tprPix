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


#include "esrc_ecoSysPlan.h"
#include "esrc_colorTableSet.h"
#include "esrc_state.h"

//--------------- Script ------------------//
#include "Script/json/json_all.h"
#include "Script/resource/ssrc_all.h" 


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
    void parse_from_single_ecoSysPlansJsonFile( const std::string &path_file_ );
    void parse_pool( const Value &densityPoolVal_, EcoSysPlan &ecoPlanREf_ );
    void parse_ecoEnt(  const Value         &densityPoolVal_, 
                        BodySize            size_,
                        const std::string   &name_, 
                        DensityPool         &densityPoolRef_
                        );


}//------------- namespace: espJson_inn end --------------//


/* ===========================================================
 *             parse_from_ecoSysPlansJsonFile
 * -----------------------------------------------------------
 * Do Not Worry About Performance !!!
 */
void parse_from_ecoSysPlansJsonFile(){

    cout << "   ----- parse_from_ecoSysPlansJsonFile: start ----- " << endl;

    esrc::is_setState("json_gameObj");
    esrc::is_setState("blueprint");

    std::vector<std::string> path_files {};
    collect_fileNames( path_jsons, "ecoSysPlans", "_files.json", path_files );
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
    EcoSysPlanType              ecoPlanType {};
    std::string                 colorTableName {};
    MapSurfaceLowSpec           mapSurfaceLowSpec {};
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
        {//--- mapSurfaceLowSpec ---//
            const auto &a = check_and_get_value( eco, "MapSurfaceLowSpec", JsonValType::String );
            mapSurfaceLowSpec = str_2_MapSurfaceLowSpec( a.GetString() );
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
        ecoPlanRef.set_mapSurfaceLowSpec( mapSurfaceLowSpec );
        ecoPlanRef.init_densityDatas( density_SeaLvlOff, *density_DivValsPtr );
        ecoPlanRef.init_goSpecDataPools_and_applyPercents();

        {//--- pools ---//
            const auto &a = check_and_get_value( eco, "pools", JsonValType::Array );
            for( const auto &densityPool : a.GetArray() ){
                espJson_inn::parse_pool( densityPool, ecoPlanRef );
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





void parse_pool( const Value &densityPoolVal_, EcoSysPlan &ecoPlanREf_ ){

    int                 densityLvl {};
    FieldDistributeType distype {};
    std::unordered_set<BodySize> bodySizes {};

    auto densityPoolUPtr = std::make_unique<DensityPool>();

    {//--- density.lvl ---//
        const auto &a = check_and_get_value( densityPoolVal_, "density.lvl", JsonValType::Int );
        densityLvl = a.GetInt();
    }
    {//--- applyPercent ---//
        const auto &a = check_and_get_value( densityPoolVal_, "applyPercent", JsonValType::Double );
        densityPoolUPtr->set_applyPercent( a.GetDouble() );
    }

    //--- fieldDistributeTypes ---//
    const auto &types = check_and_get_value( densityPoolVal_, "fieldDistributeTypes", JsonValType::Array );
    size_t times {};
    for( const auto &type : types.GetArray() ){
        tprAssert( type.IsObject() );
        auto it = type.MemberBegin(); // only one ent

        tprAssert( it->value.GetInt() > 0 );
        times = cast_2_size_t( it->value.GetInt() );
        distype = str_2_fieldDistributeType( it->name.GetString() );
        densityPoolUPtr->insert_type( distype, times );

        for( const auto &i : bodySizes_in_fieldDistributeType(distype) ){
            bodySizes.insert(i); // maybe
        }
    }
        
    //--- ecoEnts ---//
    for( const auto &bodySize : bodySizes ){
        switch (bodySize){
            case BodySize::Sml:
                parse_ecoEnt( densityPoolVal_, BodySize::Sml, "ecoEnts_Sml", *densityPoolUPtr );
                break;
            case BodySize::Mid:
                parse_ecoEnt( densityPoolVal_, BodySize::Mid, "ecoEnts_Mid", *densityPoolUPtr );
                break;
            case BodySize::Big:
                parse_ecoEnt( densityPoolVal_, BodySize::Big, "ecoEnts_Big", *densityPoolUPtr );
                break;
            case BodySize::NotCare:
            default:
                tprAssert(0);
                break;
        }
    }


    ecoPlanREf_.insert( densityLvl,  densityPoolUPtr );
}



void parse_ecoEnt(  const Value         &densityPoolVal_, 
                    BodySize            size_,
                    const std::string   &name_,
                    DensityPool         &densityPoolRef_
                    ){

    goSpecId_t              rootGoSpecId {};
    size_t                  num         {};
    std::vector<AnimLabel>  labels      {}; //- 允许是空的
    std::string afsName             {};
    MultiGoMeshType         multiGoMeshType {};

    bool isFind_animLabels          {false};
    bool isFind_MultiGoMeshType     {false};
    

    const auto &ecoEntsVal = check_and_get_value( densityPoolVal_, name_, JsonValType::Array );
    for( const auto &ecoEnt : ecoEntsVal.GetArray() ){

        //--- clear ---//
        isFind_animLabels = false;
        isFind_MultiGoMeshType = false;

        {//--- goSpecName ---//
            const auto &a = check_and_get_value( ecoEnt, "goSpecName", JsonValType::String );
            rootGoSpecId = ssrc::str_2_goSpecId( a.GetString() );
        }
        {//--- afsName ---//
            const auto &a = check_and_get_value( ecoEnt, "afsName", JsonValType::String );
            afsName = a.GetString();
            tprAssert( GoSpecFromJson::is_find_in_afsNames(rootGoSpecId, afsName) );
        }

        //--- animLabels ---//
        if( ecoEnt.HasMember("animLabels") ){
            isFind_animLabels = true;
            const auto &a = check_and_get_value( ecoEnt, "animLabels", JsonValType::Array );
            if( a.Size() > 0 ){
                for( const auto &i : a.GetArray() ){
                    labels.push_back( str_2_AnimLabel(i.GetString()) );
                }
            }
        }
        //--- MultiGoMeshType ---//
        if( ecoEnt.HasMember("MultiGoMeshType") ){
            isFind_MultiGoMeshType = true;
            const auto &a = check_and_get_value( ecoEnt, "MultiGoMeshType", JsonValType::String );
            multiGoMeshType = str_2_multiGoMeshType( a.GetString() );
        }

        tprAssert( isFind_animLabels ^ isFind_MultiGoMeshType );

        {//--- num ---//
            const auto &a = check_and_get_value( ecoEnt, "num", JsonValType::Uint64 );
            num = cast_2_size_t(a.GetUint64());
        }

        //---- goSpecData ----//
        std::unique_ptr<GoSpecData> goSpecDataUPtr {nullptr};
        if( !isFind_MultiGoMeshType ){
            goSpecDataUPtr = std::make_unique<GoSpecData>(  rootGoSpecId, 
                                                            isFind_MultiGoMeshType,
                                                            afsName,
                                                            labels );
        }else{
            goSpecDataUPtr = std::make_unique<GoSpecData>(  rootGoSpecId, 
                                                            isFind_MultiGoMeshType, 
                                                            afsName,
                                                            multiGoMeshType);
        }

        densityPoolRef_.insert_goSpecData( size_, std::move(goSpecDataUPtr) );

    }
}




}//------------- namespace: espJson_inn end --------------//
}//------------- namespace json: end ----------------

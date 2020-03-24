/*
 * ======================= villageJson.cpp =======================
 *                          -- tpr --
 *                                        CREATE -- 2019.12.04
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 * handle Village-blueprint json files
 */
#include "pch.h"
#include "VillageBlueprint.h"



//--------------- Libs ------------------//
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

#include "tprGeneral.h"

//--------------- Engine ------------------//
#include "global.h"
#include "fileIO.h"
#include "blueprint_oth.h"
#include "FloorGoType.h"

#include "json_oth.h"

#include "esrc_state.h"

//--------------- Script ------------------//

using namespace rapidjson;


namespace blueprint {//------------------ namespace: blueprint start ---------------------//
namespace villageJson_inn {//-------- namespace: villageJson_inn --------------//

    void parse_single_villageJsonFile( const std::string &path_file_ );

}//------------- namespace: villageJson_inn end --------------//


void parse_villageJsonFiles(){

    tprDebug::console( "   ----- parse_villageJsonFiles: start ----- " );


    std::vector<std::string> path_files {};
    json::collect_fileNames( path_blueprintDatas, "villages", "_files.json", path_files );
    //---
    for( const auto &i : path_files ){
        villageJson_inn::parse_single_villageJsonFile(i);
    }

    tprDebug::console( "   ----- parse_villageJsonFiles: end ----- " );
}




namespace villageJson_inn {//-------- namespace: villageJson_inn --------------//


void parse_single_villageJsonFile( const std::string &path_file_ ){
    //-----------------------------//
    //         load file
    //-----------------------------//
    auto jsonBufUPtr = read_a_file( path_file_ );

    //-----------------------------//
    //      parce JSON data
    //-----------------------------//
    Document doc;
    doc.Parse( jsonBufUPtr->c_str() );

    std::string villageName {};
    std::string pngPath_M {};
    IntVec2 frameNum {};
    size_t  totalFrameNum {};

    bool isHaveRoad                 { false };
    
    //=====//
    tprAssert( doc.IsArray() );
    for( auto &docEnt : doc.GetArray() ){

        {//--- villageName ---//
            const auto &a = json::check_and_get_value( docEnt, "villageName", json::JsonValType::String );
            villageName = a.GetString();
        }

        //--- plot ---
        auto villageId = VillageBlueprint::init_new_village( villageName );
        auto &villageRef = VillageBlueprint::get_villageBlueprintRef( villageId );

        {//--- pngLPath ---//
            const auto &a = json::check_and_get_value( docEnt, "pngLPath", json::JsonValType::String );
            std::string dirPath = json::get_jsonFile_dirPath( path_file_ ); // json 文件 所在目录的 path
            pngPath_M = tprGeneral::path_combine( dirPath, a.GetString() );
        }
        {//--- frameNum.col ---//
            const auto &a = json::check_and_get_value( docEnt, "frameNum.col", json::JsonValType::Int );
            frameNum.x =  a.GetInt();
        }
        {//--- frameNum.row ---//
            const auto &a = json::check_and_get_value( docEnt, "frameNum.row", json::JsonValType::Int );
            frameNum.y =  a.GetInt();
        }
        {//--- totalFrameNum ---//
            const auto &a = json::check_and_get_value( docEnt, "totalFrameNum", json::JsonValType::Uint64 );
            totalFrameNum =  cast_2_size_t(a.GetUint64());
            tprAssert( totalFrameNum <= cast_2_size_t(frameNum.x * frameNum.y) );
        }

        //--- isHaveRoad ---//
        if( docEnt.HasMember("isHaveRoad") ){
            const auto &a = json::check_and_get_value( docEnt, "isHaveRoad", json::JsonValType::Bool );
            isHaveRoad = a.GetBool();
        }


        // 读取解析 png 数据，
        IntVec2 frameSizeByMapEnt = parse_png_for_village( villageRef.getnc_mapDatasRef(), pngPath_M, frameNum, totalFrameNum, isHaveRoad );
        tprAssert(  (frameSizeByMapEnt.x == FIELDS_PER_SECTION<> ) && 
                    (frameSizeByMapEnt.y == FIELDS_PER_SECTION<> ) );

        //--- varTypes ---//
        const auto &varTypes = json::check_and_get_value( docEnt, "varTypes", json::JsonValType::Array );
        for( auto &varType : varTypes.GetArray() ){
            tprAssert( varType.IsObject() );

            VariableTypeIdx varTypeIdx {};          
            auto varTypeDatasUPtr = std::make_unique<VarTypeDatas_Village>();

            {//--- type ---//
                const auto &a = json::check_and_get_value( varType, "type", json::JsonValType::String );
                varTypeIdx = str_2_variableTypeIdx( a.GetString() );
            }
            {//--- isAllInstanceUseSamePlan ---//
                const auto &a = json::check_and_get_value( varType, "isAllInstanceUseSamePlan", json::JsonValType::Bool );
                varTypeDatasUPtr->set_isAllInstanceUseSamePlan( a.GetBool() );
            }
            {//--- isRoad ---//
                const auto &a = json::check_and_get_value( varType, "isRoad", json::JsonValType::Bool );
                varTypeDatasUPtr->set_isRoad( a.GetBool() );
            }
            //-- road 相关数据的处理，暂未实现 ---
            // ...

            {//--- yardPool ---//
                std::string yardName {};
                std::string yardLabel {};
                size_t      num {};

                const auto &yardPool = json::check_and_get_value( varType, "yardPool", json::JsonValType::Array );
                for( auto &ent : yardPool.GetArray() ){
                    tprAssert( ent.IsObject() );

                    {//--- num ---//
                        const auto &a = json::check_and_get_value( ent, "num", json::JsonValType::Uint );
                        num = cast_2_size_t( a.GetUint() );
                    }

                    // 允许写 0 
                    if( num == 0 ){
                        continue;
                    }

                    {//--- yardName ---//
                        const auto &a = json::check_and_get_value( ent, "yardName", json::JsonValType::String );
                        yardName = a.GetString();
                    }
                    {//--- yardLabel ---//
                        const auto &a = json::check_and_get_value( ent, "yardLabel", json::JsonValType::String );
                        yardLabel = a.GetString();
                    }

                    varTypeDatasUPtr->insert_2_getYardId_functors( YardBlueprintSet::getFunctor_getYardId(yardName, yardLabel), num );
                    
                }
            }

            varTypeDatasUPtr->init_check();
            villageRef.insert_2_varTypeDatas( varTypeIdx, std::move(varTypeDatasUPtr) );
        }

        villageRef.init_check();

    }
}



}//------------- namespace: villageJson_inn end --------------//
}//--------------------- namespace: blueprint end ------------------------//


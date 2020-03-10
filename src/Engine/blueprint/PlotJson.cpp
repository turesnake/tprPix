/*
 * ======================= plotJson.cpp =======================
 *                          -- tpr --
 *                                        CREATE -- 2019.12.03
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 * handle plot-blueprint json files
 */
#include "pch.h"
#include "PlotBlueprint.h"



//--------------- Libs ------------------//
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

#include "tprGeneral.h"

//--------------- Engine ------------------//
#include "global.h"
#include "fileIO.h"
#include "blueprint_oth.h"

#include "GoSpecFromJson.h"

#include "json_oth.h"

#include "esrc_state.h"

//--------------- Script ------------------//

using namespace rapidjson;


namespace blueprint {//------------------ namespace: blueprint start ---------------------//
namespace plotJson_inn {//-------- namespace: plotJson_inn --------------//

    void parse_single_plotJsonFile( const std::string &path_file_ );

}//------------- namespace: plotJson_inn end --------------//


void parse_plotJsonFiles(){

    tprDebug::console( "   ----- parse_plotJsonFiles: start ----- " );

    esrc::is_setState("json_gameObj"); // MUST


    std::vector<std::string> path_files {};
    json::collect_fileNames( path_blueprintDatas, "plots", "_files.json", path_files );
    //---
    for( const auto &i : path_files ){
        plotJson_inn::parse_single_plotJsonFile(i);
    }

    tprDebug::console( "   ----- parse_plotJsonFiles: end ----- " );
}



namespace plotJson_inn {//-------- namespace: plotJson_inn --------------//




void parse_single_plotJsonFile( const std::string &path_file_ ){
    //-----------------------------//
    //         load file
    //-----------------------------//
    auto jsonBufUPtr = read_a_file( path_file_ );

    //-----------------------------//
    //      parce JSON data
    //-----------------------------//
    Document doc;
    doc.Parse( jsonBufUPtr->c_str() );

    std::string plotName {};
    std::string plotLabel {};
    std::string pngPath_M {};
    IntVec2 frameNum {};
    size_t  totalFrameNum {};
    
    
    //=====//
    tprAssert( doc.IsArray() );
    for( auto &docEnt : doc.GetArray() ){


        {//--- plotName ---//
            const auto &a = json::check_and_get_value( docEnt, "plotName", json::JsonValType::String );
            plotName = a.GetString();
        }
        {//--- plotLabel ---//
            const auto &a = json::check_and_get_value( docEnt, "plotLabel", json::JsonValType::String );
            plotLabel = a.GetString();
        }

        //--- plot ---
        auto plotId = PlotBlueprint::init_new_plot( plotName, plotLabel );
        auto &plotRef = PlotBlueprint::get_plotBlueprintRef( plotId );

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

        // 读取解析 png 数据，
        IntVec2 frameSizeByMapEnt = parse_png_for_plot( plotRef.getnc_mapDatasRef(), pngPath_M, frameNum, totalFrameNum );
        plotRef.set_sizeByMapEnt( frameSizeByMapEnt ); // 不一定必须是正方形


        //--- varTypes ---//
        const auto &varTypes = json::check_and_get_value( docEnt, "varTypes", json::JsonValType::Array );
        for( auto &varType : varTypes.GetArray() ){
            tprAssert( varType.IsObject() );

            VariableTypeIdx varTypeIdx {};
            auto varTypeDatasUPtr = std::make_unique<VarTypeDatas_Plot>();

            {//--- type ---//
                const auto &a = json::check_and_get_value( varType, "type", json::JsonValType::String );
                varTypeIdx = str_2_variableTypeIdx( a.GetString() );
            }
            {//--- isAllInstanceUseSamePlan ---//
                const auto &a = json::check_and_get_value( varType, "isAllInstanceUseSamePlan", json::JsonValType::Bool );
                varTypeDatasUPtr->set_isAllInstanceUseSamePlan( a.GetBool() );

            }
            {//--- goSpecPool ---//
                size_t num  {};

                //---
                const auto &goSpecPool = json::check_and_get_value( varType, "goSpecPool", json::JsonValType::Array );
                for( auto &ent : goSpecPool.GetArray() ){

                    {//--- num ---//
                        const auto &a = json::check_and_get_value( ent, "num", json::JsonValType::Uint );
                        num = cast_2_size_t( a.GetUint() );
                    }

                    // 允许写入 0
                    if( num == 0 ){
                        continue;
                    }

                    auto goSpecUPtr = std::make_unique<GoSpec>();

                    //--- isPlaceHolder ---//
                    if( ent.HasMember("isPlaceHolder") ){
                        const auto &isPlaceHolder = json::check_and_get_value( ent, "isPlaceHolder", json::JsonValType::Bool );
                        if( isPlaceHolder.GetBool() ){
                            goSpecUPtr->isPlaceHolder = true;
                            //-- goSpecUPtr 创建完毕 --
                            varTypeDatasUPtr->insert_2_goSpecPool( std::move(goSpecUPtr), num );
                            continue;
                        }
                    }

                    // IMPORTANT!!!
                    goSpecUPtr->isPlaceHolder = false;

                    {//--- goSpeciesName ---//
                        const auto &a = json::check_and_get_value( ent, "goSpeciesName", json::JsonValType::String );
                        goSpecUPtr->goSpeciesId = GoSpecFromJson::str_2_goSpeciesId( a.GetString() );
                    }

                    //--- goLabel ---//
                    const auto &a = json::check_and_get_value( ent, "goLabel", json::JsonValType::String );
                    goSpecUPtr->goLabelId = GoAssemblePlanSet::str_2_goLabelId( a.GetString() );

                    //-- goSpecUPtr 创建完毕 --
                    varTypeDatasUPtr->insert_2_goSpecPool( std::move(goSpecUPtr), num );
                }
            }

            varTypeDatasUPtr->init_check();
            plotRef.insert_2_varTypeDatas( varTypeIdx, std::move(varTypeDatasUPtr) );
        }

        plotRef.init_check();

    }
}





}//------------- namespace: plotJson_inn end --------------//
}//--------------------- namespace: blueprint end ------------------------//


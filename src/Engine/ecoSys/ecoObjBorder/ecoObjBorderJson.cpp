/*
 * ==================== ecoObjBorderJson.cpp =======================
 *                          -- tpr --
 *                                        CREATE -- 2020.02.14
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#include "pch.h"
#include "EcoObjBorder.h"

//--------------- Libs ------------------//
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

#include "tprGeneral.h"

//--------------- Engine ------------------//
#include "global.h"
#include "fileIO.h"
#include "json_oth.h"


//--------------- Script ------------------//
using namespace rapidjson;


std::tuple<IntVec2, size_t, std::string> EcoObjBorder::parse_plotJsonFile(){

    tprDebug::console( "   ----- parse_ecoObjBorderJsonFile: start ----- " );

    //-----------------------------//
    //         load file
    //-----------------------------//
    std::string path_file = tprGeneral::path_combine(path_jsons, "/ecoObjBorder/_ecoObjBorder.json");
    auto jsonBufUPtr = read_a_file( path_file );

    //-----------------------------//
    //      parce JSON data
    //-----------------------------//
    Document doc;
    doc.Parse( jsonBufUPtr->c_str() );

    IntVec2 frameNum {};
    size_t  totalFrameNum {};
    std::string pngPath {}; // 完整路径名
    
    //=====//
    tprAssert( doc.IsObject() );
    {//--- frameNum.col ---//
        const auto &a = json::check_and_get_value( doc, "frameNum.col", json::JsonValType::Int );
        frameNum.x =  a.GetInt();
    }
    {//--- frameNum.row ---//
        const auto &a = json::check_and_get_value( doc, "frameNum.row", json::JsonValType::Int );
        frameNum.y =  a.GetInt();
    }
    {//--- totalFrameNum ---//
        const auto &a = json::check_and_get_value( doc, "totalFrameNum", json::JsonValType::Uint64 );
        totalFrameNum =  cast_2_size_t(a.GetUint64());
        tprAssert( totalFrameNum <= cast_2_size_t(frameNum.x * frameNum.y) );
    }
    {//--- pngLPath ---//
        const auto &a = json::check_and_get_value( doc, "pngLPath", json::JsonValType::String );
        std::string lPath = a.GetString();
        std::string dirPath = json::get_jsonFile_dirPath( path_file ); // json 文件 所在目录的 path
        pngPath = tprGeneral::path_combine( dirPath, lPath );
    }
    tprDebug::console( "   ----- parse_ecoObjBorderJsonFile: end ----- " );
    //===
    return { frameNum, totalFrameNum, pngPath };
}


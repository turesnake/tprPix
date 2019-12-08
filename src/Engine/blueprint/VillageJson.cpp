/*
 * ======================= VillageJson.cpp =======================
 *                          -- tpr --
 *                                        CREATE -- 2019.12.04
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 * handle Village-blueprint json files
 */
#include "VillageBlueprint.h"

//--------------- Libs ------------------//
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

#include "tprGeneral.h"

//--------------- Engine ------------------//
#include "global.h"
#include "fileIO.h"
#include "tprCast.h"
#include "blueprint_oth.h"

#include "esrc_state.h"

//--------------- Script ------------------//
#include "Script/json/json_oth.h" // tmp
#include "Script/resource/ssrc_gameObj.h" // tmp



using namespace rapidjson;

#include <iostream>
using std::cout;
using std::endl;


namespace blueprint {//------------------ namespace: blueprint start ---------------------//
namespace villageJson_inn {//-------- namespace: villageJson_inn --------------//

    void parse_from_single_villageJsonFile( const std::string &path_file_ );

}//------------- namespace: villageJson_inn end --------------//


void parse_from_villageJsonFiles(){

    cout << "   ----- parse_from_villageJsonFiles: start ----- " << endl;


    std::vector<std::string> path_files {};
    json::collect_fileNames( path_blueprintDatas, "villages", "_files.json", path_files );
    //---
    for( const auto &i : path_files ){
        villageJson_inn::parse_from_single_villageJsonFile(i);
    }

    cout << "   ----- parse_from_villageJsonFiles: end ----- " << endl;
}




namespace villageJson_inn {//-------- namespace: villageJson_inn --------------//


void parse_from_single_villageJsonFile( const std::string &path_file_ ){
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
    
    //---
    tprAssert( doc.IsObject() );
    {//--- villageName ---//
        const auto &a = json::check_and_get_value( doc, "villageName", json::JsonValType::String );
        villageName = a.GetString();
    }

    //--- plot ---
    auto villageId = VillageBlueprint::init_new_village( villageName );
    auto &villageRef = VillageBlueprint::get_villageBlueprintRef( villageId );
    

    {//--- pngLPath ---//
        const auto &a = json::check_and_get_value( doc, "pngLPath", json::JsonValType::String );
        std::string pngLPath = a.GetString();
        std::string headPath = tprGeneral::path_combine(path_blueprintDatas, "villages");
        pngPath_M = tprGeneral::path_combine( headPath, pngLPath );
    }
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

    // 读取解析 png 数据，
    IntVec2 frameSizeByMapEnt = parse_png( villageRef.getnc_mapDatasRef(), pngPath_M, frameNum, totalFrameNum, BlueprintType::Village );
    tprAssert(  (frameSizeByMapEnt.x == FIELDS_PER_CHUNK * CHUNKS_PER_SECTION ) && 
                (frameSizeByMapEnt.y == FIELDS_PER_CHUNK * CHUNKS_PER_SECTION ) );

    //--- varTypes ---//
    const auto &varTypes = json::check_and_get_value( doc, "varTypes", json::JsonValType::Array );
    for( auto &varType : varTypes.GetArray() ){
        tprAssert( varType.IsObject() );

        VariableTypeIdx varTypeIdx {};          
        std::unique_ptr<VarTypeDatas_Village> varTypeDatasUPtr = std::make_unique<VarTypeDatas_Village>();

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

        {//--- yardNames ---//
            const auto &yardNames = json::check_and_get_value( varType, "yardNames", json::JsonValType::Array );
            for( auto &ent : yardNames.GetArray() ){
                tprAssert( ent.IsString() );
                varTypeDatasUPtr->insert_2_yardIds( YardBlueprint::str_2_yardBlueprintId(ent.GetString()) );
            }
        }



        varTypeDatasUPtr->init_check();
        villageRef.insert_2_varTypeDatas( varTypeIdx, std::move(varTypeDatasUPtr) );
    }

    villageRef.init_check();
}



}//------------- namespace: villageJson_inn end --------------//
}//--------------------- namespace: blueprint end ------------------------//


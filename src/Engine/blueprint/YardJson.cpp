/*
 * ======================= yardJson.cpp =======================
 *                          -- tpr --
 *                                        CREATE -- 2019.12.04
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 * handle yard-blueprint json files
 */
#include "YardBlueprint.h"


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
#include "GoSpecFromJson.h"

#include "json_oth.h"

#include "esrc_state.h"

//--------------- Script ------------------//



using namespace rapidjson;

#include <iostream>
using std::cout;
using std::endl;


namespace blueprint {//------------------ namespace: blueprint start ---------------------//
namespace yardJson_inn {//-------- namespace: yardJson_inn --------------//

    void parse_single_yardJsonFile( const std::string &path_file_ );

}//------------- namespace: yardJson_inn end --------------//


void parse_yardJsonFiles(){

    cout << "   ----- parse_yardJsonFiles: start ----- " << endl;


    std::vector<std::string> path_files {};
    json::collect_fileNames( path_blueprintDatas, "yards", "_files.json", path_files );
    //---
    for( const auto &i : path_files ){
        yardJson_inn::parse_single_yardJsonFile(i);
    }

    cout << "   ----- parse_yardJsonFiles: end ----- " << endl;
}




namespace yardJson_inn {//-------- namespace: yardJson_inn --------------//


void parse_single_yardJsonFile( const std::string &path_file_ ){
    //-----------------------------//
    //         load file
    //-----------------------------//
    auto jsonBufUPtr = read_a_file( path_file_ );

    //-----------------------------//
    //      parce JSON data
    //-----------------------------//
    Document doc;
    doc.Parse( jsonBufUPtr->c_str() );

    std::string yardName {};
    std::string yardLabel {};
    std::string pngPath_M {};
    IntVec2 frameNum {};
    size_t  totalFrameNum {};

    bool isHaveMajorGos {};
    bool isHaveFloorGos {};
    

    //---
    tprAssert( doc.IsObject() );
    {//--- yardName ---//
        const auto &a = json::check_and_get_value( doc, "yardName", json::JsonValType::String );
        yardName = a.GetString();
    }
    {//--- yardLabel ---//
        const auto &a = json::check_and_get_value( doc, "yardLabel", json::JsonValType::String );
        yardLabel = a.GetString();
    }

    //--- yard ---
    auto yardId = YardBlueprintSet::init_new_yard( yardName, yardLabel );
    auto &yardRef = YardBlueprintSet::get_yardBlueprintRef( yardId );


    {//--- pngLPath ---//
        const auto &a = json::check_and_get_value( doc, "pngLPath", json::JsonValType::String );
        std::string pngLPath = a.GetString();
        std::string headPath = tprGeneral::path_combine(path_blueprintDatas, "yards");
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
    {//--- isHaveMajorGos ---//
        const auto &a = json::check_and_get_value( doc, "isHaveMajorGos", json::JsonValType::Bool );
        isHaveMajorGos = a.GetBool();
    }
    {//--- isHaveFloorGos ---//
        const auto &a = json::check_and_get_value( doc, "isHaveFloorGos", json::JsonValType::Bool );
        isHaveFloorGos = a.GetBool();
    }

    //-- 至少一个值 为 true --
    tprAssert( isHaveMajorGos || isHaveFloorGos );
    yardRef.set_isHaveMajorGos( isHaveMajorGos );
    yardRef.set_isHaveFloorGos( isHaveFloorGos );
    

    // 读取解析 png 数据，
    IntVec2 frameSizeByMapEnt = parse_png_for_yard( yardRef, pngPath_M, frameNum, totalFrameNum );
    yardRef.set_sizeByField( frameSizeByMapEnt );

    VariableTypeIdx varTypeIdx {}; 
    bool isAllInstanceUseSamePlan {};
    bool isFloorGo {};

    //--- varTypes ---//
    const auto &varTypes = json::check_and_get_value( doc, "varTypes", json::JsonValType::Array );
    for( auto &varType : varTypes.GetArray() ){
        tprAssert( varType.IsObject() );

        {//--- type ---//
            const auto &a = json::check_and_get_value( varType, "type", json::JsonValType::String );
            varTypeIdx = str_2_variableTypeIdx( a.GetString() );
        }
        {//--- isAllInstanceUseSamePlan ---//
            const auto &a = json::check_and_get_value( varType, "isAllInstanceUseSamePlan", json::JsonValType::Bool );
            isAllInstanceUseSamePlan = a.GetBool();
        }
        {//--- isFloorGo ---//
            const auto &a = json::check_and_get_value( varType, "isFloorGo", json::JsonValType::Bool );
            isFloorGo = a.GetBool();
        }

        if( !isFloorGo ){
            //----------------//
            //    MajorGo
            //----------------//
            std::unique_ptr<VarTypeDatas_Yard_MajorGo> varTypeDatas_majorGoUPtr = std::make_unique<VarTypeDatas_Yard_MajorGo>();
            varTypeDatas_majorGoUPtr->set_isAllInstanceUseSamePlan( isAllInstanceUseSamePlan );

            {//--- isPlotBlueprint ---//
                const auto &a = json::check_and_get_value( varType, "isPlotBlueprint", json::JsonValType::Bool );
                varTypeDatas_majorGoUPtr->set_isPlotBlueprint( a.GetBool() );
            }

            if( varTypeDatas_majorGoUPtr->get_isPlotBlueprint() ){

                std::string plotName {};
                size_t      num {};

                //--- plotPool ---//
                const auto &plotPool = json::check_and_get_value( varType, "plotPool", json::JsonValType::Array );
                for( auto &ent : plotPool.GetArray() ){
                    tprAssert( ent.IsObject() );

                    {//--- plotName ---//
                        const auto &a = json::check_and_get_value( ent, "plotName", json::JsonValType::String );
                        plotName = a.GetString();
                    }
                    {//--- num ---//
                        const auto &a = json::check_and_get_value( ent, "num", json::JsonValType::Uint64 );
                        num = cast_2_size_t( a.GetUint64() );
                        tprAssert( num > 0 );
                    }

                    varTypeDatas_majorGoUPtr->insert_2_plotIds( PlotBlueprint::str_2_plotBlueprintId(plotName), num );
                }

            }else{
                //--- goSpecPool ---//
                const auto &goSpecPool = json::check_and_get_value( varType, "goSpecPool", json::JsonValType::Array );
                for( auto &ent : goSpecPool.GetArray() ){

                    std::unique_ptr<GoSpec> goSpecUPtr = std::make_unique<GoSpec>();
                    size_t num {};

                    {//--- goSpecName ---//
                        const auto &a = json::check_and_get_value( ent, "goSpecName", json::JsonValType::String );
                        goSpecUPtr->goSpecId = GoSpecFromJson::str_2_goSpecId( a.GetString() );
                    }

                    //--- MultiGoMeshType ---//
                    if( ent.HasMember("MultiGoMeshType") ){
                        goSpecUPtr->isMultiGoMesh = true;
                        const auto &a = json::check_and_get_value( ent, "MultiGoMeshType", json::JsonValType::String );
                        goSpecUPtr->multiGoMeshType = MultiGoMesh::str_2_multiGoMeshTypeId( a.GetString() );

                    }else{
                        goSpecUPtr->isMultiGoMesh = false;

                        {//--- afsName ---//
                            const auto &a = json::check_and_get_value( ent, "afsName", json::JsonValType::String );
                            std::string afsName = a.GetString();
                            tprAssert( GoSpecFromJson::is_find_in_afsNames(goSpecUPtr->goSpecId, afsName) );
                            goSpecUPtr->afsName = afsName;
                        }
                        {//--- animLabels ---//
                            const auto &a = json::check_and_get_value( ent, "animLabels", json::JsonValType::Array );
                            if( a.Size() > 0 ){
                                for( auto &label : a.GetArray() ){//- foreach AnimLabel
                                    tprAssert( label.IsString() );
                                    goSpecUPtr->animLabels.push_back( str_2_AnimLabel(label.GetString()) );
                                }
                            }
                        }
                    }

                    {//--- num ---//
                        const auto &a = json::check_and_get_value( ent, "num", json::JsonValType::Uint64 );
                        num = cast_2_size_t( a.GetUint64() );
                        tprAssert( num > 0 );
                    }

                    //-- goSpecUPtr 创建完毕 --
                    varTypeDatas_majorGoUPtr->insert_2_goSpecPool( std::move(goSpecUPtr), num );
                }
            }

            varTypeDatas_majorGoUPtr->init_check();
            yardRef.insert_2_majorGo_varTypeDatas( varTypeIdx, std::move(varTypeDatas_majorGoUPtr) );

        }else{
            //----------------//
            //    FloorGo
            //----------------//
            std::unique_ptr<VarTypeDatas_Yard_FloorGo> varTypeDatas_floorGoUPtr = std::make_unique<VarTypeDatas_Yard_FloorGo>();
            varTypeDatas_floorGoUPtr->set_isAllInstanceUseSamePlan( isAllInstanceUseSamePlan );

            {//--- FloorGoSize ---//
                const auto &a = json::check_and_get_value( varType, "FloorGoSize", json::JsonValType::String );
                varTypeDatas_floorGoUPtr->set_floorGoSize( str_2_FloorGoSize(a.GetString()) );
            }

            {//--- goSpecPool ---//
                const auto &goSpecPool = json::check_and_get_value( varType, "goSpecPool", json::JsonValType::Array );
                for( auto &ent : goSpecPool.GetArray() ){

                    std::unique_ptr<GoSpec> goSpecUPtr = std::make_unique<GoSpec>();
                    size_t num {};

                    {//--- goSpecName ---//
                        const auto &a = json::check_and_get_value( ent, "goSpecName", json::JsonValType::String );
                        goSpecUPtr->goSpecId = GoSpecFromJson::str_2_goSpecId( a.GetString() );
                    }
                    {//--- afsName ---//
                        const auto &a = json::check_and_get_value( ent, "afsName", json::JsonValType::String );
                        std::string afsName = a.GetString();
                        tprAssert( GoSpecFromJson::is_find_in_afsNames(goSpecUPtr->goSpecId, afsName) );
                        goSpecUPtr->afsName = afsName;
                    }
                    {//--- animLabels ---//
                        const auto &a = json::check_and_get_value( ent, "animLabels", json::JsonValType::Array );
                        if( a.Size() > 0 ){
                            for( auto &label : a.GetArray() ){//- foreach AnimLabel
                                tprAssert( label.IsString() );
                                goSpecUPtr->animLabels.push_back( str_2_AnimLabel(label.GetString()) );
                            }
                        }
                    }

                                            // 万一 floorGo 是 multiGoMesh 
                                            // 那么上面这段就要扩充
                                            // ...

                    {//--- num ---//
                        const auto &a = json::check_and_get_value( ent, "num", json::JsonValType::Uint64 );
                        num = cast_2_size_t( a.GetUint64() );
                        tprAssert( num > 0 );
                    }

                    //-- goSpecUPtr 创建完毕 --
                    varTypeDatas_floorGoUPtr->insert_2_goSpecPool( std::move(goSpecUPtr), num );
                }
            }
            varTypeDatas_floorGoUPtr->init_check();
            yardRef.insert_2_floorGo_varTypeDatas( varTypeIdx, std::move(varTypeDatas_floorGoUPtr) );

        }

    }

    yardRef.init_check();
}



}//------------- namespace: yardJson_inn end --------------//
}//--------------------- namespace: blueprint end ------------------------//


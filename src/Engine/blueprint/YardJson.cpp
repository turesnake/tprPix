/*
 * ======================= yardJson.cpp =======================
 *                          -- tpr --
 *                                        CREATE -- 2019.12.04
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 * handle yard-blueprint json files
 */
#include "pch.h"
#include "YardBlueprint.h"




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
namespace yardJson_inn {//-------- namespace: yardJson_inn --------------//

    void parse_single_yardJsonFile( const std::string &path_file_ );


    std::shared_ptr<VarTypeDatas_Yard_MajorGo> parse_majorGo_varType( const Value &varType_,
                                                                bool isAllInstanceUseSamePlan_ );

    std::shared_ptr<VarTypeDatas_Yard_FloorGo> parse_floorGo_varType(   const Value &varType_,
                                                                    bool isAllInstanceUseSamePlan_ );


}//------------- namespace: yardJson_inn end --------------//


void parse_yardJsonFiles(){

    tprDebug::console( "   ----- parse_yardJsonFiles: start ----- " );


    std::vector<std::string> path_files {};
    json::collect_fileNames( path_blueprintDatas, "yards", "_files.json", path_files );
    //---
    for( const auto &i : path_files ){
        yardJson_inn::parse_single_yardJsonFile(i);
    }

    tprDebug::console( "   ----- parse_yardJsonFiles: end ----- " );
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
    std::string pngPath_M {};
    IntVec2 frameNum {};
    size_t  totalFrameNum {};

    bool isHaveMajorGos {};
    bool isHaveFloorGos {};

    VariableTypeIdx varTypeIdx {}; 
    bool isAllInstanceUseSamePlan {};
    bool isFloorGo {};

    //=====//
    if( !doc.IsArray() ){
        tprDebug::console("ERROR_filePath:\n{}", path_file_ );
    }
    tprAssert( doc.IsArray() );
    for( auto &docEnt : doc.GetArray() ){

        {//--- yardName ---//
            const auto &a = json::check_and_get_value( docEnt, "yardName", json::JsonValType::String );
            yardName = a.GetString();
        }
        
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

        //--- specialFrameAllocateTimes ---//
        // 每一帧的 分配次数，用于填入 分配池
        // 所有帧默认分配一次，个别希望提高分配律的，可以手动写入 分配个数
        std::vector<size_t> frameAllocateTimes (totalFrameNum, 1);
        if( docEnt.HasMember("specialFrameAllocateTimes") ){

            size_t frameIdx {};
            size_t allocateNum {};

            const auto &allocateTimes = json::check_and_get_value( docEnt, "specialFrameAllocateTimes", json::JsonValType::Array );
            for( const auto &allocEnt : allocateTimes.GetArray() ){
                tprAssert( allocEnt.IsObject() );

                {//--- frameIdx ---//
                    const auto &a = json::check_and_get_value( allocEnt, "frameIdx", json::JsonValType::Uint );
                    frameIdx = cast_2_size_t( a.GetUint() );
                    tprAssert( frameIdx < totalFrameNum );
                }
                {//--- allocateNum ---//
                    const auto &a = json::check_and_get_value( allocEnt, "allocateNum", json::JsonValType::Uint );
                    allocateNum = cast_2_size_t( a.GetUint() );
                        // 不做数值限制，可以写入 0,1, >1 的值
                }

                frameAllocateTimes.at(frameIdx) = allocateNum;
            }
        }


        {//--- isHaveMajorGos ---//
            const auto &a = json::check_and_get_value( docEnt, "isHaveMajorGos", json::JsonValType::Bool );
            isHaveMajorGos = a.GetBool();
        }
        {//--- isHaveFloorGos ---//
            const auto &a = json::check_and_get_value( docEnt, "isHaveFloorGos", json::JsonValType::Bool );
            isHaveFloorGos = a.GetBool();
        }

        //-- 至少一个值 为 true --
        tprAssert( isHaveMajorGos || isHaveFloorGos );



        //--- public_varTypes ---//
        // 公共数据，每一个 yardObj 都需要 insert 这些数据 
        // 因为会被 复制给 各个 yardObj ，只能用 sptr
        bool  isHave_public_varTypes {false};
        std::vector< std::pair<VariableTypeIdx, std::shared_ptr<VarTypeDatas_Yard_MajorGo>>> public_majorGo_varTypes {};
        std::vector< std::pair<VariableTypeIdx, std::shared_ptr<VarTypeDatas_Yard_FloorGo>>> public_floorGo_varTypes {};

        if( docEnt.HasMember("public_varTypes") ){
            isHave_public_varTypes = true;

            const auto &public_varTypes = json::check_and_get_value( docEnt, "public_varTypes", json::JsonValType::Array );
            for( const auto &pVarType : public_varTypes.GetArray() ){
                tprAssert( pVarType.IsObject() );

                {//--- type ---//
                    const auto &a = json::check_and_get_value( pVarType, "type", json::JsonValType::String );
                    varTypeIdx = str_2_variableTypeIdx( a.GetString() );
                }
                {//--- isAllInstanceUseSamePlan ---//
                    const auto &a = json::check_and_get_value( pVarType, "isAllInstanceUseSamePlan", json::JsonValType::Bool );
                    isAllInstanceUseSamePlan = a.GetBool();
                }
                {//--- isFloorGo ---//
                    const auto &a = json::check_and_get_value( pVarType, "isFloorGo", json::JsonValType::Bool );
                    isFloorGo = a.GetBool();
                }

                if( !isFloorGo ){
                    //----------------//
                    //    MajorGo
                    //----------------//
                    auto sptr = parse_majorGo_varType( pVarType, isAllInstanceUseSamePlan );
                    //public_majorGo_varTypes.push_back({ varTypeIdx, sptr });
                    public_majorGo_varTypes.emplace_back( varTypeIdx, sptr );
                }else{
                    //----------------//
                    //    FloorGo
                    //----------------//
                    auto sptr = parse_floorGo_varType( pVarType, isAllInstanceUseSamePlan );
                    //public_floorGo_varTypes.push_back({ varTypeIdx, sptr });
                    public_floorGo_varTypes.emplace_back( varTypeIdx, sptr );

                }
            }
        }

        
        //--- yardObjs ---//
        // yard 实例，也被分为 不同的 dir
        const auto &yardObjs = json::check_and_get_value( docEnt, "yardObjs", json::JsonValType::Array );
        for( const auto &yardObj : yardObjs.GetArray() ){
            tprAssert( yardObj.IsObject() );

            std::string     yardLabel {};
            NineDirection   yardDir {};
            bool            isAllFramesInclude {false}; // 是否本 dir，包含 png数据中的 每一帧
            size_t          fstFrameIdx {};
            size_t          frameNums {};
                                // 用法：
                                // 要么设置 “isAllFramesInclude”: true;  -- 表示全包含
                                // 要么设置 "fstFrameIdx" 和 "frameNums"  -- 表示只包含一部分帧

            {//--- yardLabel ---//
                const auto &a = json::check_and_get_value( yardObj, "yardLabel", json::JsonValType::String );
                yardLabel = a.GetString();
            }

            {//--- dir ---//
                const auto &a = json::check_and_get_value( yardObj, "dir", json::JsonValType::String );
                yardDir = str_2_nineDirection( a.GetString() );
            }

            {//--- isAllFramesInclude ---//
                if( yardObj.HasMember("isAllFramesInclude") ){
                    const auto &a = json::check_and_get_value( yardObj, "isAllFramesInclude", json::JsonValType::Bool );
                    isAllFramesInclude = a.GetBool();
                }
            }

            if( isAllFramesInclude ){
                fstFrameIdx = 0;
                frameNums = totalFrameNum;

            }else{
                {//--- fstFrameIdx ---//
                    const auto &a = json::check_and_get_value( yardObj, "fstFrameIdx", json::JsonValType::Uint );
                    fstFrameIdx = static_cast<size_t>( a.GetUint() );
                }
                {//--- frameNums ---//
                    const auto &a = json::check_and_get_value( yardObj, "frameNums", json::JsonValType::Uint );
                    frameNums = static_cast<size_t>( a.GetUint() );
                }
            }

            // 每一个实例，将会生成一个 yard 实例
            auto yardId = YardBlueprintSet::init_new_yard( yardName, yardLabel, yardDir );
            auto &yardRef = YardBlueprintSet::get_yardBlueprintRef( yardId );

            yardRef.set_isHaveMajorGos( isHaveMajorGos );
            yardRef.set_isHaveFloorGos( isHaveFloorGos );

            // 读取解析 png 数据，
            // 这里存在问题，等于要重复解析 png 文件。
            // 暂不考虑性能 ...
            IntVec2 frameSizeByMapEnt = parse_png_for_yard( yardRef, 
                                                            pngPath_M,
                                                            frameAllocateTimes, 
                                                            frameNum, 
                                                            totalFrameNum, 
                                                            fstFrameIdx, 
                                                            frameNums );
            yardRef.set_sizeByField( frameSizeByMapEnt );

            // 在读取自己数据欠，先读取之前收集好的 public_varTypes
            if( isHave_public_varTypes ){
                for( auto [idx, sptr] : public_majorGo_varTypes ){
                    yardRef.insert_2_majorGo_varTypeDatas( idx, sptr );
                }
                for( auto [idx, sptr] : public_floorGo_varTypes ){
                    yardRef.insert_2_floorGo_varTypeDatas( idx, sptr );
                }
            }

            //--- varTypes ---//
            const auto &varTypes = json::check_and_get_value( yardObj, "varTypes", json::JsonValType::Array );
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
                    auto uptr = parse_majorGo_varType( varType, isAllInstanceUseSamePlan );
                    yardRef.insert_2_majorGo_varTypeDatas( varTypeIdx, std::move(uptr) );

                }else{
                    //----------------//
                    //    FloorGo
                    //----------------//
                    auto uptr = parse_floorGo_varType( varType, isAllInstanceUseSamePlan );
                    yardRef.insert_2_floorGo_varTypeDatas( varTypeIdx, std::move(uptr) );

                }

            }

            yardRef.init_check();
        }

    }
}




std::shared_ptr<VarTypeDatas_Yard_MajorGo> parse_majorGo_varType(   const Value &varType_,
                                                                    bool isAllInstanceUseSamePlan_ ){
    //
    std::shared_ptr<VarTypeDatas_Yard_MajorGo> vmSPtr = std::make_shared<VarTypeDatas_Yard_MajorGo>();
    vmSPtr->set_isAllInstanceUseSamePlan( isAllInstanceUseSamePlan_ );

    //--- isPlotBlueprint ---//
    if( varType_.HasMember("plotPool") ){
        tprAssert( !varType_.HasMember("goSpecPool") );
        vmSPtr->set_isPlotBlueprint( true );
    }else{
        tprAssert( varType_.HasMember("goSpecPool") );
        vmSPtr->set_isPlotBlueprint( false );
    }
    

    if( vmSPtr->get_isPlotBlueprint() ){

        std::string plotName {};
        std::string plotLabel {};
        size_t      num {};

        //--- plotPool ---//
        const auto &plotPool = json::check_and_get_value( varType_, "plotPool", json::JsonValType::Array );
        for( auto &ent : plotPool.GetArray() ){
            tprAssert( ent.IsObject() );

            {//--- num ---//
                const auto &a = json::check_and_get_value( ent, "num", json::JsonValType::Uint );
                num = cast_2_size_t( a.GetUint() );
            }

            // 允许写 0 
            if( num == 0 ){
                continue;
            }

            //--- isPlaceHolder ---//
            if( ent.HasMember("isPlaceHolder") ){
                const auto &isPlaceHolder = json::check_and_get_value( ent, "isPlaceHolder", json::JsonValType::Bool );
                if( isPlaceHolder.GetBool() ){
                    vmSPtr->insert_2_plotIds( PlotBlueprint::str_2_plotBlueprintId("_PLACE_HOLDER_","_PLACE_HOLDER_"), num ); // 占位符              
                    continue;
                }
            }

            {//--- plotName ---//
                const auto &a = json::check_and_get_value( ent, "plotName", json::JsonValType::String );
                plotName = a.GetString();
                tprAssert( plotName != "_PLACE_HOLDER_" );
            }
            {//--- plotLabel ---//
                const auto &a = json::check_and_get_value( ent, "plotLabel", json::JsonValType::String );
                plotLabel = a.GetString();
                tprAssert( plotLabel != "_PLACE_HOLDER_" );
            }
            
            vmSPtr->insert_2_plotIds( PlotBlueprint::str_2_plotBlueprintId(plotName,plotLabel), num );
        }

    }else{
        //--- goSpecPool ---//
        const auto &goSpecPool = json::check_and_get_value( varType_, "goSpecPool", json::JsonValType::Array );
        for( auto &ent : goSpecPool.GetArray() ){

                            
            size_t num {};
            {//--- num ---//
                const auto &a = json::check_and_get_value( ent, "num", json::JsonValType::Uint );
                num = cast_2_size_t( a.GetUint() );
            }

            // 允许写 0 
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
                    vmSPtr->insert_2_goSpecPool( std::move(goSpecUPtr), num );
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
            tprAssert( ent.HasMember("goLabel") );
            const auto &a = json::check_and_get_value( ent, "goLabel", json::JsonValType::String );
            goSpecUPtr->goLabelId = GoAssemblePlanSet::str_2_goLabelId( a.GetString() );

            //-- goSpecUPtr 创建完毕 --
            vmSPtr->insert_2_goSpecPool( std::move(goSpecUPtr), num );
        }
    }

    vmSPtr->init_check();
    return vmSPtr;
}



std::shared_ptr<VarTypeDatas_Yard_FloorGo> parse_floorGo_varType(   const Value &varType_,
                                                                    bool isAllInstanceUseSamePlan_ ){
    //
    std::shared_ptr<VarTypeDatas_Yard_FloorGo> vfSPtr = std::make_shared<VarTypeDatas_Yard_FloorGo>();
    vfSPtr->set_isAllInstanceUseSamePlan( isAllInstanceUseSamePlan_ );

    {//--- FloorGoSize ---//
        const auto &a = json::check_and_get_value( varType_, "FloorGoSize", json::JsonValType::String );
        vfSPtr->set_floorGoSize( str_2_floorGoSize(a.GetString()) );
    }

    {//--- goSpecPool ---//
        const auto &goSpecPool = json::check_and_get_value( varType_, "goSpecPool", json::JsonValType::Array );
        for( auto &ent : goSpecPool.GetArray() ){

            size_t num {};
            {//--- num ---//
                const auto &a = json::check_and_get_value( ent, "num", json::JsonValType::Uint );
                num = cast_2_size_t( a.GetUint() );
            }

            // 允许写 0 
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
                    vfSPtr->insert_2_goSpecPool( std::move(goSpecUPtr), num );
                    continue;
                }
            }

            // IMPORTANT!!!
            goSpecUPtr->isPlaceHolder = false;

            {//--- goSpeciesName ---//
                const auto &a = json::check_and_get_value( ent, "goSpeciesName", json::JsonValType::String );
                goSpecUPtr->goSpeciesId = GoSpecFromJson::str_2_goSpeciesId( a.GetString() );
            }

            {//--- goLabel ---//
                tprAssert( ent.HasMember("goLabel") );
                const auto &a = json::check_and_get_value( ent, "goLabel", json::JsonValType::String );
                goSpecUPtr->goLabelId = GoAssemblePlanSet::str_2_goLabelId( a.GetString() );
            }

            //-- goSpecUPtr 创建完毕 --
            vfSPtr->insert_2_goSpecPool( std::move(goSpecUPtr), num );
        }
    }
    vfSPtr->init_check();
    return vfSPtr;
}









}//------------- namespace: yardJson_inn end --------------//
}//--------------------- namespace: blueprint end ------------------------//


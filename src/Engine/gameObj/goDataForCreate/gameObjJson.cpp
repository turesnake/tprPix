/*
 * ================= gameObjJson.cpp =====================
 *                          -- tpr --
 *                                        CREATE -- 2019.07.02
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#include "pch.h"
//--------------- CPP ------------------//
#include <utility>

//--------------- Libs ------------------//
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

#include "tprGeneral.h"

//-------------------- Engine --------------------//
#include "global.h"
#include "fileIO.h"
#include "GameObjType.h"
#include "AnimFrameSet.h"
#include "AnimActionEName.h"

#include "GoSpecFromJson.h"

#include "json_oth.h"

#include "esrc_state.h"


using namespace rapidjson;



namespace json{//------------- namespace json ----------------
namespace goJson_inn {//-------- namespace: goJson_inn --------------//

    void parse_single_jsonFile( const std::string &path_file_ );

}//------------- namespace: goJson_inn end --------------//



/* Do Not Worry About Performance !!!
 */
void parse_goJsonFile(){

    tprDebug::console( "   ----- parse_goJsonFile: start ----- " );


    std::vector<std::string> path_files {};
    collect_fileNames( path_gameObjDatas, "", "_files.json", path_files );

    for( const auto &i : path_files ){
        goJson_inn::parse_single_jsonFile(i);
    }

    //--
    GoSpecFromJson::check_all_extraPassableDogoSpeciesIds();

    esrc::insertState("json_gameObj");
    tprDebug::console( "   ----- parse_goJsonFile: end ----- " );
}


namespace goJson_inn {//-------- namespace: goJson_inn --------------//



void parse_moveStateTable( const Value &pngEnt_, GoSpecFromJson &goSpecFromJsonRef_ );



void parse_single_jsonFile( const std::string &path_file_ ){

    //-----------------------------//
    //         load file
    //-----------------------------//
    auto jsonBufUPtr = read_a_file( path_file_ );

    //-----------------------------//
    //      parce JSON data
    //-----------------------------//
    Document doc;
    doc.Parse( jsonBufUPtr->c_str() );

    std::string goSpeciesName {};

    tprAssert( doc.IsArray() );
    for( auto &ent : doc.GetArray() ){

        {//--- goSpeciesName ---//
            const auto &a = check_and_get_value( ent, "goSpeciesName", JsonValType::String );
            goSpeciesName = a.GetString();
        }

        auto &goSpecFromJsonRef = GoSpecFromJson::create_new_goSpecFromJson( goSpeciesName );

        {//--- family ---//
            const auto &a = check_and_get_value( ent, "family", JsonValType::String );
            goSpecFromJsonRef.family = str_2_gameObjFamily( a.GetString() );
        }
        {//--- state ---//
            const auto &a = check_and_get_value( ent, "state", JsonValType::String );
            goSpecFromJsonRef.state = str_2_gameObjState( a.GetString() );
        }
        {//--- moveState ---//
            const auto &a = check_and_get_value( ent, "moveState", JsonValType::String );
            goSpecFromJsonRef.moveState = str_2_gameObjMoveState( a.GetString() );
        }

        if( goSpecFromJsonRef.moveState == GameObjMoveState::AbsFixed ){
            goSpecFromJsonRef.moveType = MoveType::Crawl; // 此时，此值已无意义，无需 json 指定了
        }else{
            //--- moveType ---//
            const auto &a = check_and_get_value( ent, "moveType", JsonValType::String );
            goSpecFromJsonRef.moveType = str_2_moveType( a.GetString() );
        }

        {//--- isMoveCollide ---//
            const auto &a = check_and_get_value( ent, "isMoveCollide", JsonValType::Bool );
            goSpecFromJsonRef.isMoveCollide = a.GetBool();
        }
        {//--- isDoPass ---//
            const auto &a = check_and_get_value( ent, "isDoPass", JsonValType::Bool );
            goSpecFromJsonRef.isDoPass = a.GetBool();
        }
        {//--- isBePass ---//
            const auto &a = check_and_get_value( ent, "isBePass", JsonValType::Bool );
            goSpecFromJsonRef.isBePass = a.GetBool();
        }

        //--- extraPassableDogoSpeciesNames ---//
        if( (!goSpecFromJsonRef.isBePass) &&
            ent.HasMember("extraPassableDogoSpeciesNames") ){

            const auto &names = check_and_get_value( ent, "extraPassableDogoSpeciesNames", JsonValType::Array );
            for( const auto &name : names.GetArray() ){
                tprAssert( name.IsString() );
                goSpecFromJsonRef.insert_2_ExtraPassableDogoSpeciesIds( name.GetString() );
            }
        }

        //--- moveSpeedLvl ---//
        if( goSpecFromJsonRef.moveState == GameObjMoveState::AbsFixed ){
            goSpecFromJsonRef.moveSpeedLvl = SpeedLevel::LV_0;
        }else{
            const auto &a = check_and_get_value( ent, "moveSpeedLvl", JsonValType::Int );
            goSpecFromJsonRef.moveSpeedLvl = int_2_SpeedLevel( a.GetInt() );
        }

        //--- moveStateTable ---//
        if( ent.HasMember("moveStateTable") ){
            parse_moveStateTable( ent, goSpecFromJsonRef );
        }
        

        {//--- alti ---//
            const auto &a = check_and_get_value( ent, "alti", JsonValType::Number );
            goSpecFromJsonRef.alti = get_double( a );
        }

        

        {//--- lAltiRanges ---//
            const auto &lAltiRanges = check_and_get_value( ent, "lAltiRanges", JsonValType::Array );
            for( const auto &altiRangeObj : lAltiRanges.GetArray() ){
                tprAssert( altiRangeObj.IsObject() );

                GoAltiRangeLabel label {};
                GoAltiRange      val {};
                {//--- label ---//
                    const auto &a = check_and_get_value( altiRangeObj, "label", JsonValType::String );
                    label = str_2_goAltiRangeLabel( a.GetString() );
                }
                {//--- val ---//
                    const auto &a = check_and_get_value( altiRangeObj, "val", JsonValType::Array );
                    //val = a.GetDouble();
                    tprAssert( a.Size() == 2 );
                    tprAssert( a[0].IsDouble() );
                    tprAssert( a[1].IsDouble() );
                    val.set( a[0].GetDouble(), a[1].GetDouble() );
                }
                goSpecFromJsonRef.insert_2_lAltiRanges( label, val );
            }
        }

        {//--- weight ---//
            const auto &a = check_and_get_value( ent, "weight", JsonValType::Number );
            goSpecFromJsonRef.weight = get_double( a );
        }
        {//--- pub.HP ---//
            tprAssert( ent.HasMember("pub.HP") );
            const Value &a = ent["pub.HP"];
            std::pair<bool,int> pair = get_nullable_int( a );
            goSpecFromJsonRef.pubBinary.HP = (pair.first) ? pair.second : -999; //- tmp
        }
        {//--- pub.MP ---//
            tprAssert( ent.HasMember("pub.MP") );
            const Value &a = ent["pub.MP"];
            std::pair<bool,int> pair = get_nullable_int( a );
            goSpecFromJsonRef.pubBinary.MP = (pair.first) ? pair.second : -999; //- tmp
        }

        //====================================//
        //    afs.json / asm.json
        //------------------------------------//
        // xx.go.json 文件 所在 目录 的 path
        std::string dirPath = tprGeneral::get_dirPath( path_file_ );

        std::string lPath {};
        
        {//--- afs_Paths ---//
            std::string afs_Path {};
            const auto &afs_lPaths = check_and_get_value( ent, "afs_lPaths", JsonValType::Array );

            // 一些 go 是允许没有 afs 数据的，比如使用公共的 emptyPix 数据
            if( afs_lPaths.Size() != 0 ){
                for( const auto &i : afs_lPaths.GetArray() ){
                    tprAssert( i.IsString() );
                    lPath = i.GetString();
                    tprAssert( lPath != "" ); // MUST EXIST !!!
                    afs_Path = tprGeneral::path_combine( dirPath, lPath ); // 绝对路径名
                    json::parse_single_animFrameSetJsonFile(afs_Path, 
                                                            goSpecFromJsonRef.get_afsNamesPtr() );
                                                            // 目前，实际存储地 还是在 esrc 中
                }
            }            
        }

        {//--- goAsm_Paths ---//
            std::string goAsm_Path {};
            const auto &goAsm_lPaths = check_and_get_value( ent, "goAsm_lPaths", JsonValType::Array );

            // 所有 go 必须包含 .asm.json 数据
            // 以此来组织 afs以及其他数据，帮助蓝图组装出一个 完整的 go
            tprAssert( goAsm_lPaths.Size() != 0 );

            for( const auto &i : goAsm_lPaths.GetArray() ){
                tprAssert( i.IsString() );
                lPath = i.GetString();
                tprAssert( lPath != "" ); // MUST EXIST !!!
                goAsm_Path = tprGeneral::path_combine( dirPath, lPath ); // 绝对路径名
                json::parse_single_goAssemblePlanJsonFile( goAsm_Path );
                            // 数据直接存储在 GoSpecFromJson 中
            }
        }

        //====================================//
        //     init check
        //------------------------------------//
        goSpecFromJsonRef.init_check();
        
    }
}




void parse_moveStateTable( const Value &pngEnt_, GoSpecFromJson &goSpecFromJsonRef_ ){

    goSpecFromJsonRef_.moveStateTableUPtr = std::make_unique<GoSpecFromJson::MoveStateTable>();
    GoSpecFromJson::MoveStateTable &tRef = *(goSpecFromJsonRef_.moveStateTableUPtr);


    int        minILvl { 100 }; // 初始值 尽可能大
    int        maxILvl { 0 };   // 初始值 尽可能小
    int        speedILvl {};
    int        timeStepOff {};
    AnimActionEName actionEName {};

    const auto &moveStateTable = check_and_get_value( pngEnt_, "moveStateTable", JsonValType::Array );
    for( const auto &tableEnt : moveStateTable.GetArray() ){
        tprAssert( tableEnt.IsObject() );

        {//--- speedLvl ---//
            const auto &a = check_and_get_value( tableEnt, "speedLvl", JsonValType::Int );
            speedILvl = a.GetInt();
        }
        {//--- actionEName ---//
            const auto &a = check_and_get_value( tableEnt, "actionEName", JsonValType::String );
            actionEName = str_2_animActionEName( a.GetString() );
        }
        {//--- timeStepOff ---//
            const auto &a = check_and_get_value( tableEnt, "timeStepOff", JsonValType::Int );
            timeStepOff = a.GetInt();
        }

        //======================//
        if( speedILvl < minILvl ){
            minILvl = speedILvl;
        }
        if( speedILvl > maxILvl ){
            maxILvl = speedILvl;
        }

        auto [insertIt, insertBool] = tRef.table.emplace( int_2_SpeedLevel(speedILvl),
                                            std::pair<AnimActionEName,int>{ actionEName, timeStepOff } );
        tprAssert( insertBool );
    }

    tRef.minLvl = int_2_SpeedLevel( minILvl );
    tRef.maxLvl = int_2_SpeedLevel( maxILvl );

}




}//------------- namespace: goJson_inn end --------------//
}//------------- namespace json: end ----------------


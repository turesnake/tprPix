/*
 * ================= gameObjJson.cpp =====================
 *                          -- tpr --
 *                                        CREATE -- 2019.07.02
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
//--------------- CPP ------------------//
#include <unordered_map>
#include <string>
#include <utility>

//--------------- Libs ------------------//
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

#include "tprGeneral.h"

//-------------------- Engine --------------------//
#include "tprAssert.h"
#include "global.h"
#include "fileIO.h"
#include "GameObj.h"
#include "AnimFrameSet.h"

#include "GoSpecFromJson.h"

#include "json_oth.h"

#include "esrc_state.h"


using namespace rapidjson;

#include <iostream>
using std::cout;
using std::endl;

namespace json{//------------- namespace json ----------------
namespace goJson_inn {//-------- namespace: goJson_inn --------------//

    void parse_single_jsonFile( const std::string &path_file_ );

}//------------- namespace: goJson_inn end --------------//



/* Do Not Worry About Performance !!!
 */
void parse_goJsonFile(){

    cout << "   ----- parse_goJsonFile: start ----- " << endl;


    std::vector<std::string> path_files {};
    collect_fileNames( path_gameObjDatas, "", "_files.json", path_files );

    for( const auto &i : path_files ){
        goJson_inn::parse_single_jsonFile(i);
    }

    esrc::insertState("json_gameObj");
    cout << "   ----- parse_goJsonFile: end ----- " << endl;
}


namespace goJson_inn {//-------- namespace: goJson_inn --------------//


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

    tprAssert( doc.IsArray() );
    for( auto &ent : doc.GetArray() ){

        //GoJsonData  goJsonData {};

        goSpecId_t  specId {};
        {//--- specID ---//
            const auto &a = check_and_get_value( ent, "specID", JsonValType::Uint );
            specId = static_cast<goSpecId_t>( a.GetUint() );
        }

        auto &goSpecFromJsonRef = GoSpecFromJson::create_new_goSpecFromJson( specId );
        goSpecFromJsonRef.specID = specId;

        {//--- goSpecName ---//
            const auto &a = check_and_get_value( ent, "goSpecName", JsonValType::String );
            goSpecFromJsonRef.goSpecName = a.GetString();
        }
        {//--- family ---//
            const auto &a = check_and_get_value( ent, "family", JsonValType::String );
            goSpecFromJsonRef.family = str_2_GameObjFamily( a.GetString() );
        }
        {//--- state ---//
            const auto &a = check_and_get_value( ent, "state", JsonValType::String );
            goSpecFromJsonRef.state = str_2_GameObjState( a.GetString() );
        }
        {//--- moveState ---//
            const auto &a = check_and_get_value( ent, "moveState", JsonValType::String );
            goSpecFromJsonRef.moveState = str_2_GameObjMoveState( a.GetString() );
        }
        {//--- moveType ---//
            const auto &a = check_and_get_value( ent, "moveType", JsonValType::String );
            goSpecFromJsonRef.moveType = str_2_MoveType( a.GetString() );
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
        {//--- speedLvl ---//
            const auto &a = check_and_get_value( ent, "speedLvl", JsonValType::Int );
            goSpecFromJsonRef.speedLvl = int_2_SpeedLevel( a.GetInt() );
        }
        {//--- alti ---//
            const auto &a = check_and_get_value( ent, "alti", JsonValType::Number );
            goSpecFromJsonRef.alti = get_double( a );
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

        //------------------------------------//
        GoSpecFromJson::insert_2_goSpecIds_names_containers( goSpecFromJsonRef.specID, goSpecFromJsonRef.goSpecName );

        //====================================//
        //    afs.json / multiGoMesh.json
        //------------------------------------//
        // xx.go.json 文件 所在 目录 的 path
        std::string dirPath = tprGeneral::get_dirPath( path_file_ );

        std::string lPath {};
        
        {//--- afs_Paths ---//
            std::string afs_Path {};
            const auto &afs_lPaths = check_and_get_value( ent, "afs_lPaths", JsonValType::Array );
            tprAssert( afs_lPaths.Size() != 0 ); // Must Have Ents !!!
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

        {//--- multiGoMesh_Paths ---//
            std::string multiGoMesh_Path {};
            const auto &multiGoMesh_lPaths = check_and_get_value( ent, "multiGoMesh_lPaths", JsonValType::Array );
            for( const auto &i : multiGoMesh_lPaths.GetArray() ){
                tprAssert( i.IsString() );
                lPath = i.GetString();
                tprAssert( lPath != "" ); // MUST EXIST !!!
                multiGoMesh_Path = tprGeneral::path_combine( dirPath, lPath ); // 绝对路径名
                json::parse_single_multiGoMeshJsonFile( multiGoMesh_Path );
                            // 数据直接存储在 GoSpecFromJson 中
            }
        }
    }
}


}//------------- namespace: goJson_inn end --------------//
}//------------- namespace json: end ----------------


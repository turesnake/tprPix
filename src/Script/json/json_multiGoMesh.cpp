/*
 * ==================== json_multiGoMesh.cpp =====================
 *                          -- tpr --
 *                                        CREATE -- 2019.10.10
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#include "Script/json/json_multiGoMesh.h"

//--------------- CPP ------------------//
#include <unordered_map>
#include <string>
#include <utility>

//--------------- Libs ------------------//
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

#include "tprGeneral.h"

//------------------- Engine --------------------//
#include "tprAssert.h"
#include "global.h"
#include "fileIO.h"

#include "esrc_state.h"
//#include "esrc_json_multiGoMesh.h"

//--------------- Script ------------------//
#include "Script/json/json_all.h"
#include "Script/json/json_oth.h"
#include "Script/resource/ssrc_all.h" 



using namespace rapidjson;

#include <iostream>
using std::cout;
using std::endl;

namespace json{//------------- namespace json ----------------
namespace jmgm_inn {//-------- namespace: jmgm_inn --------------//

    void parse_single_jsonFile( const std::string &path_file_ );

}//------------- namespace: jmgm_inn end --------------//

//======== static ========//
ID_Manager  Json_GoMeshSet::id_manager { ID_TYPE::U32, 0};



/* ===========================================================
 *                parse_from_multiGoMeshJsonFile
 * -----------------------------------------------------------
 * Do Not Worry About Performance !!!
 */
void parse_from_multiGoMeshJsonFile(){

    cout << "   ----- parse_from_multiGoMeshJsonFile: start ----- " << endl;

    esrc::is_setState("json_gameObj");


    std::vector<std::string> path_files {};
    collect_fileNames( "multiGoMeshs", "_files.json", path_files );
    //---
    for( const auto &i : path_files ){
        jmgm_inn::parse_single_jsonFile(i);
    }

    esrc::insertState("json_multiGoMesh");
    cout << "   ----- parse_from_multiGoMeshJsonFile: end ----- " << endl;
}


namespace jmgm_inn {//-------- namespace: jmgm_inn --------------//


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


    std::string     goSpecName {};
    std::string     rootAnimFrameSetName {};
    goSpecId_t      goSpecId {};
    

    tprAssert( doc.IsArray() );
    for( auto &ent : doc.GetArray() ){

        {//--- goSpecName ---//
            const auto &a = check_and_get_value( ent, "goSpecName", JsonValType::String );
            goSpecName = a.GetString();
            goSpecId =  ssrc::str_2_goSpecId( goSpecName );
        }
        

        auto &goSpecRef = ssrc::getnc_goSpecRef( goSpecId );
        if( goSpecRef.multiGoMeshUPtr == nullptr ){
            goSpecRef.multiGoMeshUPtr = std::make_unique<json::Json_MultiGoMesh>();
        }


        
        //--- goMeshSets ---//
        const auto &goMeshSets = check_and_get_value( ent, "goMeshSets", JsonValType::Array );
        for( auto &goMeshSet : goMeshSets.GetArray() ){

            //--- skip annotate ---//
            if( goMeshSet.HasMember( "annotate" ) ){
                continue;
            }

            MultiGoMeshType type    {};
            bool            isSameGoSpec    {};

            {//--- type ---//
                const auto &a = check_and_get_value( goMeshSet, "type", JsonValType::String );
                type = str_2_multiGoMeshType( a.GetString() );
            }
            {//--- rootAnimFrameSetName ---//
                const auto &a = check_and_get_value( goMeshSet, "rootAnimFrameSetName", JsonValType::String );
                rootAnimFrameSetName = a.GetString();
            }

            auto &json_goMeshSet = goSpecRef.multiGoMeshUPtr->create_new_json_goMeshSet( type );

            {//--- isSameGoSpec ---//
                const auto &a = check_and_get_value( goMeshSet, "isSameGoSpec", JsonValType::Bool );
                isSameGoSpec = a.GetBool();
            }

            //--- goMeshs ---//
            const auto &goMeshs = check_and_get_value( goMeshSet, "goMeshs", JsonValType::Array );
            for( auto &goMesh : goMeshs.GetArray() ){ // each goMesh ent

                Json_GoMesh json_goMesh {};

                //--- animFrameSetName ---//
                if( isSameGoSpec ){
                    json_goMesh.animFrameSetName = rootAnimFrameSetName;
                }else{
                    const auto &a = check_and_get_value( goMesh, "animFrameSetName", JsonValType::String );
                    json_goMesh.animFrameSetName = a.GetString();
                }


                {//--- dpos ---//
                    const auto &a = check_and_get_value( goMesh, "dpos", JsonValType::Array );
                    // base dpos is IntVec2, align to pixes
                    tprAssert( a.Size() == 2 );
                    tprAssert( a[0].IsInt() );
                    tprAssert( a[1].IsInt() );
                    json_goMesh.dposOff.x = static_cast<double>(a[0].GetInt());
                    json_goMesh.dposOff.y = static_cast<double>(a[1].GetInt());
                }

                {//--- animLabels ---//
                    const auto &a = check_and_get_value( goMesh, "animLabels", JsonValType::Array );
                    if( a.Size() > 0 ){
                        for( auto &ent : a.GetArray() ){//- foreach AnimLabel
                            tprAssert( ent.IsString() );
                            json_goMesh.animLabels.push_back( str_2_AnimLabel(ent.GetString()) );
                        }
                    }
                }

                json_goMeshSet.gomeshs.push_back( json_goMesh );
            }

        }

    }
}








}//------------- namespace: jmgm_inn end --------------//
}//------------- namespace json: end ----------------


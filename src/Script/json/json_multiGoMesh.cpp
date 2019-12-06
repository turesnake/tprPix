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

    const std::unordered_map<std::string, MultiGoMeshType> str_multiGoMeshTypes{
        { "",        MultiGoMeshType::Default },
        { "Default", MultiGoMeshType::Default },

        { "Sml",    MultiGoMeshType::Sml },
        { "Mid",    MultiGoMeshType::Mid },
        { "Big",    MultiGoMeshType::Big },
        
        { "DForest_Sml",    MultiGoMeshType::DForest_Sml },
        { "DForest_Mid",    MultiGoMeshType::DForest_Mid },
        { "DForest_Big",    MultiGoMeshType::DForest_Big },

        { "Forest_1_Sml",    MultiGoMeshType::Forest_1_Sml },
        { "Forest_1_Mid",    MultiGoMeshType::Forest_1_Mid },
        { "Forest_1_Big",    MultiGoMeshType::Forest_1_Big },

        { "Forest_WhiteFlower_Sml",    MultiGoMeshType::Forest_WhiteFlower_Sml },
        { "Forest_WhiteFlower_Mid",    MultiGoMeshType::Forest_WhiteFlower_Mid },
        { "Forest_WhiteFlower_Big",    MultiGoMeshType::Forest_WhiteFlower_Big },

        { "Forest_LowWhiteFlower_Sml",    MultiGoMeshType::Forest_LowWhiteFlower_Sml },
        { "Forest_LowWhiteFlower_Mid",    MultiGoMeshType::Forest_LowWhiteFlower_Mid },
        { "Forest_LowWhiteFlower_Big",    MultiGoMeshType::Forest_LowWhiteFlower_Big },

        { "Savannah_1_Sml",    MultiGoMeshType::Savannah_1_Sml },
        { "Savannah_1_Mid",    MultiGoMeshType::Savannah_1_Mid },
        { "Savannah_1_Big",    MultiGoMeshType::Savannah_1_Big },

        { "Desert_1_Sml",    MultiGoMeshType::Desert_1_Sml },
        { "Desert_1_Mid",    MultiGoMeshType::Desert_1_Mid },
        { "Desert_1_Big",    MultiGoMeshType::Desert_1_Big }
        //...
    };



    class Json_GoMesh_Tmp{
    public:
        glm::dvec2              dposOff {}; // gomesh-dposoff based on go-dpos
        std::vector<AnimLabel>  animLabels {};
    };


    class Json_GoMeshSet_Tmp{
    public:
        Json_GoMeshSet_Tmp()=default;
        //---
        std::string type {};
        std::vector<Json_GoMesh_Tmp> gomeshs {};
    };


    class Json_MultiGoMesh_Tmp{
    public:
        Json_MultiGoMesh_Tmp()=default;
        //---
        std::vector<Json_GoMeshSet_Tmp> gomeshSets {};
    };




}//------------- namespace: jmgm_inn end --------------//

//======== static ========//
ID_Manager  Json_GoMeshSet::id_manager { ID_TYPE::U32, 0};




MultiGoMeshType str_2_multiGoMeshType( const std::string &str_ )noexcept{

    if( jmgm_inn::str_multiGoMeshTypes.find(str_) == jmgm_inn::str_multiGoMeshTypes.end() ){
        cout << "str_ = " << str_ << endl;
    }


    tprAssert( jmgm_inn::str_multiGoMeshTypes.find(str_) != jmgm_inn::str_multiGoMeshTypes.end() );
    return jmgm_inn::str_multiGoMeshTypes.at(str_);
}






/* ===========================================================
 *                parse_from_multiGoMeshJsonFile
 * -----------------------------------------------------------
 * Do Not Worry About Performance !!!
 */
void parse_from_multiGoMeshJsonFile(){

    cout << "   ----- parse_from_multiGoMeshJsonFile: start ----- " << endl;

    esrc::is_setState("json_gameObj");


    std::vector<std::string> path_files {};
    collect_fileNames( path_jsons, "multiGoMeshs", "_files.json", path_files );
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
    std::string     style {};
    goSpecId_t      goSpecId {};
    

    tprAssert( doc.IsArray() );
    for( auto &ent : doc.GetArray() ){

        {//--- goSpecName ---//
            const auto &a = check_and_get_value( ent, "goSpecName", JsonValType::String );
            goSpecName = a.GetString();
            goSpecId =  ssrc::str_2_goSpecId( goSpecName );
        }

        {//--- style ---//
            const auto &a = check_and_get_value( ent, "style", JsonValType::String );
            style = a.GetString();
            tprAssert( style == "SameGoSpec" );
                // -1-: SameGoSpec
                // -2-: IndependentGoSpec -- not implement yet 
                //      ...
        }

        Json_MultiGoMesh_Tmp  json_MultiGoMesh_Tmp {};
            
        //--- goMeshSets ---//
        const auto &goMeshSets = check_and_get_value( ent, "goMeshSets", JsonValType::Array );
        for( auto &goMeshSet : goMeshSets.GetArray() ){
            
            Json_GoMeshSet_Tmp json_GoMeshSet_Tmp {};

            {//--- type ---//
                const auto &a = check_and_get_value( goMeshSet, "type", JsonValType::String );
                json_GoMeshSet_Tmp.type = a.GetString();
            }

            //--- goMeshs ---//
            const auto &goMeshs = check_and_get_value( goMeshSet, "goMeshs", JsonValType::Array );
            for( auto &goMesh : goMeshs.GetArray() ){ // each goMesh ent

                Json_GoMesh_Tmp json_GoMesh_Tmp {};

                {//--- dpos ---//
                    const auto &a = check_and_get_value( goMesh, "dpos", JsonValType::Array );
                    // base dpos is IntVec2, align to pixes
                    tprAssert( a.Size() == 2 );
                    tprAssert( a[0].IsInt() );
                    tprAssert( a[1].IsInt() );
                    json_GoMesh_Tmp.dposOff.x = static_cast<double>(a[0].GetInt());
                    json_GoMesh_Tmp.dposOff.y = static_cast<double>(a[1].GetInt());
                }

                {//--- animLabels ---//
                    const auto &a = check_and_get_value( goMesh, "animLabels", JsonValType::Array );
                    if( a.Size() > 0 ){
                        for( auto &ent : a.GetArray() ){//- foreach AnimLabel
                            tprAssert( ent.IsString() );
                            json_GoMesh_Tmp.animLabels.push_back( str_2_AnimLabel(ent.GetString()) );
                        }
                    }
                }
                json_GoMeshSet_Tmp.gomeshs.push_back( json_GoMesh_Tmp ); // copy
            }
            json_MultiGoMesh_Tmp.gomeshSets.push_back( json_GoMeshSet_Tmp ); // copy
        }

        
        //--- plans ---//
        std::string     rootAnimFrameSetName {};
        std::unordered_map<std::string, std::string> typesMap {};

        const auto &plans = check_and_get_value( ent, "plans", JsonValType::Array );
        for( auto &plan : plans.GetArray() ){ // each plan

            {//--- rootAnimFrameSetName ---//
                const auto &a = check_and_get_value( plan, "rootAnimFrameSetName", JsonValType::String );
                rootAnimFrameSetName = a.GetString();
            }

            //--- types ---//
            typesMap.clear();
            const auto &types = check_and_get_value( plan, "types", JsonValType::Object );
            for( auto &typeEnt : types.GetObject() ){ // each plan
                typesMap.insert({ typeEnt.name.GetString(), typeEnt.value.GetString() });
            }


            //============
            auto &goSpecRef = ssrc::getnc_goSpecRef( goSpecId );
            if( goSpecRef.multiGoMeshUPtr == nullptr ){
                goSpecRef.multiGoMeshUPtr = std::make_unique<json::Json_MultiGoMesh>();
            }

            for( const auto &json_GoMeshSet_TmpRef : json_MultiGoMesh_Tmp.gomeshSets ){

                tprAssert( typesMap.find(json_GoMeshSet_TmpRef.type) != typesMap.end() );
                std::string realTypeVal = typesMap.at(json_GoMeshSet_TmpRef.type);

                auto &json_goMeshSet = goSpecRef.multiGoMeshUPtr->create_new_json_goMeshSet( 
                            str_2_multiGoMeshType( realTypeVal ));

            
                for( const auto &json_GoMesh_TmpRef : json_GoMeshSet_TmpRef.gomeshs ){
                    
                    Json_GoMesh json_goMesh {};
                    //---
                    json_goMesh.animFrameSetName = rootAnimFrameSetName;
                    json_goMesh.dposOff = json_GoMesh_TmpRef.dposOff;
                    for( const auto &animLabelEnt : json_GoMesh_TmpRef.animLabels ){
                        json_goMesh.animLabels.push_back( animLabelEnt ); // copy
                    }
                    //---
                    json_goMeshSet.gomeshs.push_back( json_goMesh );
                }
            }
        }

    }
}








}//------------- namespace: jmgm_inn end --------------//
}//------------- namespace json: end ----------------


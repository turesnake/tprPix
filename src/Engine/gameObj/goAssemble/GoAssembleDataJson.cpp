/*
 * ==================== GoAssembleDataJson.cpp =====================
 *                          -- tpr --
 *                                        CREATE -- 2019.10.10
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#include "GoAssembleData.h"

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

#include "GoSpecFromJson.h"

#include "json_oth.h"

#include "esrc_state.h"

//--------------- Script ------------------//
#include "Script/json/json_all.h"


using namespace rapidjson;

#include <iostream>
using std::cout;
using std::endl;

namespace json {//-------- namespace: json --------------//
namespace mgmj_inn {//-------- namespace: mgmj_inn --------------//


    class Json_GoMeshEnt{
    public:
        std::string             goMeshName {};
        glm::dvec2              dposOff {}; // gomesh-dposoff based on go-dpos
        double                  zOff    {};
        AnimActionEName         animActionEName {};
        //---
        bool                    is_animLabel {true}; // animLabel OR animLabel_hold
        AnimLabel               animLabel {};
        std::string             animLabel_hold {};
        //---
        bool                    is_afsName {true}; // afsName or afsName_hold
        std::string             afsName {};
        std::string             afsName_hold {};
    };


    class Json_GoMeshSet{
    public:
        Json_GoMeshSet()=default;
        //---
        std::string type {};

        GoAltiRangeLabel goAltiRangeLabel {};

        std::vector<Json_GoMeshEnt> gomeshs {};

    };


    class Json_GoAssembleData{
    public:
        Json_GoAssembleData()=default;
        //---
        std::vector<Json_GoMeshSet> gomeshSets {};
    };


}//------------- namespace: mgmj_inn end --------------//


void parse_single_goAssembleDataJsonFile( const std::string &path_file_ ){
    //-----------------------------//
    //         load file
    //-----------------------------//
    auto jsonBufUPtr = read_a_file( path_file_ );

    //-----------------------------//
    //      parce JSON data
    //-----------------------------//
    Document doc;
    doc.Parse( jsonBufUPtr->c_str() );

    goSpeciesId_t      goSpeciesId {};

    tprAssert( doc.IsArray() );
    for( auto &ent : doc.GetArray() ){

        {//--- goSpeciesName ---//
            const auto &a = json::check_and_get_value( ent, "goSpeciesName", json::JsonValType::String );
            goSpeciesId =  GoSpecFromJson::str_2_goSpeciesId( a.GetString() );
        }

        mgmj_inn::Json_GoAssembleData  json_MultiGoMesh {};
            
        //--- goMeshSets ---//
        const auto &goMeshSets = json::check_and_get_value( ent, "goMeshSets", json::JsonValType::Array );
        for( auto &goMeshSet : goMeshSets.GetArray() ){
            
            mgmj_inn::Json_GoMeshSet json_GoMeshSet {};

            //--- skip annotation ---
            if( goMeshSet.HasMember("annotation") ){
                continue;
            }

            {//--- type ---//
                const auto &a = json::check_and_get_value( goMeshSet, "type", json::JsonValType::String );
                json_GoMeshSet.type = a.GetString();
            }

            //--- goAltiRangeLabel ---//
            if( goMeshSet.HasMember("goAltiRangeLabel") ){
                const auto &a = json::check_and_get_value( goMeshSet, "goAltiRangeLabel", json::JsonValType::String );
                json_GoMeshSet.goAltiRangeLabel = str_2_goAltiRangeLabel( a.GetString() );
            }else{
                json_GoMeshSet.goAltiRangeLabel = GoAltiRangeLabel::Default; // DEFAULT
            }

            //--- goMeshs ---//
            bool isHave_goMeshName {false}; // 运行用户手动标明 “goMeshName”， 或者什么也不写，依靠自动生成
            bool isHave_animAction {false};
            size_t goMeshIdx {0}; // 计数器
            const auto &goMeshs = json::check_and_get_value( goMeshSet, "goMeshs", json::JsonValType::Array );
            for( auto &goMesh : goMeshs.GetArray() ){ // each goMesh ent

                mgmj_inn::Json_GoMeshEnt json_GoMeshEnt {};

                if( goMeshIdx == 0 ){
                    isHave_goMeshName = goMesh.HasMember("goMeshName");
                    isHave_animAction = goMesh.HasMember("animAction");
                }

                //--- goMeshName ---//
                if( isHave_goMeshName ){
                    const auto &a = json::check_and_get_value( goMesh, "goMeshName", json::JsonValType::String );
                    std::string readName = a.GetString();
                    // 第一个元素必须命名为 "root"，后面的元素，必须不能为 "root"
                    if( goMeshIdx == 0 ){
                        tprAssert( readName == "root" );
                    }else{
                        tprAssert( readName != "root" );
                    }
                    json_GoMeshEnt.goMeshName = readName;
                }else{
                    // DEFAULT
                    if( goMeshIdx == 0 ){
                        json_GoMeshEnt.goMeshName = "root";
                    }else{
                        json_GoMeshEnt.goMeshName = tprGeneral::nameString_combine("m_", goMeshIdx, ""); // "m_1", "m_2"
                    }
                }

                //--- dpos ---//
                if( goMesh.HasMember("dpos") ){
                    const auto &a = json::check_and_get_value( goMesh, "dpos", json::JsonValType::Array );
                    // base dpos is IntVec2, align to pixes
                    tprAssert( a.Size() == 2 );
                    tprAssert( a[0].IsInt() );
                    tprAssert( a[1].IsInt() );
                    json_GoMeshEnt.dposOff.x = static_cast<double>(a[0].GetInt());
                    json_GoMeshEnt.dposOff.y = static_cast<double>(a[1].GetInt());
                }else{
                    json_GoMeshEnt.dposOff = glm::dvec2{ 0.0, 0.0 }; // DEFAULT
                }

                //--- zOff ---//
                if( goMesh.HasMember("zOff") ){
                    const auto &a = json::check_and_get_value( goMesh, "zOff", json::JsonValType::Double );
                    json_GoMeshEnt.zOff = a.GetDouble();
                }else{
                    json_GoMeshEnt.zOff = 0.0; // DEFAULT
                }


                //--- afsName OR afsType ---//
                if( goMesh.HasMember("afsName") ){
                    const auto &a = json::check_and_get_value( goMesh, "afsName", json::JsonValType::String );
                    json_GoMeshEnt.afsName = a.GetString();
                    json_GoMeshEnt.is_afsName = true;

                }else{
                    tprAssert( goMesh.HasMember("afsName.hold") );
                    const auto &a = json::check_and_get_value( goMesh, "afsName.hold", json::JsonValType::String );
                    json_GoMeshEnt.afsName_hold = a.GetString();
                    json_GoMeshEnt.is_afsName = false;
                }


                if( goMesh.HasMember("animLabel") ){
                    const auto &a = json::check_and_get_value( goMesh, "animLabel", json::JsonValType::String );
                    json_GoMeshEnt.animLabel = str_2_animLabel(a.GetString() );
                    json_GoMeshEnt.is_animLabel = true;
                }else{
                    tprAssert( goMesh.HasMember("animLabel.hold") );
                    const auto &a = json::check_and_get_value( goMesh, "animLabel.hold", json::JsonValType::String );
                    json_GoMeshEnt.animLabel_hold = a.GetString();
                    json_GoMeshEnt.is_animLabel = false;
                }

                //--- animAction ---//
                if( isHave_animAction ){
                    const auto &a = json::check_and_get_value( goMesh, "animAction", json::JsonValType::String );
                    json_GoMeshEnt.animActionEName = str_2_animActionEName( a.GetString() );
                }else{
                    json_GoMeshEnt.animActionEName = AnimActionEName::Idle; // DEFAULT
                }

                //===
                json_GoMeshSet.gomeshs.push_back( json_GoMeshEnt ); // copy
                goMeshIdx++;
            }
            json_MultiGoMesh.gomeshSets.push_back( json_GoMeshSet ); // copy
        }

        //=======================//
        //         plans
        //=======================//
        std::unordered_map<std::string, std::string> afsName_holds {};
        std::unordered_map<std::string, std::string> animLabel_holds {};

        std::unordered_map<std::string, std::string> typesMap {};
        

        const auto &plans = json::check_and_get_value( ent, "plans", json::JsonValType::Array );
        for( auto &plan : plans.GetArray() ){ // each plan

            //--- afsName_holds ---//
            // 允许这组数据不存在
            afsName_holds.clear();
            if( plan.HasMember("afsName.holds") ){
                const auto &types = json::check_and_get_value( plan, "afsName.holds", json::JsonValType::Object );
                for( auto &typeEnt : types.GetObject() ){
                    tprAssert( typeEnt.value.IsString() );
                    afsName_holds.insert({ typeEnt.name.GetString(), typeEnt.value.GetString() });
                }
            }

            //--- animLabel.holds ---//
            animLabel_holds.clear();
            if( plan.HasMember("animLabel.holds") ){
                const auto &types = json::check_and_get_value( plan, "animLabel.holds", json::JsonValType::Object );
                for( auto &typeEnt : types.GetObject() ){
                    tprAssert( typeEnt.value.IsString() );
                    animLabel_holds.insert({ typeEnt.name.GetString(), typeEnt.value.GetString() });
                }
            }

            {//--- types ---//
                typesMap.clear();
                const auto &types = json::check_and_get_value( plan, "types", json::JsonValType::Object );
                for( auto &typeEnt : types.GetObject() ){ // each plan
                    tprAssert( typeEnt.value.IsString() );
                    typesMap.insert({ typeEnt.name.GetString(), typeEnt.value.GetString() });
                }
            }

            //============
            auto &goSpecFromJsonRef = GoSpecFromJson::getnc_goSpecFromJsonRef( goSpeciesId );
            if( goSpecFromJsonRef.goAssembleDataUPtr == nullptr ){
                goSpecFromJsonRef.goAssembleDataUPtr = std::make_unique<GoAssembleData>();
            }

            for( const auto &json_GoMeshSetRef : json_MultiGoMesh.gomeshSets ){

                tprAssert( typesMap.find(json_GoMeshSetRef.type) != typesMap.end() );
                std::string realTypeVal = typesMap.at(json_GoMeshSetRef.type);

                GoAssembleData::insert_2_goLabelIds( realTypeVal );
                GoMeshSet &goMeshSet = goSpecFromJsonRef.goAssembleDataUPtr->create_new_goMeshSet( 
                                            GoAssembleData::str_2_goLabelId( realTypeVal ));

                goMeshSet.goAltiRangeLabel = json_GoMeshSetRef.goAltiRangeLabel;
            
                for( const auto &json_GoMeshEntRef : json_GoMeshSetRef.gomeshs ){
                    
                    GoMeshEnt goMeshEnt {};
                    //---
                    
                    // afsName
                    if( json_GoMeshEntRef.is_afsName ){
                        tprAssert( json_GoMeshEntRef.afsName != "" );
                        goMeshEnt.animFrameSetName = json_GoMeshEntRef.afsName;
                    }else{
                        tprAssert( afsName_holds.find(json_GoMeshEntRef.afsName_hold) != afsName_holds.end() );
                        goMeshEnt.animFrameSetName = afsName_holds.at(json_GoMeshEntRef.afsName_hold);
                    }

                    // animLabel
                    if( json_GoMeshEntRef.is_animLabel ){
                        goMeshEnt.animLabel = json_GoMeshEntRef.animLabel;
                    }else{
                        tprAssert( animLabel_holds.find(json_GoMeshEntRef.animLabel_hold) != animLabel_holds.end() );
                        goMeshEnt.animLabel = str_2_animLabel( animLabel_holds.at(json_GoMeshEntRef.animLabel_hold) );
                    }

                    goMeshEnt.goMeshName = json_GoMeshEntRef.goMeshName;
                    goMeshEnt.dposOff = json_GoMeshEntRef.dposOff;
                    goMeshEnt.zOff = json_GoMeshEntRef.zOff;
                    goMeshEnt.animActionEName = json_GoMeshEntRef.animActionEName;
                    //---
                    goMeshSet.gomeshs.push_back( goMeshEnt );
                }
            }
        }

    }
}


}//------------- namespace: json end --------------//


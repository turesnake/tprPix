/*
 * ==================== GoAssemblePlanJson.cpp =====================
 *                          -- tpr --
 *                                        CREATE -- 2019.10.10
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#include "GoAssemblePlan.h"

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
#include "RenderLayerType.h"
#include "ShaderType.h"
#include "FloorGoType.h"

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
        //---

        RenderLayerType         renderLayerType {};
        ShaderType              shaderType   {};
        bool                    isVisible {};

        //------- optional_vals -------//
        std::optional<FloorGoLayer> floorGoLayer { std::nullopt }; // only for FloorGo 

    };


    class Json_Plan{
    public:
        Json_Plan()=default;
        //---
        bool        is_goLabel {true};
        std::string goLabel      {};
        std::string goLabel_hold {};

        GoAltiRangeLabel goAltiRangeLabel {};

        std::vector<Json_GoMeshEnt> gomeshs {};

    };


    class Json_GoAssemblePlanSet{
    public:
        Json_GoAssemblePlanSet()=default;
        //---
        std::vector<Json_Plan> plans {};
    };


}//------------- namespace: mgmj_inn end --------------//


void parse_single_goAssemblePlanJsonFile( const std::string &path_file_ ){
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
    for( auto &docEnt : doc.GetArray() ){

        goSpeciesId_t       goSpeciesId {};

        RenderLayerType     go_renderLayerType {};
        ShaderType          go_shaderType   {};


        std::optional<FloorGoLayer> go_floorGoLayer { std::nullopt }; // only for FloorGo 


        {//--- goSpeciesName ---//
            const auto &a = json::check_and_get_value( docEnt, "goSpeciesName", json::JsonValType::String );
            goSpeciesId =  GoSpecFromJson::str_2_goSpeciesId( a.GetString() );
        }

        mgmj_inn::Json_GoAssemblePlanSet  json_goAssemblePlanSet {};


        //--- without_holds ---//
        // 当 “plans”:[] 数据中完全不含有 "hold" 类数据时，
        // json 文件将变成最简形态，直接读取 "plans" 中数据，然后导出最终数据
        // 否则，就要配合 "holds":[] 中的数据，合成出最终的数据
        bool without_holds = false;
        if( docEnt.HasMember("without_holds") ){
            const auto &wh = json::check_and_get_value( docEnt, "without_holds", json::JsonValType::Bool );
            if( wh.GetBool() == true ){
                without_holds = true;
            }
        }

        //--- renderLayerType ---//
        // 可以分别在 go层，"plan" "goMesh" 三层定义 此值，较小作用域的值 会屏蔽 较大作用域的值
        // 其中，go层 为强制要求 显示定义。剩余二层可省略
        {
            const auto &a = json::check_and_get_value( docEnt, "renderLayerType", json::JsonValType::String );
            go_renderLayerType = str_2_renderLayerType( a.GetString() );
        }

        //--- shaderType ---//
        // 可以分别在 go层，"plan" "goMesh" 三层定义 此值，较小作用域的值 会屏蔽 较大作用域的值
        // 其中，go层 为强制要求 显示定义。剩余二层可省略
        {
            const auto &a = json::check_and_get_value( docEnt, "shaderType", json::JsonValType::String );
            go_shaderType = str_2_shaderType( a.GetString() );
        }


        //--- opt.floorGoLayer ---//
        // 暂时仅支持在 顶层定义
        if( docEnt.HasMember("opt.floorGoLayer") ){
            const auto &a = json::check_and_get_value( docEnt, "opt.floorGoLayer", json::JsonValType::String );
            go_floorGoLayer = { str_2_floorGoLayer(a.GetString()) };
        }


        //--- plans ---//
        const auto &plansArray = json::check_and_get_value( docEnt, "plans", json::JsonValType::Array );
        for( auto &pEnt : plansArray.GetArray() ){

            RenderLayerType     plan_renderLayerType {};
            ShaderType          plan_shaderType   {};
            bool                isHave_plan_renderLayerType {false};
            bool                isHave_plan_shaderType  {false};

            
            mgmj_inn::Json_Plan json_plan {};

            //--- skip annotation ---
            if( pEnt.HasMember("annotation") ||
                pEnt.HasMember("Notes") ){
                continue;
            }

            //--- goLabel / goLabel.hold ---//
            if( !without_holds ){
                // MUST have "goLabel.hold"
                const auto &a = json::check_and_get_value( pEnt, "goLabel.hold", json::JsonValType::String );
                json_plan.goLabel_hold = a.GetString();
                json_plan.is_goLabel = false;
            }else{
                // MUST have "goLabel"
                const auto &a = json::check_and_get_value( pEnt, "goLabel", json::JsonValType::String );
                json_plan.goLabel = a.GetString();
                json_plan.is_goLabel = true;
            }


            //--- goAltiRangeLabel ---//
            if( pEnt.HasMember("goAltiRangeLabel") ){
                const auto &a = json::check_and_get_value( pEnt, "goAltiRangeLabel", json::JsonValType::String );
                json_plan.goAltiRangeLabel = str_2_goAltiRangeLabel( a.GetString() );
            }else{
                json_plan.goAltiRangeLabel = GoAltiRangeLabel::Default; // DEFAULT
            }


            //--- renderLayerType ---//
            if( pEnt.HasMember("renderLayerType") ){
                isHave_plan_renderLayerType = true;
                const auto &a = json::check_and_get_value( pEnt, "renderLayerType", json::JsonValType::String );
                plan_renderLayerType = str_2_renderLayerType( a.GetString() );
            }

            //--- shaderType ---//
            if( pEnt.HasMember("shaderType") ){
                isHave_plan_shaderType = true;
                const auto &a = json::check_and_get_value( pEnt, "shaderType", json::JsonValType::String );
                plan_shaderType = str_2_shaderType( a.GetString() );
            }

            //--- goMeshs ---//
            size_t goMeshIdx {0}; // 计数器
            const auto &goMeshs = json::check_and_get_value( pEnt, "goMeshs", json::JsonValType::Array );
            for( auto &goMesh : goMeshs.GetArray() ){ // each goMesh ent

                mgmj_inn::Json_GoMeshEnt json_GoMeshEnt {};

                //--- goMeshName ---//
                if( goMesh.HasMember("goMeshName") ){
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


                //--- afsName / afsName.hold ---//
                if( !without_holds && goMesh.HasMember("afsName.hold") ){
                    const auto &a = json::check_and_get_value( goMesh, "afsName.hold", json::JsonValType::String );
                    json_GoMeshEnt.afsName_hold = a.GetString();
                    json_GoMeshEnt.is_afsName = false;
                }else{
                    tprAssert( goMesh.HasMember("afsName") );
                    const auto &a = json::check_and_get_value( goMesh, "afsName", json::JsonValType::String );
                    json_GoMeshEnt.afsName = a.GetString();
                    json_GoMeshEnt.is_afsName = true;
                }


                //--- animLabel / animLabel.hold ---//
                if( !without_holds && goMesh.HasMember("animLabel.hold") ){
                    const auto &a = json::check_and_get_value( goMesh, "animLabel.hold", json::JsonValType::String );
                    json_GoMeshEnt.animLabel_hold = a.GetString();
                    json_GoMeshEnt.is_animLabel = false;
                }else{
                    tprAssert( goMesh.HasMember("animLabel") );
                    const auto &a = json::check_and_get_value( goMesh, "animLabel", json::JsonValType::String );
                    json_GoMeshEnt.animLabel = str_2_animLabel(a.GetString() );
                    json_GoMeshEnt.is_animLabel = true;
                }


                //--- animAction ---//
                if( goMesh.HasMember("animAction") ){
                    const auto &a = json::check_and_get_value( goMesh, "animAction", json::JsonValType::String );
                    json_GoMeshEnt.animActionEName = str_2_animActionEName( a.GetString() );
                }else{
                    json_GoMeshEnt.animActionEName = AnimActionEName::Idle; // DEFAULT
                }


                //--- renderLayerType ---//
                // 层层屏蔽
                if( goMesh.HasMember("renderLayerType") ){
                    const auto &a = json::check_and_get_value( goMesh, "renderLayerType", json::JsonValType::String );
                    json_GoMeshEnt.renderLayerType = str_2_renderLayerType( a.GetString() );
                }else{
                    ( isHave_plan_renderLayerType ) ?
                        json_GoMeshEnt.renderLayerType = plan_renderLayerType :
                        json_GoMeshEnt.renderLayerType = go_renderLayerType;
                }

                //--- shaderType ---//
                // 层层屏蔽
                if( goMesh.HasMember("shaderType") ){
                    const auto &a = json::check_and_get_value( goMesh, "shaderType", json::JsonValType::String );
                    json_GoMeshEnt.shaderType = str_2_shaderType( a.GetString() );
                }else{
                    ( isHave_plan_shaderType ) ?
                        json_GoMeshEnt.shaderType = plan_shaderType :
                        json_GoMeshEnt.shaderType = go_shaderType;
                }


                //--- isVisible ---//
                if( goMesh.HasMember("isVisible") ){
                    const auto &a = json::check_and_get_value( goMesh, "isVisible", json::JsonValType::Bool );
                    json_GoMeshEnt.isVisible = a.GetBool();
                }else{
                    json_GoMeshEnt.isVisible = true;
                }

                //--- opt.floorGoLayer ---//
                json_GoMeshEnt.floorGoLayer = go_floorGoLayer; // maybe has val

                //===
                json_plan.gomeshs.push_back( json_GoMeshEnt ); // copy
                goMeshIdx++;
            }
            json_goAssemblePlanSet.plans.push_back( json_plan ); // copy
        }



        //=======================//
        //    without_holds
        //=======================//
        // 不在读取 "holds" 中的数据，直接将 “plans” 中的每一份数据，制作为最终数据
        if( without_holds ){

            auto &goSpecFromJsonRef = GoSpecFromJson::getnc_goSpecFromJsonRef( goSpeciesId );
            if( goSpecFromJsonRef.goAssemblePlanSetUPtr == nullptr ){
                goSpecFromJsonRef.goAssemblePlanSetUPtr = std::make_unique<GoAssemblePlanSet>();
            }

            for( const auto &json_planRef : json_goAssemblePlanSet.plans ){

                // goLabel
                tprAssert( json_planRef.is_goLabel );
                GoAssemblePlanSet::insert_2_goLabelIds( json_planRef.goLabel );
                GoAssemblePlanSet::Plan &planRef = goSpecFromJsonRef.goAssemblePlanSetUPtr->create_new_plan( 
                                            GoAssemblePlanSet::str_2_goLabelId( json_planRef.goLabel ));

                planRef.goAltiRangeLabel = json_planRef.goAltiRangeLabel;
            
                for( const auto &json_GoMeshEntRef : json_planRef.gomeshs ){
                    
                    GoAssemblePlanSet::GoMeshEnt goMeshEnt {};
                    //---
                    
                    // afsName
                    tprAssert( json_GoMeshEntRef.is_afsName );
                    tprAssert( json_GoMeshEntRef.afsName != "" );
                    goMeshEnt.animFrameSetName = json_GoMeshEntRef.afsName;

                    // animLabel
                    tprAssert( json_GoMeshEntRef.is_animLabel );
                    goMeshEnt.animLabel = json_GoMeshEntRef.animLabel;

                    goMeshEnt.goMeshName = json_GoMeshEntRef.goMeshName;
                    goMeshEnt.dposOff = json_GoMeshEntRef.dposOff;
                    goMeshEnt.zOff = json_GoMeshEntRef.zOff;
                    goMeshEnt.animActionEName = json_GoMeshEntRef.animActionEName;

                    goMeshEnt.renderLayerType = json_GoMeshEntRef.renderLayerType;
                    goMeshEnt.shaderType = json_GoMeshEntRef.shaderType;
                    goMeshEnt.isVisible = json_GoMeshEntRef.isVisible;

                    // maybe has val
                    goMeshEnt.floorGoLayer = json_GoMeshEntRef.floorGoLayer;

                    //---
                    planRef.gomeshs.push_back( goMeshEnt );
                }
            }

            return;
        }


        //=======================//
        //         plans
        //=======================//
        std::unordered_map<std::string, std::string> afsName_holds {};
        std::unordered_map<std::string, std::string> animLabel_holds {};
        std::unordered_map<std::string, std::string> goLabel_holds {};

        // 遍历 "holds" 中每一份数据，将它们与每一份 “plans” 中的数据整合 为最终数据
        const auto &holdsArray = json::check_and_get_value( docEnt, "holds", json::JsonValType::Array );
        for( auto &hEnt : holdsArray.GetArray() ){ // each plan

            //--- afsName_holds ---//
            // 允许这组数据不存在
            afsName_holds.clear();
            if( hEnt.HasMember("afsName.holds") ){
                const auto &types = json::check_and_get_value( hEnt, "afsName.holds", json::JsonValType::Object );
                for( auto &typeEnt : types.GetObject() ){
                    tprAssert( typeEnt.value.IsString() );
                    afsName_holds.insert({ typeEnt.name.GetString(), typeEnt.value.GetString() });
                }
            }

            //--- animLabel.holds ---//
            animLabel_holds.clear();
            if( hEnt.HasMember("animLabel.holds") ){
                const auto &types = json::check_and_get_value( hEnt, "animLabel.holds", json::JsonValType::Object );
                for( auto &typeEnt : types.GetObject() ){
                    tprAssert( typeEnt.value.IsString() );
                    animLabel_holds.insert({ typeEnt.name.GetString(), typeEnt.value.GetString() });
                }
            }

            {//--- goLabel.holds ---//
                goLabel_holds.clear();
                const auto &ghArray = json::check_and_get_value( hEnt, "goLabel.holds", json::JsonValType::Object );
                for( auto &ghEnt : ghArray.GetObject() ){ // each plan
                    tprAssert( ghEnt.value.IsString() );
                    goLabel_holds.insert({ ghEnt.name.GetString(), ghEnt.value.GetString() });
                }
            }

            //============
            auto &goSpecFromJsonRef = GoSpecFromJson::getnc_goSpecFromJsonRef( goSpeciesId );
            if( goSpecFromJsonRef.goAssemblePlanSetUPtr == nullptr ){
                goSpecFromJsonRef.goAssemblePlanSetUPtr = std::make_unique<GoAssemblePlanSet>();
            }

            for( const auto &json_planRef : json_goAssemblePlanSet.plans ){

                tprAssert( goLabel_holds.find(json_planRef.goLabel_hold) != goLabel_holds.end() );
                std::string goLabel = goLabel_holds.at(json_planRef.goLabel_hold);

                GoAssemblePlanSet::insert_2_goLabelIds( goLabel );
                GoAssemblePlanSet::Plan &planRef = goSpecFromJsonRef.goAssemblePlanSetUPtr->create_new_plan( 
                                            GoAssemblePlanSet::str_2_goLabelId( goLabel ));

                planRef.goAltiRangeLabel = json_planRef.goAltiRangeLabel;
            
                for( const auto &json_GoMeshEntRef : json_planRef.gomeshs ){
                    
                    GoAssemblePlanSet::GoMeshEnt goMeshEnt {};
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

                    goMeshEnt.renderLayerType = json_GoMeshEntRef.renderLayerType;
                    goMeshEnt.shaderType = json_GoMeshEntRef.shaderType;
                    goMeshEnt.isVisible = json_GoMeshEntRef.isVisible;

                    // maybe has val
                    goMeshEnt.floorGoLayer = json_GoMeshEntRef.floorGoLayer;
                    //---
                    planRef.gomeshs.push_back( goMeshEnt );
                }
            }
        }
    }
}


}//------------- namespace: json end --------------//


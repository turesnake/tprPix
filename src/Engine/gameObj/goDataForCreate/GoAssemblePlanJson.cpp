/*
 * ==================== GoAssemblePlanJson.cpp =====================
 *                          -- tpr --
 *                                        CREATE -- 2019.10.10
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#include "pch.h"
#include "GoAssemblePlan.h"

//--------------- CPP ------------------//
#include <utility>

//--------------- Libs ------------------//
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

#include "tprGeneral.h"

//------------------- Engine --------------------//
#include "global.h"
#include "fileIO.h"
#include "RenderLayerType.h"
#include "ShaderType.h"
#include "FloorGoType.h"
#include "GoSpecFromJson.h"
#include "json_oth.h"
#include "SignInMapEnts_Square_Type.h"
#include "ColliderType.h"

#include "calc_colliPoints.h"

#include "esrc_state.h"


//--------------- Script ------------------//
#include "Script/json/json_all.h"


using namespace rapidjson;



namespace json {//-------- namespace: json --------------//
namespace mgmj_inn {//-------- namespace: mgmj_inn --------------//


    // asm.json 文件中，允许用 "xx.hold" 来表达一种 暂用符
    // 延迟到未来再确定具体的值
    // 使用本容器来管理这些 hold变量
    template< typename T>
    class Hold{
    public:
        Hold()=default;

        inline void init_by_holdStr( const std::string holdStr_ )noexcept{
            this->isInit = true;
            this->isHold = true;
            this->holdStr = holdStr_;
        }

        inline void init_by_value( const T &value_ )noexcept{
            this->isInit = true;
            this->isHold = false;
            this->value = value_;
        }

        inline bool is_hold()const noexcept{ return this->isHold; }

        inline std::string get_holdStr()const noexcept{
            tprAssert( this->isInit );
            tprAssert( this->isHold );
            return this->holdStr;
        }

        inline T get_value()const noexcept{
            tprAssert( this->isInit );
            tprAssert( !this->isHold );
            return this->value;
        }

        inline void replace_2_value( const T &value_ )noexcept{
            tprAssert( this->isInit );
            tprAssert( this->isHold );
            this->isHold = false;
            this->holdStr = "";
            this->value = value_;
        }

    private:
        bool        isInit  {false};
        bool        isHold  {};
        std::string holdStr {};
        T           value   {};
    };




    
    class Json_Collide{
    public:
        Json_Collide()=default;
        ColliderType collideType {};
        SignInMapEnts_Square_Type signInMapEnts_square_type {};
        double moveColliRadius  { 0.0 };
        double skillColliRadius { 0.0 };
    };
    std::shared_ptr<Json_Collide> parse_collide( const Value &jsonObj_ );
    void assemble_colliDataFromJson( GoAssemblePlanSet::Plan &planRef, const Json_Collide &jCollide_ );




    class Json_GoMeshEnt{
    public:
        std::string             goMeshName {};
        glm::dvec2              dposOff {}; // gomesh-dposoff based on go-dpos
        double                  zOff    {};
        AnimActionEName         animActionEName {};

        Hold<std::string>       animLabel {}; // animLabel OR animLabel_hold
        Hold<std::string>       afsName {}; // afsName or afsName_hold


        RenderLayerType         renderLayerType {};
        ShaderType              shaderType   {};
        bool                    isVisible {};
        bool                    isAutoInit {};

        NineDirection           default_dir {};
        BrokenLvl               default_brokenLvl {};
                                        // dir / broken 数据，存在 3 层设置
                                        // -1- 未在 asm.json 中显式声明，直接使用默认值
                                        // -2- 在 asm.json 中显式声明，以上两种记录的都是 default 值
                                        // -3- 在 蓝图 / byHand 中显式设置 实际值，将会覆盖 default 值
                                        //     每个后一层都会覆盖前一层

        //------- optional_vals -------//
        std::optional<FloorGoLayer> floorGoLayer  { std::nullopt }; // only for FloorGo 
        
    };


    class Json_Plan{
    public:
        Json_Plan()=default;
        //---
        
        Hold<std::string>                       goLabel {};   
        Hold< std::shared_ptr<Json_Collide> >   json_collideSPtr {};
        GoAltiRangeLabel                        goAltiRangeLabel {};


        std::vector<Json_GoMeshEnt>             gomeshs {};

        //------- optional_vals -------//
        std::optional<SignInMapEnts_Square_Type> signInMapEnts_square_type  { std::nullopt }; // only for SquGo
    };
    void aseembel_jsonPlan_2_plan(  GoAssemblePlanSet &goAssemblePlanSetRef_,
                                    const Json_Plan &json_planRef_ );
    


    class Json_GoAssemblePlanSet{
    public:
        Json_GoAssemblePlanSet()=default;
        //---
        std::vector<std::unique_ptr<Json_Plan>> planUPtrs {};
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

            auto json_planUPtr = std::make_unique<mgmj_inn::Json_Plan>();

            //--- skip annotation ---
            /*
            if( pEnt.HasMember("annotation") ||
                pEnt.HasMember("Notes") ){
                continue;
            }
            */

            //--- goLabel / goLabel.hold ---//
            if( pEnt.HasMember("goLabel") ){
                const auto &a = json::check_and_get_value( pEnt, "goLabel", json::JsonValType::String );
                json_planUPtr->goLabel.init_by_value( a.GetString() );
            }else{
                const auto &a = json::check_and_get_value( pEnt, "goLabel.hold", json::JsonValType::String );
                json_planUPtr->goLabel.init_by_holdStr( a.GetString() );
            }


            //--- collide / collide.hold ---//
            if( pEnt.HasMember("collide") ){
                const auto &a = json::check_and_get_value( pEnt, "collide", json::JsonValType::Object );
                json_planUPtr->json_collideSPtr.init_by_value( mgmj_inn::parse_collide(a) );
            }else{
                const auto &a = json::check_and_get_value( pEnt, "collide.hold", json::JsonValType::String );
                json_planUPtr->json_collideSPtr.init_by_holdStr( a.GetString() );
            }


            //--- goAltiRangeLabel ---//
            if( pEnt.HasMember("goAltiRangeLabel") ){
                const auto &a = json::check_and_get_value( pEnt, "goAltiRangeLabel", json::JsonValType::String );
                json_planUPtr->goAltiRangeLabel = str_2_goAltiRangeLabel( a.GetString() );
            }else{
                json_planUPtr->goAltiRangeLabel = GoAltiRangeLabel::Default; // DEFAULT
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

            //--- opt.signInMapEnts_square_type ---//
            // 暂时仅支持在 "plan" 层设置
            if( pEnt.HasMember("opt.signInMapEnts_square_type") ){
                const auto &a = json::check_and_get_value( pEnt, "opt.signInMapEnts_square_type", json::JsonValType::String );
                json_planUPtr->signInMapEnts_square_type = str_2_signInMapEnts_square_type( a.GetString() );
            }

            //--- goMeshs ---//
            size_t goMeshIdx {0}; // 计数器
            const auto &goMeshs = json::check_and_get_value( pEnt, "goMeshs", json::JsonValType::Array );
            for( auto &goMesh : goMeshs.GetArray() ){ // each goMesh ent

                mgmj_inn::Json_GoMeshEnt json_GoMeshEnt {};

                //--- isAutoInit ---//
                if( goMesh.HasMember("isAutoInit") ){
                    const auto &a = json::check_and_get_value( goMesh, "isAutoInit", json::JsonValType::Bool );
                    json_GoMeshEnt.isAutoInit = a.GetBool();
                }else{
                    json_GoMeshEnt.isAutoInit = true; // Default
                }


                //--- goMeshName ---//
                if( goMesh.HasMember("goMeshName") ){
                    const auto &a = json::check_and_get_value( goMesh, "goMeshName", json::JsonValType::String );
                    std::string readName = a.GetString();

                    if( json_GoMeshEnt.isAutoInit ){
                        // 第一个元素必须命名为 "root"，后面的元素，必须不能为 "root"
                        if( goMeshIdx == 0 ){
                            tprAssert( readName == "root" );
                        }else{
                            tprAssert( readName != "root" );
                        }
                    }else{
                        // 当 gomesh 并不需要 auto init 时，不对其 name 做要求
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
                    json_GoMeshEnt.afsName.init_by_holdStr( a.GetString() );
                }else{
                    tprAssert( goMesh.HasMember("afsName") );
                    const auto &a = json::check_and_get_value( goMesh, "afsName", json::JsonValType::String );
                    json_GoMeshEnt.afsName.init_by_value( a.GetString() );
                }


                //--- animLabel / animLabel.hold ---//
                if( !without_holds && goMesh.HasMember("animLabel.hold") ){
                    const auto &a = json::check_and_get_value( goMesh, "animLabel.hold", json::JsonValType::String );
                    json_GoMeshEnt.animLabel.init_by_holdStr( a.GetString() );
                }else{
                    tprAssert( goMesh.HasMember("animLabel") );
                    const auto &a = json::check_and_get_value( goMesh, "animLabel", json::JsonValType::String );
                    json_GoMeshEnt.animLabel.init_by_value( a.GetString()  );
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

                //--- default.dir ---//
                if( goMesh.HasMember("default.dir") ){
                    const auto &a = json::check_and_get_value( goMesh, "default.dir", json::JsonValType::String );
                    json_GoMeshEnt.default_dir = str_2_nineDirection( a.GetString() );
                }else{
                    json_GoMeshEnt.default_dir = NineDirection::Center; // default val
                }

                //--- default.brokenLvl ---//
                if( goMesh.HasMember("default.brokenLvl") ){
                    const auto &a = json::check_and_get_value( goMesh, "default.brokenLvl", json::JsonValType::String );
                    json_GoMeshEnt.default_brokenLvl = str_2_brokenLvl( a.GetString() );
                }else{
                    json_GoMeshEnt.default_brokenLvl = BrokenLvl::Lvl_0; // default val
                }


                //--- opt.floorGoLayer ---//
                json_GoMeshEnt.floorGoLayer = go_floorGoLayer; // maybe has val

                //===
                json_planUPtr->gomeshs.push_back( json_GoMeshEnt ); // copy
                goMeshIdx++;
            }
            json_goAssemblePlanSet.planUPtrs.push_back( std::move(json_planUPtr) );
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

            for( const auto &jPlanUPtr : json_goAssemblePlanSet.planUPtrs ){
                const auto &json_planRef = *jPlanUPtr;
                mgmj_inn::aseembel_jsonPlan_2_plan( *(goSpecFromJsonRef.goAssemblePlanSetUPtr), json_planRef );
            }

            return;
        }


        //=======================//
        //        holds
        //=======================//
        std::unordered_map<std::string, std::string> goLabel_holds {};
        std::unordered_map<std::string, std::string> afsName_holds {};
        std::unordered_map<std::string, std::string> animLabel_holds {};
        std::unordered_map<std::string, std::shared_ptr<mgmj_inn::Json_Collide> > collide_holds {};

        // 遍历 "holds" 中每一份数据，将它们与每一份 “plans” 中的数据整合 为最终数据
        const auto &holdsArray = json::check_and_get_value( docEnt, "holds", json::JsonValType::Array );
        for( auto &hEnt : holdsArray.GetArray() ){ // each plan

            //--- goLabel.holds ---//
            goLabel_holds.clear();
            if( hEnt.HasMember("goLabel.holds") ){
                const auto &ghArray = json::check_and_get_value( hEnt, "goLabel.holds", json::JsonValType::Object );
                for( auto &ghEnt : ghArray.GetObject() ){ // each plan
                    tprAssert( ghEnt.value.IsString() );
                    auto [insertIt, insertBool] = goLabel_holds.emplace( ghEnt.name.GetString(), ghEnt.value.GetString() );
                    tprAssert( insertBool );
                }
            }

            //--- afsName_holds ---//
            afsName_holds.clear();
            if( hEnt.HasMember("afsName.holds") ){
                const auto &types = json::check_and_get_value( hEnt, "afsName.holds", json::JsonValType::Object );
                for( auto &typeEnt : types.GetObject() ){
                    tprAssert( typeEnt.value.IsString() );
                    auto [insertIt, insertBool] = afsName_holds.emplace( typeEnt.name.GetString(), typeEnt.value.GetString() );
                    tprAssert( insertBool );
                }
            }

            //--- animLabel.holds ---//
            animLabel_holds.clear();
            if( hEnt.HasMember("animLabel.holds") ){
                const auto &types = json::check_and_get_value( hEnt, "animLabel.holds", json::JsonValType::Object );
                for( auto &typeEnt : types.GetObject() ){
                    tprAssert( typeEnt.value.IsString() );
                    auto [insertIt, insertBool] = animLabel_holds.emplace( typeEnt.name.GetString(), typeEnt.value.GetString() );
                    tprAssert( insertBool );
                }
            }

            //--- collide.holds ---//
            collide_holds.clear();
            if( hEnt.HasMember("collide.holds") ){
                const auto &types = json::check_and_get_value( hEnt, "collide.holds", json::JsonValType::Object );
                for( auto &cEnt : types.GetObject() ){
                    tprAssert( cEnt.value.IsObject() );
                    auto [insertIt, insertBool] = collide_holds.emplace( cEnt.name.GetString(), mgmj_inn::parse_collide(cEnt.value) );
                    tprAssert( insertBool );
                }
            }

            //============
            auto &goSpecFromJsonRef = GoSpecFromJson::getnc_goSpecFromJsonRef( goSpeciesId );
            if( goSpecFromJsonRef.goAssemblePlanSetUPtr == nullptr ){
                goSpecFromJsonRef.goAssemblePlanSetUPtr = std::make_unique<GoAssemblePlanSet>();
            }

            tprAssert( !json_goAssemblePlanSet.planUPtrs.empty() ); // MUST NOT EMPTY
            for( auto &jPlanUPtr : json_goAssemblePlanSet.planUPtrs ){ // 需要 copy sptr, 不能用 const
                auto &origin_json_planRef = *jPlanUPtr;

                // 复制一份 json plan 实例 
                mgmj_inn::Json_Plan json_plan = origin_json_planRef;

                //-------------------------//
                //  先把所有 .hold 变量 value 化
                //-------------------------//
                // goLabel.holds 
                if( json_plan.goLabel.is_hold() ){
                    std::string holdStr = json_plan.goLabel.get_holdStr();
                    tprAssert( goLabel_holds.find(holdStr) != goLabel_holds.end() );
                    json_plan.goLabel.replace_2_value( goLabel_holds.at(holdStr) );
                }


                // collide.holds 
                if( json_plan.json_collideSPtr.is_hold() ){
                    std::string holdStr = json_plan.json_collideSPtr.get_holdStr();
                    tprAssert( collide_holds.find(holdStr) != collide_holds.end() );
                    json_plan.json_collideSPtr.replace_2_value( collide_holds.at(holdStr) );
                }


                for( auto &json_GoMeshEntRef : json_plan.gomeshs ){

                    // afsName
                    if( json_GoMeshEntRef.afsName.is_hold() ){
                        std::string holdStr = json_GoMeshEntRef.afsName.get_holdStr();
                        tprAssert( afsName_holds.find(holdStr) != afsName_holds.end() );
                        json_GoMeshEntRef.afsName.replace_2_value( afsName_holds.at(holdStr) );
                    }

                    // animLabel
                    if( json_GoMeshEntRef.animLabel.is_hold() ){
                        std::string holdStr = json_GoMeshEntRef.animLabel.get_holdStr();
                        tprAssert( animLabel_holds.find(holdStr) != animLabel_holds.end() );
                        json_GoMeshEntRef.animLabel.replace_2_value( animLabel_holds.at(holdStr) );
                    }
                }

                //-------------------------//
                //    正式配置  plan
                //-------------------------//
                mgmj_inn::aseembel_jsonPlan_2_plan( *(goSpecFromJsonRef.goAssemblePlanSetUPtr), json_plan );
            }
        }
    }
}





namespace mgmj_inn {//-------- namespace: mgmj_inn --------------//



std::shared_ptr<Json_Collide> parse_collide( const Value &jsonObj_ ){

    tprAssert( jsonObj_.IsObject() );

    auto jCollideSPtr = std::make_shared<Json_Collide>();

    {//--- collideType ---//
        const auto &a = json::check_and_get_value( jsonObj_, "collideType", json::JsonValType::String );
        jCollideSPtr->collideType = str_2_colliderType( a.GetString() );
    }

    if( jCollideSPtr->collideType == ColliderType::Nil ){
        // do nothing 
    }else if( jCollideSPtr->collideType == ColliderType::Circular ){
        {//--- moveColliRadius ---//
            const auto &a = json::check_and_get_value( jsonObj_, "moveColliRadius", json::JsonValType::Double );
            jCollideSPtr->moveColliRadius = a.GetDouble();
            tprAssert( jCollideSPtr->moveColliRadius >= 0.0 );
        }
        {//--- skillColliRadius ---//
            const auto &a = json::check_and_get_value( jsonObj_, "skillColliRadius", json::JsonValType::Double );
            jCollideSPtr->skillColliRadius = a.GetDouble();
            tprAssert( jCollideSPtr->skillColliRadius >= 0.0 );
        }
    }else if( jCollideSPtr->collideType == ColliderType::Square ){
        {//--- signInMapEnts_square_type ---//
            const auto &a = json::check_and_get_value( jsonObj_, "signInMapEnts_square_type", json::JsonValType::String );
            jCollideSPtr->signInMapEnts_square_type = str_2_signInMapEnts_square_type( a.GetString() );
        }
    }else{
        //- Arc 尚未实现 ...
        tprAssert(0);
    }
    //===
    return jCollideSPtr; 
}




void assemble_colliDataFromJson( GoAssemblePlanSet::Plan &planRef_, const Json_Collide &jCollide_ ){

    if( jCollide_.collideType == ColliderType::Nil ){

        auto nilUPtr = std::make_unique<ColliDataFromJson_Nil>();
        //nilUPtr->colliderType = ColliderType::Nil;
        planRef_.colliDataFromJsonUPtr.reset( nilUPtr.release() );//- move uptr

    }else if( jCollide_.collideType == ColliderType::Circular ){

        auto cirUPtr = std::make_unique<ColliDataFromJson_Circular>();
        //cirUPtr->colliderType = ColliderType::Circular;
        cirUPtr->moveColliRadius  = glm::length( jCollide_.moveColliRadius );
        cirUPtr->skillColliRadius = glm::length( jCollide_.skillColliRadius );
        //-- colliPoints --
        calc_colliPoints_for_circular( cirUPtr->colliPointDPosOffs, tprMax(cirUPtr->moveColliRadius, cirUPtr->skillColliRadius));
        cirUPtr->makeSure_colliPointDPosOffs_isNotEmpty();
        planRef_.colliDataFromJsonUPtr.reset( cirUPtr.release() );//- move uptr

    }else if( jCollide_.collideType == ColliderType::Square ){
        
        auto squUPtr = std::make_unique<ColliDataFromJson_Square>();
        //squUPtr->colliderType = ColliderType::Square;
        squUPtr->signInMapEnts_square_type = jCollide_.signInMapEnts_square_type;
        planRef_.colliDataFromJsonUPtr.reset( squUPtr.release() );//- move uptr

    }else{
        tprAssert(0);
    }
}



// 统一组装 plan
// 调用之前，应该将 所有 .hold 数据 处理好
void aseembel_jsonPlan_2_plan(  GoAssemblePlanSet &goAssemblePlanSetRef_, const Json_Plan &json_planRef_ ){

    // goLabel
    tprAssert( !json_planRef_.goLabel.is_hold() );
    std::string goLabel = json_planRef_.goLabel.get_value();
    GoAssemblePlanSet::insert_2_goLabelIds( goLabel );

    GoAssemblePlanSet::Plan &planRef_ = goAssemblePlanSetRef_.create_new_plan( GoAssemblePlanSet::str_2_goLabelId( goLabel ));

    // goAltiRangeLabel
    planRef_.goAltiRangeLabel = json_planRef_.goAltiRangeLabel;

    // json_collideSPtr
    tprAssert( !json_planRef_.json_collideSPtr.is_hold() );
    tprAssert( json_planRef_.json_collideSPtr.get_value() ); // Must sptr != nullptr
    mgmj_inn::assemble_colliDataFromJson( planRef_, *(json_planRef_.json_collideSPtr.get_value()) );

            
    for( const auto &json_GoMeshEntRef : json_planRef_.gomeshs ){
              
        GoAssemblePlanSet::GoMeshEnt goMeshEnt {};
                    
        // afsName
        tprAssert( !json_GoMeshEntRef.afsName.is_hold() );
        goMeshEnt.animFrameSetName = json_GoMeshEntRef.afsName.get_value();

        // animLabel
        tprAssert( !json_GoMeshEntRef.animLabel.is_hold() );
        goMeshEnt.animLabel = json_GoMeshEntRef.animLabel.get_value();

        goMeshEnt.goMeshName = json_GoMeshEntRef.goMeshName;
        goMeshEnt.dposOff = json_GoMeshEntRef.dposOff;
        goMeshEnt.zOff = json_GoMeshEntRef.zOff;
        goMeshEnt.animActionEName = json_GoMeshEntRef.animActionEName;

        goMeshEnt.renderLayerType = json_GoMeshEntRef.renderLayerType;
        goMeshEnt.shaderType = json_GoMeshEntRef.shaderType;
        goMeshEnt.isVisible = json_GoMeshEntRef.isVisible;
        goMeshEnt.isAutoInit = json_GoMeshEntRef.isAutoInit;

        goMeshEnt.default_dir = json_GoMeshEntRef.default_dir;
        goMeshEnt.default_brokenLvl = json_GoMeshEntRef.default_brokenLvl;

        // maybe has val
        goMeshEnt.floorGoLayer = json_GoMeshEntRef.floorGoLayer;

        //---
        planRef_.gomeshs.emplace( goMeshEnt.goMeshName, goMeshEnt );
    }
}



}//------------- namespace: mgmj_inn end --------------//
}//------------- namespace: json end --------------//


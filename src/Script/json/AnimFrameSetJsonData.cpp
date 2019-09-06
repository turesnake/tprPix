/*
 * ================== AnimFrameSetJsonData.cpp =====================
 *                          -- tpr --
 *                                        创建 -- 2019.07.06
 *                                        修改 -- 
 * ----------------------------------------------------------
 */
//--------------- CPP ------------------//
#include <unordered_map>
#include <string>
#include <utility>
#include <vector>
#include <memory>

//--------------- Libs ------------------//
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

#include "tprGeneral.h"

//-------------------- Engine --------------------//
#include "tprAssert.h"
#include "tprCast.h"
#include "global.h"
#include "fileIO.h"
#include "AnimFrameSet.h"
#include "AnimAction.h"
#include "ColliderType.h"
#include "esrc_animFrameSet.h"


//--------------- Script ------------------//
#include "Script/json/AnimFrameSetJsonData.h"
#include "Script/json/json_oth.h"
#include "Script/resource/ssrc.h" 

using namespace rapidjson;

#include <iostream>
using std::cout;
using std::endl;


namespace afsJson_inn {//-------- namespace: afsJson_inn --------------//

    class AFSPng{
    public:
        std::string path {};
        IntVec2 frameNum {};
        size_t totalFrameNum {};
        bool isHaveShadow {};
        bool isPjtSingle {};
        bool isShadowSingle {};
        //bool isColliCapsule {};
        ColliderType  colliderType {};
        std::vector<std::shared_ptr<AnimActionParam>> actionParams {};
    };


    class AnimFrameSetJsonData{
    public:
        std::string name {};
        std::vector<std::shared_ptr<AFSPng>> afsPngs {};
    };

    std::shared_ptr<AFSPng> parse_AFSPng( const Value &pngEnt_ );
    void parse_AnimActionParam( const Value &actionParamEnt_,
                            std::vector<std::shared_ptr<AnimActionParam>> &params_ );
    std::shared_ptr<AnimActionParam> singleFrame( const Value &actionParamEnt_ );
    std::vector<std::shared_ptr<AnimActionParam>> singleFrame_batch( const Value &actionParamEnt_ );
    std::shared_ptr<AnimActionParam> multiFrame( const Value &actionParamEnt_, bool isSameTimeStep_ );
}//------------- namespace: afsJson_inn end --------------//


/* ===========================================================
 *             parse_from_animFrameSetJsonFile
 * -----------------------------------------------------------
 * Do Not Worry About Performance !!!
 */
void parse_from_animFrameSetJsonFile(){

    cout << "   ----- parse_from_animFrameSetJsonFile: start ----- " << endl;

    //-----------------------------//
    //         load file
    //-----------------------------//
    std::string path_file = tprGeneral::path_combine(path_jsons, "animFrameSets.json");
    auto jsonBufUPtr = read_a_file( path_file );

    //-----------------------------//
    //      parce JSON data
    //-----------------------------//
    Document doc;
    doc.Parse( jsonBufUPtr->c_str() );

    tprAssert( doc.IsArray() );
    for( auto &ent : doc.GetArray() ){

        afsJson_inn::AnimFrameSetJsonData jsonData {};

        {//--- name ---//
            const auto &a = json_inn::check_and_get_value( ent, "name", json_inn::JsonValType::String );
            jsonData.name = a.GetString();
        }
        {//--- pngs [] ---//
            const auto &a = json_inn::check_and_get_value( ent, "pngs", json_inn::JsonValType::Array );
            for( auto &pngEnt : a.GetArray() ){//- foreach pngs
                jsonData.afsPngs.push_back( afsJson_inn::parse_AFSPng( pngEnt ) );
            }
        }

        //--- insert to esrc::animFrameSets
        auto &animFrameSetRef = esrc::insert_new_animFrameSet( jsonData.name );
        for( auto &pngSPtr : jsonData.afsPngs ){

            animFrameSetRef.insert_a_png(   pngSPtr->path,
                                            pngSPtr->frameNum,
                                            pngSPtr->totalFrameNum,
                                            pngSPtr->isHaveShadow,
                                            pngSPtr->isPjtSingle,
                                            pngSPtr->isShadowSingle,
                                            pngSPtr->colliderType,
                                            pngSPtr->actionParams );
        }
    }

    cout << "   ----- parse_from_animFrameSetJsonFile: end ----- " << endl;
}


namespace afsJson_inn {//-------- namespace: afsJson_inn --------------//

/* ===========================================================
 *                    parse_AFSPng
 * -----------------------------------------------------------
 */
std::shared_ptr<AFSPng> parse_AFSPng( const Value &pngEnt_ ){

    auto afsPng = std::make_shared<AFSPng>();

    {//--- path ---//
        const auto &a = json_inn::check_and_get_value( pngEnt_, "path", json_inn::JsonValType::String );
        afsPng->path = a.GetString();
    }
    {//--- frameNum.col ---//
        const auto &a = json_inn::check_and_get_value( pngEnt_, "frameNum.col", json_inn::JsonValType::Int );
        afsPng->frameNum.x =  a.GetInt();
    }
    {//--- frameNum.row ---//
        const auto &a = json_inn::check_and_get_value( pngEnt_, "frameNum.row", json_inn::JsonValType::Int );
        afsPng->frameNum.y =  a.GetInt();
    }
    {//--- totalFrameNum ---//
        const auto &a = json_inn::check_and_get_value( pngEnt_, "totalFrameNum", json_inn::JsonValType::Uint64 );
        afsPng->totalFrameNum =  cast_2_size_t(a.GetUint64());
    }
    {//--- isHaveShadow ---//
        const auto &a = json_inn::check_and_get_value( pngEnt_, "isHaveShadow", json_inn::JsonValType::Bool );
        afsPng->isHaveShadow =  a.GetBool();
    }
    {//--- isPjtSingle ---//
        const auto &a = json_inn::check_and_get_value( pngEnt_, "isPjtSingle", json_inn::JsonValType::Bool );
        afsPng->isPjtSingle =  a.GetBool();
    }
    {//--- isShadowSingle ---//
        const auto &a = json_inn::check_and_get_value( pngEnt_, "isShadowSingle", json_inn::JsonValType::Bool );
        afsPng->isShadowSingle =  a.GetBool();
    }
    {//--- ColliderType ---//
        const auto &a = json_inn::check_and_get_value( pngEnt_, "ColliderType", json_inn::JsonValType::String );
        afsPng->colliderType =  str_2_ColliderType( a.GetString() );
    }
    {//--- AnimActionParams ---//
        const auto &a = json_inn::check_and_get_value( pngEnt_, "AnimActionParams", json_inn::JsonValType::Array );
        for( auto &ent : a.GetArray() ){//- foreach AnimActionParam
            afsJson_inn::parse_AnimActionParam( ent, afsPng->actionParams );
        }
    }

    return afsPng;
}

/* ===========================================================
 *                    parse_AnimActionParam
 * -----------------------------------------------------------
 */
void parse_AnimActionParam( const Value &actionParamEnt_,
                            std::vector<std::shared_ptr<AnimActionParam>> &params_ ){

    std::string type {};
    const std::string singleFrame {"singleFrame"};
    const std::string singleFrame_batch {"singleFrame_batch"};
    const std::string multiFrame_SameTimeStep {"multiFrame_SameTimeStep"};
    const std::string multiFrame_DiffTimeStep {"multiFrame_DiffTimeStep"};

    {//--- type ---//
        const auto &a = json_inn::check_and_get_value( actionParamEnt_, "type", json_inn::JsonValType::String );
        type = a.GetString();
    }

    if( type == singleFrame ){
        params_.push_back( afsJson_inn::singleFrame(actionParamEnt_) );
    }else if( type == singleFrame_batch ){
        auto p = afsJson_inn::singleFrame_batch(actionParamEnt_);
        for( auto &i : p ){
            params_.push_back( i );
        }
    }else if( type == multiFrame_SameTimeStep ){
        params_.push_back( afsJson_inn::multiFrame(actionParamEnt_, true) );
    }else if( type == multiFrame_DiffTimeStep ){
        params_.push_back( afsJson_inn::multiFrame(actionParamEnt_, false) );
    }else{
        tprAssert(0);
    }
}

/* ===========================================================
 *                    singleFrame
 * -----------------------------------------------------------
 */
std::shared_ptr<AnimActionParam> singleFrame( const Value &actionParamEnt_ ){

    std::string actionName {};
    size_t      lFrameIdx  {};
    bool        isOpaque   {};
    {//--- actionName ---//
        const auto &a = json_inn::check_and_get_value( actionParamEnt_, "actionName", json_inn::JsonValType::String );
        actionName = a.GetString();
    }
    {//--- lFrameIdx ---//
        const auto &a = json_inn::check_and_get_value( actionParamEnt_, "lFrameIdx", json_inn::JsonValType::Uint64 );
        lFrameIdx = cast_2_size_t(a.GetUint64());
    }
    {//--- isOpaque ---//
        const auto &a = json_inn::check_and_get_value( actionParamEnt_, "isOpaque", json_inn::JsonValType::Bool );
        isOpaque = a.GetBool();
    }

    return std::make_shared<AnimActionParam>( actionName, lFrameIdx, isOpaque );
}

/* ===========================================================
 *                    singleFrame_batch
 * -----------------------------------------------------------
 */
std::vector<std::shared_ptr<AnimActionParam>> singleFrame_batch( const Value &actionParamEnt_ ){

    std::string prefix {};
    size_t      num    {};
    std::string suffix {};
    std::string actionName {};
    std::vector<std::shared_ptr<AnimActionParam>> params {};
    bool        isOpaque   {};
    {//--- actionName.prefix ---//
        const auto &a = json_inn::check_and_get_value( actionParamEnt_, "actionName.prefix", json_inn::JsonValType::String );
        prefix = a.GetString();
    }
    {//--- actionName.suffix ---//
        const auto &a = json_inn::check_and_get_value( actionParamEnt_, "actionName.suffix", json_inn::JsonValType::String );
        suffix = a.GetString();
    }
    {//--- actionName.midNum ---//
        const auto &a = json_inn::check_and_get_value( actionParamEnt_, "actionName.midNum", json_inn::JsonValType::Uint64 );
        num = cast_2_size_t(a.GetUint64());
    }
    {//--- isOpaque ---//
        const auto &a = json_inn::check_and_get_value( actionParamEnt_, "isOpaque", json_inn::JsonValType::Bool );
        isOpaque = a.GetBool();
    }
    //---
    for( size_t i=0; i<num; i++ ){
        actionName = tprGeneral::nameString_combine( prefix, i, suffix );
        params.push_back( std::make_shared<AnimActionParam>(actionName,i,isOpaque) );
    }
    return params;
}

/* ===========================================================
 *                    multiFrame
 * -----------------------------------------------------------
 */
std::shared_ptr<AnimActionParam> multiFrame( const Value &actionParamEnt_, bool isSameTimeStep_ ){

    std::string         actionName {};
    AnimActionType      actionType {};
    bool                isOrder {};
    bool                isOpaque   {};
    std::vector<size_t> lFrameIdxs {};
    std::vector<size_t> timeSteps {}; //- only for DiffTimeStep
    size_t              timeStep  {}; //- only for SameTimeStep

    {//--- actionName ---//
        const auto &a = json_inn::check_and_get_value( actionParamEnt_, "actionName", json_inn::JsonValType::String );
        actionName = a.GetString();
    }
    {//--- actionType ---//
        const auto &a = json_inn::check_and_get_value( actionParamEnt_, "actionType", json_inn::JsonValType::String );
        actionType = str_2_AnimActionType( a.GetString() );
    }
    {//--- isOrder ---//
        const auto &a = json_inn::check_and_get_value( actionParamEnt_, "isOrder", json_inn::JsonValType::Bool );
        isOrder = a.GetBool();
    }
    {//--- isOpaque ---//
        const auto &a = json_inn::check_and_get_value( actionParamEnt_, "isOpaque", json_inn::JsonValType::Bool );
        isOpaque = a.GetBool();
    }
    {//--- lFrameIdxs [] ---//
        const auto &a = json_inn::check_and_get_value( actionParamEnt_, "lFrameIdxs", json_inn::JsonValType::Array );
        for( SizeType i=0; i<a.Size(); i++ ){//- foreach lFrameIdx
            lFrameIdxs.push_back( cast_2_size_t(a[i].GetUint64()) );
        }
    }
    
    if( isSameTimeStep_ ){
        //--- timeStep ---//
        const auto &a = json_inn::check_and_get_value( actionParamEnt_, "timeStep", json_inn::JsonValType::Uint64 );
        timeStep = cast_2_size_t(a.GetUint64());
        //---
        return std::make_shared<AnimActionParam>(   actionName,
                                                    actionType,
                                                    isOrder,
                                                    isOpaque,
                                                    lFrameIdxs,
                                                    timeStep );
    }else{
        //--- timeSteps [] ---//
        const auto &a = json_inn::check_and_get_value( actionParamEnt_, "timeSteps", json_inn::JsonValType::Array );
        for( SizeType i=0; i<a.Size(); i++ ){//- foreach timeStep
            timeSteps.push_back( cast_2_size_t(a[i].GetUint64()) );
        }
        //---
        return std::make_shared<AnimActionParam>(   actionName,
                                                    actionType,
                                                    isOrder,
                                                    isOpaque,
                                                    lFrameIdxs,
                                                    timeSteps );
    }
}



}//------------- namespace: afsJson_inn end --------------//


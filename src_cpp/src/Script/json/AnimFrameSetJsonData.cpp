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

#include "SysConfig.h" // MUST BEFORE TPR_OS_WIN32_ !!!
#if defined TPR_OS_WIN32_ 
    #include "tprFileSys_win.h"
#elif defined TPR_OS_UNIX_ 
    #include "tprFileSys_unix.h"
#endif

//-------------------- Engine --------------------//
#include "tprAssert.h"
#include "global.h"
#include "AnimFrameSet.h"
#include "AnimAction.h"
#include "esrc_animFrameSet.h"


//--------------- Script ------------------//
#include "Script/json/AnimFrameSetJsonData.h"
#include "Script/resource/ssrc.h" 

using namespace rapidjson;

#include <iostream>
using std::cout;
using std::endl;


class AFSPng{
public:
    std::string path {};
    IntVec2 frameNum {};
    size_t totalFrameNum {};
    bool isHaveShadow {};
    bool isPjtSingle {};
    bool isShadowSingle {};
    std::vector<std::shared_ptr<AnimActionParam>> actionParams {};
};


class AnimFrameSetJsonData{
public:
    std::string name {};
    std::vector<std::shared_ptr<AFSPng>> afsPngs {};
};



namespace afsJson_inn {//-------- namespace: afsJson_inn --------------//
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
    std::string jsonBuf {};
    std::string path_file = tprGeneral::path_combine(path_jsons, "animFrameSets.json");

    //-- read files --
#if defined TPR_OS_WIN32_
    tprWin::file_load( path_file, jsonBuf );
#elif defined TPR_OS_UNIX_
    tprUnix::file_load( path_file, jsonBuf );
#endif

    //-----------------------------//
    //      parce JSON data
    //-----------------------------//
    Document doc;
    doc.Parse( jsonBuf.c_str() );

    tprAssert( doc.IsArray() );
    for( auto &ent : doc.GetArray() ){

        AnimFrameSetJsonData jsonData {};

        {//--- name ---//
            tprAssert( ent.HasMember("name") );
            const Value &a = ent["name"];
            tprAssert( a.IsString() );
            jsonData.name = a.GetString();
        }
        {//--- pngs [] ---//
            tprAssert( ent.HasMember("pngs") );
            const Value &a = ent["pngs"];
            tprAssert( a.IsArray() );
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
        tprAssert( pngEnt_.HasMember("path") );
        const Value &a = pngEnt_["path"];
        tprAssert( a.IsString() );
        afsPng->path = a.GetString();
    }
    {//--- frameNum.col ---//
        tprAssert( pngEnt_.HasMember("frameNum.col") );
        const Value &a = pngEnt_["frameNum.col"];
        tprAssert( a.IsInt() );
        afsPng->frameNum.x =  a.GetInt();
    }
    {//--- frameNum.row ---//
        tprAssert( pngEnt_.HasMember("frameNum.row") );
        const Value &a = pngEnt_["frameNum.row"];
        tprAssert( a.IsInt() );
        afsPng->frameNum.y =  a.GetInt();
    }
    {//--- totalFrameNum ---//
        tprAssert( pngEnt_.HasMember("totalFrameNum") );
        const Value &a = pngEnt_["totalFrameNum"];
        tprAssert( a.IsUint64() );
        afsPng->totalFrameNum =  static_cast<size_t>(a.GetUint64());
    }
    {//--- isHaveShadow ---//
        tprAssert( pngEnt_.HasMember("isHaveShadow") );
        const Value &a = pngEnt_["isHaveShadow"];
        tprAssert( a.IsBool() );
        afsPng->isHaveShadow =  a.GetBool();
    }
    {//--- isPjtSingle ---//
        tprAssert( pngEnt_.HasMember("isPjtSingle") );
        const Value &a = pngEnt_["isPjtSingle"];
        tprAssert( a.IsBool() );
        afsPng->isPjtSingle =  a.GetBool();
    }
    {//--- isShadowSingle ---//
        tprAssert( pngEnt_.HasMember("isShadowSingle") );
        const Value &a = pngEnt_["isShadowSingle"];
        tprAssert( a.IsBool() );
        afsPng->isShadowSingle =  a.GetBool();
    }
    {//--- AnimActionParams ---//
        tprAssert( pngEnt_.HasMember("AnimActionParams") );
        const Value &a = pngEnt_["AnimActionParams"];
        tprAssert( a.IsArray() );
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
        tprAssert( actionParamEnt_.HasMember("type") );
        const Value &a = actionParamEnt_["type"];
        tprAssert( a.IsString() );
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
    {//--- actionName ---//
        tprAssert( actionParamEnt_.HasMember("actionName") );
        const Value &a = actionParamEnt_["actionName"];
        tprAssert( a.IsString() );
        actionName = a.GetString();
    }
    {//--- lFrameIdx ---//
        tprAssert( actionParamEnt_.HasMember("lFrameIdx") );
        const Value &a = actionParamEnt_["lFrameIdx"];
        tprAssert( a.IsUint64() );
        lFrameIdx = static_cast<size_t>(a.GetUint64());
    }
    return std::make_shared<AnimActionParam>( actionName, lFrameIdx );
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
    {//--- actionName.prefix ---//
        tprAssert( actionParamEnt_.HasMember("actionName.prefix") );
        const Value &a = actionParamEnt_["actionName.prefix"];
        tprAssert( a.IsString() );
        prefix = a.GetString();
    }
    {//--- actionName.suffix ---//
        tprAssert( actionParamEnt_.HasMember("actionName.suffix") );
        const Value &a = actionParamEnt_["actionName.suffix"];
        tprAssert( a.IsString() );
        suffix = a.GetString();
    }
    {//--- actionName.midNum ---//
        tprAssert( actionParamEnt_.HasMember("actionName.midNum") );
        const Value &a = actionParamEnt_["actionName.midNum"];
        tprAssert( a.IsUint64() );
        num = static_cast<size_t>(a.GetUint64());
    }
    //---
    for( size_t i=0; i<num; i++ ){
        actionName = tprGeneral::nameString_combine( prefix, i, suffix );
        params.push_back( std::make_shared<AnimActionParam>(actionName,i) );
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
    std::vector<size_t> lFrameIdxs {};
    std::vector<size_t> timeSteps {}; //- only for DiffTimeStep
    size_t              timeStep  {}; //- only for SameTimeStep

    {//--- actionName ---//
        tprAssert( actionParamEnt_.HasMember("actionName") );
        const Value &a = actionParamEnt_["actionName"];
        tprAssert( a.IsString() );
        actionName = a.GetString();
    }
    {//--- actionType ---//
        tprAssert( actionParamEnt_.HasMember("actionType") );
        const Value &a = actionParamEnt_["actionType"];
        tprAssert( a.IsString() );
        actionType = str_2_AnimActionType( a.GetString() );
    }
    {//--- isOrder ---//
        tprAssert( actionParamEnt_.HasMember("isOrder") );
        const Value &a = actionParamEnt_["isOrder"];
        tprAssert( a.IsBool() );
        isOrder = a.GetBool();
    }
    {//--- lFrameIdxs [] ---//
        tprAssert( actionParamEnt_.HasMember("lFrameIdxs") );
        const Value &a = actionParamEnt_["lFrameIdxs"];
        tprAssert( a.IsArray() );
        for( SizeType i=0; i<a.Size(); i++ ){//- foreach lFrameIdx
            lFrameIdxs.push_back( static_cast<size_t>(a[i].GetUint64()) );
        }
    }
    
    if( isSameTimeStep_ ){
        //--- timeStep ---//
        tprAssert( actionParamEnt_.HasMember("timeStep") );
        const Value &a = actionParamEnt_["timeStep"];
        tprAssert( a.IsUint64() );
        timeStep = static_cast<size_t>(a.GetUint64());
        //---
        return std::make_shared<AnimActionParam>(   actionName,
                                                    actionType,
                                                    isOrder,
                                                    lFrameIdxs,
                                                    timeStep );
    }else{
        //--- timeSteps [] ---//
        tprAssert( actionParamEnt_.HasMember("timeSteps") );
        const Value &a = actionParamEnt_["timeSteps"];
        tprAssert( a.IsArray() );
        for( SizeType i=0; i<a.Size(); i++ ){//- foreach timeStep
            timeSteps.push_back( static_cast<size_t>(a[i].GetUint64()) );
        }
        //---
        return std::make_shared<AnimActionParam>(   actionName,
                                                    actionType,
                                                    isOrder,
                                                    lFrameIdxs,
                                                    timeSteps );
    }
}



}//------------- namespace: afsJson_inn end --------------//


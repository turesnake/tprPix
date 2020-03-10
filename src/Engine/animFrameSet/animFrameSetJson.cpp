/*
 * ================== animFrameSetJson.cpp =====================
 *                          -- tpr --
 *                                        创建 -- 2019.07.06
 *                                        修改 -- 
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
#include "AnimFrameSet.h"
#include "AnimAction.h"

#include "json_oth.h"

#include "esrc_animFrameSet.h"
#include "esrc_state.h"


using namespace rapidjson;


namespace json{//------------- namespace json ----------------
namespace afsJson_inn {//-------- namespace: afsJson_inn --------------//

    std::string dirPath {}; // 每个 afs.json 文件 所在目录 的path

    class AFSPng{
    public:
        std::string path {}; // 绝对 path
        IntVec2 frameNum {};
        size_t totalFrameNum {};
        bool isHaveShadow {};
        bool isPjtSingle {};
        bool isShadowSingle {};
        std::vector<std::shared_ptr<AnimActionParam>> actionParams {};
    };

    class AnimFrameSetJsonData{
    public:
        std::string afsName {};
        std::vector<std::shared_ptr<AFSPng>> afsPngs {};
    };

    //===== funcs =====//
    std::shared_ptr<AFSPng> parse_AFSPng( const Value &pngEnt_ );

    void parse_subspec_in_handleType( const Value &subspecEnt_,
                            std::vector<std::shared_ptr<AnimActionParam>> &params_,
                            bool isPjtSingle_ );
    void parse_subspec_in_batchType( const Value &subspecEnt_,
                            std::vector<std::shared_ptr<AnimActionParam>> &params_,
                            bool isPjtSingle_ );

    void parse_AnimActionParam( size_t  subspeciesIdx_,
                                const Value &actionParamEnt_,
                                std::vector<std::shared_ptr<AnimActionParam>> &params_,
                                const std::string &label_,
                                bool isPjtSingle_ );

    std::shared_ptr<AnimActionParam> singleFrame(   size_t  subspeciesIdx_,
                                                    const Value &actionParamEnt_,
                                                    const std::string &label_,
                                                    bool isPjtSingle_ );
    
    std::shared_ptr<AnimActionParam> multiFrame(    size_t  subspeciesIdx_,
                                                    const Value &actionParamEnt_, 
                                                    const std::string &label_ );

    NineDirection jsonStr_2_ninedirection( const std::string &str_ );
}//------------- namespace: afsJson_inn end --------------//


/* Do Not Worry About Performance !!!
 */
void parse_animFrameSetJsonFile(){

    tprDebug::console( "   ----- parse_animFrameSetJsonFile: start ----- " );

    std::vector<std::string> path_files {};
    collect_fileNames( path_jsons, "animFrameSet", "_files.json", path_files );

    //-----------------------------//
    //         load file
    //-----------------------------//
    for( const auto &path : path_files ){
        parse_single_animFrameSetJsonFile( path );
    }

    //-------- special ids ----------//
    esrc::set_nilCollide_emptyPixId( esrc::apply_a_random_animSubspeciesId( "emptyPix_NilCollide", "", 1) );
    esrc::set_squareCollide_emptyPixId( esrc::apply_a_random_animSubspeciesId( "emptyPix_SquareCollide", "", 1) );
    esrc::set_fieldRimId( esrc::apply_a_random_animSubspeciesId( "fieldRim", "", 1) );
    //...
    
    //-------------
    esrc::insertState("json_animFrameSet");
    tprDebug::console( "   ----- parse_animFrameSetJsonFile: end ----- " );
}


// param: path_file_  -- 绝对path
// param: out_afsNamesPtr_ -- 将所有 afsNames 传递给外部
void parse_single_animFrameSetJsonFile( const std::string &path_file_,
                                        std::unordered_set<std::string> *out_afsNamesPtr_ ){

    //-----------------------------//
    //        dirPath
    //-----------------------------//
    afsJson_inn::dirPath = tprGeneral::get_dirPath( path_file_ );

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

        afsJson_inn::AnimFrameSetJsonData jsonData {};

        {//--- afsName ---//
            const auto &a = check_and_get_value( ent, "afsName", JsonValType::String );
            jsonData.afsName = a.GetString();
            if( out_afsNamesPtr_ != nullptr ){
                out_afsNamesPtr_->insert( jsonData.afsName );
            }
        }
        {//--- pngs [] ---//
            const auto &a = check_and_get_value( ent, "pngs", JsonValType::Array );
            for( auto &pngEnt : a.GetArray() ){//- foreach pngs
                jsonData.afsPngs.push_back( afsJson_inn::parse_AFSPng( pngEnt ) );
            }
        }

        //--- insert to esrc::animFrameSets
        auto &animFrameSetRef = esrc::insert_new_animFrameSet( jsonData.afsName );
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
}


namespace afsJson_inn {//-------- namespace: afsJson_inn --------------//


/* ===========================================================
 *                    parse_AFSPng
 * -----------------------------------------------------------
 */
std::shared_ptr<AFSPng> parse_AFSPng( const Value &pngEnt_ ){

    auto afsPng = std::make_shared<AFSPng>();

    {//--- path ---//
        const auto &a = check_and_get_value( pngEnt_, "path", JsonValType::String );
        std::string lPath = a.GetString();
        tprAssert( lPath != "" );
        afsPng->path = tprGeneral::path_combine( afsJson_inn::dirPath, lPath );
    }
    {//--- frameNum.col ---//
        const auto &a = check_and_get_value( pngEnt_, "frameNum.col", JsonValType::Int );
        afsPng->frameNum.x =  a.GetInt();
    }
    {//--- frameNum.row ---//
        const auto &a = check_and_get_value( pngEnt_, "frameNum.row", JsonValType::Int );
        afsPng->frameNum.y =  a.GetInt();
    }
    {//--- totalFrameNum ---//
        const auto &a = check_and_get_value( pngEnt_, "totalFrameNum", JsonValType::Uint64 );
        afsPng->totalFrameNum =  cast_2_size_t(a.GetUint64());
    }
    {//--- isHaveShadow ---//
        const auto &a = check_and_get_value( pngEnt_, "isHaveShadow", JsonValType::Bool );
        afsPng->isHaveShadow =  a.GetBool();
    }
    {//--- isPjtSingle ---//
        const auto &a = check_and_get_value( pngEnt_, "isPjtSingle", JsonValType::Bool );
        afsPng->isPjtSingle =  a.GetBool();
    }

    //--- isShadowSingle ---//
    if( afsPng->isHaveShadow ){
        const auto &a = check_and_get_value( pngEnt_, "isShadowSingle", JsonValType::Bool );
        afsPng->isShadowSingle =  a.GetBool();
    }else{
        afsPng->isShadowSingle = true; // 此时无意义
    }

    {//--- subspecs ---//
        const auto &a = check_and_get_value( pngEnt_, "subspecs", JsonValType::Array );
        for( auto &ent : a.GetArray() ){//- foreach subspecs
            //--- type ---//
            const auto &typeStr = check_and_get_value( ent, "type", JsonValType::String );
            std::string subType = typeStr.GetString();
            if( subType == "batch" ){
                afsJson_inn::parse_subspec_in_batchType( ent, afsPng->actionParams, afsPng->isPjtSingle );
            }else if( subType == "handle" ){
                afsJson_inn::parse_subspec_in_handleType( ent, afsPng->actionParams, afsPng->isPjtSingle );
            }else{
                tprAssert(0);
            }
        }
    }

    return afsPng;
}




/* ===========================================================
 *                parse_subspec_in_handleType
 * -----------------------------------------------------------
 */
void parse_subspec_in_handleType(  const Value &subspecEnt_,
                        std::vector<std::shared_ptr<AnimActionParam>> &params_,
                        bool isPjtSingle_ ){

    
    std::string     animLabel {};
    size_t          subIdx  {};

    {//--- animLabel ---//
        const auto &a = check_and_get_value( subspecEnt_, "animLabel", JsonValType::String );        
        animLabel = a.GetString();
    }
    {//--- subIdx ---//
        const auto &a = check_and_get_value( subspecEnt_, "subIdx", JsonValType::Uint64 );
        subIdx =  cast_2_size_t(a.GetUint64());
    }
    {//--- AnimActionParams ---//
        const auto &a = check_and_get_value( subspecEnt_, "AnimActionParams", JsonValType::Array );
        for( auto &ent : a.GetArray() ){//- foreach AnimActionParam
            afsJson_inn::parse_AnimActionParam( subIdx, ent, params_, animLabel, isPjtSingle_ );
        }
    }
}


/* ===========================================================
 *                parse_subspec_in_batchType
 * -----------------------------------------------------------
 */
void parse_subspec_in_batchType(  const Value &subspecEnt_,
                        std::vector<std::shared_ptr<AnimActionParam>> &params_,
                        bool isPjtSingle_ ){
    
    std::string     animLabel {};
    AnimActionEName actionEName;
    NineDirection actionDir {};
    BrokenLvl     actionBrokenLvl {};
    size_t        fstSubIdx  {};
    size_t        fstIdx     {};
    size_t        idxNums    {};
    size_t        jFrameIdx  {};
    bool          isOpaque   {};

    {//--- animLabel ---//
        const auto &a = check_and_get_value( subspecEnt_, "animLabel", JsonValType::String );        
        animLabel = a.GetString();
    }

    {//--- fstSubIdx ---//
        const auto &a = check_and_get_value( subspecEnt_, "fstSubIdx", JsonValType::Uint64 );
        fstSubIdx =  cast_2_size_t(a.GetUint64());
    }
    {//--- fstIdx ---//
        const auto &a = check_and_get_value( subspecEnt_, "fstIdx", JsonValType::Uint64 );
        fstIdx =  cast_2_size_t(a.GetUint64());
    }
    {//--- idxNums ---//
        const auto &a = check_and_get_value( subspecEnt_, "idxNums", JsonValType::Uint64 );
        idxNums =  cast_2_size_t(a.GetUint64());
    }
    {//--- actionEName ---//
        const auto &a = check_and_get_value( subspecEnt_, "actionEName", JsonValType::String );
        actionEName = str_2_animActionEName( a.GetString() );
    }
    {//--- dir ---//
        const auto &a = check_and_get_value( subspecEnt_, "dir", JsonValType::String );
        actionDir = jsonStr_2_ninedirection( a.GetString() );
    }
    {//--- brokenLvl ---//
        const auto &a = check_and_get_value( subspecEnt_, "brokenLvl", JsonValType::Int );
        actionBrokenLvl = int_2_brokenLvl( a.GetInt() );
    }
    {//--- isOpaque ---//
        const auto &a = check_and_get_value( subspecEnt_, "isOpaque", JsonValType::Bool );
        isOpaque = a.GetBool();
    }

    //---
    for( size_t i=0; i<idxNums; i++ ){
        isPjtSingle_ ?
            jFrameIdx = 0 :
            jFrameIdx = i;
        params_.push_back( std::make_shared<AnimActionParam>(i+fstSubIdx, actionEName, actionDir, actionBrokenLvl, jFrameIdx, i+fstIdx, isOpaque, animLabel) );
    }

}




/* ===========================================================
 *                    parse_AnimActionParam
 * -----------------------------------------------------------
 */
void parse_AnimActionParam( size_t  subspeciesIdx_,
                            const Value &actionParamEnt_,
                            std::vector<std::shared_ptr<AnimActionParam>> &params_,
                            const std::string &label_,
                            bool isPjtSingle_ ){

    std::string type {};

    {//--- type ---//
        const auto &a = check_and_get_value( actionParamEnt_, "type", JsonValType::String );
        type = a.GetString();
    }
    if( type == "singleFrame" ){
        params_.push_back( afsJson_inn::singleFrame(subspeciesIdx_, actionParamEnt_, label_, isPjtSingle_ ) );
    }
    else if( type == "multiFrame" ){
        params_.push_back( afsJson_inn::multiFrame(subspeciesIdx_, actionParamEnt_, label_) );
    }else{
        tprAssert(0);
    }
}


/* ===========================================================
 *                    singleFrame
 * -----------------------------------------------------------
 */
std::shared_ptr<AnimActionParam> singleFrame(   size_t  subspeciesIdx_,
                                                const Value &actionParamEnt_,
                                                const std::string &label_,
                                                bool isPjtSingle_ ){

    AnimActionEName actionEName {};
    NineDirection actionDir {};
    BrokenLvl     actionBrokenLvl {};
    size_t        jFrameIdx  {};
    size_t        lFrameIdx  {};
    bool          isOpaque   {};
    {//--- actionEName ---//
        const auto &a = check_and_get_value( actionParamEnt_, "actionEName", JsonValType::String );
        actionEName = str_2_animActionEName( a.GetString() );
    }
    {//--- dir ---//
        const auto &a = check_and_get_value( actionParamEnt_, "dir", JsonValType::String );
        actionDir = jsonStr_2_ninedirection( a.GetString() );
    }
    {//--- brokenLvl ---//
        const auto &a = check_and_get_value( actionParamEnt_, "brokenLvl", JsonValType::Int );
        actionBrokenLvl = int_2_brokenLvl( a.GetInt() );
    }
    {//--- lFrameIdx ---//
        const auto &a = check_and_get_value( actionParamEnt_, "lFrameIdx", JsonValType::Uint64 );
        lFrameIdx = cast_2_size_t(a.GetUint64());
    }
    {//--- isOpaque ---//
        const auto &a = check_and_get_value( actionParamEnt_, "isOpaque", JsonValType::Bool );
        isOpaque = a.GetBool();
    }

    isPjtSingle_ ?
        jFrameIdx = 0 :
        jFrameIdx = lFrameIdx;

    return std::make_shared<AnimActionParam>( subspeciesIdx_, actionEName, actionDir, actionBrokenLvl, jFrameIdx, lFrameIdx, isOpaque, label_ );
}


/* ===========================================================
 *                    multiFrame
 * -----------------------------------------------------------
 */
std::shared_ptr<AnimActionParam> multiFrame(size_t  subspeciesIdx_,
                                            const Value &actionParamEnt_, 
                                            const std::string &label_ ){

    AnimActionEName actionEName {};
    NineDirection       actionDir {};
    BrokenLvl           actionBrokenLvl {};
    AnimAction::PlayType  actionPlayType {};
    bool                isOrder {};
    bool                isOpaque   {};
    bool                isSeries   {}; //- 是否存储为 连续帧
    bool                isTimeStepSame  {};
    size_t              jFrameIdx  {0}; // tmp, 如果某action 有多个 frame，那么 本值一定为 0 ...
    std::vector<size_t> lFrameIdxs {};
    std::vector<size_t> timeSteps {}; //- only for DiffTimeStep
    size_t              timeStep  {}; //- only for SameTimeStep

    {//--- actionEName ---//
        const auto &a = check_and_get_value( actionParamEnt_, "actionEName", JsonValType::String );
        actionEName = str_2_animActionEName( a.GetString() );
    }
    {//--- dir ---//
        const auto &a = check_and_get_value( actionParamEnt_, "dir", JsonValType::String );
        actionDir = jsonStr_2_ninedirection( a.GetString() );
    }
    {//--- brokenLvl ---//
        const auto &a = check_and_get_value( actionParamEnt_, "brokenLvl", JsonValType::Int );
        actionBrokenLvl = int_2_brokenLvl( a.GetInt() );
    }
    {//--- actionPlayType ---//
        const auto &a = check_and_get_value( actionParamEnt_, "actionPlayType", JsonValType::String );
        actionPlayType = AnimAction::str_2_playType( a.GetString() );
    }
    {//--- isOrder ---//
        const auto &a = check_and_get_value( actionParamEnt_, "isOrder", JsonValType::Bool );
        isOrder = a.GetBool();
    }
    {//--- isOpaque ---//
        const auto &a = check_and_get_value( actionParamEnt_, "isOpaque", JsonValType::Bool );
        isOpaque = a.GetBool();
    }
    {//--- isSeries ---//
        const auto &a = check_and_get_value( actionParamEnt_, "isSeries", JsonValType::Bool );
        isSeries = a.GetBool();
    }
    {//--- isTimeStepSame ---//
        const auto &a = check_and_get_value( actionParamEnt_, "isTimeStepSame", JsonValType::Bool );
        isTimeStepSame = a.GetBool();
    }

    if( isSeries ){//- 动画帧在 png中 连续存储
        size_t fstFrameIdx {};
        size_t frameNums   {};
        {//--- fstFrameIdx ---//
            const auto &a = check_and_get_value( actionParamEnt_, "fstFrameIdx", JsonValType::Uint64 );
            fstFrameIdx = cast_2_size_t(a.GetUint64());
        }
        {//--- frameNums ---//
            const auto &a = check_and_get_value( actionParamEnt_, "frameNums", JsonValType::Uint64 );
            frameNums = cast_2_size_t(a.GetUint64());
        }
        for( size_t i=0; i<frameNums; i++ ){
            lFrameIdxs.push_back( fstFrameIdx + i );
        }

    }else{//- 动画帧在 png中 非连续存储，需要手动收集所有帧 idx
        {//--- lFrameIdxs [] ---//
            const auto &a = check_and_get_value( actionParamEnt_, "lFrameIdxs", JsonValType::Array );
            for( SizeType i=0; i<a.Size(); i++ ){//- foreach lFrameIdx
                lFrameIdxs.push_back( cast_2_size_t(a[i].GetUint64()) );
            }
        }

    }

    if( isTimeStepSame ){
        {//--- timeStep ---//
            const auto &a = check_and_get_value( actionParamEnt_, "timeStep", JsonValType::Uint64 );
            timeStep = cast_2_size_t(a.GetUint64());
        }
        //---
        return std::make_shared<AnimActionParam>(   subspeciesIdx_,
                                                    actionEName,
                                                    actionDir,
                                                    actionBrokenLvl,
                                                    actionPlayType,
                                                    isOrder,
                                                    isOpaque,
                                                    jFrameIdx,
                                                    lFrameIdxs,
                                                    timeStep,
                                                    label_ );
    }else{
        {//--- timeSteps [] ---//
            const auto &a = check_and_get_value( actionParamEnt_, "timeSteps", JsonValType::Array );
            for( SizeType i=0; i<a.Size(); i++ ){//- foreach timeStep
                timeSteps.push_back( cast_2_size_t(a[i].GetUint64()) );
            }
        }
        //---
        return std::make_shared<AnimActionParam>(   subspeciesIdx_,
                                                    actionEName,
                                                    actionDir,
                                                    actionBrokenLvl,
                                                    actionPlayType,
                                                    isOrder,
                                                    isOpaque,
                                                    jFrameIdx,
                                                    lFrameIdxs,
                                                    timeSteps,
                                                    label_ );
    }
}


// 缩写版 str
NineDirection jsonStr_2_ninedirection( const std::string &str_ ){

    if(       str_ == "Center" ){ return NineDirection::Center;
    }else if( str_ == "L" ){   return NineDirection::Left;
    }else if( str_ == "LT" ){  return NineDirection::LeftTop;
    }else if( str_ == "T" ){   return NineDirection::Top;
    }else if( str_ == "RT" ){  return NineDirection::RightTop;
    }else if( str_ == "R" ){   return NineDirection::Right;
    }else if( str_ == "RB" ){  return NineDirection::RightBottom;
    }else if( str_ == "B" ){   return NineDirection::Bottom;
    }else if( str_ == "LB" ){  return NineDirection::LeftBottom;
    }else{
        tprAssert(0);
        return NineDirection::Center; // never reach
    }
}


}//------------- namespace: afsJson_inn end --------------//
}//------------- namespace json: end ----------------

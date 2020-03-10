/*
 * =================== json_windowConfig.cpp =====================
 *                          -- tpr --
 *                                        CREATE -- 2019.08.08
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#include "pch.h"

//--------------- Libs ------------------//
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

#include "tprGeneral.h"

//-------------------- Engine --------------------//
#include "global.h"
#include "fileIO.h"

#include "json_oth.h"

#include "esrc_state.h"

//--------------- Script ------------------//
#include "Script/json/json_all.h"

using namespace rapidjson;


namespace json{//------------- namespace json ----------------


/* ===========================================================
 *                parse_windowConfigJsonFile
 * -----------------------------------------------------------
 * Do Not Worry About Performance !!!
 */
WindowConfigJsonData parse_windowConfigJsonFile(){

    tprDebug::console( "   ----- parse_windowConfigJsonFile: start ----- " );

    //-----------------------------//
    //         load file
    //-----------------------------//
    std::string path_file = tprGeneral::path_combine(path_jsons, "windowConfig.json");
    auto jsonBufUPtr = read_a_file( path_file );

    //-----------------------------//
    //      parce JSON data
    //-----------------------------//
    WindowConfigJsonData windowConfig {}; // 这组数据很小，直接值传递

    Document doc;
    doc.Parse( jsonBufUPtr->c_str() );

    tprAssert( doc.IsObject() );
    {//--- windowPixW ---//
        const auto &a = check_and_get_value( doc, "windowPixW", JsonValType::Int );
        windowConfig.windowPixW = a.GetInt();
        tprAssert( windowConfig.windowPixW > 0 );
    }
    {//--- windowPixH ---//
        const auto &a = check_and_get_value( doc, "windowPixH", JsonValType::Int );
        windowConfig.windowPixH = a.GetInt();
        tprAssert( windowConfig.windowPixH > 0 );
    }
    {//--- isFullScreen ---//
        const auto &a = check_and_get_value( doc, "isFullScreen", JsonValType::Bool );
        windowConfig.isFullScreen = a.GetBool();
    }

    tprDebug::console( "   ----- parse_windowConfigJsonFile: end ----- " );

    esrc::insertState("json_windowConfig");
    return windowConfig;
}


}//------------- namespace json: end ----------------

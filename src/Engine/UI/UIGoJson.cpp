/*
 * ====================== UIGoJson.cpp =====================
 *                          -- tpr --
 *                                        CREATE -- 2019.07.02
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#include "pch.h"
#include "UIGoSpecFromJson.h"

//--------------- CPP ------------------//
#include <utility>

//--------------- Libs ------------------//
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

//-------------------- Engine --------------------//
#include "global.h"
#include "fileIO.h"

#include "json_oth.h"

#include "esrc_state.h"


using namespace rapidjson;


namespace json{//------------- namespace json ----------------
namespace uiJson_inn {//-------- namespace: uiJson_inn --------------//

    void parse_single_uiGoJsonFile( const std::string &path_file_ );

}//------------- namespace: uiJson_inn end --------------//




/* Do Not Worry About Performance !!!
 */
void parse_uiGoJsonFile(){

    tprDebug::console( "   ----- parse_uiGoJsonFile: start ----- " );


    std::vector<std::string> path_files {};
    collect_fileNames( path_jsons, "uiGos", "_files.json", path_files );
    //---
    for( const auto &i : path_files ){
        uiJson_inn::parse_single_uiGoJsonFile(i);
    }

    esrc::insertState("json_uiGo");
    tprDebug::console( "   ----- parse_uiGoJsonFile: end ----- " );
}



namespace uiJson_inn {//-------- namespace: uiJson_inn --------------//



void parse_single_uiGoJsonFile( const std::string &path_file_ ){
    //-----------------------------//
    //         load file
    //-----------------------------//
    auto jsonBufUPtr = read_a_file( path_file_ );

    //-----------------------------//
    //      parce JSON data
    //-----------------------------//
    Document doc;
    doc.Parse( jsonBufUPtr->c_str() );


    std::string  uiGoSpeciesName {};

    tprAssert( doc.IsArray() );
    for( auto &ent : doc.GetArray() ){

        {//--- goSpeciesName ---//
            const auto &a = check_and_get_value( ent, "goSpeciesName", JsonValType::String );
            uiGoSpeciesName = a.GetString();
        }

        UIGoSpecFromJson &uiGoJsonDataRef = UIGoSpecFromJson::create_new_UIGoSpecFromJson( uiGoSpeciesName );

        {//--- family ---//
            const auto &a = check_and_get_value( ent, "family", JsonValType::String );
            uiGoJsonDataRef.family = str_2_gameObjFamily( a.GetString() );
        }
        {//--- moveType ---//
            const auto &a = check_and_get_value( ent, "moveType", JsonValType::String );
            uiGoJsonDataRef.moveType = str_2_moveType( a.GetString() );
        }
        {//--- moveSpeedLvl ---//
            const auto &a = check_and_get_value( ent, "moveSpeedLvl", JsonValType::Int );
            uiGoJsonDataRef.moveSpeedLvl = int_2_SpeedLevel( a.GetInt() );
        }
    }
}



}//------------- namespace: uiJson_inn end --------------//
}//------------- namespace json: end ----------------


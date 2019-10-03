/*
 * ================= json_colorTable.cpp =====================
 *                          -- tpr --
 *                                        CREATE -- 2019.09.23
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
//--------------- CPP ------------------//
#include <unordered_map>
#include <string>
#include <utility>

//--------------- Libs ------------------//
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

#include "tprGeneral.h"
#include "tprDataType.h" 

//-------------------- Engine --------------------//
#include "tprAssert.h"
#include "global.h"
#include "fileIO.h"

#include "esrc_colorTableSet.h"
#include "esrc_state.h"

//--------------- Script ------------------//
#include "Script/json/json_all.h"
#include "Script/json/json_oth.h"
#include "Script/resource/ssrc.h" 

using namespace rapidjson;

#include <iostream>
using std::cout;
using std::endl;


namespace colorTableJson_inn {//-------- namespace: colorTableJson_inn --------------//

    const std::vector<std::string> lpath_files {
        "/colorTables/colorTables.json",
    };

    void parse_single_colorTableJsonFile( const std::string &lPath_file_ );

}//------------- namespace: colorTableJson_inn end --------------//



/* ===========================================================
 *                parse_from_colorTableJsonFile
 * -----------------------------------------------------------
 * Do Not Worry About Performance !!!
 */
void parse_from_colorTableJsonFile(){

    cout << "   ----- parse_from_colorTableJsonFile: start ----- " << endl;

    tprAssert( esrc::is_setState("colorTableSet") );

    for( const auto &i : colorTableJson_inn::lpath_files ){
        colorTableJson_inn::parse_single_colorTableJsonFile(i);
    }

    esrc::insertState("json_colorTableSet");
    cout << "   ----- parse_from_colorTableJsonFile: end ----- " << endl;
}

namespace colorTableJson_inn {//-------- namespace: colorTableJson_inn --------------//

void parse_single_colorTableJsonFile( const std::string &lPath_file_ ){
    //-----------------------------//
    //         load file
    //-----------------------------//
    std::string path_file = tprGeneral::path_combine(path_jsons, lPath_file_);
    auto jsonBufUPtr = read_a_file( path_file );

    //-----------------------------//
    //      parce JSON data
    //-----------------------------//
    Document doc;
    doc.Parse( jsonBufUPtr->c_str() );

    auto &colorTableSetRef = esrc::get_colorTabelSet();

    tprAssert( doc.IsArray() );
    for( auto &ent : doc.GetArray() ){

        std::string colorTableName {};        
        std::string colorName {};
        FloatVec4   colorVal {};
        int         iChannel {};
        std::vector<float> fChannels {};

        {//--- colorTableName ---//
            const auto &a = json_inn::check_and_get_value( ent, "colorTableName", json_inn::JsonValType::String );
            colorTableName = a.GetString();
        }

        ColorTable &colorTableRef = colorTableSetRef.apply_new_colorTable( colorTableName );

        {//--- colors ---//
            const auto &colors = json_inn::check_and_get_value( ent, "colors", json_inn::JsonValType::Array );
            for( auto &colorEnt : colors.GetArray() ){//- foreach colorEnt
                {//--- colorName ---//
                    const auto &a = json_inn::check_and_get_value( colorEnt, "name", json_inn::JsonValType::String );
                    colorName = a.GetString();
                }
                {//--- color ---//
                    const auto &a = json_inn::check_and_get_value( colorEnt, "color", json_inn::JsonValType::Array );
                    tprAssert( a.Size() == 4 );
                    //-- read colorVal --
                    fChannels.resize( 4, 0.0f );
                    for (SizeType i = 0; i < a.Size(); i++){
                        tprAssert( a[i].IsInt() );
                        iChannel = a[i].GetInt();
                        tprAssert( (iChannel>=0) && (iChannel<=255) );
                        fChannels[i] = static_cast<float>(iChannel) / 255.0f;
                    }
                    colorVal.r = fChannels[0];
                    colorVal.g = fChannels[1];
                    colorVal.b = fChannels[2];
                    colorVal.a = fChannels[3];
                }
                colorTableRef.insert_a_color( colorName, colorVal );
            }
            colorTableRef.final_check();
        }
    }
    colorTableSetRef.final_check();

}


}//------------- namespace: colorTableJson_inn end --------------//

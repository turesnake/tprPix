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

//-------------------- Engine --------------------//
#include "tprAssert.h"
#include "global.h"
#include "fileIO.h"

#include "json_oth.h"

#include "esrc_colorTableSet.h"
#include "esrc_state.h"

//--------------- Script ------------------//
#include "Script/json/json_all.h"


using namespace rapidjson;

#include <iostream>
using std::cout;
using std::endl;

namespace json{//------------- namespace json ----------------
namespace colorTableJson_inn {//-------- namespace: colorTableJson_inn --------------//

    void parse_single_colorTableJsonFile( const std::string &path_file_ );

}//------------- namespace: colorTableJson_inn end --------------//



/* Do Not Worry About Performance !!!
 */
void parse_colorTableJsonFile(){

    cout << "   ----- parse_colorTableJsonFile: start ----- " << endl;

    tprAssert( esrc::is_setState("colorTableSet") );

    std::vector<std::string> path_files {};
    collect_fileNames( path_jsons, "colorTables", "_files.json", path_files );
    //---
    for( const auto &i : path_files ){
        colorTableJson_inn::parse_single_colorTableJsonFile(i);
    }

    esrc::insertState("json_colorTableSet");
    cout << "   ----- parse_colorTableJsonFile: end ----- " << endl;
}

namespace colorTableJson_inn {//-------- namespace: colorTableJson_inn --------------//

void parse_single_colorTableJsonFile( const std::string &path_file_ ){
    //-----------------------------//
    //         load file
    //-----------------------------//
    auto jsonBufUPtr = read_a_file( path_file_ );

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
            const auto &a = check_and_get_value( ent, "colorTableName", JsonValType::String );
            colorTableName = a.GetString();
        }

        ColorTable &colorTableRef = colorTableSetRef.apply_new_colorTable( colorTableName );

        {//--- colors ---//
            const auto &colors = check_and_get_value( ent, "colors", JsonValType::Array );
            for( auto &colorEnt : colors.GetArray() ){//- foreach colorEnt
                {//--- colorName ---//
                    const auto &a = check_and_get_value( colorEnt, "name", JsonValType::String );
                    colorName = a.GetString();
                }
                {//--- color ---//
                    const auto &a = check_and_get_value( colorEnt, "color", JsonValType::Array );
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
        }

        colorTableRef.final_check();
    }
    colorTableSetRef.final_check();

}


}//------------- namespace: colorTableJson_inn end --------------//
}//------------- namespace json: end ----------------

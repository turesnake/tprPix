/*
 * ================ json_fieldDistributePlan.cpp =====================
 *                          -- tpr --
 *                                        CREATE -- 2019.10.07
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
#include "BodySize.h"
#include "MapField.h"

#include "json_oth.h"

#include "esrc_fieldDistributePlan.h"
#include "esrc_state.h"

//--------------- Script ------------------//
#include "Script/json/json_all.h"
#include "Script/resource/ssrc_all.h" 

using namespace rapidjson;

#include <iostream>
using std::cout;
using std::endl;


namespace json{//------------- namespace json ----------------
namespace fdpJson_inn {//-------- namespace: fdpJson_inn --------------//

    void parse_single_jsonFile( const std::string &path_file_,
                                FieldDistributePlanSet &planSetRef_ );

}//------------- namespace: fdpJson_inn end --------------//



/* ===========================================================
 *                parse_from_fieldDistributePlanJsonFile
 * -----------------------------------------------------------
 * Do Not Worry About Performance !!!
 */
void parse_from_fieldDistributePlanJsonFile(){

    cout << "   ----- parse_from_fieldDistributePlanGoJsonFile: start ----- " << endl;

    esrc::is_setState("fieldDistributePlanSet");


    auto &planSetRef = esrc::get_fieldDistributePlanSet();

    std::vector<std::string> path_files {};
    collect_fileNames( path_jsons, "fieldDistributePlan", "_files.json", path_files );
    //---
    for( const auto &i : path_files ){
        fdpJson_inn::parse_single_jsonFile( i, planSetRef );
    }

    //  do some check ........
    planSetRef.final_check();

    esrc::insertState("json_fieldDistributePlan");
    cout << "   ----- parse_from_fieldDistributePlanGoJsonFile: end ----- " << endl;
}




namespace fdpJson_inn {//-------- namespace: fdpJson_inn --------------//
/* ===========================================================
 *                parse_single_jsonFile
 * -----------------------------------------------------------
 */
void parse_single_jsonFile( const std::string &path_file_,
                            FieldDistributePlanSet &planSetRef_ ){
    //-----------------------------//
    //         load file
    //-----------------------------//
    auto jsonBufUPtr = read_a_file( path_file_ );

    //-----------------------------//
    //      parce JSON data
    //-----------------------------//
    Document doc;
    doc.Parse( jsonBufUPtr->c_str() );

    FieldDistributeType disType {};
    BodySize            bodySize {};
    glm::dvec2          dposOff {};
    
    tprAssert( doc.IsObject() );
    {//--- type ---//
        const auto &a = check_and_get_value( doc, "type", JsonValType::String );
        disType = str_2_fieldDistributeType( a.GetString() );
    }

    planSetRef_.perpare_targetType( disType ); // Must !

    //--- points ---//
    const auto &points = check_and_get_value( doc, "points", JsonValType::Array );
    for( const auto &point : points.GetArray() ){

        auto &planRef = planSetRef_.create_new_fieldDistributePlan( disType );

        //--- point ---//
        tprAssert( point.IsArray() );
        for( const auto &ent : point.GetArray() ){

            tprAssert( ent.IsObject() );
            {//--- type ---//
                const auto &a = check_and_get_value( ent, "BodySize", JsonValType::String );
                bodySize = str_2_bodySize( a.GetString() );
            }
            {//--- dpos ---//
                const auto &a = check_and_get_value( ent, "dpos", JsonValType::Array );
                const auto &pos = a.GetArray();
                tprAssert( pos.Size() == 2 );
                tprAssert( pos[0].IsInt() );
                tprAssert( pos[1].IsInt() );
                dposOff.x = static_cast<double>(pos[0].GetInt());
                dposOff.y = static_cast<double>(pos[1].GetInt());

                tprAssert( std::abs(dposOff.x) < MapField::halfField );
                tprAssert( std::abs(dposOff.y) < MapField::halfField );
            }

            planRef.push_back_point( bodySize, dposOff );
        }
    }

}



}//------------- namespace: fdpJson_inn end --------------//
}//------------- namespace json: end ----------------


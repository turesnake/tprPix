/*
 * ================= json_gameObj.cpp =====================
 *                          -- tpr --
 *                                        CREATE -- 2019.07.02
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

#include "GoSpecFromJson.h"

#include "json_oth.h"

#include "esrc_state.h"

//--------------- Script ------------------//
#include "Script/json/json_all.h"
#include "Script/resource/ssrc_all.h" 

using namespace rapidjson;

#include <iostream>
using std::cout;
using std::endl;

namespace json{//------------- namespace json ----------------
namespace goJson_inn {//-------- namespace: goJson_inn --------------//

    void parse_single_jsonFile( const std::string &path_file_ );

}//------------- namespace: goJson_inn end --------------//



/* ===========================================================
 *                parse_from_goJsonFile
 * -----------------------------------------------------------
 * Do Not Worry About Performance !!!
 */
void parse_from_goJsonFile(){

    cout << "   ----- parse_from_goJsonFile: start ----- " << endl;


    std::vector<std::string> path_files {};
    collect_fileNames( path_jsons, "gameObjs", "_files.json", path_files );

    for( const auto &i : path_files ){
        goJson_inn::parse_single_jsonFile(i);
    }

    esrc::insertState("json_gameObj");
    cout << "   ----- parse_from_goJsonFile: end ----- " << endl;
}

/* ===========================================================
 *              assemble_goSpec_2_newGo    
 * -----------------------------------------------------------
 */
void assemble_goSpec_2_newGo( goSpecId_t specID_, 
                                GameObj &goRef_ ){

    const auto &d = GoSpecFromJson::get_goSpecFromJsonRef( specID_ );

    goRef_.species   = d.specID;
    goRef_.parentId  = d.parentID;
    goRef_.family    = d.family;
    goRef_.state     = d.state;
    goRef_.moveState = d.moveState;
    goRef_.move.set_MoveType( d.moveType );

    goRef_.isTopGo  = d.isTopGo;
    goRef_.isMoveCollide = d.isMoveCollide;
    goRef_.get_collisionRef().set_isDoPass( d.isDoPass );
    goRef_.get_collisionRef().set_isBePass( d.isBePass );

    goRef_.move.set_speedLvl( d.speedLvl );
    goRef_.set_pos_alti( d.alti );
    goRef_.weight = d.weight;

    //------ pubBinary -------//
    goRef_.pubBinary.HP = d.pubBinary.HP;
    goRef_.pubBinary.MP = d.pubBinary.MP;

    //...
    //------ default -------//
    goRef_.isDirty = false;
    goRef_.isControlByPlayer = false;
    //...
    //------ tmp -------//
    goRef_.isActive = true; //- tmp. 是否进入激活圈，应该由 mpos 计算出来 未实现
    //...
}


namespace goJson_inn {//-------- namespace: goJson_inn --------------//

void parse_single_jsonFile( const std::string &path_file_ ){

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

        //GoJsonData  goJsonData {};

        goSpecId_t  specId {};
        {//--- specID ---//
            const auto &a = check_and_get_value( ent, "specID", JsonValType::Uint );
            specId = static_cast<goSpecId_t>( a.GetUint() );
        }

        auto &goSpecFromJsonRef = GoSpecFromJson::create_new_goSpecFromJson( specId );
        goSpecFromJsonRef.specID = specId;

        {//--- goSpecName ---//
            const auto &a = check_and_get_value( ent, "goSpecName", JsonValType::String );
            goSpecFromJsonRef.goSpecName = a.GetString();
        }
        {//--- parentID ---//
            const auto &a = check_and_get_value( ent, "parentID", JsonValType::Uint64 );
            goSpecFromJsonRef.parentID = static_cast<goid_t>( a.GetUint64() );
        }
        {//--- family ---//
            const auto &a = check_and_get_value( ent, "family", JsonValType::String );
            goSpecFromJsonRef.family = str_2_GameObjFamily( a.GetString() );
        }
        {//--- state ---//
            const auto &a = check_and_get_value( ent, "state", JsonValType::String );
            goSpecFromJsonRef.state = str_2_GameObjState( a.GetString() );
        }
        {//--- moveState ---//
            const auto &a = check_and_get_value( ent, "moveState", JsonValType::String );
            goSpecFromJsonRef.moveState = str_2_GameObjMoveState( a.GetString() );
        }
        {//--- moveType ---//
            const auto &a = check_and_get_value( ent, "moveType", JsonValType::String );
            goSpecFromJsonRef.moveType = str_2_MoveType( a.GetString() );
        }
        {//--- isTopGo ---//
            const auto &a = check_and_get_value( ent, "isTopGo", JsonValType::Bool );
            goSpecFromJsonRef.isTopGo = a.GetBool();
        }
        {//--- isMoveCollide ---//
            const auto &a = check_and_get_value( ent, "isMoveCollide", JsonValType::Bool );
            goSpecFromJsonRef.isMoveCollide = a.GetBool();
        }
        {//--- isDoPass ---//
            const auto &a = check_and_get_value( ent, "isDoPass", JsonValType::Bool );
            goSpecFromJsonRef.isDoPass = a.GetBool();
        }
        {//--- isBePass ---//
            const auto &a = check_and_get_value( ent, "isBePass", JsonValType::Bool );
            goSpecFromJsonRef.isBePass = a.GetBool();
        }
        {//--- afsNames ---//
            const auto &afsNames = check_and_get_value( ent, "afsNames", JsonValType::Array );
            for( const auto &i : afsNames.GetArray() ){
                tprAssert( i.IsString() );
                goSpecFromJsonRef.insert_2_afsNames( i.GetString() );
            }
        }
        {//--- speedLvl ---//
            const auto &a = check_and_get_value( ent, "speedLvl", JsonValType::Int );
            goSpecFromJsonRef.speedLvl = int_2_SpeedLevel( a.GetInt() );
        }
        {//--- alti ---//
            const auto &a = check_and_get_value( ent, "alti", JsonValType::Number );
            goSpecFromJsonRef.alti = get_double( a );
        }
        {//--- weight ---//
            const auto &a = check_and_get_value( ent, "weight", JsonValType::Number );
            goSpecFromJsonRef.weight = get_double( a );
        }
        {//--- pub.HP ---//
            tprAssert( ent.HasMember("pub.HP") );
            const Value &a = ent["pub.HP"];
            std::pair<bool,int> pair = get_nullable_int( a );
            goSpecFromJsonRef.pubBinary.HP = (pair.first) ? pair.second : -999; //- tmp
        }
        {//--- pub.MP ---//
            tprAssert( ent.HasMember("pub.MP") );
            const Value &a = ent["pub.MP"];
            std::pair<bool,int> pair = get_nullable_int( a );
            goSpecFromJsonRef.pubBinary.MP = (pair.first) ? pair.second : -999; //- tmp
        }

        //====================================//
        //  insert it into oth ssrc::containers
        //------------------------------------//
        ssrc::insert_2_go_specId_names_containers( goSpecFromJsonRef.specID, goSpecFromJsonRef.goSpecName );
    }
}

}//------------- namespace: goJson_inn end --------------//
}//------------- namespace json: end ----------------


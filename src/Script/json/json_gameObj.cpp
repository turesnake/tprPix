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

#include "esrc_state.h"

//--------------- Script ------------------//
#include "Script/json/json_all.h"
#include "Script/json/json_oth.h"
#include "Script/resource/ssrc.h" 

using namespace rapidjson;

#include <iostream>
using std::cout;
using std::endl;

namespace json{//------------- namespace json ----------------
namespace goJson_inn {//-------- namespace: goJson_inn --------------//

    const std::vector<std::string> lpath_files {
        "gameObjs/gos.json",
        "gameObjs/oth.json",
        "gameObjs/trees.json"
    };

    void parse_single_goJsonFile( const std::string &lPath_file_ );

}//------------- namespace: goJson_inn end --------------//



/* ===========================================================
 *                parse_from_goJsonFile
 * -----------------------------------------------------------
 * Do Not Worry About Performance !!!
 */
void parse_from_goJsonFile(){

    cout << "   ----- parse_from_goJsonFile: start ----- " << endl;

    for( const auto &i : goJson_inn::lpath_files ){
        goJson_inn::parse_single_goJsonFile(i);
    }

    esrc::insertState("json_gameObj");
    cout << "   ----- parse_from_goJsonFile: end ----- " << endl;
}

/* ===========================================================
 *               assemble_goJsonData_2_go     
 * -----------------------------------------------------------
 */
void assemble_goJsonData_2_newGo( goSpecId_t specID_, 
                                GameObj &goRef_ ){

    const auto &d = ssrc::get_goJsonData( specID_ );

    goRef_.species   = d.specID;
    goRef_.parentId  = d.parentID;
    goRef_.family    = d.family;
    goRef_.state     = d.state;
    goRef_.moveState = d.moveState;
    goRef_.move.set_MoveType( d.moveType );

    goRef_.isTopGo  = d.isTopGo;
    goRef_.isMoveCollide = d.isMoveCollide;
    goRef_.set_collision_isDoPass( d.isDoPass );
    goRef_.set_collision_isBePass( d.isBePass );

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

void parse_single_goJsonFile( const std::string &lPath_file_ ){

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

    tprAssert( doc.IsArray() );
    for( auto &ent : doc.GetArray() ){

        GoJsonData  goJsonData {};

        {//--- gameObjType ---//
            const auto &a = check_and_get_value( ent, "gameObjType", JsonValType::String );
            goJsonData.gameObjType = a.GetString();
        }
        {//--- specID ---//
            const auto &a = check_and_get_value( ent, "specID", JsonValType::Uint );
            goJsonData.specID = static_cast<goSpecId_t>( a.GetUint() );
        }
        {//--- parentID ---//
            const auto &a = check_and_get_value( ent, "parentID", JsonValType::Uint64 );
            goJsonData.parentID = static_cast<goid_t>( a.GetUint64() );
        }
        {//--- family ---//
            const auto &a = check_and_get_value( ent, "family", JsonValType::String );
            goJsonData.family = str_2_GameObjFamily( a.GetString() );
        }
        {//--- state ---//
            const auto &a = check_and_get_value( ent, "state", JsonValType::String );
            goJsonData.state = str_2_GameObjState( a.GetString() );
        }
        {//--- moveState ---//
            const auto &a = check_and_get_value( ent, "moveState", JsonValType::String );
            goJsonData.moveState = str_2_GameObjMoveState( a.GetString() );
        }
        {//--- moveType ---//
            const auto &a = check_and_get_value( ent, "moveType", JsonValType::String );
            goJsonData.moveType = str_2_MoveType( a.GetString() );
        }
        {//--- isTopGo ---//
            const auto &a = check_and_get_value( ent, "isTopGo", JsonValType::Bool );
            goJsonData.isTopGo = a.GetBool();
        }
        {//--- isMoveCollide ---//
            const auto &a = check_and_get_value( ent, "isMoveCollide", JsonValType::Bool );
            goJsonData.isMoveCollide = a.GetBool();
        }
        {//--- isDoPass ---//
            const auto &a = check_and_get_value( ent, "isDoPass", JsonValType::Bool );
            goJsonData.isDoPass = a.GetBool();
        }
        {//--- isBePass ---//
            const auto &a = check_and_get_value( ent, "isBePass", JsonValType::Bool );
            goJsonData.isBePass = a.GetBool();
        }
        {//--- speedLvl ---//
            const auto &a = check_and_get_value( ent, "speedLvl", JsonValType::Int );
            goJsonData.speedLvl = int_2_SpeedLevel( a.GetInt() );
        }
        {//--- alti ---//
            const auto &a = check_and_get_value( ent, "alti", JsonValType::Number );
            goJsonData.alti = get_double( a );
        }
        {//--- weight ---//
            const auto &a = check_and_get_value( ent, "weight", JsonValType::Number );
            goJsonData.weight = get_double( a );
        }
        {//--- pub.HP ---//
            tprAssert( ent.HasMember("pub.HP") );
            const Value &a = ent["pub.HP"];
            std::pair<bool,int> pair = get_nullable_int( a );
            (pair.first) ?
                goJsonData.pubBinary.HP = pair.second :
                goJsonData.pubBinary.HP = -999; //- tmp
        }
        {//--- pub.MP ---//
            tprAssert( ent.HasMember("pub.MP") );
            const Value &a = ent["pub.MP"];
            std::pair<bool,int> pair = get_nullable_int( a );
            (pair.first) ?
                goJsonData.pubBinary.MP = pair.second :
                goJsonData.pubBinary.MP = -999; //- tmp
        }

        //====================================//
        //  Now, the goJsonData is inited
        //  insert it into all ssrc::containers
        //------------------------------------//
        ssrc::insert_2_go_specId_names_containers( goJsonData.specID, goJsonData.gameObjType );
        ssrc::insert_2_go_jsonDatas( goJsonData );
    }
}

}//------------- namespace: goJson_inn end --------------//
}//------------- namespace json: end ----------------


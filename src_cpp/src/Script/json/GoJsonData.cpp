/*
 * ================= GoJsonData.cpp =====================
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

#include "SysConfig.h" // MUST BEFORE TPR_OS_WIN32_ !!!
#if defined TPR_OS_WIN32_ 
    #include "tprFileSys_win.h"
#elif defined TPR_OS_UNIX_ 
    #include "tprFileSys_unix.h"
#endif

//-------------------- Engine --------------------//
#include "tprAssert.h"
#include "global.h"


//--------------- Script ------------------//
#include "Script/json/GoJsonData.h"
#include "Script/json/json_oth.h"
#include "Script/resource/ssrc.h" 


using namespace rapidjson;

#include <iostream>
using std::cout;
using std::endl;


//namespace goJson_inn {//-------- namespace: goJson_inn --------------//
//}//------------- namespace: goJson_inn end --------------//

/* ===========================================================
 *                parse_from_goJsonFile
 * -----------------------------------------------------------
 * Do Not Worry About Performance !!!
 */
void parse_from_goJsonFile(){

    cout << "   ----- parse_from_goJsonFile: start ----- " << endl;

    //-----------------------------//
    //         load file
    //-----------------------------//
    std::string jsonBuf {};
    std::string path_file = tprGeneral::path_combine(path_jsons, "goJsonDatas.json");

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

        GoJsonData  goJsonData {};

        {//--- gameObjType ---//
            tprAssert( ent.HasMember("gameObjType") );
            const Value &a = ent["gameObjType"];
            tprAssert( a.IsString() );
            goJsonData.gameObjType = a.GetString();
        }
        {//--- specID ---//
            tprAssert( ent.HasMember("specID") );
            const Value &a = ent["specID"];
            tprAssert( a.IsUint() );
            goJsonData.specID = static_cast<goSpecId_t>( a.GetUint() );
        }
        {//--- parentID ---//
            tprAssert( ent.HasMember("parentID") );
            const Value &a = ent["parentID"];
            tprAssert( a.IsUint64() );
            goJsonData.parentID = static_cast<goid_t>( a.GetUint64() );
        }
        {//--- family ---//
            tprAssert( ent.HasMember("family") );
            const Value &a = ent["family"];
            tprAssert( a.IsString() );
            goJsonData.family = str_2_GameObjFamily( a.GetString() );
        }
        {//--- state ---//
            tprAssert( ent.HasMember("state") );
            const Value &a = ent["state"];
            tprAssert( a.IsString() );
            goJsonData.state = str_2_GameObjState( a.GetString() );
        }
        {//--- moveState ---//
            tprAssert( ent.HasMember("moveState") );
            const Value &a = ent["moveState"];
            tprAssert( a.IsString() );
            goJsonData.moveState = str_2_GameObjMoveState( a.GetString() );
        }
        {//--- moveType ---//
            tprAssert( ent.HasMember("moveType") );
            const Value &a = ent["moveType"];
            tprAssert( a.IsString() );
            goJsonData.moveType = str_2_MoveType( a.GetString() );
        }
        {//--- isTopGo ---//
            tprAssert( ent.HasMember("isTopGo") );
            const Value &a = ent["isTopGo"];
            tprAssert( a.IsBool() );
            goJsonData.isTopGo = a.GetBool();
        }
        {//--- isDoPass ---//
            tprAssert( ent.HasMember("isDoPass") );
            const Value &a = ent["isDoPass"];
            tprAssert( a.IsBool() );
            goJsonData.isDoPass = a.GetBool();
        }
        {//--- isBePass ---//
            tprAssert( ent.HasMember("isBePass") );
            const Value &a = ent["isBePass"];
            tprAssert( a.IsBool() );
            goJsonData.isBePass = a.GetBool();
        }
        {//--- speedLvl ---//
            tprAssert( ent.HasMember("speedLvl") );
            const Value &a = ent["speedLvl"];
            tprAssert( a.IsInt() );
            goJsonData.speedLvl = int_2_SpeedLevel( a.GetInt() );
        }
        {//--- alti ---//
            tprAssert( ent.HasMember("alti") );
            const Value &a = ent["alti"];
            tprAssert( a.IsNumber() );
            goJsonData.alti = json_inn::get_double( a );
        }
        {//--- weight ---//
            tprAssert( ent.HasMember("weight") );
            const Value &a = ent["weight"];
            tprAssert( a.IsNumber() );
            goJsonData.weight = json_inn::get_double( a );
        }
        {//--- pub.HP ---//
            tprAssert( ent.HasMember("pub.HP") );
            const Value &a = ent["pub.HP"];
            std::pair<bool,int> pair = json_inn::get_nullable_int( a );
            (pair.first) ?
                goJsonData.pubBinary.HP = pair.second :
                goJsonData.pubBinary.HP = -999; //- tmp
        }
        {//--- pub.MP ---//
            tprAssert( ent.HasMember("pub.MP") );
            const Value &a = ent["pub.MP"];
            std::pair<bool,int> pair = json_inn::get_nullable_int( a );
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

    cout << "   ----- parse_from_goJsonFile: end ----- " << endl;
}


/* ===========================================================
 *               assemble_goJsonData_2_go     
 * -----------------------------------------------------------
 * -- 
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
    goRef_.set_collision_isDoPass( d.isDoPass );
    goRef_.set_collision_isBePass( d.isBePass );

    goRef_.move.set_speedLvl( d.speedLvl );
    goRef_.goPos.set_alti( d.alti );
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






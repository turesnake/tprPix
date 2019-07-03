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
#include "Script/gameObjs/GoJsonData.h"
#include "Script/resource/ssrc.h" 

using namespace rapidjson;

#include <iostream>
using std::cout;
using std::endl;


namespace goJson_inn {//-------- namespace: goJson_inn --------------//
    float get_float( const Value &val_ );
}//------------- namespace: goJson_inn end --------------//


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
    std::string goJsonBuf {};
    std::string path_goJsonDatas = tprGeneral::path_combine(path_jsons, "goJsonDatas.json");

    //-- read files --
#if defined TPR_OS_WIN32_
    tprWin::file_load( path_goJsonDatas, goJsonBuf );
#elif defined TPR_OS_UNIX_
    tprUnix::file_load( path_goJsonDatas, goJsonBuf );
#endif

    //-----------------------------//
    //      parce JSON data
    //-----------------------------//
    Document doc;
    doc.Parse( goJsonBuf.c_str() );

    tprAssert( doc.IsArray() );
    for( auto &goEnt : doc.GetArray() ){

        GoJsonData  goJsonData {};

        //-------------------//
        //    gameObjType
        //-------------------//
        {
            tprAssert( goEnt.HasMember("gameObjType") );
            const Value &a = goEnt["gameObjType"];
            tprAssert( a.IsString() );
            goJsonData.gameObjType = a.GetString();
        }
        //-------------------//
        //      specID
        //-------------------//
        {
            tprAssert( goEnt.HasMember("specID") );
            const Value &a = goEnt["specID"];
            tprAssert( a.IsUint() );
            goJsonData.specID = static_cast<goSpecId_t>( a.GetUint() );
        }
        //-------------------//
        //     parentID
        //-------------------//
        {
            tprAssert( goEnt.HasMember("parentID") );
            const Value &a = goEnt["parentID"];
            tprAssert( a.IsUint64() );
            goJsonData.parentID = static_cast<goid_t>( a.GetUint64() );
        }
        //-------------------//
        //      family
        //-------------------//
        {
            tprAssert( goEnt.HasMember("family") );
            const Value &a = goEnt["family"];
            tprAssert( a.IsString() );
            goJsonData.family = str_2_GameObjFamily( a.GetString() );
        }
        //-------------------//
        //      state
        //-------------------//
        {
            tprAssert( goEnt.HasMember("state") );
            const Value &a = goEnt["state"];
            tprAssert( a.IsString() );
            goJsonData.state = str_2_GameObjState( a.GetString() );
        }
        //-------------------//
        //    moveState
        //-------------------//
        {
            tprAssert( goEnt.HasMember("moveState") );
            const Value &a = goEnt["moveState"];
            tprAssert( a.IsString() );
            goJsonData.moveState = str_2_GameObjMoveState( a.GetString() );
        }
        //-------------------//
        //     moveType
        //-------------------//
        {
            tprAssert( goEnt.HasMember("moveType") );
            const Value &a = goEnt["moveType"];
            tprAssert( a.IsString() );
            goJsonData.moveType = str_2_MoveType( a.GetString() );
        }

        //-------------------//
        //      isTopGo
        //-------------------//
        {
            tprAssert( goEnt.HasMember("isTopGo") );
            const Value &a = goEnt["isTopGo"];
            tprAssert( a.IsBool() );
            goJsonData.isTopGo = a.GetBool();
        }
        //-------------------//
        //      isActive
        //-------------------//
        {
            tprAssert( goEnt.HasMember("isActive") );
            const Value &a = goEnt["isActive"];
            tprAssert( a.IsBool() );
            goJsonData.isActive = a.GetBool();
        }
        //-------------------//
        //      isDoPass
        //-------------------//
        {
            tprAssert( goEnt.HasMember("isDoPass") );
            const Value &a = goEnt["isDoPass"];
            tprAssert( a.IsBool() );
            goJsonData.isDoPass = a.GetBool();
        }
        //-------------------//
        //      isBePass
        //-------------------//
        {
            tprAssert( goEnt.HasMember("isBePass") );
            const Value &a = goEnt["isBePass"];
            tprAssert( a.IsBool() );
            goJsonData.isBePass = a.GetBool();
        }
        //-------------------//
        //      speedLvl
        //-------------------//
        {
            tprAssert( goEnt.HasMember("speedLvl") );
            const Value &a = goEnt["speedLvl"];
            tprAssert( a.IsInt() );
            goJsonData.speedLvl = int_2_SpeedLevel( a.GetInt() );
        }
        //-------------------//
        //      alti
        //-------------------//
        {
            tprAssert( goEnt.HasMember("alti") );
            const Value &a = goEnt["alti"];
            tprAssert( a.IsNumber() );
            goJsonData.alti = goJson_inn::get_float( a );
        }
        //-------------------//
        //      weight
        //-------------------//
        {
            tprAssert( goEnt.HasMember("weight") );
            const Value &a = goEnt["weight"];
            tprAssert( a.IsNumber() );
            goJsonData.weight = goJson_inn::get_float( a );
        }

        //--------------------------------//
        //  Now, the goJsonData is inited
        //  insert it into all ssrc::containers
        //--------------------------------//
        ssrc::insert_2_go_specId_names_containers( goJsonData.specID, goJsonData.gameObjType );
        ssrc::insert_2_go_jsonDatas( goJsonData );

    }

    cout << "   ----- parse_from_goJsonFile: end ----- " << endl;
    //exit(789);
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
    goRef_.isActive = d.isActive;
    goRef_.set_collision_isDoPass( d.isDoPass );
    goRef_.set_collision_isBePass( d.isBePass );

    goRef_.move.set_speedLvl( d.speedLvl );
    goRef_.goPos.set_alti( d.alti );
    goRef_.weight = d.weight;
    //...
    //------ default -------//
    goRef_.isDirty = false;
    goRef_.isControlByPlayer = false;
    //...
}




namespace goJson_inn {//-------- namespace: goJson_inn --------------//


float get_float( const Value &val_ ){
    if( val_.IsFloat() ){
        return val_.GetFloat();
    }else if( val_.IsInt() ){
        return static_cast<float>( val_.GetInt() );
    }else{
        tprAssert(0);
        return 0.0f; //- never reach
    }
}

}//------------- namespace: goJson_inn end --------------//








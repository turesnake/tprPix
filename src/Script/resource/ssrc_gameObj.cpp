/*
 * ===================== ssrc_gameObj.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.10.09
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#include "Script/resource/ssrc_gameObj.h"


//-------------------- CPP --------------------//
#include <unordered_map>
#include <map>


//-------------------- Engine --------------------//
#include "tprAssert.h"


//--------------- Script ------------------//

#include "tprDebug.h" 


namespace ssrc {//------------------ namespace: ssrc -------------------------//
namespace go_inn {//------------------ namespace: go_inn -------------------------//

    //-- 资源持续整个游戏生命期，不用释放
    std::unordered_map<goSpecId_t, json::GoJsonData> go_jsonDatas {};

    std::unordered_map<goSpecId_t, std::string> go_specId_names {};
    std::unordered_map<std::string, goSpecId_t> go_name_specIds {};

    std::unordered_map<goSpecId_t, F_GO_INIT> goInit_funcs {}; 


}//--------------------- namespace: go_inn end -------------------------//



void insert_2_go_specId_names_containers( goSpecId_t id_, const std::string &name_ ){
    auto out1 = go_inn::go_specId_names.insert({ id_, name_ });
    auto out2 = go_inn::go_name_specIds.insert({ name_, id_ });
    tprAssert( out1.second );
    tprAssert( out2.second );
}

goSpecId_t get_goSpecId( const std::string &name_ ){
        if( go_inn::go_name_specIds.find(name_) == go_inn::go_name_specIds.end() ){
            cout << "can not find name_: " << name_ << endl;
        }
        tprAssert( go_inn::go_name_specIds.find(name_) != go_inn::go_name_specIds.end() );
    return go_inn::go_name_specIds.at(name_);
}



void insert_2_go_jsonDatas( const json::GoJsonData &goJsonData_ ){
    auto outPair = go_inn::go_jsonDatas.insert({ goJsonData_.specID, goJsonData_ }); //- copy 不考虑性能
    tprAssert( outPair.second );
}



const json::GoJsonData &get_goJsonData( goSpecId_t id_ ){
        tprAssert( go_inn::go_jsonDatas.find(id_) != go_inn::go_jsonDatas.end() );
    return go_inn::go_jsonDatas.at(id_);
}

bool find_from_goInit_funcs( goSpecId_t goSpecId_ ){
    return (go_inn::goInit_funcs.find(goSpecId_) != go_inn::goInit_funcs.end());
}


void call_goInit_func(  goSpecId_t id_,
                        GameObj &goRef_,
                        const DyParam &dyParams_  ){
    go_inn::goInit_funcs.at(id_)( goRef_, dyParams_ );
}


void insert_2_goInit_funcs( const std::string &goTypeName_,
                            const F_GO_INIT &functor_ ){
    goSpecId_t id = ssrc::get_goSpecId( goTypeName_ );
    auto outPair = go_inn::goInit_funcs.insert({ id, functor_ });
    tprAssert( outPair.second );
}



}//---------------------- namespace: ssrc -------------------------//


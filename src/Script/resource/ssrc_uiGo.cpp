/*
 * ======================= ssrc_uiGo.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.10.09
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#include "Script/resource/ssrc_uiGo.h" 


//-------------------- CPP --------------------//
#include <unordered_map>
#include <map>

//-------------------- Engine --------------------//
#include "tprAssert.h"


//--------------- Script ------------------//

#include "tprDebug.h" 


namespace ssrc {//------------------ namespace: ssrc -------------------------//
namespace ui_inn {//------------------ namespace: ui_inn -------------------------//


    std::unordered_map<goSpecId_t, json::UIGoJsonData> uiGo_jsonDatas {};

    std::unordered_map<goSpecId_t, std::string> uiGo_specId_names {};
    std::unordered_map<std::string, goSpecId_t> uiGo_name_specIds {};

    std::unordered_map<goSpecId_t, F_GO_INIT> uiGoInit_funcs {};
                                                    // 其实可以被整合进 goInit_funcs 中



}//--------------------- namespace: ui_inn end -------------------------//



void insert_2_uiGo_specId_names_containers( goSpecId_t id_, const std::string &name_ ){
    auto out1 = ui_inn::uiGo_specId_names.insert({ id_, name_ });
    auto out2 = ui_inn::uiGo_name_specIds.insert({ name_, id_ });
    tprAssert( out1.second );
    tprAssert( out2.second );
}


void insert_2_uiGo_jsonDatas( const json::UIGoJsonData &uiGoJsonData_ ){
    auto outPair = ui_inn::uiGo_jsonDatas.insert({ uiGoJsonData_.specID, uiGoJsonData_ }); //- copy 不考虑性能
    tprAssert( outPair.second );
}


goSpecId_t get_uiGoSpecId( const std::string &name_ ){
        if( ui_inn::uiGo_name_specIds.find(name_) == ui_inn::uiGo_name_specIds.end() ){
            cout << "can not find name_: " << name_ << endl;
        }
        tprAssert( ui_inn::uiGo_name_specIds.find(name_) != ui_inn::uiGo_name_specIds.end() );
    return ui_inn::uiGo_name_specIds.at(name_);
}


const json::UIGoJsonData &get_uiGoJsonData( goSpecId_t id_ ){
        tprAssert( ui_inn::uiGo_jsonDatas.find(id_) != ui_inn::uiGo_jsonDatas.end() );
    return ui_inn::uiGo_jsonDatas.at(id_);
}


bool find_from_uiGoInit_funcs( goSpecId_t goSpecId_ ){
    return (ui_inn::uiGoInit_funcs.find(goSpecId_) != ui_inn::uiGoInit_funcs.end());
}



void call_uiGoInit_func(  goSpecId_t id_,
                        GameObj &goRef_,
                        const DyParam &dyParams_  ){
    ui_inn::uiGoInit_funcs.at(id_)( goRef_, dyParams_ );
}



void insert_2_uiGoInit_funcs( const std::string &goTypeName_,
                            const F_GO_INIT &functor_ ){
    goSpecId_t id = ssrc::get_uiGoSpecId( goTypeName_ );
    auto outPair = ui_inn::uiGoInit_funcs.insert({ id, functor_ });
    tprAssert( outPair.second );
}



}//---------------------- namespace: ssrc -------------------------//



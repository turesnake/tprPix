/*
 * ========================= ssrc.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2018.12.10
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *   tmp
 * ----------------------------
 */
#include "Script/resource/ssrc.h" 


//-------------------- Engine --------------------//
#include "tprAssert.h"


//--------------- Script ------------------//
#include "Script/json/GoJsonData.h"
#include "Script/json/UIGoJsonData.h"

#include "tprDebug.h" 


namespace ssrc {//------------------ namespace: ssrc -------------------------//


namespace ssrc_inn {//------------------ namespace: ssrc_inn -------------------------//

    std::unordered_map<u32_t, std::string> acionHdle_typeId_names {};
    std::unordered_map<std::string, u32_t> acionHdle_name_typeIds {};


    //-- 资源持续整个游戏生命期，不用释放
    std::unordered_map<goSpecId_t, GoJsonData> go_jsonDatas {};

    std::unordered_map<goSpecId_t, std::string> go_specId_names {};
    std::unordered_map<std::string, goSpecId_t> go_name_specIds {};


    std::unordered_map<goSpecId_t, F_GO_INIT> goInit_funcs {}; 

    //--- uiGo ---
    std::unordered_map<goSpecId_t, UIGoJsonData> uiGo_jsonDatas {}; //- new

    std::unordered_map<goSpecId_t, std::string> uiGo_specId_names {}; //- new
    std::unordered_map<std::string, goSpecId_t> uiGo_name_specIds {}; //- new

    std::unordered_map<goSpecId_t, F_GO_INIT> uiGoInit_funcs {}; //- new
                                                    // 其实可以被整合进 goInit_funcs 中



}//--------------------- namespace: ssrc_inn end -------------------------//


/* ===========================================================
 *                  clear ...
 * -----------------------------------------------------------
 */
void clear_goInit_funcs(){
    ssrc_inn::goInit_funcs.clear();
}
void clear_uiGoInit_funcs(){
    ssrc_inn::uiGoInit_funcs.clear();
}
void clear_go_specId_names(){
    ssrc_inn::go_specId_names.clear();
}
void clear_go_name_specIds(){
    ssrc_inn::go_name_specIds.clear();
}
void clear_uiGo_specId_names(){
    ssrc_inn::uiGo_specId_names.clear();
}
void clear_uiGo_name_specIds(){
    ssrc_inn::uiGo_name_specIds.clear();
}




void insert_2_go_specId_names_containers( goSpecId_t id_, const std::string &name_ ){
    ssrc_inn::go_specId_names.insert({ id_, name_ });
    ssrc_inn::go_name_specIds.insert({ name_, id_ });
}

void insert_2_uiGo_specId_names_containers( goSpecId_t id_, const std::string &name_ ){
    ssrc_inn::uiGo_specId_names.insert({ id_, name_ });
    ssrc_inn::uiGo_name_specIds.insert({ name_, id_ });
}

void insert_2_go_jsonDatas( const GoJsonData &goJsonData_ ){
    ssrc_inn::go_jsonDatas.insert({ goJsonData_.specID, goJsonData_ }); //- copy 不考虑性能
}

void insert_2_uiGo_jsonDatas( const UIGoJsonData &uiGoJsonData_ ){
    ssrc_inn::uiGo_jsonDatas.insert({ uiGoJsonData_.specID, uiGoJsonData_ }); //- copy 不考虑性能
}


/* ===========================================================
 *                     get_goSpecId
 * -----------------------------------------------------------
 */
goSpecId_t get_goSpecId( const std::string &name_ ){
        if( ssrc_inn::go_name_specIds.find(name_) == ssrc_inn::go_name_specIds.end() ){
            cout << "can not find name_: " << name_ << endl;
        }
        tprAssert( ssrc_inn::go_name_specIds.find(name_) != ssrc_inn::go_name_specIds.end() );
    return ssrc_inn::go_name_specIds.at(name_);
}


goSpecId_t get_uiGoSpecId( const std::string &name_ ){
        if( ssrc_inn::uiGo_name_specIds.find(name_) == ssrc_inn::uiGo_name_specIds.end() ){
            cout << "can not find name_: " << name_ << endl;
        }
        tprAssert( ssrc_inn::uiGo_name_specIds.find(name_) != ssrc_inn::uiGo_name_specIds.end() );
    return ssrc_inn::uiGo_name_specIds.at(name_);
}


/* ===========================================================
 *                     get_goJsonData
 * -----------------------------------------------------------
 */
const GoJsonData &get_goJsonData( goSpecId_t id_ ){
        tprAssert( ssrc_inn::go_jsonDatas.find(id_) != ssrc_inn::go_jsonDatas.end() );
    return ssrc_inn::go_jsonDatas.at(id_);
}


const UIGoJsonData &get_uiGoJsonData( goSpecId_t id_ ){
        tprAssert( ssrc_inn::uiGo_jsonDatas.find(id_) != ssrc_inn::uiGo_jsonDatas.end() );
    return ssrc_inn::uiGo_jsonDatas.at(id_);
}


/* ===========================================================
 *                  find_from_goInit_funcs
 * -----------------------------------------------------------
 */
bool find_from_goInit_funcs( goSpecId_t goSpecId_ ){
    return (ssrc_inn::goInit_funcs.find(goSpecId_) != ssrc_inn::goInit_funcs.end());
}

bool find_from_uiGoInit_funcs( goSpecId_t goSpecId_ ){
    return (ssrc_inn::uiGoInit_funcs.find(goSpecId_) != ssrc_inn::uiGoInit_funcs.end());
}


/* ===========================================================
 *                  call_goInit_func
 * -----------------------------------------------------------
 */
void call_goInit_func(  goSpecId_t id_,
                        GameObj &goRef_,
                        const ParamBinary &dyParams_  ){
    ssrc_inn::goInit_funcs.at(id_)( goRef_, dyParams_ );
}


void call_uiGoInit_func(  goSpecId_t id_,
                        GameObj &goRef_,
                        const ParamBinary &dyParams_  ){
    ssrc_inn::uiGoInit_funcs.at(id_)( goRef_, dyParams_ );
}


/* ===========================================================
 *                 insert_2_goInit_funcs
 * -----------------------------------------------------------
 */
void insert_2_goInit_funcs( const std::string &goTypeName_,
                            const F_GO_INIT &functor_ ){
    goSpecId_t id = ssrc::get_goSpecId( goTypeName_ );
    ssrc_inn::goInit_funcs.insert({ id, functor_ });
}


void insert_2_uiGoInit_funcs( const std::string &goTypeName_,
                            const F_GO_INIT &functor_ ){
    goSpecId_t id = ssrc::get_uiGoSpecId( goTypeName_ );
    ssrc_inn::uiGoInit_funcs.insert({ id, functor_ });
}



}//---------------------- namespace: ssrc -------------------------//



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


namespace ssrc {//------------------ namespace: ssrc -------------------------//


namespace _inn {//------------------ namespace: _inn -------------------------//

    std::unordered_map<u32_t, std::string> acionHdle_typeId_names {};
    std::unordered_map<std::string, u32_t> acionHdle_name_typeIds {};


    std::unordered_map<goSpecId_t, std::string> go_specId_names {};
    std::unordered_map<std::string, goSpecId_t> go_name_specIds {};


    std::unordered_map<goSpecId_t, F_GO_INIT> goInit_funcs {}; 

    std::unordered_map<uiObjSpecId_t, std::string> ui_specId_names {};
    std::unordered_map<std::string, uiObjSpecId_t> ui_name_specIds {};



    std::unordered_map<uiObjSpecId_t, F_UI_INIT> uiInit_funcs {}; 

}//--------------------- namespace: _inn end -------------------------//


/* ===========================================================
 *                  clear ...
 * -----------------------------------------------------------
 */
void clear_uiInit_funcs(){
    _inn::uiInit_funcs.clear();
}
void clear_goInit_funcs(){
    _inn::goInit_funcs.clear();
}
void clear_go_specId_names(){
    _inn::go_specId_names.clear();
}
void clear_go_name_specIds(){
    _inn::go_name_specIds.clear();
}
void clear_ui_specId_names(){
    _inn::ui_specId_names.clear();
}
void clear_ui_name_specIds(){
    _inn::ui_name_specIds.clear();
}



void insert_2_go_specId_names_containers( goSpecId_t id_, const std::string &name_ ){
    _inn::go_specId_names.insert({ id_, name_ });
    _inn::go_name_specIds.insert({ name_, id_ });
}

void insert_2_ui_specId_names_containers( uiObjSpecId_t id_, const std::string &name_ ){
    _inn::ui_specId_names.insert({ id_, name_ });
    _inn::ui_name_specIds.insert({ name_, id_ });
}



/* ===========================================================
 *                     get_goSpecId
 * -----------------------------------------------------------
 */
goSpecId_t get_goSpecId( const std::string &name_ ){
        tprAssert( _inn::go_name_specIds.find(name_) != _inn::go_name_specIds.end() );
    return _inn::go_name_specIds.at(name_);
}


/* ===========================================================
 *                     get_uiSpecId
 * -----------------------------------------------------------
 */
uiObjSpecId_t get_uiSpecId( const std::string &name_ ){
        tprAssert( _inn::ui_name_specIds.find(name_) != _inn::ui_name_specIds.end() );
    return _inn::ui_name_specIds.at(name_);
}


/* ===========================================================
 *                  find_from_goInit_funcs
 * -----------------------------------------------------------
 */
bool find_from_goInit_funcs( goSpecId_t goSpecId_ ){
    return (_inn::goInit_funcs.find(goSpecId_) != _inn::goInit_funcs.end());
}


/* ===========================================================
 *                  call_goInit_func
 * -----------------------------------------------------------
 */
void call_goInit_func(  goSpecId_t id_,
                        GameObj &goRef_,
                        const IntVec2 &mpos_,
					    float fieldWeight_,
					    const MapAltitude &alti_,
					    const Density &density_  ){
    
    _inn::goInit_funcs.at(id_)( goRef_, mpos_, fieldWeight_, alti_, density_ );
}


/* ===========================================================
 *                 insert_2_goInit_funcs
 * -----------------------------------------------------------
 */
void insert_2_goInit_funcs( goSpecId_t goSpecId_,
                            const F_GO_INIT &functor_ ){
    _inn::goInit_funcs.insert({ goSpecId_, functor_ });
}




/* ===========================================================
 *                  find_from_uiInit_funcs
 * -----------------------------------------------------------
 */
bool find_from_uiInit_funcs( uiObjSpecId_t uiSpecId_ ){
    return (_inn::uiInit_funcs.find(uiSpecId_) != _inn::uiInit_funcs.end());
}



/* ===========================================================
 *                 call_uiInit_func
 * -----------------------------------------------------------
 */
void call_uiInit_func(  uiObjSpecId_t id_,  
                        UIObj *uiObjPtr_,
                        const glm::vec2 &fpos_ ){
    _inn::uiInit_funcs.at(id_)( uiObjPtr_, fpos_ );           
}


/* ===========================================================
 *               insert_2_uiInit_funcs
 * -----------------------------------------------------------
 */
void insert_2_uiInit_funcs( uiObjSpecId_t id_,
                            const F_UI_INIT &functor_ ){

    _inn::uiInit_funcs.insert({ id_, functor_ });
}




}//---------------------- namespace: ssrc -------------------------//



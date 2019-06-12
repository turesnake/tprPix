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



void insert_2_go_specId_names_containers( goSpecId_t _id, const std::string &_name ){
    _inn::go_specId_names.insert({ _id, _name });
    _inn::go_name_specIds.insert({ _name, _id });
}

void insert_2_ui_specId_names_containers( uiObjSpecId_t _id, const std::string &_name ){
    _inn::ui_specId_names.insert({ _id, _name });
    _inn::ui_name_specIds.insert({ _name, _id });
}



/* ===========================================================
 *                     get_goSpecId
 * -----------------------------------------------------------
 */
goSpecId_t get_goSpecId( const std::string &_name ){
        tprAssert( _inn::go_name_specIds.find(_name) != _inn::go_name_specIds.end() );
    return _inn::go_name_specIds.at(_name);
}


/* ===========================================================
 *                     get_uiSpecId
 * -----------------------------------------------------------
 */
uiObjSpecId_t get_uiSpecId( const std::string &_name ){
        tprAssert( _inn::ui_name_specIds.find(_name) != _inn::ui_name_specIds.end() );
    return _inn::ui_name_specIds.at(_name);
}


/* ===========================================================
 *                  find_from_goInit_funcs
 * -----------------------------------------------------------
 */
bool find_from_goInit_funcs( goSpecId_t _goSpecId ){
    return (_inn::goInit_funcs.find(_goSpecId) != _inn::goInit_funcs.end());
}


/* ===========================================================
 *                  call_goInit_func
 * -----------------------------------------------------------
 */
void call_goInit_func(  goSpecId_t _id,
                        GameObj *_goPtr,
                        const IntVec2 &_mpos,
					    float _fieldWeight,
					    const MapAltitude &_alti,
					    const Density &_density  ){
    
    _inn::goInit_funcs.at(_id)( _goPtr, _mpos, _fieldWeight, _alti, _density );
}


/* ===========================================================
 *                 insert_2_goInit_funcs
 * -----------------------------------------------------------
 */
void insert_2_goInit_funcs( goSpecId_t _goSpecId,
                            const F_GO_INIT &_functor ){
    _inn::goInit_funcs.insert({ _goSpecId, _functor });
}




/* ===========================================================
 *                  find_from_uiInit_funcs
 * -----------------------------------------------------------
 */
bool find_from_uiInit_funcs( uiObjSpecId_t _uiSpecId ){
    return (_inn::uiInit_funcs.find(_uiSpecId) != _inn::uiInit_funcs.end());
}



/* ===========================================================
 *                 call_uiInit_func
 * -----------------------------------------------------------
 */
void call_uiInit_func(  uiObjSpecId_t _id,  
                        UIObj *_uiObjPtr,
                        const glm::vec2 &_fpos ){
    _inn::uiInit_funcs.at(_id)( _uiObjPtr, _fpos );           
}


/* ===========================================================
 *               insert_2_uiInit_funcs
 * -----------------------------------------------------------
 */
void insert_2_uiInit_funcs( uiObjSpecId_t _id,
                            const F_UI_INIT &_functor ){

    _inn::uiInit_funcs.insert({ _id, _functor });
}




}//---------------------- namespace: ssrc -------------------------//



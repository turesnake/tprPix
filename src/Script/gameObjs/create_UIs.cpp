/*
 * ========================= create_UIs.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.04.30
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#include "Script/gameObjs/create_UIs.h"


//-------------------- C --------------------//
#include <cassert>

//-------------------- Engine --------------------//
//#include "esrc_gameObj.h" 
#include "esrc_ui.h" 


//--------------- Script ------------------//
//#include "Script/gameObjs/allGoes.h"
#include "Script/gameObjs/allUIs.h"
#include "Script/resource/srcs_script.h"


#include "debug.h"


namespace uis{//------------- namespace uis ----------------



/* ===========================================================
 *                  create_a_ui
 * -----------------------------------------------------------
 * -- tmp 
 */
goid_t create_a_ui( goSpecId_t _uiSpecId,
                    const IntVec2 &_mpos ){

    goid_t goid = esrc::insert_new_ui();
    GameObj *goPtr = esrc::get_memUIPtr( goid ); //- 获取目标go指针

        assert( ssrc::find_from_uiInit_funcs(_uiSpecId) );
        ssrc::uiInit_funcs.at(_uiSpecId)(   goPtr,
                                            _mpos );

    //------------------------------//
    //esrc::signUp_newGO_to_mapEnt( goPtr );
        esrc::uiIds_active.insert( goid ); //- tmp
    return  goid;
}


}//------------- namespace uis: end ----------------


/*
 * ========================= create_goes.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.04.10
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#include "Script/gameObjs/create_goes.h"


//-------------------- C --------------------//
#include <cassert>

//-------------------- Engine --------------------//
#include "esrc_gameObj.h" 


//--------------- Script ------------------//
#include "Script/gameObjs/allGoes.h"
#include "Script/resource/srcs_script.h"


#include "debug.h"


namespace gameObjs{//------------- namespace gameObjs ----------------



/* ===========================================================
 *                  create_a_Go
 * -----------------------------------------------------------
 * -- tmp 
 */
goid_t create_a_Go( goSpecId_t _goSpecId,
                    const IntVec2 &_mpos,
					float _fieldWeight,
					const Altitude &_alti,
					const Density &_density ){

    goid_t goid = esrc::insert_new_gameObj();
    GameObj *goPtr = esrc::get_memGameObjPtr( goid ); //- 获取目标go指针

        assert( ssrc::find_from_goInit_funcs(_goSpecId) );
        ssrc::goInit_funcs.at(_goSpecId)(   goPtr,
                                            _mpos,
                                            _fieldWeight,
                                            _alti,
                                            _density  );

    //------------------------------//
    esrc::signUp_newGO_to_mapEnt( goPtr );
        esrc::goids_active.insert( goid ); //- tmp
    return  goid;
}


}//------------- namespace gameObjs: end ----------------


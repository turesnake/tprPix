/*
 * ========================= create_goes.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.04.10
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#include "Script/gameObjs/create_goes.h"

//-------------------- Engine --------------------//
#include "tprAssert.h"
#include "esrc_gameObj.h" 


//--------------- Script ------------------//
#include "Script/gameObjs/allGoes.h"
#include "Script/resource/ssrc.h"


#include "tprDebug.h"


namespace gameObjs{//------------- namespace gameObjs ----------------



/* ===========================================================
 *                  create_a_Go       [tmp]
 * -----------------------------------------------------------
 * -- tmp 
 */
goid_t create_a_Go( goSpecId_t goSpecId_,
                    const IntVec2 &_mpos,
					float fieldWeight_,
					const MapAltitude &alti_,
					const Density &_density ){

    goid_t goid = esrc::insert_new_gameObj();
    GameObj &goRef = esrc::get_goRef( goid );

        tprAssert( ssrc::find_from_goInit_funcs(goSpecId_) );
        ssrc::call_goInit_func( goSpecId_,
                                goRef,
                                _mpos,
                                fieldWeight_,
                                alti_,
                                _density );

    //------------------------------//
    esrc::signUp_newGO_to_mapEnt( goRef );
        esrc::get_goids_active().insert( goid ); //- tmp
    
    return  goid;
}



/* ===========================================================
 *                  rebind_a_disk_Go       [tmp]
 * -----------------------------------------------------------
 * 从 db读取一个 go 的数据，并用此数据，重建一个 mem态 go实例
 */
void rebind_a_disk_Go( const DiskGameObj &_diskGo,
                        float fieldWeight_,
					    const MapAltitude &alti_,
					    const Density &_density  ){

    esrc::insert_a_disk_gameObj( _diskGo.goid );
    GameObj &goRef = esrc::get_goRef( _diskGo.goid );

    tprAssert( ssrc::find_from_goInit_funcs(_diskGo.goSpecId) );
    ssrc::call_goInit_func( _diskGo.goSpecId,
                            goRef,
                            _diskGo.mpos,
                            fieldWeight_,
                            alti_,
                            _density );

            //-- 临时方案，最好使用 具象go类 rebind 系列函数 
            
    //------------------------------//
    esrc::signUp_newGO_to_mapEnt( goRef );
        esrc::get_goids_active().insert( _diskGo.goid ); //- tmp
}



}//------------- namespace gameObjs: end ----------------


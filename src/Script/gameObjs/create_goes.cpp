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
#include "Script/resource/ssrc.h"


#include "tprDebug.h"


namespace gameObjs{//------------- namespace gameObjs ----------------



/* ===========================================================
 *                  create_a_Go       [tmp]
 * -----------------------------------------------------------
 * -- tmp 
 */
goid_t create_a_Go( goSpecId_t _goSpecId,
                    const IntVec2 &_mpos,
					float _fieldWeight,
					const MapAltitude &_alti,
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



/* ===========================================================
 *                  rebind_a_disk_Go       [tmp]
 * -----------------------------------------------------------
 * 从 db读取一个 go 的数据，并用此数据，重建一个 mem态 go实例
 */
void rebind_a_disk_Go( const DiskGameObj &_diskGo,
                        float _fieldWeight,
					    const MapAltitude &_alti,
					    const Density &_density  ){

    esrc::insert_a_disk_gameObj( _diskGo.goid );
    GameObj *goPtr = esrc::get_memGameObjPtr( _diskGo.goid ); //- 获取目标go指针

    assert( ssrc::find_from_goInit_funcs(_diskGo.goSpecId) );
    ssrc::goInit_funcs.at(_diskGo.goSpecId)(   goPtr,
                                        _diskGo.mpos,
                                        _fieldWeight,
                                        _alti,
                                        _density  );

            //-- 临时方案，最好使用 具象go类 rebind 系列函数 
            
    //------------------------------//
    esrc::signUp_newGO_to_mapEnt( goPtr );
        esrc::goids_active.insert( _diskGo.goid ); //- tmp
}






}//------------- namespace gameObjs: end ----------------


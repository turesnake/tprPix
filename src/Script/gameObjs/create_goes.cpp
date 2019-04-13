/*
 * ========================= create_goes.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.04.10
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#include "Script/gameObjs/create_goes.h"


//-------------------- Engine --------------------//
#include "srcs_engine.h" 


//-------------------- Script --------------------//
#include "Script/gameObjs/Norman.h"
#include "Script/gameObjs/BigMan.h"
#include "Script/gameObjs/OakTree.h"
#include "Script/gameObjs/mapSurfaces/Lichen.h"
#include "Script/gameObjs/bush/Wheat.h"
#include "Script/gameObjs/bush/SingleStone_Desert.h"


namespace gameObjs{//------------- namespace gameObjs ----------------


/* ===========================================================
 *                  create_a_Norman 
 * -----------------------------------------------------------
 * -- tmp 
 */
goid_t create_a_Norman( const IntVec2 &_mpos ){
    goid_t goid = esrc::insert_new_gameObj();
    GameObj *goPtr = esrc::get_memGameObjPtr( goid ); //- 获取目标go指针

        gameObjs::norman.init( goPtr );

    goPtr->goPos.init_by_currentMCPos( MapCoord{_mpos} );
    //------------------------------//
    esrc::signUp_newGO_to_mapEnt( goPtr );
        esrc::goids_active.insert( goid ); //- tmp
    return  goid;
}



/* ===========================================================
 *                   create_a_BigMan
 * -----------------------------------------------------------
 * -- tmp 
 */
goid_t create_a_BigMan( const IntVec2 &_mpos ){
    goid_t goid = esrc::insert_new_gameObj();
    GameObj *goPtr = esrc::get_memGameObjPtr( goid ); //- 获取目标go指针

        gameObjs::big_man.init( goPtr );

    goPtr->goPos.init_by_currentMCPos( MapCoord{_mpos} );
    //------------------------------//
    esrc::signUp_newGO_to_mapEnt( goPtr );
        esrc::goids_active.insert( goid ); //- tmp
    return goid;
}


/* ===========================================================
 *                   create_a_OakTree  
 * -----------------------------------------------------------
 * -- tmp 
 */
goid_t create_a_OakTree( const IntVec2 &_mpos, 
                        int _age, 
                        bool _isSingleTrunk,
                        bool _isFlipOver ){
    goid_t goid = esrc::insert_new_gameObj();
    GameObj *goPtr = esrc::get_memGameObjPtr( goid ); //- 获取目标go指针

        gameObjs::oakTree.init( goPtr, _age, _isSingleTrunk, _isFlipOver );

    goPtr->goPos.init_by_currentMCPos( MapCoord{_mpos} );
    //------------------------------//
    esrc::signUp_newGO_to_mapEnt( goPtr );
        esrc::goids_active.insert( goid ); //- tmp
    return goid;
}



/* ===========================================================
 *                  create_a_Lichen
 * -----------------------------------------------------------
 * -- tmp 
 */
goid_t create_a_Lichen( const IntVec2 &_mpos,
                        float _fieldWeight ){
    goid_t goid = esrc::insert_new_gameObj();
    GameObj *goPtr = esrc::get_memGameObjPtr( goid ); //- 获取目标go指针

        gameObjs::lichen.init( goPtr, _fieldWeight );

    goPtr->goPos.init_by_currentMCPos( MapCoord{_mpos} );
    //------------------------------//
    esrc::signUp_newGO_to_mapEnt( goPtr );
        esrc::goids_active.insert( goid ); //- tmp
    return  goid;
}



/* ===========================================================
 *                  create_a_Wheat
 * -----------------------------------------------------------
 * -- tmp 
 */
goid_t create_a_Wheat( const IntVec2 &_mpos,
                        float _fieldWeight ){
    goid_t goid = esrc::insert_new_gameObj();
    GameObj *goPtr = esrc::get_memGameObjPtr( goid ); //- 获取目标go指针

        gameObjs::wheat.init( goPtr, _fieldWeight );

    goPtr->goPos.init_by_currentMCPos( MapCoord{_mpos} );
    //------------------------------//
    esrc::signUp_newGO_to_mapEnt( goPtr );
        esrc::goids_active.insert( goid ); //- tmp
    return  goid;
}



/* ===========================================================
 *                  create_a_SingleStone_Desert
 * -----------------------------------------------------------
 * -- tmp 
 */
goid_t create_a_SingleStone_Desert( const IntVec2 &_mpos,
                                    float _fieldWeight ){
    goid_t goid = esrc::insert_new_gameObj();
    GameObj *goPtr = esrc::get_memGameObjPtr( goid ); //- 获取目标go指针

        gameObjs::singleStone_Desert.init( goPtr, _fieldWeight );

    goPtr->goPos.init_by_currentMCPos( MapCoord{_mpos} );
    //------------------------------//
    esrc::signUp_newGO_to_mapEnt( goPtr );
        esrc::goids_active.insert( goid ); //- tmp
    return  goid;
}





}//------------- namespace gameObjs: end ----------------


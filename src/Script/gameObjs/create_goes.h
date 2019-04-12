/*
 * ========================= create_goes.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.04.10
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#ifndef _TPR_CREATE_GOES_H_
#define _TPR_CREATE_GOES_H_


//-------------------- Engine --------------------//
#include "IntVec.h"
#include "GameObjType.h"


namespace gameObjs{//------------- namespace gameObjs ----------------


goid_t create_a_Norman( const IntVec2 &_mpos );
goid_t create_a_BigMan( const IntVec2 &_mpos );
goid_t create_a_OakTree( const IntVec2 &_mpos, 
                        int _age, 
                        bool _isSingleTrunk,
                        bool _isFlipOver );
                        
goid_t create_a_Lichen( const IntVec2 &_mpos,
                        float _fieldWeight );

goid_t create_a_Wheat( const IntVec2 &_mpos,
                        float _fieldWeight );

goid_t create_a_SingleStone_Desert( const IntVec2 &_mpos,
                                    float _fieldWeight );





}//------------- namespace gameObjs: end ----------------
#endif 


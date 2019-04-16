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
#include "Altitude.h"
#include "Density.h"


namespace gameObjs{//------------- namespace gameObjs ----------------


goid_t create_a_Go( goSpecId_t _goSpecId,
                    const IntVec2 &_mpos,
					float _fieldWeight,
					const Altitude &_alti,
					const Density &_density );


}//------------- namespace gameObjs: end ----------------
#endif 


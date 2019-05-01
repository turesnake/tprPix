/*
 * ========================= create_UIs.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.04.30
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#ifndef _TPR_CREATE_UIS_H_
#define _TPR_CREATE_UIS_H_


//-------------------- Engine --------------------//
#include "IntVec.h"
#include "GameObjType.h"


namespace uis{//------------- namespace uis ----------------


goid_t create_a_ui( goSpecId_t _goSpecId,
                    const IntVec2 &_mpos );


}//------------- namespace uis: end ----------------
#endif 


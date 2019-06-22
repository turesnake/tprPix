/*
 * ========================= create_UIs.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.04.30
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#ifndef TPR_CREATE_UIS_H_
#define TPR_CREATE_UIS_H_

//--- glm - 0.9.9.5 ---
#include "glm_no_warnings.h"


//-------------------- Engine --------------------//
#include "IntVec.h"
#include "UIObjType.h"


namespace uis{//------------- namespace uis ----------------


uiObjId_t create_a_ui( uiObjSpecId_t _uiObjSpecId,
                    const glm::vec2 &_fpos );


}//------------- namespace uis: end ----------------
#endif 


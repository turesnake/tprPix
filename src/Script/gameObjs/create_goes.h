/*
 * ========================= create_goes.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.04.10
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#ifndef TPR_CREATE_GOES_H
#define TPR_CREATE_GOES_H

//--- glm - 0.9.9.5 ---
#include "glm_no_warnings.h"

//-------------------- Engine --------------------//
#include "IntVec.h"
//#include "GameObjType.h"
#include "MapAltitude.h"
#include "Density.h"
#include "DiskGameObj.h"
#include "ParamBinary.h"


namespace gameObjs{//------------- namespace gameObjs ----------------


goid_t create_a_Go( goSpecId_t goSpecId_,
                    const IntVec2 mpos_, 
                    const IntVec2 pposOff_,
                    const ParamBinary &dyParams_ );


void rebind_a_disk_Go(  const DiskGameObj &diskGo_,
                        const IntVec2 mpos_, 
                        const IntVec2 pposOff_,
                        const ParamBinary &dyParams_  );

}//------------- namespace gameObjs: end ----------------


namespace uiGos{//------------- namespace uiGos ----------------

goid_t create_a_UIGo( goSpecId_t goSpecId_,
                    const IntVec2 mpos_, 
                    const IntVec2 pposOff_,
                    const ParamBinary &dyParams_ );

}//------------- namespace uiGos: end ----------------

#endif 


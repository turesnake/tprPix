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
#include "MapAltitude.h"
#include "Density.h"
#include "DiskGameObj.h"
#include "ParamBinary.h"
#include "UIAnchor.h"


namespace gameObjs{//------------- namespace gameObjs ----------------


goid_t create_a_Go( goSpecId_t goSpecId_,
                    const glm::dvec2 &dpos_,
                    const ParamBinary &dyParams_ );


void rebind_a_disk_Go(  const DiskGameObj &diskGo_,
                        const glm::dvec2 &dpos_,
                        const ParamBinary &dyParams_  );

}//------------- namespace gameObjs: end ----------------


namespace uiGos{//------------- namespace uiGos ----------------

goid_t create_a_UIGo( goSpecId_t goSpecId_,
                    const glm::dvec2 &basePointProportion_, 
                    const glm::dvec2 &offDPos_,
                    const ParamBinary &dyParams_ );


goid_t create_a_UIGo( goSpecId_t goSpecId_,
                    const UIAnchor &uiAnchor_,
                    const ParamBinary &dyParams_ );




}//------------- namespace uiGos: end ----------------

#endif 


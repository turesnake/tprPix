/*
 * ======================= UBO_Camera.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.09.24
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#ifndef TPR_UBO_CAMERA_H
#define TPR_UBO_CAMERA_H

//--- glm - 0.9.9.5 ---
#include "glm_no_warnings.h"

//-------------------- Engine --------------------//
#include "FloatVec.h"

namespace ubo{//------------- namespace ubo ----------------

// struct:
// glm::mat4  mat4_view        (64)
// glm::mat4  mat4_projection  (64)
// FloatVec2  canvasCFPos      (8)    
// ...
inline constexpr size_t UBO_Camera_size { 64 + 64 + 8 };


// called when:
// -1- app.init
// -2- camera state changed
void write_ubo_Camera();

}//------------- namespace ubo: end ----------------
#endif 

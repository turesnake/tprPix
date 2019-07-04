/*
 * ========================= esrc_camera.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.04.19
 *                                        MODIFY --
 * ----------------------------------------------------------
 */
#ifndef TPR_ESRC_CAMERA_H
#define TPR_ESRC_CAMERA_H

//-------------------- Engine --------------------//
#include "Camera.h"

namespace esrc {//------------------ namespace: esrc -------------------------//

void init_camera();
Camera &get_camera();

}//---------------------- namespace: esrc -------------------------//
#endif


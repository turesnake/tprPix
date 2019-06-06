/*
 * ========================= esrc_camera.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.04.19
 *                                        MODIFY --
 * ----------------------------------------------------------
 */
#ifndef TPR_ESRC_CAMERA_H_
#define TPR_ESRC_CAMERA_H_

//-------------------- Engine --------------------//
#include "Camera.h"

namespace esrc {//------------------ namespace: esrc -------------------------//

//inline Camera camera {}; //-- 本游戏暂时只有 一个 摄像机
Camera &get_camera();

}//---------------------- namespace: esrc -------------------------//
#endif


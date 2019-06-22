/*
 * ====================== esrc_camera.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.06.03
 *                                        MODIFY --
 * ----------------------------------------------------------
 */
#include "esrc_camera.h"


namespace esrc {//------------------ namespace: esrc -------------------------//

namespace camera_inn {//------------------ namespace: camera_inn -------------------------//

    Camera camera {}; //-- 本游戏暂时只有 一个 摄像机

}//--------------------- namespace: camera_inn end -------------------------//


Camera &get_camera(){
    return camera_inn::camera;
}


}//---------------------- namespace: esrc -------------------------//


/*
 * ====================== esrc_camera.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.06.03
 *                                        MODIFY --
 * ----------------------------------------------------------
 */
#include "esrc_camera.h"
#include <memory>


namespace esrc {//------------------ namespace: esrc -------------------------//
namespace camera_inn {//------------------ namespace: camera_inn -------------------------//
    std::unique_ptr<Camera> cameraPtr; // tmp only-one 
}//--------------------- namespace: camera_inn end -------------------------//

void init_camera(){
    camera_inn::cameraPtr = std::make_unique<Camera>();
}

Camera &get_camera(){
    return *(camera_inn::cameraPtr.get());
}


}//---------------------- namespace: esrc -------------------------//


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
    std::unique_ptr<Camera> cameraUPtr; // tmp only-one 
}//--------------------- namespace: camera_inn end -------------------------//

void init_camera(){
    camera_inn::cameraUPtr = std::make_unique<Camera>();
}

Camera &get_camera(){
    return *(camera_inn::cameraUPtr.get());
}


}//---------------------- namespace: esrc -------------------------//


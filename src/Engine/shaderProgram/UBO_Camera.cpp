/*
 * ====================== UBO_Camera.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.09.24
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#include "UBO_Camera.h"

//-------------------- Engine --------------------//
#include "esrc_uniformBlockObj.h"
#include "esrc_camera.h"


void write_ubo_Camera(){
    
    auto &cameraRef  = esrc::get_camera();
    //-- check if it's necessary to write --
    bool isNeedToWrite {};
    if( cameraRef.get_isFirstFrame() ){
        isNeedToWrite = true;
    }else{
        isNeedToWrite = cameraRef.get_isMoving() ? true : false;
    }
    if( !isNeedToWrite ){
        return;
    }
    //--- DO WRITE ---//
    auto &ubo_camera = esrc::get_uniformBlockObjRef( UBOType::Camera );
    //-- 无法组成 struct，只能分段传入 --
    //--- mat4_view ---//
    ubo_camera.write( 0, 64, glm::value_ptr(cameraRef.update_mat4_view()) );

    //--- mat4_projection ---//
    if( !cameraRef.get_isProjectionSet() ){ // only once
        ubo_camera.write( 64, 64, glm::value_ptr(cameraRef.update_mat4_projection()) );
    }
    //--- canvasCFPos ---//
    FloatVec2 canvasCFPos = cameraRef.calc_canvasCFPos();
    ubo_camera.write( 128, 8, static_cast<const GLvoid*>(&canvasCFPos) );
}


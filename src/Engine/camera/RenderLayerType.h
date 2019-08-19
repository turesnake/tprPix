/*
 * ========================= RenderLayerType.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.01.22
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *   render layer
 * ----------------------------
 */
#ifndef TPR_RENDER_LAYER_H
#define TPR_RENDER_LAYER_H

// from zFar -> zNear 
enum class RenderLayerType{

    Ground,
    Chunks,
    WaterAnim,
    MapSurfaces,
    GoShadows,
    Debug,
    MajorGoes,
    //....

    PlayerGoIndication, //- not be used yet
    UIs
};


#endif


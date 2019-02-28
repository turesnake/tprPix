/*
 * ========================= ViewingBox.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.01.22
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *   camera.viewingBox
 * ----------------------------
 */
#ifndef _TPR_VIEWING_BOX_H_
#define _TPR_VIEWING_BOX_H_

//-------------------- Engine --------------------//
#include "config.h" // SCR_WIDTH, SCR_HEIGHT


//-- each camera have a viewingBox
class ViewingBox{
public: 
    //======= statix =======//
    static float x;
    static float y;
    static float z;

    //-- distance from zFar(to zNear) --
    //   camera.zFar 是个动态值，此处只能保存一个 相对偏移
    static float  chunks_zOff;
    static float  goShadows_zOff;
    static float  mapEntSlices_zOff;
};

inline float ViewingBox::x { (float)WORK_WIDTH };
inline float ViewingBox::y { (float)WORK_HEIGHT };
inline float ViewingBox::z { (float)VIEWING_BOX_Z_DEEP };

inline float  ViewingBox::chunks_zOff        { 10.0f };
inline float  ViewingBox::goShadows_zOff     { 20.0f };
inline float  ViewingBox::mapEntSlices_zOff  { 30.0f };


#endif


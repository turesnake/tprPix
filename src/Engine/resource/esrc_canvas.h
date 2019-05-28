/*
 * ========================= esrc_canvas.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.04.19
 *                                        MODIFY --
 * ----------------------------------------------------------
 */
#ifndef _TPR_ESRC_CANVAS_H_
#define _TPR_ESRC_CANVAS_H_

//-------------------- Engine --------------------//
#include "Canvas.h"


namespace esrc {//------------------ namespace: esrc -------------------------//


inline Canvas  groundCanvas {};
inline Canvas  waterAnimCanvas {};

void init_canvases();
void draw_groundCanvas();
void draw_waterAnimCanvas();


}//---------------------- namespace: esrc -------------------------//
#endif


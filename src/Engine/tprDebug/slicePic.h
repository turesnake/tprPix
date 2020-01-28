/*
 * ========================= slicePic.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.01.05
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#ifndef TPR_SLICE_PIC_H
#define TPR_SLICE_PIC_H
//=== *** glad FIRST, glfw SECEND *** ===
// Don't include glfw3.h ALONE!!!
#include <glad/glad.h> 

//-------------------- CPP --------------------//
#include <vector>

//-------------------- Engine --------------------//
#include "RGBA.h"
#include "IntVec.h"


namespace tprDebug {//---------- namespace: tprDebug --------------//

//-- 这段完全可以在 函数内创建...


inline RGBA e_ { 0,0,0,0 };       //- 空白
inline RGBA CO { 255,0,0,150 }; //- 有颜色

//-- mapEntSlice 使用的 “黄色边框” img
//-- 这个 边框宽度已经失效果了。
inline std::vector<RGBA> slicePic {};
inline IntVec2 slicePicSize { 64, 64 };
inline  GLuint  texName_slice {}; //- the only texName




//-- 一个 2*2 的小亮点
inline std::vector<RGBA> pointPic{
    CO, CO, 
    CO, CO
};
inline IntVec2  pointPicSize { 2, 2 };
inline  GLuint  texName_pointPic {}; //- the only texName



}//-------------------- namespace: tprDebug end --------------//
#endif 


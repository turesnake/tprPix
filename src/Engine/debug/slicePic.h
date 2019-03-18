/*
 * ========================= slicePic.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.01.05
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#ifndef _TPR_SLICE_PIC_H_
#define _TPR_SLICE_PIC_H_
//=== *** glad FIRST, glfw SECEND *** ===
#include <glad/glad.h> 

//-------------------- CPP --------------------//
#include <vector>

//-------------------- Engine --------------------//
#include "RGBA.h"
#include "IntVec.h"


namespace debug {//---------- namespace: debug --------------//

//-- 这段完全可以在 函数内创建...

namespace{//-------- namespace ----------//
    inline RGBA e_ { 0,0,0,0 };       //- 空白
    inline RGBA CO { 255,255,0,150 }; //- 有颜色
}//-------------- namespace: end --------//

//-- mapEntSlice 使用的 “黄色边框” img
inline std::vector<RGBA> slicePic{
    CO, CO, CO, CO, CO, CO, CO, CO,
    CO, e_, e_, e_, e_, e_, e_, CO,
    CO, e_, e_, e_, e_, e_, e_, CO,
    CO, e_, e_, e_, e_, e_, e_, CO,
    CO, e_, e_, e_, e_, e_, e_, CO,
    CO, e_, e_, e_, e_, e_, e_, CO,
    CO, e_, e_, e_, e_, e_, e_, CO,
    CO, CO, CO, CO, CO, CO, CO, CO
};
inline IntVec2 slicePicSize { 8, 8 };
inline  GLuint  texName_slice {}; //- the only texName




//-- 一个 2*2 的小亮点
inline std::vector<RGBA> pointPic{
    CO, CO, 
    CO, CO
};
inline IntVec2  pointPicSize { 2, 2 };
inline  GLuint  texName_pointPic {}; //- the only texName



}//-------------------- namespace: debug end --------------//
#endif 


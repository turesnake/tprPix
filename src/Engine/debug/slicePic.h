/*
 * ========================= slicePic.h ==========================
 *                          -- tpr --
 *                                        创建 -- 2019.01.05
 *                                        修改 -- 2019.01.05
 * ----------------------------------------------------------
 *    
 * ----------------------------
 */
#ifndef _TPR_SLICE_PIC_H_
#define _TPR_SLICE_PIC_H_
//=== *** glad FIRST, glfw SECEND *** ===
#include <glad/glad.h> 

//-------------------- CPP --------------------//
#include <vector>

//-------------------- Engine --------------------//
#include "RGBA.h"

namespace debug {//---------- namespace: debug --------------//

//-- 这段完全可以在 函数内创建...

namespace{//-------- namespace ----------//
    inline RGBA empty { 0,0,0,0 };
    inline RGBA color { 255,255,0,200 };
}//-------------- namespace: end --------//

//-- mapEntSlice 使用的 “黄色边框” img
inline std::vector<RGBA> slicePic{
    color, color, color,
    color, empty, color,
    color, color, color
};


inline  GLuint  texName_slice {}; //- the only texName


}//-------------------- namespace: debug end --------------//
#endif 


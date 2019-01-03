/*
 * ========================= VAOVBO.h ==========================
 *                          -- tpr --
 *                                        创建 -- 2018.12.24
 *                                        修改 -- 2018.12.24
 * ----------------------------------------------------------
 *   本游戏只有一份 VAO，VBO。
 *   所有的 mesh 都应该包含 本文件，直接使用里面的 VAO,VBO
 * ----------------------------
 */
#ifndef _TPR_VAO_VBO_H_
#define _TPR_VAO_VBO_H_

/* -- 确保 glad GLFW 两个库 的引用顺序 ---
 * --    glad.h 包含了正确的OpenGL头文件（如GL/gl.h），
 * --    所以需要在其它依赖于OpenGL的头文件之前 包含 glad.h
 */
#include <glad/glad.h>  


inline GLuint VAO  {0}; //- obj id
inline GLuint VBO  {0}; //- obj id

void VAOVBO_init(); //- 创建 VAO, VBO
void VAOVBO_del();  //- 删除 VAO, VBO

#endif

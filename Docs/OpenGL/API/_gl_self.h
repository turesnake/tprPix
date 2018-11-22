//
//========================= _gl_self.h ==========================
//                         -- OpenGL --
//                                        创建 -- 2018.08.09
//                                        修改 -- 2018.08.09
//----------------------------------------------------------
//   定义／誊写 一些 常用的 gl 类型
//   
//----------------------------

#ifndef _OPENGL_GL_SELF_H_
#define _OPENGL_GL_SELF_H_



typedef unsigned int GLenum;
typedef void GLvoid;
typedef unsigned int GLuint;
typedef int GLsizei;
typedef int GLint;
typedef unsigned char GLboolean;



//-- GLsizeiptr -----
//-- Non-negative binary integer size, for memory offsets and ranges
#if defined(__ENVIRONMENT_MAC_OS_X_VERSION_MIN_REQUIRED__) && (__ENVIRONMENT_MAC_OS_X_VERSION_MIN_REQUIRED__ > 1060)
typedef long GLsizeiptr;
#else
typedef ptrdiff_t GLsizeiptr; // ptrdiff_t 为止
#endif









#endif

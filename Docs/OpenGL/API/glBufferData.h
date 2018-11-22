
//=======================================================//
//                   glBufferData
//=======================================================//

#include "_gl_self.h"  //-- 抄写了 gl 的各种 类型定义



//-- creates and initializes a buffer object's data store
void glBufferData(	GLenum target,
 	                GLsizeiptr size,
 	                const GLvoid * data,
 	                GLenum usage);
    //-- the buffer object currently bound to target is used

void glNamedBufferData(	GLuint buffer,
 	                    GLsizei size,
 	                    const void *data,
 	                    GLenum usage);
    //-- a buffer object associated with ID specified by the caller in buffer 
    //-- will be used instead

    //--
    //--
    //--
    //-- param: usage  -- 指定存储数据的 预期用途。可选值有：
            //-- GL_STREAM_DRAW
            //-- GL_STREAM_READ
            //-- GL_STREAM_COPY
            //-- GL_STATIC_DRAW
            //-- GL_STATIC_READ
            //-- GL_STATIC_COPY
            //-- GL_DYNAMIC_DRAW
            //-- GL_DYNAMIC_READ
            //-- GL_DYNAMIC_COPY











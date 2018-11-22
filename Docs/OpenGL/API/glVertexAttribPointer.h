
//=======================================================//
//                   glVertexAttribPointer
//=======================================================//

#include "_gl_self.h"  //-- 抄写了 gl 的各种 类型定义



//-- 告诉 OpenGL，该如何 解析 顶点数据／Vertex Attribute 
//--                                                 //(例子)
void glVertexAttribPointer(	GLuint index,            // 0
 	                        GLint size,              // 3
 	                        GLenum type,             // GL_FLOAT
 	                        GLboolean normalized,    // GL_FALSE
 	                        GLsizei stride,          // 3 * sizeof(float)
 	                        const GLvoid * pointer); // (void*)0
    //-- param: index 
                // 指定了我们要配置 顶点属性 的 index 号。
                // 每个 顶点着色器 有 16 个属性。在此如果我们将参数 index 设为0
                // 就能在 vetex shader 程序中，通过：
                //    layout (location = 0) in vec3 aPos;
                // 来取得 此处设置的 属性数据。

    //-- param: size
                // 指定了 顶点属性 个体的大小（bytes）。
                // 例子中是 vec3 数据，那么 参数size 就被 设置为 3

    //-- param: type
                // 指定 目标顶点属性 的元素数据的 类型。例中是 GL_FLOAT。
                // 因为 vec3 的元素 都是 float 类型。

    //-- param: normalized
                // 定义 是否希望数据被 标准化／Normalize。
                // 若设为 GL_TRUE： 
                //       unsigned数据 会被映射为  0 到 1 之间
                //         signed数据 会被映射为 -1 到 1 之间
                
                // 例中，我们设此参数为 GL_FALSE

    //-- param: stride
                // 步长。即 输入的数据中，每个数据的 大小。
                // 例中，vec3 的步长，当然是： (3 * sizeof(float))

                // 若将 此参数 设为 0， OpenGL 会自动检测并设置。
                //  （只有当输入数据紧密排列时 才正确 ）
                

    //-- param: pointer
                // 表示 位置数据 在缓冲中起始位置的 偏移量(Offset)。
                // 由于例中 位置数据 在数组的开头，所以这里是0

    //-- 每个顶点属性从一个 VBO 管理的内存中获得它的数据，
    //-- 而具体是从哪个 VBO（程序中可以有多个VBO）获取则是通过在
    //-- 调用 glVetexAttribPointer 时绑定到 GL_ARRAY_BUFFER 的 VBO 决定的。
    //-- 由于在调用 glVetexAttribPointer 之前绑定的是先前定义的 VBO 对象，
    //-- 顶点属性0 现在会链接到它的顶点数据。

    //-- 
    //--
    //--
    //--
    //--
    //--
    //--







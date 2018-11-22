
//=======================================================//
//                   glDrawElements
//=======================================================//

#include "_gl_self.h"  //-- 抄写了 gl 的各种 类型定义




//-- 根据 VBO，EBO 中的数据，正式绘制图形。常用于 while 主渲染循环中。
void glDrawElements( GLenum mode,            //- GL_TRIANGLES
 	                GLsizei count,           //- 6
 	                GLenum type,             //- GL_UNSIGNED_INT
 	                const GLvoid * indices); //- 0

    //-- param: mode
                //- 要绘制的模式。可选值有：
                        //-- GL_POINTS      -- 单独的点
                        //-- GL_LINE_STRIP  -- 单线
                        //-- GL_LINE_LOOP   -- 首尾相连的线
                        //-- GL_LINES       -- 数条线
                        //-- GL_LINE_STRIP_ADJACENCY  -- 
                        //-- GL_LINES_ADJACENCY
                        //-- GL_TRIANGLE_STRIP
                        //-- GL_TRIANGLE_FAN
                        //-- GL_TRIANGLES          --   三角形（最常用）
                        //-- GL_TRIANGLE_STRIP_ADJACENCY
                        //-- GL_TRIANGLES_ADJACENCY
                        //-- GL_PATCHES
                
    //-- param: count
                //- 打算绘制的 顶点 的个数
                //- 例中有 2 个三角形，共 6个 顶点。

    //-- param: type
                //- 索引的类型。可选值有：
                        //- GL_UNSIGNED_BYTE    -- u8
                        //- GL_UNSIGNED_SHORT   -- u16
                        //- GL_UNSIGNED_INT     -- u32 
                        
    //-- param: indices
                //- 指定EBO中的偏移量
                //- 例中 EBO 数据只有我们这个 矩形，所以自然是 0
                

    //-- return:
    //-- 若 参数 mode 不为可选值，跳出error： GL_INVALID_ENUM
    //-- 若 参数 count 是 负值， 跳出error： GL_INVALID_VALUE
    //-- 若 参数 
    //-- 若 参数 
    //-- 



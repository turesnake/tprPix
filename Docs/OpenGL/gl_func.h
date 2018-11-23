//
//========================= gl_func.h ==========================
//                         -- OpenGL --
//                                        创建 -- 2018.11.22
//                                        修改 -- 2018.11.22
//----------------------------------------------------------
//   gl API 的所有函数 简介
//   https://www.khronos.org/registry/OpenGL-Refpages/gl4//
//
//   挑重要的 记录
//----------------------------

using GLsizei = int;
using GLuint  = unsigned int;
using GLenum  = unsigned int;
using GLint   = int;
using GLvoid  = void;
using GLsizeiptr = long;
using GLboolean  = unsigned char;


//----------------------------------------------------------
//                       TEXTURE
//----------------------------------------------------------

//- 常规 png 图片 采用 左上坐标系
//- texture 采用 左下坐标系。
//- 所以，在通过 stb_image.h 加载 png 图片时。
//- 需要调用： stbi_set_flip_vertically_on_load( 1 ); 来上下颠倒图形


//----------------------------
//- 申请 n 个 texture 实例，并为它们分配 name。
//- param: n        -- 申请分配 n 个 text names
//- param: textures -- 一个指针，存储分配到的 text names 
void glGenTextures(	GLsizei n, GLuint * textures);
    //- 可以一次申请数个，申请到的 text names, 作为一个 int值，
    //- 连续存储在 指针 textures 指向的空间
        
    //- 可以把此处的 textture name 看作一种 id号
    //- names 都是 u32 类型，
    //- 0 值被保留,用来表示 text target 的默认 text 实例 name值
        
    //- 不保证，分配到的 names 的int值是连续的。
    //- 但保证，它们当前没有被使用。
    //- 直到你在未来 调用 glDeleteTextures 释放它们。

    //- 刚申请到的 name 所对应的，在 gl状态机内的 text 实例，是没有尺寸预设的。
    //- 直到你第一次调用 glBindTexture 将它们绑定为 某种具体的 texture

    //- ERROR:
    //- GL_INVALID_VALUE -- 当 n 为负数时


//----------------------------
//- 将一个 已命名的 text实例，绑定到 一个 text target 上
//- param: target  -- 目标
//- param: texture -- text name
void glBindTexture(	GLenum target, GLuint texture);
    //- target 似乎是一个 单独的对象：
    //- 当一个新的 text实例 被绑定到 某 target 上时。
    //- 这个 target 原先绑定的 text实例 会被自动解脱

    //- 此次绑定 会始终处于激活态，直到这个 target 被另一个 text实例绑定
    //- 或者，这个 text实例 被 glDeleteTextures 删除

    //- 一个被分配了 name 的 text实例，可以被反复 绑定到 同一个 target上
    //- 通常，这种 重新绑定的 速度 会很快
    //- 至少比 用  glTexImage2D 之类的函数 重新加载 text img 文件 要快
    
    //- target 必须属于如下之一: 
    //- GL_TEXTURE_1D
    // -GL_TEXTURE_2D  ---  常用这个 
    //- GL_TEXTURE_3D
    //- GL_TEXTURE_1D_ARRAY
    //- GL_TEXTURE_2D_ARRAY
    //- GL_TEXTURE_RECTANGLE
    //- GL_TEXTURE_CUBE_MAP
    //- GL_TEXTURE_CUBE_MAP_ARRAY
    //- GL_TEXTURE_BUFFER
    //- GL_TEXTURE_2D_MULTISAMPLE      -- 仅用于 GL 3.2 及以上版本
    //- GL_TEXTURE_2D_MULTISAMPLE_ARRAY -- 仅用于 GL 3.2 及以上版本

    //- ERROR:
    //- GL_INVALID_ENUM -- 当 target 不是允许的值
    //- GL_INVALID_VALUE -- 当 name 不是有效值
    //- GL_INVALID_OPERATION -- 当 目标 text实例 原来不是 和 此次 target 匹配的 时

    

//----------------------------
//- 删除 n 个 已命名的 text实例
//- param: n -- 要删除的 个数
//- param: textures -- 要删除的 text实例的 names 的数组指针
void glDeleteTextures(	GLsizei n, const GLuint * textures);
    //- 被删除的 names 会被释放，可重现使用

    //- 若被删除的 某个 text 正处于绑定状态（绑在某个 target 上）
    //- 这个 text实例 会被删除，相关的 target 会被绑定到 0 号 text实例
    //- （一个状态机 默认值）

    //- ERROR:
    //- GL_INVALID_VALUE -- 当 n 为负数时

//----------------------------
//- 将 图形数据，喂给 text target 实例。
void glTexImage2D(	GLenum target, //- 目标 target 实例（唯一）
 	                GLint level,   //- 通常写为 0
 	                GLint internalformat, //- 希望把 text 储存为什么格式，推荐 GL_RGBA／GL_RGB
 	                GLsizei width,  //-
 	                GLsizei height, //-
 	                GLint border,   //- 总是被设为0（历史遗留问题
 	                GLenum format,  //- 源图格式， png图 就写为 GL_RGBA
 	                GLenum type,    //- 源图的数据类型，png图 就写为 GL_UNSIGNED_BYTE
 	                const GLvoid * data); //- 数据指针，传入口
    
    //- 在此之后，gl状态机会 自行存储这份 图形数据，用户代码可以把之前读取 图形数据 丢弃／释放

    //- 第一个元素应该是 位图的 左下角 像素
    //- 随后一个元素，指向 右侧一像素。
    //- 依次读取完 整行，然后再读取 更高一行...

    //- 参数 level 写为 0，意味着，在多级渐远img，也就是 mipmap 中，只有最基础的那张大图被加载
    //- 若要加载所有 mipmap 图像，需要将这个值，设置为 n （mipmap 中图像的个数）
    //- 更简单的方法是：
    //- 在 用 level = 0 生成text之后，调用 glGenerateMipmap 函数来自动生成一张 mipmap


//----------------------------
//- 激活一个 texture unit
void glActiveTexture( GLenum texture);
    //- 参数 texture，通常写为 (GL_TEXTURE0 + n)，或者 GL_TEXTURE18 这种

    //- gl状态机 保证至少有 80 个 text units。
    //- 可以通过 GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS 得知 units 个数上限。
    //- 这个值在 mac 中是 35661. 几乎用不完了

    //- 最初的 unit 为 GL_TEXTURE0



//----------------------------
//- 根据 array data 来渲染 一个 图元
void glDrawArrays(	GLenum mode, //- 渲染模式
 	                GLint first, //- 要渲染的数据中，首元素的 idx
 	                GLsizei count); //- 要渲染的数据中，元素个数（顶点个数）

    //- 参数 mode 应属于 下列之一
    //- GL_POINTS
    //- GL_LINE_STRIP
    //- GL_LINE_LOOP
    //- GL_LINES
    //- GL_LINE_STRIP_ADJACENCY -- 仅用于 GL 3.2 及以上版本
    //- GL_LINES_ADJACENCY      -- 仅用于 GL 3.2 及以上版本
    //- GL_TRIANGLE_STRIP
    //- GL_TRIANGLE_FAN
    //- GL_TRIANGLES
    //- GL_TRIANGLE_STRIP_ADJACENCY -- 仅用于 GL 3.2 及以上版本
    //- GL_TRIANGLES_ADJACENCY      -- 仅用于 GL 3.2 及以上版本
    //- GL_PATCHES

    //- 每个图元每一帧，都需要执行：
    //- 针对 目标图元 需要的 n 个texture，对每一个 text，逐个执行
    //  glActiveTexture  -- 激活一个 texture unit
    //  glBindTexture    -- 将一个 text name 绑定到 text target 上
    //- 然后： glBindVertexArray -- 绑定 VAO
    //- 最后： glDrawArrays      -- 正式渲染此 图元 （可能仅仅是 渲染申请）
                                //- 具体的渲染 将由 gl状态机决定，我猜
    
    //- 在每一帧中，需要对 window中的 每一个 图元，执行上述操作。
    //- 由于 pixel 游戏的视角受限，我们可以在 每一帧的逻辑运算中，
    //- 剔除掉那些 肯定不会出现在 window 中的 图元。




//----------------------------
// 申请 n 个 buffer obj, 并获得其 names
void glGenBuffers(	GLsizei n, GLuint * buffers);
    //- 使用思路类似 glGenTextures

    //- 此函数执行完后，申请到的 buffer obj 并没有初始化完毕
    //- 直到执行 glBindBuffer 

    //- 常见的用途是 :
    //- glGenBuffers( 1, &VBO );
    //- glGenBuffers( 1, &EBO );


//----------------------------
// 绑定一个 已命名的 buffer obj 到 buffer target
void glBindBuffer(	GLenum target,
 	                GLuint buffer); //- buffer name

    //- 参数 target 应属于 下列之一
    //- GL_ARRAY_BUFFER           --- VBO
    //- GL_ATOMIC_COUNTER_BUFFER
    //- GL_COPY_READ_BUFFER
    //- GL_COPY_WRITE_BUFFER
    //- GL_DISPATCH_INDIRECT_BUFFER
    //- GL_DRAW_INDIRECT_BUFFER
    //- GL_ELEMENT_ARRAY_BUFFER   --- EBO
    //- GL_PIXEL_PACK_BUFFER
    //- GL_PIXEL_UNPACK_BUFFER
    //- GL_QUERY_BUFFER
    //- GL_SHADER_STORAGE_BUFFER
    //- GL_TEXTURE_BUFFER
    //- GL_TRANSFORM_FEEDBACK_BUFFER
    //- GL_UNIFORM_BUFFER

    //- buffer target 有点像 texture  target。 都是全局唯一的
    //-
    //-
    //- 


//----------------------------
//- creates and initializes a buffer object's data store
//- 创建并初始化 一个 buffer obj 的 数据存储区（一个）
void glBufferData(	GLenum target,     //- buffer target
 	                GLsizeiptr size,   //- buffer obj 的新 data store 的字节数
 	                const GLvoid * data, //- 要复制进 gl状态机 的数据指针，可写为 nullptr
 	                GLenum usage);   //- 决定了新的 data store 将被如何访问

    //- 本函数的 目标，就是 创建并初始化一个 data store
    //- 此处的 data store, 可以看作一个 buffer target 的一个 数据存储区

    //- 参数 usage 应属于 下列之一
    //- GL_STREAM_DRAW -- 每次绘制时，传入的数据都要被改写。比如模型本身在变形
    //- GL_STREAM_READ
    //- GL_STREAM_COPY
    //- GL_STATIC_DRAW  -- 静态写。当传入的数据不太需要改写时，使用它。比如模型是 刚性的，从来不形变
    //- GL_STATIC_READ
    //- GL_STATIC_COPY
    //- GL_DYNAMIC_DRAW -- 动态写，介于 STREAM_DRAW 和 STATIC_DRAW 之间。偶尔要该数据
    //- GL_DYNAMIC_READ
    //- or GL_DYNAMIC_COPY

    //- 上诉的 STREAM / STATIC / DYNAMIC 会影响 gl状态机 把 目标数据 存储在 什么样的内存中
    //- 是 支持高速写入的内存，还是 其他更简单的内存

    //- DRAW -- 存储区数据 由 用户代码改写。
    //             然后，被当成 gl绘制 的资源，或 img特殊指令 来使用
    //- READ -- 存储区数据 由 从 gl读取到的数据 来改写
    //             然后，当用户代码访问时，将这些数据 传输给 用户代码
    //- COPY -- 存储区数据 由 从 gl读取到的数据 来改写
    //             然后，被当成 gl绘制 的资源，或 img特殊指令 来使用
    //             在这个用途下，数据 完全在 gl内部打转

    //- 当创建了这个 新的存储区。且 data 不为 nullptr。其地址上的原有数据将被 立即删除
    //- 若 data 为 nullptr，存储区仍将被创建，但存储区不会被初始化

    //- 这个函数 看起来用途很大，但目前只用了 VBO，EBO 这小部分。


//----------------------------
// define an array of generic vertex attribute data
// 链接 顶点属性
void glVertexAttribPointer(	GLuint index, //- 
 	                        GLint size,   //-
 	                        GLenum type,  //- 
 	                        GLboolean normalized,  //-
 	                        GLsizei stride,        //-
 	                        const GLvoid * pointer); //-



    //- 参数 index，就是在 vs着色器程序中，通过 
    //  layout (location = 0) in vec3 aPos;
    //  指定 的 0  这个 idx。

    //- 
    //-

    //- 每个顶点属性从一个VBO管理的内存中获得它的数据，
    //- 而具体是从哪个VBO（程序中可以有多个VBO）获取? 
    //- 则是通过在调用 本函数 时绑定到 GL_ARRAY_BUFFER 的 VBO 决定
    //- 由于在调用 本函数 之前，绑定的是先前定义的VBO对象，
    //- 顶点属性 0 现在会链接到它的顶点数据。


    //-
    //- 



//----------------------------
//- 启用／停用 顶点属性 
void glEnableVertexAttribArray(	GLuint index);
void glDisableVertexAttribArray( GLuint index);












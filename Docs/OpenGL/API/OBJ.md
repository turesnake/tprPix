

# ================================================================//
#                      OpenGL OBJ
# ================================================================//

OpenGL 中有很多 obj 对象。





#-- void glGen* (GLsizei n​, GLuint *objects​);
    -- 创建／申请 n个 obj名。
    --  param: n      -- 申请 obj名 的个数。（常见的写1.）
    -- param: objects​ -- obj名 的存放地。值为 非0 的 u32 值。

    -- return：
    -- 若 参数 n 为负数，出现error：GL_INVALID_VALUE


#-- void glDelete* (GLsizei n​, const GLuint *objects​);
    -- 销毁／释放 n 个 obj名。
    -- 如果 参数中的 obj名 是无效的（未曾被分配），或时 obj 0（即值为0），将被 无声地忽略。
    -- 这些 被销毁的 obj名 会不会在后续被复用，将由 具体实现决定。OpenGL 未做要求。

    -- if the object is bound to the current context 
        (and note this only applies to the current context), 
        then the object will be unbound from all binding to the context.
    -- 如果一个 被销毁的 obj 仍与 current context 绑定，这个 obj 会与此 context 解开所有绑定。

    -- 注意，此处为 "binding", 而不是"attachment"：
         binding：    obj 与 context 之间的关系
         attachment： obj 与 obj 之间的关系 （ attachment 不会在 glDelete* 函数中被 割断。 ）

    -- 若 被销毁的obj 正与 一个 容器obj attached，并且，这个 容器obj 正与 current context bound：
          此时，目标obj 将于 容器obj 解除 attached。
    -- 若 这个 容器obj 不与 current context bound：
          此时，目标obj 将不于 容器obj 解除 attached。

    --  在 OpenGL 4.4 及更低版本，有些 obj 会以 非常规方式 与 context bind，比如以下方法：
            - Buffer Objects bound to indexed targets via glBindBufferRange or its equivalents.
            - Textures bound as images.
       这些情况下，当obj被销毁，这些 binds 不会解绑。
       此时，只有 通过 glBind* 函数 实现的 bind 才能在 delete 时 自动解绑。
    
    -- 在 OpenGL 4.5 及更高版本，这些非常规 bind ，在delete时也会被自动 解绑。

    -- 调用本函数 不会立即 销毁obj 数据内容，甚至不会立即销毁 obj名。
        当一个 obj 在被 delete 之后仍然 in use。这个 obj 将会 remain alive。

        如下情况时，一个 obj 会 in use：
        - 当此 obj bind to a context（此 context 不一定要是 current 的）
        - 当此 obj attached to a 容器obj

    -- return：
    -- 若 参数 n 为负数，出现error：GL_INVALID_VALUE



#-- void glBind*(GLenum target​, GLuint object​);
    -- Binding objects to the context

    -- param: target -- obj 绑定的 目标
    -- param: object

    -- 若 target 已经被 之前的一个obj 绑定，
        那么调用本函数 将解绑 旧的obj，然后绑定 新的obj。


#-- Object Zero

    -- 对于 大部分 obj，  0 意味着 NULL

    -- 对于有些obj，0 意味着 "default object"。
       比如 Textures， 但是 用起来非常复杂。

    -- 对于 Framebuffers， 0 意味着 Default Framebuffer，
        单仍然很复杂，不推荐使用。

#-- Multibind
    -- 许多 Multibind 只能用来 使用这些 objs，不是用来 改写 这些objs 的。
    -- ...

#-- Object Sharing
    -- contexts 之间是彻底独立的。但是在 context 的创建期间，
        可以创建一个 context，它能与 另一个 已经存在的 context 分享objs。
        即意味着，你可以 在一个 context 创建obj，然后在 另一个 context 使用这个 obj。
    
    -- 不是所有 obj 类型 都可在 contexts 之间共享。
        Objects that contain references to other objects cannot be shared
        拥有其它 obj引用 的obj，无法被共享。

    -- 注意，在一个 context 中改变 共享obj的状态后，在另一个context 中无法（保证）立即见到变化。


#-- Object names

#-- void glObjectLabel (GLenum identifier​, GLuint name​, GLsizei length​, const char * label​);
#-- void glObjectPtrLabel (void * ptr​, GLsizei length​, const char * label​);
    以上 函数 只在 OpenGL 4.3 版本及以上 才有。







# ======================================== #
#             Buffer Object
# ======================================== #
Identifier: GL_BUFFER
regular obj


# ======================================== #
#             Shader Object
# ======================================== #
Identifier: GL_SHADER

# ======================================== #
#             Program Object
# ======================================== #
Identifier: GL_PROGRAM

# ======================================== #
#         Vertex Array Object ／ VAO
# ======================================== #
Identifier: GL_VERTEX_ARRAY
Container obj

VAO 可以看作是个 管理型 obj，
    -- VAO 拥有 16个 vertex attribute pointer／顶点属性锚点
    -- 这些锚点可以被看作 指针。通常用 下标号 [0~15] 访问这些 顶点属性锚点。
    -- 可以通过 glVertexAttribPointer 函数，将 某锚点 链接到 一个具体的 VBO 对象上，
       从而获得 VBO 上的 顶点数据。
       但是这种 链接 并不是 实际数据的复制。后期VBO数据改变时，VAO也会发生变化。

    -- 顶点属性锚点 默认是 禁用状态， 可以通过 glEnableVertexAttribArray 函数
        启动某个 锚点。


    -- VAO 还拥有一个 element buffer object 锚点，也就是 EBO锚点
        EBO ／索引缓冲对象，用来 解释 VBO数据的 使用方式：
        最常见的，指定 VBO 中的 三个顶点 来 构成三角形。

    -- 当同时配置好了 VBO，EBO。可以在 while 主渲染循环中，
        使用 glDrawElements 函数 来绘制图形。
    
    -- 不要随便 解绑 EBO，这可能会导致 EBO 与 VAO 的解绑。

    -- VAO 无法在 context 之间共享。
       




# ======================================== #
#             Query Object
# ======================================== #
Identifier: GL_QUERY
regular obj

# ======================================== #
#             Program Pipeline Object
# ======================================== #
Identifier: GL_PROGRAM_PIPELINE
Container obj

# ======================================== #
#             Transform Feedback Object
# ======================================== #
Identifier: GL_TRANSFORM_FEEDBACK
Container obj


# ======================================== #
#             Sampler Object
# ======================================== #
Identifier: GL_SAMPLER
regular obj

# ======================================== #
#             Texture Object
# ======================================== #
Identifier: GL_TEXTURE
regular obj

# ======================================== #
#             Renderbuffer Object
# ======================================== #
Identifier: GL_RENDERBUFFER
regular obj

# ======================================== #
#            Framebuffer Object
# ======================================== #
Identifier: GL_FRAMEBUFFER
Container obj





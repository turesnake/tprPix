

# ================================================================//
#                          hints 
# ================================================================//

#-- glfwInit
    所有的 hints 在 glfwInit 被调用时，自动设置为 默认值。

#-- glfwWindowHint
    在 window & context 被创建之前，手动更改 每个 hint 的设置。

#-- glfwDefaultWindowHints
    也可调用 glfwDefaultWindowHints 一次性地 将所有 hints 改回 默认值。


#-------------- hard ／ soft constraints（硬限制／软限制）--------------

#---- 以下几个 始终是 hard constraints／限制：
    GLFW_STEREO
    GLFW_DOUBLEBUFFER
    GLFW_CLIENT_API
    GLFW_CONTEXT_CREATION_API


#---- 以下几个 when requesting an OpenGL context 时是 hard限制。
#---- when requesting an OpenGL ES context 时是 soft限制（会被忽略）
    GLFW_OPENGL_FORWARD_COMPAT
    GLFW_OPENGL_PROFILE

#=========================================#
#          Window related hints
#=========================================#

#-- GLFW_RESIZABLE
    表示 窗口模式的 window 是否可被 用户 更改尺寸。
    可通过 glfwSetWindowSize 更改 window 尺寸。
    此 hint 会被 全屏模式的 window 忽略。

    Supported values：
        -- GLFW_TRUE  ( Default )
        -- GLFW_FALSE


#-- GLFW_VISIBLE
    表示 窗口模式的 window 是否 initially visible（ 初始化时 就可视 ）
    此 hint 会被 全屏模式的 window 忽略。

    Supported values：
        -- GLFW_TRUE  ( Default )
        -- GLFW_FALSE

#-- GLFW_DECORATED
    表示 窗口模式的 window 是否会有 window装饰。比如：
        a border／边界
        a close widget／关闭按钮？
        ...

    在某些平台，一个 undecorated／不允许装饰 的 window 仍允许用户生成 close event
    此 hint 会被 全屏模式的 window 忽略。

    Supported values：
        -- GLFW_TRUE  ( Default )
        -- GLFW_FALSE

#-- GLFW_FOCUSED
    表示 一个窗口模式的 window 在被创建时，是否 会获得 input focus。
    此 hint 会被 全屏模式的 window 忽略。

    Supported values：
        -- GLFW_TRUE  ( Default )
        -- GLFW_FALSE


#-- GLFW_AUTO_ICONIFY
    表示 当一个 全屏模式的 window， loss input focus 时
    是否会 自动地 iconify and restore the previous video mode

    此 hint 会被 窗口模式的 window 忽略。

    Supported values：
        -- GLFW_TRUE  ( Default )
        -- GLFW_FALSE


#-- GLFW_FLOATING
    表示 一个 窗口模式的 window，是否会 处于 所有windows 的最上层。
    这一配置，主要用于 debug窗口，
    不能用于 全屏模式的 window

    此 hint 会被 全屏模式的 window 忽略。

    Supported values：
        -- GLFW_TRUE  ( Default )
        -- GLFW_FALSE


#-- GLFW_MAXIMIZED
    表明 一个 窗口模式的 window 在被创建时 是否会 maximized／尺寸最大化
    此 hint 会被 全屏模式的 window 忽略。

    Supported values：
        -- GLFW_TRUE  ( Default )
        -- GLFW_FALSE

#=========================================#
#        Framebuffer related hints
#=========================================#

#-- GLFW_RED_BITS        8 （ Default ）
#-- GLFW_GREEN_BITS      8 （ Default ）
#-- GLFW_BLUE_BITS       8 （ Default ）
#-- GLFW_ALPHA_BITS      8 （ Default ）
#-- GLFW_DEPTH_BITS     24 （ Default ）
#-- GLFW_STENCIL_BITS    8 （ Default ）
    specify the desired bit depths of the various components of the default framebuffer
    设置 default framebuffer 的 每个 组件的 bit depths／位深 的值。

    Supported values：
        -- 0 to INT_MAX
        -- GLFW_DONT_CARE （ 表示本程序 不关心，也不想设置这些 ）

#++（ 不推荐使用 ）++#
#-- GLFW_ACCUM_RED_BITS       0 （ Default ）
#-- GLFW_ACCUM_GREEN_BITS     0 （ Default ）
#-- GLFW_ACCUM_BLUE_BITS      0 （ Default ）
#-- GLFW_ACCUM_ALPHA_BITS     0 （ Default ）
    specify the desired bit depths of the various components of the accumulation buffer
    设置 accumulation buffer 的 每个 组件的 bit depths／位深 的值。

    ！！！   accumulation buffer 是 旧版本 OpenGL 的东西   ！！！
    ！！！            新版本里 不应使用                    ！！！

    Supported values：
        -- 0 to INT_MAX
        -- GLFW_DONT_CARE （ 表示本程序 不关心，也不想设置这些 ）

#++（ 不推荐使用 ）++#
#-- GLFW_AUX_BUFFERS
    specifies the desired number of auxiliary buffers

    ！！！   auxiliary buffer 是 旧版本 OpenGL 的东西      ！！！
    ！！！            新版本里 不应使用                    ！！！

    Supported values：
        -- 0 to INT_MAX
        -- GLFW_DONT_CARE （ 表示本程序 不关心，也不想设置这些 ）

#-- GLFW_STEREO
    specifies whether to use stereoscopic rendering.
    表明 是否使用 stereoscopic rendering／立体渲染？

    <- 这是一个 hard constraint / 硬限制 ->

    Supported values：
        -- GLFW_TRUE
        -- GLFW_FALSE   ( Default )

#-- GLFW_SAMPLES        0  ( Default )
    specifies the desired number of samples to use for multisampling
    设置 用于 multisampling 的 sample 的个数

    若设为0，则表示 禁用 multisampling

    Supported values：
        -- 0 to INT_MAX    
        -- GLFW_DONT_CARE （ 表示本程序 不关心，也不想设置这些 ）


#-- GLFW_SRGB_CAPABLE
    specifies whether the framebuffer should be sRGB capable
    表示 framebuffer 是否为 sRGB capable

    若设为 GLFW_TRUE，则 一个新建的 context 将会支持：
        GL_FRAMEBUFFER_SRGB ， 也叫 GL_FRAMEBUFFER_SRGB_EXT， 用来 控制 sRGB 渲染。
    并且，一个 新建的 context 始终会 拥有 sRGB 渲染 的能力。


    Supported values：
        -- GLFW_TRUE
        -- GLFW_FALSE   ( Default )

#-- GLFW_DOUBLEBUFFER
    specifies whether the framebuffer should be double buffered
    表示，framebuffer 是否为 double buffered。

    double buffered 始终是需要的。

    <- 这是一个 hard constraint / 硬限制 ->

    Supported values：
        -- GLFW_TRUE    ( Default )
        -- GLFW_FALSE



#=========================================#
#         Monitor related hints
#=========================================#

#-- GLFW_REFRESH_RATE 
    specifies the desired refresh rate for full screen windows
    设置 全屏模式的 window 的 refresh rate／刷新频率。

    若设为 GLFW_DONT_CARE，平台将使用 最大 刷新率。

    此 hint 会被 窗口模式的 window 忽略。

    Supported values：
        -- 0 to INT_MAX    
        -- GLFW_DONT_CARE  ( Default )（ 表示本程序 不关心，也不想设置这些 ）


#=========================================#
#        Context related hints
#=========================================#

#-- GLFW_CLIENT_API
    specifies which client API to create the context for
    表示 使用哪套 API 来创建 context
    通常就是 GLFW_OPENGL_API。 

    <- 这是一个 hard constraint / 硬限制 ->

    Supported values：
        -- GLFW_OPENGL_API     ( Default )
        -- GLFW_OPENGL_ES_API
        -- GLFW_NO_API


#-- GLFW_CONTEXT_CREATION_API
    specifies which context creation API to use to create the context
    表示 使用 哪套 context creation API

    If no client API is requested, this hint is ignored
    若 GLFW_CLIENT_API 被设为 GLFW_NO_API
    那么 本 hint 将被忽略。

        _OSX_: 
            GLFW_EGL_CONTEXT_API 不被支持，使用它将导致出错。

        _Wayland_ /_Mir_: 
            GLFW_EGL_CONTEXT_API 是 原生API，所以 本hint无效（不用被设置）

    特定平台的 OpenGL扩展 加载库 有时会预设 使用哪个 context creation API，
    在这种时候，如果你更改 本hint，将会导致 这类 加载库 的允许失败。
    通过 glfwGetProcAddress 来加载，就能避免这类问题

    在有些 Linux 系统，通过 GLFW_NATIVE_CONTEXT_API 或 GLFW_EGL_CONTEXT_API
    来 创建 context 会导致 应用程序 出错／segfault
    Stick to one API or the other on Linux for now. ？？？

    <- 这是一个 hard constraint / 硬限制 ->

    Supported values：
        -- GLFW_NATIVE_CONTEXT_API     ( Default )
        -- GLFW_EGL_CONTEXT_API

#-- GLFW_CONTEXT_VERSION_MAJOR   3  （ 我们使用 3.3 ）
#-- GLFW_CONTEXT_VERSION_MINOR   3  （ 我们使用 3.3 ）
    specify the client API version that the created context must be compatible with
    指定一个： ”当创建一个 context 时 必须兼容的“ client API version


#-- GLFW_OPENGL_FORWARD_COMPAT  
    specifies whether the OpenGL context should be forward-compatible
    表示 OpenGL context 是否要 向旧版本兼容。

    _OSX_:
        此平台 需设为 GLFW_TRUE。

    Supported values：
        -- GLFW_TRUE   
        -- GLFW_FALSE   ( Default )


#-- GLFW_OPENGL_DEBUG_CONTEXT
    specifies whether to create a debug OpenGL context, 
    which may have additional error and performance issue reporting functionality
    表明，是否生成一个 debug 版的 context
    它将 额外具备 错误报告功能。

    在 OpenGL ES 平台，此 hint 会被忽略。

    Supported values：
        -- GLFW_TRUE   
        -- GLFW_FALSE   ( Default )


#-- GLFW_OPENGL_PROFILE
    specifies which OpenGL profile to create the context for
    指定 用于创建 context 的 OpenGL profile 。

    若使用的 OpenGL 版本低于 3.2， 本hint 必须使用 GLFW_OPENGL_ANY_PROFILE
    若使用 OpenGL ES， 此hint 将被 忽略。

    Supported values：
        -- GLFW_OPENGL_CORE_PROFILE   （在 learnOpenGL 教材中，选用了 此值）
        -- GLFW_OPENGL_COMPAT_PROFILE
        -- GLFW_OPENGL_ANY_PROFILE   ( Default ) （不要求特定 profile 版本）

#-- GLFW_CONTEXT_ROBUSTNESS
    specifies the robustness strategy to be used by the context
    指定 context 选用的 robustness strategy／鲁棒策略。


    Supported values：
        -- GLFW_NO_ROBUSTNESS          ( Default ) （不设置 鲁棒策略）
        -- GLFW_NO_RESET_NOTIFICATION
        -- GLFW_LOSE_CONTEXT_ON_RESET


#-- GLFW_CONTEXT_RELEASE_BEHAVIOR
    specifies the release behavior to be used by the context
    指定 context 使用的 release behavior／释放策略／释放行为方案

    https://www.khronos.org/registry/OpenGL/extensions/KHR/KHR_context_flush_control.txt
    -- 更多 有关信息／说明。

    Supported values：
        -- GLFW_ANY_RELEASE_BEHAVIOR    ( Default ) --（context creation API 的默认方案）
        -- GLFW_RELEASE_BEHAVIOR_FLUSH         -- 当 context 不再是 current one 时，
                                                   pipeline will be flushed
        -- GLFW_RELEASE_BEHAVIOR_NONE          -- pipeline will not be flushed on release



#++ 实验版本 ++#
#-- GLFW_CONTEXT_NO_ERROR
    specifies whether errors should be generated by the context

    若设为 允许，有些情况时 会主动报错，而不是引发 未定义行为。

    ！！！ 此hint 目前还是 实验版本  ！！！

    https://www.khronos.org/registry/OpenGL/extensions/KHR/KHR_no_error.txt
    -- 更多相关信息／说明



#--

#--

#--

#--

#--

#--

#--

#--

#--

#--















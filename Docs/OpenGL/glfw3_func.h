//
//========================= glfw3_func.h ==========================
//                         -- OpenGL --
//                                        创建 -- 2018.08.09
//                                        修改 -- 2018.08.09
//----------------------------------------------------------
//   glfw 库 的所有函数 简介
//   
//----------------------------


//=====================================================//

//-- 初始化 glfw 库。
//-- 先于 其它任何 glfw 函数调用。
//-- 与 glfwTerminate 函数 相呼应。
int glfwInit(void);
    //-- 调用 glfwInit 成功后，若再次调用它（在 glfwTerminate 之前）
    //-- 此时，本函数将会立即 返回 GL_TRUE （我猜 此时不会执行任何实际内容） 

    //-- 此函数 只能被 主线程(main thread) 调用

    //-- return
    //-- 若成功，返回 GL_TRUE  (1) （ 需要用户在 程序结束前，手动调用 glfwTerminate
    //-- 若出错，返回 GL_FALSE (0)（ 此时，本函数会在自己返回前，自行调用 glfwTerminate。



//-- 终止 glfw 库的使用。 确保在 进程终止前 调用此函数。
//-- 此函数 会释放所有的 相关资源。
void glfwTerminate(void);
    //-- 调用本函数 成功关闭 glfw 库后，可以再次调用 glfwInit 来重开一个。

    //-- 若在 调用 glfwInit 之前调用本函数，不会发生错误（本函数会什么也不做，直接 return）
    //-- 此函数 不可在 callback 中调用
    //-- 此函数 只能被 主线程(main thread) 调用



//-- 取回 glfw 库的 版本号
void glfwGetVersion(int* major, int* minor, int* rev);
    //-- 此函数永远能成功运行。 三个参数 若有哪个值未定义，就会写入 NULL
    //-- 此函数 可在 glfwInit 之前调用。
    //-- 此函数 可在 任何线程 使用。



//-- 返回一个字符串指针，提供 编译时 配置信息
const char* glfwGetVersionString(void);
    //-- 返回信息包括 version, platform, compiler and any platform-specific compile-time options.
    //-- 不要通过分析 这个字符串 来获得 glfw 版本号，直接使用 glfwGetVersion
    //-- 此函数 总能成功调用
    //-- 可在 glfwInit 之前调用。
    //-- 返回的 字符串是 static，且是在 编译时生成的
    //-- 此函数 可在 任何线程 使用。



//-- 函数指针，用于 error callback 函数
//-- param: int     -- an error code
//-- param: char*   -- 错误描述信息 UTF-8
typedef void (* GLFWerrorfun)(int,const char*);
//--  设置 error callback 函数
GLFWerrorfun glfwSetErrorCallback(GLFWerrorfun cbfun);
    //-- param: cbfun 新设置的 错误回调函数。
    //          cbfun 可被设置为 NULL，此时函数 将 移除 此 error 现有的 回调函数。

    //-- 错误回调函数 会在 错误发生的那个 线程 被调用
    //-- 某个 错误回调函数 一旦被设置，就算 flgw 被 Terminate 了，这个回调函数 也不会被取消

    //-- 可在 glfwInit 之前调用。
    //-- 只能被 主线程(main thread) 调用

    //-- return
    //-- 返回旧的 错误回调函数。
    //-- 如果 此 error 没有绑定 回调函数，则返回 NULL




//-- Opaque monitor / 不透明 显示器 obj
typedef struct GLFWmonitor GLFWmonitor;
//-- 返回当前连接的 显示器
GLFWmonitor** glfwGetMonitors(int* count);
    //-- param: count -- 存放 返回值中的 monitor 个数
    //                  若函数出错，此值将被设置为 0

    //-- 返回的 数组 由 glfw 来 分配和释放内存。用户不应该 自行 释放它们。
    //-- 此数组数据 始终有效，直到 minotor 配置被修改，或者 flgw 被 terminated

    //-- 只能被 主线程(main thread) 调用

    //-- return：
    //-- 返回一个 monitor handles 的数组，这些 monitor 都和当前连接
    //   主 monitor 总是会在 返回数组中的 第一个。
    //-- 若没找到 合适的 monitor，返回 NULL
    //-- 若 函数出错，也返回 NULL



//-- 返回 主 monitor
//-- 主 monitor 常常是 task bar，global menu bar 放置的场合
GLFWmonitor* glfwGetPrimaryMonitor(void);
    //-- 只能被 主线程(main thread) 调用

    //-- return：
    //-- 若 成功运行，返回 主monitor
    //-- 若 没找到，返回 NULL
    //-- 若 出错， 返回 NULL


//-- Returns the position of the monitor's viewport on the virtual screen.
//-- 返回 以 screen坐标计算的 pos，左上坐标系
void glfwGetMonitorPos(GLFWmonitor* monitor, int* xpos, int* ypos);
    //-- param: monitor  -- 目标 monitor
    //-- param: xpos     -- 可设为 NULL，如果不想获取此数据
    //-- param: ypos     -- 可设为 NULL，如果不想获取此数据

    //-- 只能被 主线程(main thread) 调用

    //-- return：
    //-- 若成功，通过参数 返回数据。
    //-- 若出错，所有没有被设置为 NULL 的 pos参数 都会写入 0



//-- 返回 目标 monitor 的 显示区域的 物理尺寸（毫米级）
//-- 有些系统中 无法提供 精确返回值
void glfwGetMonitorPhysicalSize(GLFWmonitor* monitor, int* widthMM, int* heightMM);
    //-- param: monitor   -- 
    //-- param: widthMM   -- 可设为 NULL，如果不想获取此数据
    //-- param: heightMM  -- 可设为 NULL，如果不想获取此数据

    //-- 只能被 主线程(main thread) 调用

    //-- return：
    //-- 若成功，通过参数 返回数据。
    //-- 若出错，所有没有被设置为 NULL 的 size参数 都会写入 0



//-- 返回 目标monitor 的名字 UTF-8
//-- 此名 特地显示 monitor 的 make 和 model 。
//-- 此名 无法保证在 所有 当前连接 monitor 中是 唯一的
const char* glfwGetMonitorName(GLFWmonitor* monitor);
    //-- 返回的 字符串 由 flgw 分配和释放内存。用户不应该 自行 释放它们
    //-- 此字符串始终有效，直到 特定monitor 取消连接，或者 flgw terminated

    //-- 只能被 主线程(main thread) 调用

    //-- return：
    //-- 若成功，返回 字符串
    //-- 若出错，返回 NULL



//-- monitor 配置 回调函数 函数指针
//-- param: GLFWmonitor*  -- 目标 monitor，（ connected or disconnected
//-- param: int           -- `GLFW_CONNECTED` or `GLFW_DISCONNECTED`
typedef void (* GLFWmonitorfun)(GLFWmonitor*,int);
//-- 设置 monitor 配置回调函数，
GLFWmonitorfun glfwSetMonitorCallback(GLFWmonitorfun cbfun);
    //-- param: cbfun -- 新的回调函数。
    //                  也可设置为 NULL，用来清除 当前绑定的 回调函数

    //-- X11 的本函数 有问题
    //-- 只能被 主线程(main thread) 调用

    //-- return
    //-- 若成功，返回 旧的 回调函数
    //-- 若本来就没有 回调函数，返回 NULL
    //-- 若 flgw 没有 init，也返回 NULL



//-- Video mode type.
typedef struct GLFWvidmode
{
    // The width, in screen coordinates, of the video mode.
    int width;
    // The height, in screen coordinates, of the video mode.
    int height;
    // The bit depth of the red channel of the video mode.
    int redBits;
    // The bit depth of the green channel of the video mode.
    int greenBits;
    // The bit depth of the blue channel of the video mode.
    int blueBits;
    // The refresh rate, in Hz, of the video mode.
    int refreshRate;
} GLFWvidmode;
//-- 返回 目标monitor 的 可用 video modes
const GLFWvidmode* glfwGetVideoModes(GLFWmonitor* monitor, int* count);
    //-- param: monitor
    //-- param: count   -- 存放 返回值数组的 元素个数。
    //                     当函数出错，此值放入 0

    //-- 返回的 数据 由 flgw 分配和释放内存。用户不应该 自行 释放它们
    //-- 此数据始终有效，直到 特定monitor 取消连接，或者 此函数再次被调用，或者 flgw terminated

    //-- 只能被 主线程(main thread) 调用

    //-- return：
    //-- 若成功，返回一个 数组，存放所有 有效的 video modes。按 升序排列 
    //-- 以 color bit depth 为排序第一依据，其次依据是 resolution area
        //-- color bit depth -- 所有 channel depth 的总和。
        //-- resolution area -- 面积
    //-- 若出错，返回 NULL



//-- 返回 目标 monitor 的当前 video mode
const GLFWvidmode* glfwGetVideoMode(GLFWmonitor* monitor);
    //-- param：monitor

    //-- 返回的 数据 由 flgw 分配和释放内存。用户不应该 自行 释放它们
    //-- 此数据始终有效，直到 特定monitor 取消连接，，或者 flgw terminated

    //-- 只能被 主线程(main thread) 调用

    //-- return：
    //-- 若 为 目标monitor 生成了一个 全屏窗口，返回值 取决于 这个窗口 是否 be iconified。
    //-- 若 出错，返回 NULL。




//-- Generates a gamma ramp and sets it for the specified monitor.
void glfwSetGamma(GLFWmonitor* monitor, float gamma);
    //-- param: monitor --
    //-- param: gamma   -- The desired exponent

    //-- 本函数从 特定 exponent 生成一个 256-element gamma ramp
    //-- 然后 拿它来 调用 glfwSetGammaRamp
    //-- 此值 必须为 大于 0 的有限值。

    //-- 只能被 主线程(main thread) 调用




typedef struct GLFWgammaramp
{
    // An array of value describing the response of the red channel.
    unsigned short* red;
    // An array of value describing the response of the green channel.
    unsigned short* green;
    // An array of value describing the response of the blue channel.
    unsigned short* blue;
    // The number of elements in each array.
    unsigned int size;
} GLFWgammaramp;
//-- Returns the current gamma ramp for the 目标monitor.
const GLFWgammaramp* glfwGetGammaRamp(GLFWmonitor* monitor);
    //-- 返回的 数据 由 flgw 分配和释放内存。用户不应该 自行 释放它们
    //-- 此数据始终有效，直到 特定monitor 取消连接，
    //--       或 针对此monitor的此函数 被再次调用，或者 flgw terminated

    //-- 只能被 主线程(main thread) 调用

    //-- return：
    //-- 若成功，返回 current gamma ramp
    //-- 若出错，返回 NULL。




//-- Sets the current gamma ramp for the 目标 monitor.
void glfwSetGammaRamp(GLFWmonitor* monitor, const GLFWgammaramp* ramp);
    //-- The original gamma ramp for that monitor is saved by GLFW 
    //-- the first time this function is called 
    //-- and is restored by @ref glfwTerminate.

    //-- 注意。 Gamma ramp sizes other than 256 are not supported by 
    //-- all platforms or graphics hardware.

    //-- 注意。 Windows ：The gamma ramp size must be 256.

    //-- The specified gamma ramp is copied before this function returns.

    //-- 只能被 主线程(main thread) 调用




//-- 将所有 windows hints 重设为 初始值
void glfwDefaultWindowHints(void);
    //-- 只能被 主线程(main thread) 调用



//-- 设置 目标 windows hint 的值，
void glfwWindowHint(int target, int hint);
    //-- param: target -- 目标 window hint
    //-- param: hint   -- 新的 hint 值

    //-- 首先调用本函数，然后调用 glfwCreateWindow，然后，这些新的 hint值就生效了
    //-- 它们将持续有效，直到 下次调用 本函数，或 glfwDefaultWindowHints， 或 glfw 被 terminated

    //-- 只能被 主线程(main thread) 调用



//-- 不透明的 window obj
typedef struct GLFWwindow GLFWwindow;
//-- 创建一个 window，以及与之相关联的 上下文（ OpenGL 或 OpenGL ES ）
GLFWwindow* glfwCreateWindow(int width, int height, const char* title, 
                            GLFWmonitor* monitor, GLFWwindow* share);
    //-- param: width   -- 必须大于 0
    //-- param: height  -- 必须大于 0
    //-- param: title   -- UTF-8
    //-- param: monitor -- 为全屏 所用的 monitor， 可设为 NULL，此时将使用 窗口模式
    //-- param: share   -- 新建window 将和 参数指向的这个window 的上下文 共享资源。
                            // 若设为 NULL，则放弃此功能。

    //-- windoew 及其 上下文 该被如何创建，主要由 window hints 来控制

    //-- 若函数调用成功，不会改写 当前上下文。
    //-- 在你能使用 新的 上下文之前，你需要让它设置 当前上下文。

    //-- 由于硬件限制，新创建的 window 及其上下文，不会和你 需要的 完全一致。
    //-- 包括 window 的size，尤其是全屏的。
    //-- 想要获得 真实的 window信息，framebuffer, context. 
    //-- 可以调用函数：glfwGetWindowAttrib, glfwGetWindowSize, glfwGetFramebufferSize

    //-- 为了创建一个 全屏 window，需要 指定一个 monitor，这个monitor将会被 window 覆盖。
    //-- 若没有指定 monitor，windowed mode 就会被使用。
    //-- 推荐指定 主monitor（拿来被覆盖）。

    //-- ...

    //-- 通过设置 hint： GLFW_VISIBLE 来控制 新window  是否可见

    //-- 在创建期间，swap interval 没有被设置。其初始值 依赖于 显卡驱动设置 和 默认值。

    //-- OSX -- glfw window 没有 icon。
    //--        但是 the dock icon 是和 application bundle's icon 相同的

    //-- OSX -- 在 retina屏上，帧率 不会是全速的， 需要做 额外设置。

    //-- X11 -- 目前没有机制 可用来设置 window icon。
    //-- X11 -- 由于 X11 是移步的，新创建的 window 需要点时间才能创建完毕
    //--        如果在创建后 立即 访问 这个 window 的size，position，或其它信息
    //--        将无法得到正确结果。

    //-- 本函数 无法在 回调函数 中使用
    //-- 只能被 主线程(main thread) 调用

    //-- GLFW 3.0. 起， 旧函数 glfwOpenWindow 将被本函数 取代。

    //-- return：
    //-- 若成功，返回 The handle of the created window
    //-- 若失败，返回 NULL



//-- 销毁 一个 window，及其 上下文
void glfwDestroyWindow(GLFWwindow* window);
    //-- 本函数一经调用，此window 的回调函数 将不再生效。

    //-- 如果目标window 的上下文 在主线程上是 current。它将先被 分离，后被销毁。
    //-- 目标window 的 上下文 不应该 在任何 线程上是 current。

    //-- 本函数 无法在 回调函数 中使用
    //-- 只能被 主线程(main thread) 调用

    //-- GLFW 3.0. 起， 旧函数 glfwCloseWindow 将被本函数 取代。



//-- 检测 目标window 的 close flag 
int glfwWindowShouldClose(GLFWwindow* window);
    //-- 此函数 可被任何 线程 调用。 且 是 异步的。

    //-- 通常用于 while 循环 的判断语句:
    //    while( !glfwWindowShouldClose( window ) ){...}

    //-- return：
    //-- 返回 目标window 的 close flag 的值



//-- 设置 目标window 的 close flag 的值
void glfwSetWindowShouldClose(GLFWwindow* window, int value);
    //-- param：value -- close flag 的新值。
    //-- 本函数可用来 否决  之前对目标window 的关闭。
    //-- 此函数 可被任何 线程 调用。 且 是 异步的。



//-- 设置 目标window 的 title  UTF-8
void glfwSetWindowTitle(GLFWwindow* window, const char* title);
    //-- OSX -- 新设置的 title 不会被更新。 updated until the next time you process events.
    //-- 只能被 主线程(main thread) 调用



//-- 获取 目标window 的 client area 的 pos。 screen coordinate，左上坐标系。
void glfwGetWindowPos(GLFWwindow* window, int* xpos, int* ypos);
    //-- param: window --
    //-- param: xpos  -- 可被设为 NULL，如果不想接受此数据
    //-- param: ypos  -- 可被设为 NULL，如果不想接受此数据

    //-- 只能被 主线程(main thread) 调用

    //-- return：
    //-- 若出错，任何 非 NULL 的 pos参数，都被写入 0



//-- 设置 目标window 的 client area 的 pos。 screen coordinate，左上坐标系。
void glfwSetWindowPos(GLFWwindow* window, int xpos, int ypos);
    //-- 若 目标window 是 全屏模式，此函数 什么也不做。
    //-- 不要用此函数 去更改已经可视化的window。这会让用户困扰。

    //-- window manager 会限制一些 更改范围。
    //-- 只能被 主线程(main thread) 调用



//-- 获得 目标window 的 client area 的尺寸。 screen coordinates，
void glfwGetWindowSize(GLFWwindow* window, int* width, int* height);
    //-- param: window 
    //-- param: width  -- 可被设为 NULL，若不想要此数据
    //-- param: height -- 可被设为 NULL，若不想要此数据

    //-- 只能被 主线程(main thread) 调用

    //-- return：
    //-- 若出错，所有 非 NULL 的 size参数，都被写入 0



//-- 设置 目标window 的 client area 的尺寸。 screen coordinates，
void glfwSetWindowSize(GLFWwindow* window, int width, int height);
    //-- 若目标window 是 全屏模式，则会在 不影响 此window 上下文的情况下 
    //-- 切换到 最接近 用户需求的 尺寸。
    //-- 由于此时上下文未受影响， the bit depths of the framebuffer 保持不变。

    //-- window manager 会限制一些 更改范围。
    //-- 只能被 主线程(main thread) 调用



//-- 获得 目标window 的 framebuffer 的尺寸。像素，
void glfwGetFramebufferSize(GLFWwindow* window, int* width, int* height);
    //-- param: window 
    //-- param: width  -- 可被设为 NULL，若不想要此数据
    //-- param: height -- 可被设为 NULL，若不想要此数据

    //-- 只能被 主线程(main thread) 调用


//-- 获得 目标window 的 frame 的尺寸。screen coordinates
void glfwGetWindowFrameSize(GLFWwindow* window, int* left, int* top, int* right, int* bottom);
    //-- param: window --
    //-- param: left   -- 可被设为 NULL，若不想要此数据
    //-- param: top    -- 可被设为 NULL，若不想要此数据
    //-- param: right  -- 可被设为 NULL，若不想要此数据
    //-- param: bottom -- 可被设为 NULL，若不想要此数据

    //-- 此尺寸，包含 title bar（如果此window 有）

    //-- 此 frame 尺寸，可能收到 某些 hint 的影响。[window-related hints](@ref window_hints_wnd)

    //-- 由于此函数 返回的是 size of each window frame edge，
    //-- and not the offset along a particular coordinate axis
    //-- 所以，返回值 一定是 0 或 正数。

    //-- 只能被 主线程(main thread) 调用

    //-- return：
    //-- 若出错，所有 非 NULL 的 size参数，都被写入 0



//-- 使 目标window 最小化／iconifies (minimizes)
void glfwIconifyWindow(GLFWwindow* window);
    //-- 若 目标window 处于 复原状态／restored，它将被最小化。
    //-- 若 目标window 已经处于 最小化，再次调用本函数 将什么也不做。

    //-- If the specified window is a full screen window, the original monitor
    //-- resolution is restored until the window is restored.

    //-- 只能被 主线程(main thread) 调用



//-- 复原 目标window
void glfwRestoreWindow(GLFWwindow* window);
    //-- 若 目标window 处于 iconified 状态，它将被 复原
    //-- 若 目标window 已经 被复原，调用本函数 将什么也不做。

    //-- If the specified window is a full screen window, the resolution chosen for
    //-- the window is restored on the selected monitor.

    //-- 只能被 主线程(main thread) 调用



//-- 让 目标window 可视化。
void glfwShowWindow(GLFWwindow* window);
    //-- 若 目标window 已经可视化，本函数 将什么也不做。

    //-- 只能被 主线程(main thread) 调用




//-- 使 目标window 隐藏／不可见
void glfwHideWindow(GLFWwindow* window);
    //-- 若 目标window 已经隐藏，调用此函数 将什么也不做

    //-- 只能被 主线程(main thread) 调用




//-- 返回 目标window（正处于 全屏模式）的 monitor／显示屏 （ the handle of the monitor ）
GLFWmonitor* glfwGetWindowMonitor(GLFWwindow* window);
    //-- 只能被 主线程(main thread) 调用

    //-- return：
    //-- 若 目标window 正处于 全屏模式，返回其 monitor
    //-- 若 目标window 处于 窗口模式，返回 NULL
    //-- 若出错，也返回 NULL




//-- 获得 目标window 的 某个属性／attribute 的值
int glfwGetWindowAttrib(GLFWwindow* window, int attrib);
    //-- 此属性值 可以是 window 的，也可以是 OpenGL 或 OpenGL ES 上下文的。
    //-- 与 Framebuffer 相关的 hints 不属于 window属性。See @ref window_attribs_fb
    //-- 只能被 主线程(main thread) 调用

    //-- GLFW 3.0 起，本函数 取代函数：glfwGetWindowParam 和 glfwGetGLVersion

    //-- return：
    //-- 若成功，返回 目标属性的 值
    //-- 若出错，返回 0
        //-- 注意，0 是 许多window 和 上下文相关的 属性的 有效值
        //-- 所以不能通过 返回值为0 来判断 此函数是否出错。
        //-- 只要传入参数正确，且 glfw 已经 initialized。 此函数将始终正确运行。




//-- 设置 目标window 的 user-defined pointer 的值
void glfwSetWindowUserPointer(GLFWwindow* window, void* pointer);
    //-- param: window
    //-- param: pointer -- user pointer 的新值。

    //-- user pointer 初始值为 NULL，本函数设定的值 将持续有效，直到 目标window 被销毁。

    //-- 本函数 可在任何线程调用。 且异步。




//-- 获取 目标window 的 user-defined pointer 的 当前值。
void* glfwGetWindowUserPointer(GLFWwindow* window);
    //-- user pointer 初始值为 NULL

    //-- 本函数 可在任何线程调用。 且异步。

    //-- return：
    //-- 返回 指向此值的 指针。




//-- window position callback func 的函数指针。
//-- param: GLFWwindow* -- 目标window
//-- param: int         -- 新的 xpos值，screen coordinates，左上坐标，client area
//-- param: int         -- 新的 ypos值，screen coordinates，左上坐标，client area
typedef void (* GLFWwindowposfun)(GLFWwindow*,int,int);
GLFWwindowposfun glfwSetWindowPosCallback(GLFWwindow* window, GLFWwindowposfun cbfun);
    //-- param: window  --
    //-- param: cbfun   -- 新的 回调函数。若设为NULL，意在 清除 当前设置的 回调函数。

    //-- 只能被 主线程(main thread) 调用

    //-- return：
    //-- 若成功，返回 旧的 回调函数
    //-- 若没有旧的，返回 NULL
    //-- 若 flgw 未 initialized，也返回 NULL



//-- window size callback func 函数指针，此回调函数 将会在 window 尺寸更改时 被调用。
//-- param: GLFWwindow*
//-- param: int        -- width 新值，screen coordinates，client area
//-- param: int        -- height 新值，screen coordinates，client area
typedef void (* GLFWwindowsizefun)(GLFWwindow*,int,int);
GLFWwindowsizefun glfwSetWindowSizeCallback(GLFWwindow* window, GLFWwindowsizefun cbfun);
    //-- param: window  --
    //-- param: cbfun   -- 新的 回调函数。若设为NULL，意在 清除 当前设置的 回调函数。

    //-- 只能被 主线程(main thread) 调用

    //-- return：
    //-- 若成功，返回 旧的 回调函数
    //-- 若没有旧的，返回 NULL
    //-- 若 flgw 未 initialized，也返回 NULL



//-- window close callback 函数指针，此回调函数 在 window 将被 closed 时 调用。
typedef void (* GLFWwindowclosefun)(GLFWwindow*);
GLFWwindowclosefun glfwSetWindowCloseCallback(GLFWwindow* window, GLFWwindowclosefun cbfun);
    //-- param: window  --
    //-- param: cbfun   -- 新的 回调函数。若设为NULL，意在 清除 当前设置的 回调函数。

    //-- 在window close回调函数 调用之前，close flag 会被先设置。 但可通过 glfwSetWindowShouldClose 在任何时刻更改它。
    //-- 通过调用 glfwDestroyWindow，window close 回调函数 将不会被触发。
    //-- OSX -- Selecting Quit from the application menu 会 触发所有 window 的 close 回调函数。

    //-- 只能被 主线程(main thread) 调用

    //-- return：
    //-- 若成功，返回 旧的 回调函数
    //-- 若没有旧的，返回 NULL
    //-- 若 flgw 未 initialized，也返回 NULL



//-- window content refresh callbacks 函数指针。 当window的 client area 需要被 重新绘制／刷新 时 调用此 回调函数。
typedef void (* GLFWwindowrefreshfun)(GLFWwindow*);
GLFWwindowrefreshfun glfwSetWindowRefreshCallback(GLFWwindow* window, GLFWwindowrefreshfun cbfun);
    //-- param: window  --
    //-- param: cbfun   -- 新的 回调函数。若设为NULL，意在 清除 当前设置的 回调函数。

    //-- 在 某些 复合window系统，例如：Aero, Compiz or Aqua
    //-- 当 window 的上下文 are saved off-screen， 此refresh回调函数 将不会被调用，或很少调用。

    //-- 只能被 主线程(main thread) 调用

    //-- return：
    //-- 若成功，返回 旧的 回调函数
    //-- 若没有旧的，返回 NULL
    //-- 若 flgw 未 initialized，也返回 NULL



//-- window focus/defocus callbacks 函数指针，获得 或 失去 input focus 时 此回调函数 被调用。
//-- param: GLFWwindow* -- 获得 或 失去 input focus 的 window
//-- param: int         -- 或 获得 input focus，设为 GL_TRUE。或 失去 input focus，设为 GL_FALSE
typedef void (* GLFWwindowfocusfun)(GLFWwindow*,int);
GLFWwindowfocusfun glfwSetWindowFocusCallback(GLFWwindow* window, GLFWwindowfocusfun cbfun);
    //-- param: window  --
    //-- param: cbfun   -- 新的 回调函数。若设为NULL，意在 清除 当前设置的 回调函数。

    //-- 只能被 主线程(main thread) 调用

    //-- 当失去 input focus，出发 回调函数时。所有处于 pressed 状态的 组合键 以及 鼠标按键 的 release events
    //-- 会被 generated／生成。
    //-- see @ref glfwSetKeyCallback and @ref glfwSetMouseButtonCallback

    //-- return：
    //-- 若成功，返回 旧的 回调函数
    //-- 若没有旧的，返回 NULL
    //-- 若 flgw 未 initialized，也返回 NULL



//-- window iconify/restore callbacks。 当 目标window iconified or restored 时，调用 此回调函数
//-- param: GLFWwindow* -- 最小化 或 restore 的 window
//-- param: int         -- 若 window 已经 iconified，设为 GL_TRUE。若 window 已经 restored，设为 GL_FALSE
typedef void (* GLFWwindowiconifyfun)(GLFWwindow*,int);
GLFWwindowiconifyfun glfwSetWindowIconifyCallback(GLFWwindow* window, GLFWwindowiconifyfun cbfun);
    //-- param: window  --
    //-- param: cbfun   -- 新的 回调函数。若设为NULL，意在 清除 当前设置的 回调函数。

    //-- 只能被 主线程(main thread) 调用

    //-- return：
    //-- 若成功，返回 旧的 回调函数
    //-- 若没有旧的，返回 NULL
    //-- 若 flgw 未 initialized，也返回 NULL



//-- framebuffer resize callback 。 当目标window 的 framebuffer size 改写时，调用此 回调函数
//-- param: GLFWwindow* -- 
//-- param: int         -- width 新值， in pixels, of the framebuffer
//-- param: int         -- height 新值， in pixels, of the framebuffer
typedef void (* GLFWframebuffersizefun)(GLFWwindow*,int,int);
GLFWframebuffersizefun glfwSetFramebufferSizeCallback(GLFWwindow* window, GLFWframebuffersizefun cbfun);
    //-- param: window  --
    //-- param: cbfun   -- 新的 回调函数。若设为NULL，意在 清除 当前设置的 回调函数。

    //-- 只能被 主线程(main thread) 调用

    //-- return：
    //-- 若成功，返回 旧的 回调函数
    //-- 若没有旧的，返回 NULL
    //-- 若 flgw 未 initialized，也返回 NULL



//-- Processes all pending events. ／ 处理所有 待决事件。
void glfwPollEvents(void);
    //-- 此函数只处理 已处于 event queue 中的 事件。然后立即返回
    //-- 期间，与这些 event 关联的 window／input 回调函数 将会被调用。

    //-- 在某些平台，window的 move, resize or menu operation 将会导致 事件处理的 阻塞。
    //-- 这取决于 平台的实现。
    //-- 此时，可使用 [window refresh callback](@ref window_refresh) 来 重新绘制 此window 的上下文。

    //-- 在某些平台，某些事件 会被直接发送给 applicaition，而不是进入 event queue 排队。
    //-- causing callbacks to be called outside of a 
    //-- call to one of the event processing functions.

    //-- Event processing is not required for joystick input to work.
    //-- 对于 joystick input 的运作来说，event processing 不是必须的？

    //-- 此函数 不可在 回调函数中 被调用。
    //-- 只能被 主线程(main thread) 调用



//-- 等待，直到新的 event 进入 event queue，然后处理它们。
void glfwWaitEvents(void);
    //-- 本函数 使 调用者线程睡眠，直到遇到第一个 event（排在 event queue 中）
    //-- 之后的行为很像 glfwPollEvents 函数：queue中的 ecent 得到处理，然后本函数 立即返回。
    //-- 期间，与这些 event 关联的 window／input 回调函数 将会被调用。

    //-- 鉴于 不是所有 event 都与 回调函数 关联，所以此函数可能直接返回，而不调用一个 回调函数
    //-- 哪怕 you are monitoring all callbacks.

    //-- 在某些平台，window的 move, resize or menu operation 将会导致 事件处理的 阻塞。
    //-- 这取决于 平台的实现。
    //-- 此时，可使用 [window refresh callback](@ref window_refresh) 来 重新绘制 此window 的上下文。

    //-- 在某些平台，某些 回调函数 会被 调用：outside of a call to one of the event processing functions.

    //-- 若程序中没有 一个 window，调用本函数 将立即返回。
    //-- 对于程序中 没有创建 window 的异步线程，use your threading library of choice.

    //-- Event processing is not required for joystick input to work.
    //-- 对于 joystick input 的运作来说，event processing 不是必须的？

    //-- 此函数 不可在 回调函数中 被调用。
    //-- 只能被 主线程(main thread) 调用



//-- Posts an empty event to the event queue.(from the current thread)
//-- causing @ref glfwWaitEvents to return.
void glfwPostEmptyEvent(void);
    //-- 若程序中没有 一个 window，调用本函数 将立即返回。
    //-- 对于程序中 没有创建 window 的异步线程，use your threading library of choice.

    //-- 本函数可在 任何线程被调用。




//-- 返回 目标window 的 input option 的值
//--
int glfwGetInputMode(GLFWwindow* window, int mode);
    //-- param: window
    //-- param: mode -- 必须被设置为 GLFW_CURSOR, GLFW_STICKY_KEYS or GLFW_STICKY_MOUSE_BUTTONS 三个之一。

    //-- 只能被 主线程(main thread) 调用

    //-- return：
    //-- 参数mode 的值。



//-- 设置 目标window 的 input option 的值
void glfwSetInputMode(GLFWwindow* window, int mode, int value);
    //-- param: window
    //-- param: mode -- 必须被设置为 GLFW_CURSOR, GLFW_STICKY_KEYS or GLFW_STICKY_MOUSE_BUTTONS 三个之一。
    //-- param: value -- mode 的新值。

    //-- mode: GLFW_CURSOR
        //-- value 必须为以下值：
        //--    GLFW_CURSOR_NORMAL   -- makes the cursor visible and behaving normally
        //--    GLFW_CURSOR_HIDDEN   -- makes the cursor invisible when it is over the 
                                //          client area of the window
                                //          but does not restrict the cursor from leaving.
        //--    GLFW_CURSOR_DISABLED -- hides and grabs the cursor, providing virtual
                                //          and unlimited cursor movement.
                                //          This is useful for implementing for
                                //          example 3D camera controls.

    //-- mode: GLFW_STICKY_KEYS
        //-- value 必须为以下值：
        //--    GL_TRUE   -- enable sticky keys
                        //       此时，当下次 glfwGetKey 被调用时，
                        //       一个 key press 会确保 glfwGetKey 返回 GLFW_PRESS，
                        //       哪怕在这个 调用前，这个 key 已经 released 了。
                        //  这个功能很有效，在你想要 了解某个 key 是否被触发过。（且不关心其触发时间 和触发顺序）

        //--    GL_FALSE  -- disable sticky keys

    //-- mode: GLFW_STICKY_MOUSE_BUTTONS
        //-- value 必须为以下值：
        //--    GL_TRUE   -- enable mouse buttons
                        //       此时，当下次 glfwGetMouseButton 被调用时，
                        //       一个 mouse button press 会确保 glfwGetMouseButton 返回 GLFW_PRESS，
                        //       哪怕在这个 调用前，这个 mouse button 已经 released 了。
                        //  这个功能很有效，在你想要 了解某个 mouse button 是否被触发过。（且不关心其触发时间 和触发顺序）

        //--    GL_FALSE  -- disable mouse buttons

    //-- 只能被 主线程(main thread) 调用

    //-- GLFW 3.0. 之后，  本函数取代  `glfwEnable` and `glfwDisable`.




//-- 返回 目标window 的 目标键盘按键 最新 一次状态／last reported state。
int glfwGetKey(GLFWwindow* window, int key);
    //-- param: window --
    //-- param: key    -- 目标 key， GLFW_KEY_UNKNOWN 在此是 无效的。

    //-- The higher-level action `GLFW_REPEAT` is only reported to the key callback.
     
    //-- 不要用此功能来实现 字符打印。改用 Unicode character callback

    //-- The [modifier key bit masks](@ref mods) are not key tokens and cannot be used with this function.

    //-- 只能被 主线程(main thread) 调用

    //-- 本函数 可在 回调函数中被调用。
    //   这意味着，可以 检测 复合键，比如：Ctrl+F3：
    //      当你获得一个 GLFW_KEY_F3 的 key press event 时
    //      通过 glfwGetKey(window, GLFW_KEY_LEFT_CONTROL);
    //          glfwGetKey(window, GLFW_KEY_RIGHT_CONTROL);
    //      去检测 左／右 CTRL 的情况。 来获知 是否出现了 Ctrl+F3。
    //

    //-- return：
    //-- GLFW_PRESS   -- 目标键 正被按下 （对于STICKY_KEY，则是 曾被按下过）
                        //              If the `GLFW_STICKY_KEYS` input mode is enabled

    //-- GLFW_RELEASE -- 目标键 已经释放 （对于STICKY_KEY，则是 从未被按下过）




//-- 返回 目标window 的 目标 mouse button 最新 一次状态／last reported state。
int glfwGetMouseButton(GLFWwindow* window, int button);
    //-- param: window --
    //-- param: button    -- 目标 mouse button， 

    //-- 只能被 主线程(main thread) 调用

    //-- return：
    //-- GLFW_PRESS   -- 目标键 正被按下 （对于STICKY_KEY，则是 曾被按下过）
                        //         If the `GLFW_STICKY_MOUSE_BUTTONS` input mode is enabled

    //-- GLFW_RELEASE -- 目标键 已经释放 （对于STICKY_KEY，则是 从未被按下过）




//-- 获取 目标window 的 client area 的 cursor position。（ in screen coordinates，左上坐标，
void glfwGetCursorPos(GLFWwindow* window, double* xpos, double* ypos);
    //-- param: window
    //-- param: xpos  -- 可设为 NULL，若不想获得此数据
    //-- param: ypos  -- 可设为 NULL，若不想获得此数据

    //-- 若目标 cursor mode 是  GLFW_CURSOR_DISABLED
    //-- 此时的 cursor pos 是未绑定的，其值的大小极限 仅 受限于 double 类型。

    //--  坐标系 可通过 floor函数／地板除， 转换为 整形数值。
    //-- Casting directly to an integer type works for positive coordinates, 
    //-- but fails for negative ones.

    //-- 只能被 主线程(main thread) 调用

    //-- Added in GLFW 3.0.  Replaces `glfwGetMousePos`.

    //-- return：
    //-- 若出错，所有 没被设为 NULL 的pos参数 都被写入 0




//-- 设置 目标window 的 client area 的 cursor position。（ in screen coordinates，左上坐标，
void glfwSetCursorPos(GLFWwindow* window, double xpos, double ypos);
    //-- param: window
    //-- param: xpos  
    //-- param: ypos

    //-- 目标window 必须处于 input focus，若没有，调用本函数 将导致 无声的错误／fails silently

    //-- 不要将本函数用于 实现类似 相机控制／camera controls 之类的功能。
    //-- glfw 已经提供了 `GLFW_CURSOR_DISABLED` cursor mode：
    //--    它将：hides the cursor, transparently re-centers it 
    //--         and provides unconstrained cursor motion.
    //--    See @ref glfwSetInputMode for more information.

    //-- 若目标 cursor mode 是  GLFW_CURSOR_DISABLED
    //-- 此时的 cursor pos 是不受束缚的，其值的大小极限 仅 受限于 double 类型。

    //-- X11，由于 X11 是异步的，需要点时间让 window focus event 到达
    //-- 这意味着，无法在调用本函数后，立即直接设置 cursor pos

    //-- 只能被 主线程(main thread) 调用

    //-- Added in GLFW 3.0.  Replaces `glfwSetMousePos`.




//-- Image data.
typedef struct GLFWimage
{
    int width;   // The width, in pixels, of this image.
    int height;   // The height, in pixels, of this image.
    // The pixel data of this image, arranged left-to-right, top-to-bottom.
    unsigned char* pixels;
} GLFWimage;
//-- 不透明的 cursor object.
typedef struct GLFWcursor GLFWcursor;
//-- Creates a custom cursor.
GLFWcursor* glfwCreateCursor(const GLFWimage* image, int xhot, int yhot);
    //-- param: image -- cursor 图形
    //-- param: xhot  -- x-coordinate, in pixels, of the cursor hotspot／热点
    //-- param: yhot  -- y-coordinate, in pixels, of the cursor hotspot／热点

    //-- 创建一个 新的 cursor image，它可被 glfwSetCursor 使用。
    //-- 此 cursor 可被 glfwDestroyCursor 销毁。

    //-- 所有遗留的 cursor，都被 glfwTerminate 销毁。

    //-- The pixels are 32-bit, little-endian, non-premultiplied RGBA, 
    //--    i.e. eight bits per channel.

    //-- cursor 的 hotspot／热点 是 pixels，相对于 cursor image 的 左上坐标系 值。

    //-- 指定的 图形数据，会在本函数返回之前，被copy。（所有不用关心它的生命周期

    //-- 此函数 不能在 回调函数中 被调用
    //-- 只能被 主线程(main thread) 调用

    //-- return：
    //-- 若成功，返回 The handle of the created cursor
    //-- 若出错，返回 NULL



//-- Creates a cursor with a standard shape.
GLFWcursor* glfwCreateStandardCursor(int shape);
    //-- param: shape -- One of the [standard shapes](@ref shapes).

    //-- 此函数 不能在 回调函数中 被调用
    //-- 只能被 主线程(main thread) 调用

    //-- return：
    //-- 若成功，返回一个 新cursor，可被 glfwSetCursor 调用。
    //-- 若出错，返回 NULL。



//-- Destroys a cursor.
void glfwDestroyCursor(GLFWcursor* cursor);

    //-- 所有遗留的 cursor，都被 glfwTerminate 销毁。

    //-- 此函数 不能在 回调函数中 被调用
    //-- 只能被 主线程(main thread) 调用



//-- Sets the cursor for the window.
void glfwSetCursor(GLFWwindow* window, GLFWcursor* cursor);
    //-- param: window
    //-- param: cursor -- 可设为 NULL，若想 使用 default arrow cursor.

    //-- 只有在 目标window 处于 GLFW_CURSOR_NORMAL 模式，新设置的 cursor 才可见。

    //-- 在某些平台，只有在 目标window 处于 input focus 状态，新设的 cursor 才可见。

    //-- 只能被 主线程(main thread) 调用




//-- keyboard key callbacks 函数指针
//-- param: GLFWwindow*
//-- param: int key      -- key
//-- param: int scancode -- The system-specific scancode of the key
//-- param: int action   -- `GLFW_PRESS`, `GLFW_RELEASE` or `GLFW_REPEAT`
//-- param: int mods     -- Bit field describing which [modifier keys](@ref mods) 
                            // were held down.
typedef void (* GLFWkeyfun)(GLFWwindow*,int,int,int,int);
//-- Sets the key callback.
GLFWkeyfun glfwSetKeyCallback(GLFWwindow* window, GLFWkeyfun cbfun);
    //-- param: window 
    //-- param: cbfun  -- key callback 新值。可设为，NULL，若想 清除当前设置的 回调函数。

    //-- 不要用此函数来 获得 文本输入
    //-- 改用 glfwSetCharCallback

    //-- 当一个 window 失去 input focus，
    //-- 它将向 所有 处于 pressed 状态的 key，生成 同步的 key release event

    //-- 在调用 glfwSetWindowFocusCallback 做有关设置后，
    //-- 用户可以 通过 user-generated event tell 上文中的 key release event ？？？

    //-- scancode of a key 对于不同的机器 和 平台 都是独立／特定的。
    //-- scancode 意在允许 用户 绑定那些 没有 GLFW key token 的 key。
    //-- 这样的 key have `key` set to `GLFW_KEY_UNKNOWN`,
    //-- 它们的 状态是 not saved，所以无法通过 glfwGetKey 来查询。

    //-- 有时，GLFW needs to generate synthetic key events
    //-- 这会导致  scancode 可能为 0.

    //-- 只能被 主线程(main thread) 调用

    //-- return：
    //-- 若成功，返回 旧的 回调函数。
    //-- 若没有旧的，返回 NULL
    //-- 若 glfw 未 initialized，也返回 NULL。



//-- Unicode character callbacks 函数指针。
//-- param: GLFWwindow*
//-- param: int codepoint -- The Unicode code point of the character.
typedef void (* GLFWcharfun)(GLFWwindow*,unsigned int);
//-- Sets the Unicode character callback. of the specified window
GLFWcharfun glfwSetCharCallback(GLFWwindow* window, GLFWcharfun cbfun);
    //-- param: window
    //-- param: cbfun -- new char回调函数。可设为NULL，若想 清除当前设置的 回调函数

    //-- 当 a Unicode character is input，char回调函数会被 调用。

    //-- char回调函数 意在获得 文本input。

    //-- 当 修饰健（比如 shift）按下，char回调函数 不会被调用。
    //-- 这些 修饰键 的回调函数 将被 glfwSetCharModsCallback 设置。

    //-- 只能被 主线程(main thread) 调用

    //-- return：
    //-- 若成功，返回 旧的 回调函数。
    //-- 若没有旧的，返回 NULL
    //-- 若 glfw 未 initialized，也返回 NULL。




//-- Unicode character with modifiers callbacks./ 修饰键 回调函数
//-- param: GLFWwindow*
//-- param: int codepoint -- The Unicode code point of the character.
//-- param: int mods      -- Bit field describing which [modifier keys](@ref mods) 
                                // were held down.
typedef void (* GLFWcharmodsfun)(GLFWwindow*,unsigned int,int);
//-- Sets the Unicode character with modifiers callback. ／ 修饰键
GLFWcharmodsfun glfwSetCharModsCallback(GLFWwindow* window, GLFWcharmodsfun cbfun);
    //-- param: window
    //-- param: cbfun -- new char回调函数。可设为NULL，若想 清除当前设置的 回调函数

    //-- which is called when a Unicode character is input regardless of 
    //-- what modifier keys are used.

    //-- charmods回调函数 意在实现 custom Unicode character input ／客户定制的 input
    
    //-- 只能被 主线程(main thread) 调用

    //-- return：
    //-- 若成功，返回 旧的 回调函数。
    //-- 若没有旧的，返回 NULL
    //-- 若 glfw 未 initialized，也返回 NULL。





//-- mouse button callbacks. 回调函数指针
//-- keyboard key callbacks 函数指针
//-- param: GLFWwindow*
//-- param: int button   -- The mouse button that was pressed or released.
//-- param: int action   -- `GLFW_PRESS` or `GLFW_RELEASE` 
//-- param: int mods     -- Bit field describing which [modifier keys](@ref mods) 
                            // were held down.
typedef void (* GLFWmousebuttonfun)(GLFWwindow*,int,int,int);
//-- Sets the mouse button callback.
GLFWmousebuttonfun glfwSetMouseButtonCallback(GLFWwindow* window, GLFWmousebuttonfun cbfun);
    //-- param: window
    //-- param: cbfun -- new char回调函数。可设为NULL，若想 清除当前设置的 回调函数

    //-- 当一个 window 失去 input focus，
    //-- 它将向 所有 处于 pressed 状态的 mouse button，
    //-- 生成 同步的 mouse button release event

    //-- 在调用 glfwSetWindowFocusCallback 做有关设置后，
    //-- 用户可以 通过 user-generated event tell 上文中的 mouse button release event ？？？

    //-- 只能被 主线程(main thread) 调用

    //-- return：
    //-- 若成功，返回 旧的 回调函数。
    //-- 若没有旧的，返回 NULL
    //-- 若 glfw 未 initialized，也返回 NULL。



//-- cursor position callbacks 回调函数指针
//-- param: GLFWwindow*
//-- param: double  xpos -- The new x-coordinate, in screen coordinates, of the cursor.
//-- param: double  ypos -- The new y-coordinate, in screen coordinates, of the cursor.
typedef void (* GLFWcursorposfun)(GLFWwindow*,double,double);
//-- sets the cursor position callback of the 目标 window,
GLFWcursorposfun glfwSetCursorPosCallback(GLFWwindow* window, GLFWcursorposfun cbfun);
    //-- param: window
    //-- param: cbfun -- new char回调函数。可设为NULL，若想 清除当前设置的 回调函数

    //-- cursorpos回调函数 在 cursor move 后 被调用。

    //-- 只能被 主线程(main thread) 调用

    //-- Added in GLFW 3.0.  Replaces `glfwSetMousePosCallback`.

    //-- return：
    //-- 若成功，返回 旧的 回调函数。
    //-- 若没有旧的，返回 NULL
    //-- 若 glfw 未 initialized，也返回 NULL。




//-- cursor enter/leave callbacks 回调函数指针
//-- param: GLFWwindow*
//-- param: int entered -- `GL_TRUE` if the cursor entered the window's client area, 
                            // or `GL_FALSE` if it left it.
typedef void (* GLFWcursorenterfun)(GLFWwindow*,int);
//-- sets the cursor boundary／边界 crossing callback of the 目标 window
GLFWcursorenterfun glfwSetCursorEnterCallback(GLFWwindow* window, GLFWcursorenterfun cbfun);
    //-- param: window
    //-- param: cbfun -- new char回调函数。可设为NULL，若想 清除当前设置的 回调函数

    //-- 当 目标cursor 进入／离开 目标window 的 client area 时，本函数 被调用

    //-- 只能被 主线程(main thread) 调用

    //-- return：
    //-- 若成功，返回 旧的 回调函数。
    //-- 若没有旧的，返回 NULL
    //-- 若 glfw 未 initialized，也返回 NULL。




//-- scroll callback 回调函数指针
//-- param: GLFWwindow*
//-- param: double xoffset -- The scroll offset along the x-axis
//-- param: double yoffset -- The scroll offset along the y-axis
typedef void (* GLFWscrollfun)(GLFWwindow*,double,double);
//-- sets the scroll callback of the 目标 window
GLFWscrollfun glfwSetScrollCallback(GLFWwindow* window, GLFWscrollfun cbfun);
    //-- param: window
    //-- param: cbfun -- new char回调函数。可设为NULL，若想 清除当前设置的 回调函数

    //-- 当 一个 scrolling device 被使用，（例如 鼠标滚轮，或触摸板 ）时
    //-- scroll回调函数 被调用。

    //-- 只能被 主线程(main thread) 调用

    //-- Added in GLFW 3.0.  Replaces `glfwSetMouseWheelCallback`.

    //-- return：
    //-- 若成功，返回 旧的 回调函数。
    //-- 若没有旧的，返回 NULL
    //-- 若 glfw 未 initialized，也返回 NULL。



//-- file drop callbacks 回调函数指针
//-- param: GLFWwindow*
//-- param: int    count -- The number of dropped files.
//-- param: char** paths -- The UTF-8 file and/or directory pathnames.
typedef void (* GLFWdropfun)(GLFWwindow*,int,const char**);
//-- sets the file drop callback of the 目标 window
GLFWdropfun glfwSetDropCallback(GLFWwindow* window, GLFWdropfun cbfun);
    //-- param: window
    //-- param: cbfun -- new char回调函数。可设为NULL，若想 清除当前设置的 回调函数

    //-- 当 one or more dragged files are dropped on the window，此回调函数 被调用。
    //-- 当一个 或 数个文件 被拖拽到 window 中时，

    //-- 由于 路径名数组，及其路径名字符串，是单独为这个 事件创建的
    //-- 当 本回调函数 返回时，这些数据无法保证 还有效。
    //-- 若想在后面访问这些数据，用户需要 自行 复制存放它们。

    //-- 只能被 主线程(main thread) 调用

    //-- return：
    //-- 若成功，返回 旧的 回调函数。
    //-- 若没有旧的，返回 NULL
    //-- 若 glfw 未 initialized，也返回 NULL。



/* ==========================================================
 *                    Joystick
 *-----------------------------------------------------------
 * glfw 最多支持 16 个 joystick。
 * 包含： 
 *     GLFW_JOYSTICK_1  (=0)
 *     GLFW_JOYSTICK_2
 *     ...
 *     GLFW_JOYSTICK_16    (=15)
 *     GLFW_JOYSTICK_LAST  (=15)
 * ------
 *  glfw 初始化之后，被查明的 joysticks 将被添加到一个 列表中 
 *  每个 joystick 将保持自己的 id号不变，直到它的链接终止，或者 glfw结束
 * ------
 *  如果想要在程序中实时获得 joystick 的 连接和断开信息，使用：
 *     joystick_callback() 
 *  官网有详细教程。
 * 
 * 
 */


//-- 检测 目标joystick 是否存在。
int glfwJoystickPresent(int joy);
    //-- 只能在 主线程／main thread 被调用。

    //-- Added in GLFW 3.0.  Replaces `glfwGetJoystickParam`.

    //-- return：
    //-- 若存在，返回    GL_TRUE
    //-- 若不存在，返回  GL_FALSE




//-- Returns the values of all axes／轴 of the specified joystick.
const float* glfwGetJoystickAxes(int joy, int* count);
    //-- param: joy
    //-- param: count -- 存放 “返回值数组中 轴值 的个数”。 当函数出错，此值被写入 0

    //-- 轴值 区间 -1.0 到 1.0。

    //-- 返回值 由 glfw 分配和释放内存，用户不应手动释放。
    //-- 此值将持续有效，直到 目标joystick 被 取消连接。或者 此函数被再次调用，或者 glfw 库 terminated。
    
    //---------------------------------------
    //   实践证明，这组函数需要在每1帧访问前，都调用

    //-- 只能在 主线程／main thread 被调用。

    //-- Added in GLFW 3.0.  Replaces `glfwGetJoystickPos`.

    //-- return:
    //-- 若成功，返回一个数组，元素是 各个 轴值。
    //-- 若 joystick 不存在，返回 NULL。




//-- Returns the state of all buttons of the specified joystick.
const unsigned char* glfwGetJoystickButtons(int joy, int* count);
    //-- param: joy   --
    //-- param: count -- 存放 “返回值中 元素 的个数”。 当函数出错，此值被写入 0

    //-- 返回值 由 glfw 分配和释放内存，用户不应手动释放。
    //-- 此值将持续有效，直到 目标joystick 被 取消连接。或者 此函数被再次调用，或者 glfw 库 terminated。

    //---------------------------------------
    //   实践证明，这组函数需要在每1帧访问前，都调用


    //-- 只能在 主线程／main thread 被调用。

    //-- return：
    //-- 若成功，返回一个数组，元素是 各个 button states。值为 `GLFW_PRESS` or `GLFW_RELEASE`
    //-- 若出错，返回 NULL


//-- 一个 hat 就是 传统手柄上的的 十字键 
const unsigned char* glfwGetJoystickHats( int jid, int *count ); // [3.3]
    //-- 用法和 glfwGetJoystickButtons 基本一致。

    //---------------------------------------
    //   实践证明，这组函数需要在每1帧访问前，都调用

    // --------
    //  name:               value:
    // GLFW_HAT_CENTERED	0
    // GLFW_HAT_UP	        1
    // GLFW_HAT_RIGHT	    2
    // GLFW_HAT_DOWN	    4
    // GLFW_HAT_LEFT	    8
    // GLFW_HAT_RIGHT_UP	GLFW_HAT_RIGHT | GLFW_HAT_UP
    // GLFW_HAT_RIGHT_DOWN	GLFW_HAT_RIGHT | GLFW_HAT_DOWN
    // GLFW_HAT_LEFT_UP	    GLFW_HAT_LEFT | GLFW_HAT_UP
    // GLFW_HAT_LEFT_DOWN	GLFW_HAT_LEFT | GLFW_HAT_DOWN
    // --------
    // 可以看出，这是使用 一个 u8_t 来存储 整个 hat 数据

    // 这个函数 可以一次取出全部的 hats 数据，（不止一个）



//-- Returns the name of the specified joystick. UTF-8，
const char* glfwGetJoystickName(int joy);
    //-- 返回值 由 glfw 分配和释放内存，用户不应手动释放。
    //-- 此值将持续有效，直到 目标joystick 被 取消连接。或者 此函数被再次调用，或者 glfw 库 terminated。

    //-- 只能在 主线程／main thread 被调用。

    //-- return：
    //-- 若成功，返回 name 
    //-- 若 目标joystick 不存在，返回 NULL



//-- 暂时没看出来，这组函数有什么用 --
void glfwSetJoystickUserPointer( int jid, void *pointer );
void* glfwGetJoystickUserPointer( int jid );




//-- sets the system clipboard to the specified, UTF-8 encoded string.
void glfwSetClipboardString(GLFWwindow* window, const char* string);
    //-- 目标字符串 在本函数返回前 会被copy，所以不用担心其 生命周期。

    //-- 只能在 主线程／main thread 被调用。




//-- Returns the contents of the clipboard as a string.
const char* glfwGetClipboardString(GLFWwindow* window);
    //-- 返回值 由 glfw 分配和释放内存，用户不应手动释放。
    //-- 此值将持续有效，直到 glfwGetClipboardString 或 glfwSetClipboardString 函数被再次调用，
    //-- 或者 glfw 库 terminated。

    //-- 只能在 主线程／main thread 被调用。

    //-- return
    //-- 若 system clipboard 里有数据，且数据可被转换为 UTF-8格式，返回这些数据（字符串）。
    //-- 若 system clipboard 是空的，或数据无法被转换为 UTF-8，返回 NULL，
                                            //  且生成 error： GLFW_FORMAT_UNAVAILABLE




//-- Returns the value of the GLFW timer.
double glfwGetTime(void);
    //-- 调用本函数前，确保 timer 已被 glfwSetTime 设置。

    //-- timer 计算自 glfw initialized 起 到现在 的时间。
    //-- timer 的构成由 系统决定。

    //-- 本函数 可被任何 线程调用，且异步
 
    //-- return：
    //-- 若成功，返回 timer 当前值，秒为单位。
    //-- 若出错，返回 0.




//-- Sets the GLFW timer.
void glfwSetTime(double time);
    //-- param: time -- 新设定的值，秒。必须为正数，小于等于 18446744073.0（ 约584.5年 ）

    //-- 只能在 主线程／main thread 被调用。




//-- Makes the context of the 目标window current for the calling thread.
void glfwMakeContextCurrent(GLFWwindow* window);
    //-- param: window -- 若设为NULL，意在 分离／detach the current context

    //-- 一个 上下文 在单一时间 只能在 单个线程中 be made current
    //-- 且 每个线程 在单一时间 只能由 一个 上下文 be made current

    //-- 默认情况下，让一个 上下文 non-current 会立即 促使 pipeline flush

    //-- 对于支持 GL_KHR_context_flush_control 的机器，
    //-- 用户可以通过 [GLFW_CONTEXT_RELEASE_BEHAVIOR](@ref window_hints_ctx)
    //-- 来控制 这个 上下文变化 是否会触发 flush

    //-- 本函数 可被 任何线程 调用。




//-- Returns the window whose context is current on the calling thread.
GLFWwindow* glfwGetCurrentContext(void);
    //-- 本函数 可被 任何线程 调用。

    //-- return：
    //-- 若存在 current window，返回它
    //-- 若不存在，返回 NULL





//-- Swaps the front and back buffers of the 目标window.
void glfwSwapBuffers(GLFWwindow* window);
    //-- 如果 swap 间隙大于0，GPU驱动 将在执行 swap之前，
    //-- 等待 the specified number of screen updates

    //-- 本函数 可被 任何线程 调用。





//--  Sets the swap interval／间隙 for the current context.
void glfwSwapInterval(int interval);
    //-- param: interval -- The minimum number of screen updates 
                        //    to wait for until the buffers are swapped 
                        //    by @ref glfwSwapBuffers.

    //-- the number of screen updates to wait 
    //-- from the time glfwSwapBuffers was called 
    //-- before swapping the buffers and returning.
    //-- 有时这被称为：
    //      _vertical synchronization_         ／ 垂直同步
    //      _vertical retrace synchronization_ ／ 垂直同步
    //      _vsync_                            ／ 垂直同步

    //-- 支持 `WGL_EXT_swap_control_tear` 以及 `GLX_EXT_swap_control_tear` 扩展的
    //-- 上下文 同时会 接受 negative swap intervals／间隙

    //-- 这会允许 GPU驱动 执行 swap，even if a frame arrives a little bit late
    //-- 用户可通过 glfwExtensionSupported 来检查 这些 extensions／扩展 是否存在。

    //-- 注意，此函数 不在 上下文创建时期 被调用。
    //-- 由平台决定，是否将 swap间歇 设定为 默认值。
    //-- 这是因为，一旦将 swap间隙值 设定为 非 0 值，
    //-- glfw的某些 扩展 不允许 将 swap间歇值 重设为 0。

    //-- 注意，某些 GPU驱动 并不尊重 这些 用户定义的 swap间隙值，
    //-- 要么是因为 用户设定 覆盖／否定 了程序需求。 或则是 GPU驱动 存在 bug

    //-- 本函数 可被 任何线程 调用。




//-- Returns whether the 目标extension is available.
int glfwExtensionSupported(const char* extension);
    //-- param: extension -- ASCII str name of the extension

    //-- It searches both for OpenGL and OpenGL ES extension 
    //-- and “平台特定context creation API extensions”.

    //-- 调用本函数的 线程必须存在一个 current context。
    //-- 否则将 引起 GLFW_NO_CURRENT_CONTEXT error。

    //-- it is recommended that you cache its results 
    //-- if it is going to be used frequently.  
    //-- The extension strings will not change during the lifetime of a context, 
    //-- so there is no danger in doing this.

    //-- 本函数 可被 任何线程 调用。

    //-- return:
    //-- 若 目标extension is available，返回 GL_TRUE。
    //-- 否则，返回 GL_FALSE。





//-- Client API function pointer type.
typedef void (*GLFWglproc)(void);
//-- Returns the address of the 目标func for the current context.
GLFWglproc glfwGetProcAddress(const char* procname);
    //-- param: procname -- ASCII

    //-- 调用本函数的 线程必须存在一个 current context。
    //-- 否则将 引起 GLFW_NO_CURRENT_CONTEXT error。

    //-- 注意，不同 context 的 目标函数 的地址 不保证是 相同的。

    //-- 注意，尽管 与目标函数 有关的 glfw扩展 可能 not available，
    //-- 本函数 仍有可能返回 non-‘NULL’ 值。
    //-- 所以，在使用本函数前，始终优先 确保 context version or extension string

    //-- 返回的 函数指针 始终有效，直到 此 context 被销毁，或者 glfw库 被 terminated。

    //-- 本函数 可被 任何线程 调用。

    //-- return:
    //-- 若成功，返回 目标函数 的地址 （函数指针）
    //-- 若出错，返回 NULL








































# ================================================================//
#                          GLFW
# ================================================================//

GLFW is a free, Open Source, multi-platform library for OpenGL, OpenGL ES 
and Vulkan application development. 
It provides a simple, platform-independent API for 
creating windows, contexts and surfaces, 
reading input, handling events, etc.
-----
GLFW 是一个 自由，开源，多平台 代码库。
适用于 OpenGL, OpenGL ES， Vulkan 的应用程序开发。
它提供一套 简单，平台独立的 API，可用于：
    -- 创建 window，context(OpenGL)，surface(Vulkan)
    -- 读取 input
    -- hanlding event／处理事件。


GLFW 是一个专门针对 OpenGL 的 C语言库，
它提供了一些 渲染物体 所需的最低限度的 接口。
它允许用户创建 OpenGL 上下文，
定义 窗口参数 以及 处理用户输入。


#-- 如何 获得 GLFW 库 ------

-- 下载网址：
http://www.glfw.org/download.html
    -- Source package
        源文件，需自己编译，推荐。

    -- binaries
        若是选择 已经编译好的 二进制文件，请不要选择 64 位的。


#============================================#
#        GLFW 在各平台使用的 system APIs
#============================================#

#-- Winodws --
    -- plain Win32     用于window 创建，input 管理。
    -- WGL / EGL       实现 OpenGL，OpenGL ES context

#-- OSX --
    -- Cocoa         用于window 创建，input 管理。
    -- NSOpenGL      实现 OpenGL，OpenGL ES context

#-- Unix-like systems using the X Window System --
    -- Xlib API      用于window 创建，input 管理。
    -- GLX / EGL     实现 OpenGL，OpenGL ES context

#-- Linux using Wayland --
    -- Wayland      用于window 创建，input 管理。
    -- EGL          实现 OpenGL，OpenGL ES context

#-- Linux using Mir --
    -- Mir        用于window 创建，input 管理。
    -- EGL        实现 OpenGL，OpenGL ES context





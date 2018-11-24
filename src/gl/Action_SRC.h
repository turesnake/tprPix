/*
 * ========================= Action_SRC.h ==========================
 *                          -- GL_CPP --
 *                                        创建 -- 2018.11.23
 *                                        修改 -- 2018.11.23
 * ----------------------------------------------------------
 *   资源类
 *   存储一个 动画动作 的所有 帧text names
 *   以及每一帧 的 投影单位集
 *   本游戏只支持 png 图片格式。
 * 
 *   -----------------
 *   一个完整的 动画动作, 会被存储为 2 张 png 图片。
 *   一张 存储 画面，一张 存储 投影区域
 *   图片名字格式为： 
 *       dog_ack_01.P.65_112.3_4.17_2.png (画面)
 *       dog_ack_01.J.png            (投影)
 *    格式拆分如下：
 *      dog_ack_01 --- 真正的名字，记录到 name 中
 *      P / J      --- P==画面， J==投影
 *      65_112     --- 单帧 宽高 像素值
 *      3_4        --- 本图片横排放 3 帧，一共 4 排。
 *      17_2       --- 每一帧的锚点。 "中下点"
 * 
 * 
 *      以 文件名表示信息的 方式，可能会被配置表取代。
 * 
 * ----------------------------
 */
#ifndef _TPR_ACTION_SRC_H_
#define _TPR_ACTION_SRC_H_

/* -- 确保 glad GLFW 两个库 的引用顺序 ---
 * --    glad.h 包含了正确的OpenGL头文件（如GL/gl.h），
 * --    所以需要在其它依赖于OpenGL的头文件之前 包含 glad.h
 */
#include <glad/glad.h> 

//-------------------- CPP --------------------//
#include <string>
#include <vector>

//------------------- Libs --------------------//
#include "nocopyable.h" 
#include "tprDataType.h"



class Action_SRC : tpr::nocopyable {

public:
    explicit Action_SRC(  )
        //:
        {}

    ~Action_SRC() = default;


    void init();


private:

    //-- 本动画动作 的name。 起到 id 的作用
    std::string  name;

    //- 画面 贴图集的 相对路径名。一个动作的所有帧图片，存储为一张 png图。
    //- 这个 路径名 只在游戏启动阶段被使用，之后 预存于此
    std::string  lpath_pic;

    //- 画面 投影单位集的 相对路径名。一个动作的所有帧图片，存储为一张 png图。
    std::string  lpath_pjt;   

    //- 单帧画面 的 长宽 像素值
    u32  frame_w_pix {};
    u32  frame_h_pix {};  

    //- 画面中，横排有几帧，一共有几排
    u32  w_frames {};
    u32  h_frames {};

    //- 每一帧中，"中下" 锚点 像素值
    u32  anchor_w {};
    u32  anchor_h {};

    //- 动画中的每一帧图都会被 存储为 一个 texture实例。
    //- 具体数据存储在 gl状态机内。 此处存储其 textel names 
    std::vector<GLuint> textelNames;   

    




};













#endif

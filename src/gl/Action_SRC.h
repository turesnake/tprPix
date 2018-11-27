/*
 * ========================= Action_SRC.h ==========================
 *                          -- tpr --
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


//-- 1个 png 像素 的 data 结构 --
struct PngPix{
    u8 r {0};
    u8 g {0};
    u8 b {0};
    u8 a {0};
};


class Action_SRC{

public:
    explicit Action_SRC(const std::string &_lpath_pic, 
                        u32 _pixes_per_frame_w,
                        u32 _pixes_per_frame_h,
                        u32 _frames_w,
                        u32 _frames_h,
                        u32 _root_anchor_w,
                        u32 _root_anchor_h
                        ):
        lpath_pic(_lpath_pic),
        pixes_per_frame_w(_pixes_per_frame_w),
        pixes_per_frame_h(_pixes_per_frame_h),
        frames_w(_frames_w),
        frames_h(_frames_h),
        root_anchor_w(_root_anchor_w),
        root_anchor_h(_root_anchor_h)
        {}


    void init();

    //--- 分别 加载，分解 pic / pjt 两张图 --- 
    void load_and_divide_png( bool _is_pic,
                std::vector< std::vector<PngPix>> &_frame_data_ary );


private:

    //-- 本动画动作 的name。 起到 id 的作用
    std::string  name;

    //- 画面 贴图集的 相对路径名。一个动作的所有帧图片，存储为一张 png图。
    //- 这个 路径名 只在游戏启动阶段被使用，之后 预存于此
    //- 相对路径名，从 path_action_srcs 目录开始
    std::string  lpath_pic;

    //- 画面 投影单位集的 相对路径名。一个动作的所有帧图片，存储为一张 png图。
    std::string  lpath_pjt;   

    //- 单帧画面 的 长宽 像素值
    int  pixes_per_frame_w {};
    int  pixes_per_frame_h {};  

    //- 画面中，横排可分为几帧，纵向可分为几帧
    int  frames_w {};
    int  frames_h {};

    //- 每一帧中，root锚点 像素值
    int  root_anchor_w {};
    int  root_anchor_h {};

    //- 动画中的每一帧图都会被 存储为 一个 texture实例。
    //- 具体数据存储在 gl状态机内。 此处存储其 textel names 
    //- 帧排序 符合 左上坐标系 --
    std::vector<GLuint> texNames;   

    //- 一整张 png 图的数据，真实存储区。
    //- 通过 stbi_load 直接读取到的数据
    //   ***  AS 类并不存储这些 数据  ***
    //std::vector<u8>  png_P_total_buf; //- pic
    //std::vector<u8>  png_J_total_buf; //- pjt


};













#endif

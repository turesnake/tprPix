/*
 * ========================= Action.h ==========================
 *                          -- tpr --
 *                                        创建 -- 2018.11.23
 *                                        修改 -- 2018.11.28
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
 *      以 文件名表示信息的 方式，可能会被配置表取代。
 *   
 *   -----------------
 *   texture 管理：
 *   gl中，texture资源的唯一限制在于 显存大小。
 *   所以，在目前版本中，假定现存无限大，
 *   将游戏的所有 texture资源 一股脑存入 显存中
 *  （等正式爆发 显存危机了，再做调整...）
 *   ---
 *   随着 tex动态生成器的 引入，texture 的数量将呈爆炸态
 *   此时就有必要进入 更为完善的 tex显存管理（到时再说）
 * ----------------------------
 */
#ifndef _TPR_ACTION_H_
#define _TPR_ACTION_H_
//=== *** glad FIRST, glfw SECEND *** ===
#include <glad/glad.h> 

//-------------------- CPP --------------------//
#include <string>
#include <vector>

//------------------- Libs --------------------//
#include "nocopyable.h" 
#include "tprDataType.h"

//-------------------- Engine --------------------//
#include "IntVec.h" 
#include "RGBA.h" 
#include "FramePos.h"


//-- AS实例 需要的参数 --
struct ActionParams{
    std::string lpath_pic;
    IntVec2  pixes_per_frame;
    IntVec2  frameNums;
    IntVec2  anchor_root;

};


//-- 动画帧类型。以后有用处 ---
enum class ActionType{
    Null      = 1, //- 空
    Walk_loop = 2  //- tmp
};


//-- 作为纯粹的 图像资源类，Action 应该被设计得尽可能简洁 --
//   不负责其他任何数据 
//   Action 是唯一的类，不存在 具象as类。
class Action{
public:
    Action( const std::string &_lpath_pic, 
                IntVec2  _pixes_per_frame,
                IntVec2  _frameNum,
                int      _totalFrameNum 
                ):
        lpath_pic(_lpath_pic),
        pixes_per_frame(_pixes_per_frame),
        frameNum(_frameNum),
        totalFrameNum(_totalFrameNum)
        {}

    void init();

    //void debug() const; //- 向终端输出 本Action的信息，用来 debug
    //---------------------- vals -------------------------//

    //-- 本动画动作 的name。 起到 id 的作用。
    //-- 是否使用字符串有待商榷，取决于，是否会跟随go数据存入硬盘中。
    std::string  name;

    ActionType   type {ActionType::Null};  //- 类型，以后可能有用

    //- 画面 贴图集的 相对路径名。一个动作的所有帧图片，存储为一张 png图。
    //- 这个 路径名 只在游戏启动阶段被使用，之后 预存于此
    //- local path，based on path_action_srcs
    std::string  lpath_pic;    //-- picture
    std::string  lpath_pjt;    //-- collients
    std::string  lpath_shadow; //-- shadow



    IntVec2  pixes_per_frame {};  //- 单帧画面 的 长宽 像素值
    IntVec2  frameNum {};        //- 画面中，横排可分为几帧，纵向可分为几帧
    int      totalFrameNum {};   //- 总 图元帧 个数

    //- 动画中的每一帧图都会被 存储为 一个 texture实例。
    //- 具体数据存储在 gl状态机内。 此处存储其 textel names 
    //- 帧排序 符合 左上坐标系（也就是我们排列动画帧的坐标系） --
    std::vector<GLuint> texNames_pic; 
    std::vector<GLuint> texNames_shadow; 

    //-- each frame --
    std::vector<FramePos>  framePoses {};

private:
    void create_texNames( std::vector<std::vector<RGBA>> &_frame_data_ary,
                          std::vector<GLuint> &_texNames );

};


#endif


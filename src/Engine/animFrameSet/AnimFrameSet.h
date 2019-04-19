/*
 * =================== AnimFrameSet.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2018.11.23
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *   资源类
 *   存储一个 动画动作 的所有 帧text names
 *   以及每一帧 的 投影单位集
 *   本游戏只支持 png 图片格式。
 * 
 *   -----------------
 *   一个完整的 动画动作, 会被存储为 3 张 png 图片。
 *   一张 存储 画面，一张 存储 投影区域
 *   图片名字格式为： 
 *       dog_ack_01.P.png  (画面)
 *       dog_ack_01.J.png  (投影)
 *       dog_ack_01.S.png  (阴影)
 *    格式拆分如下：
 *      dog_ack_01 --- 真正的名字，记录到 name 中
 *      P / J / S  --- P==画面， J==投影, S==阴影
 *   
 *   -----------------
 *   texture 管理：
 *   gl中，texture资源的唯一限制在于 显存大小。
 *   所以，在目前版本中，假定显存无限大，
 *   将游戏的所有 texture资源 一股脑存入 显存中
 *  （等正式爆发 显存危机了，再做调整...）
 *   ---
 *   随着 tex动态生成器的 引入，texture 的数量将呈爆炸态
 *   此时就有必要进入 更为完善的 tex显存管理（到时再说）
 * ----------------------------
 */
#ifndef _TPR_ANIM_FRAME_SET_H_
#define _TPR_ANIM_FRAME_SET_H_
//=== *** glad FIRST, glfw SECEND *** ===
#include <glad/glad.h> 

//-------------------- C --------------------//
#include <cassert> //- assert

//-------------------- CPP --------------------//
#include <string>
#include <vector>

//------------------- Libs --------------------//
#include "tprDataType.h"

//-------------------- Engine --------------------//
#include "IntVec.h" 
#include "RGBA.h" 
#include "FramePos.h"


//-- AS实例 需要的参数 --
//  -* 暂时无用 *-
struct AnimFrameSetParams{
    std::string lpath_pic;
    IntVec2  pixNum_per_frame;
    IntVec2  frameNums;
    IntVec2  anchor_root;
};


//-- 作为纯粹的 图像资源类，AnimFrameSet 应该被设计得尽可能简洁 --
//   不负责其他任何数据 
//   AnimFrameSet 没有 具象as类。
class AnimFrameSet{
public:
    AnimFrameSet( const std::string &_lpath_pic, 
                IntVec2             _frameNum,
                int                 _totalFrameNum,
                bool                _isHaveShadow,
                bool                _isTimeStepsManualSet, //- 若为 false，参数 _timeSteps 可为空容器
                const std::vector<int> &_timeSteps
                ):
        lpath_pic(_lpath_pic),
        frameNum(_frameNum),
        totalFrameNum(_totalFrameNum),
        isHaveShadow(_isHaveShadow),
        isTimeStepsManualSet(_isTimeStepsManualSet)
        {
            if( this->isTimeStepsManualSet ){
                assert( _timeSteps.size() == this->totalFrameNum );
                this->timeSteps.insert( this->timeSteps.end(), _timeSteps.begin(), _timeSteps.end() );
            }else{
                this->timeSteps.resize( this->totalFrameNum, 6 ); //- 默认值，统统为6
            } 
        }

    void init();

    inline const IntVec2 &get_pixNum_per_frame() const {
        return this->pixNum_per_frame;
    }

    inline const IntVec2 &get_frameNum() const {
        return this->frameNum;
    }

    inline const int &get_totalFrameNum() const {
        return this->totalFrameNum;
    }

    inline const std::vector<FramePos> &get_framePoses() const {
        return this->framePoses;
    }

    inline const std::vector<int> &get_timeSteps() const {
        return this->timeSteps;
    }

    inline const std::vector<GLuint> &get_texNames_pic() const {
        return this->texNames_pic;
    }

    inline const std::vector<GLuint> &get_texNames_shadow() const {
        assert( this->isHaveShadow );
        return this->texNames_shadow;
    }


    //======== flags ========//
    //-- 一个 AnimFrameSet 实例，必须有 P/J 两张图，但不一定要有 Shadow 图。
    bool  isHaveShadow  {};
    bool  isTimeStepsManualSet {}; //- 若为 false，则使用默认 timeSteps值 (每一帧都为6)

private:
    void build_three_lpaths();
    void handle_pjt();
    void handle_shadow();

    //======== vals ========//
    //-- 本动画动作 的name。 起到 id 的作用。
    //-- 是否使用字符串有待商榷，取决于，是否会跟随go数据存入硬盘中。
    std::string  name;

    //- 画面 贴图集的 相对路径名。一个动作的所有帧图片，存储为一张 png图。
    //- 这个 路径名 只在游戏启动阶段被使用，之后 预存于此
    //- local path，based on path_animFrameSet_srcs
    std::string  lpath_pic;    //-- picture
    std::string  lpath_pjt;    //-- collients
    std::string  lpath_shadow; //-- shadow  (此文件也许可为空...)


    IntVec2  pixNum_per_frame {};  //- 单帧画面 的 长宽 像素值
    IntVec2  frameNum {};        //- 画面中，横排可分为几帧，纵向可分为几帧
    int      totalFrameNum {};   //- 总 图元帧 个数

    //- 动画中的每一帧图都会被 存储为 一个 texture实例。
    //- 具体数据存储在 gl内。 此处存储其 textel names 
    //- 帧排序 符合 左上坐标系（也就是我们排列动画帧的坐标系） --
    std::vector<GLuint> texNames_pic    {}; 
    std::vector<GLuint> texNames_shadow {}; //- 不一定有 shadow

    //-- each frame --
    std::vector<FramePos>  framePoses {};

    //-- step time for each frame -- 
    std::vector<int> timeSteps {};

};


#endif


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
 *   -----------------
 *   一个完整的 动画动作, 会被存储为 3 种 png 图片。
 *   图片名字格式为： 
 *       dog_ack_01.P.png  (画面)
 *       dog_ack_01.J.png  (投影)
 *       dog_ack_01.S.png  (阴影)
 *    格式拆分如下：
 *      dog_ack_01 --- 真正的名字，记录到 name 中
 *      P / J / S  --- P==画面， J==投影, S==阴影
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
#ifndef TPR_ANIM_FRAME_SET_H_
#define TPR_ANIM_FRAME_SET_H_
//=== *** glad FIRST, glfw SECEND *** ===
#include <glad/glad.h> 

//-------------------- CPP --------------------//
#include <string>
#include <vector>
#include <unordered_map>

//------------------- Libs --------------------//
#include "tprDataType.h"

//-------------------- Engine --------------------//
#include "tprAssert.h"
#include "IntVec.h" 
#include "RGBA.h" 
#include "FramePos.h"
#include "AnimAction.h"


//-- 作为纯粹的 图像资源类，AnimFrameSet 应该被设计得尽可能简洁 --
//   不负责其他任何数据 
//   AnimFrameSet 没有 具象as类。
class AnimFrameSet{
public:
    AnimFrameSet( const std::string &_name ):
        name(_name)
        {}

    void insert_a_png(
                const std::string &_lpath_pic, 
                IntVec2             _frameNum,
                size_t              _totalFrameNum,
                bool                _isHaveShadow,
                bool                _isPjtSingleFrame,
                bool                _isShadowSingleFrame,
                const std::vector<AnimActionParam> &_animActionParams
                );

    inline AnimAction *getnc_animActionPtr( const std::string &_actionName ){
       tprAssert( this->animActions.find(_actionName) != this->animActions.end() );
       return  &this->animActions.at(_actionName);
    }

    inline const std::vector<GLuint> *get_texNames_pic_ptr() const {
        return &(this->texNames_pic);
    }
    inline const std::vector<GLuint> *get_texNames_shadow_ptr() const {
        return &(this->texNames_shadow);
    }
    inline const std::vector<FramePos> *get_framePosesPtr() const {
        return &(this->framePoses);
    }

private:
    void handle_pjt();
    void handle_shadow();

    //======== vals ========//

    //-- 通常为 图元所属的 obj name --
    std::string  name; //-- 目前来看 用处不大

    //- 动画中的每一帧图都会被 存储为 一个 texture实例。
    //- 具体数据存储在 gl内。 此处存储其 textel names 
    //- 帧排序 符合 左上坐标系（也就是我们排列动画帧的坐标系） --
    std::vector<GLuint> texNames_pic    {}; 
    std::vector<GLuint> texNames_shadow {}; //- 就算没有 shadow数据，也要填写0 来占位

    //-- each frame --
    std::vector<FramePos>  framePoses {};

    std::unordered_map<std::string, AnimAction> animActions {};
};


#endif


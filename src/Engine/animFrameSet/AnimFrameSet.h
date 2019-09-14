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
#ifndef TPR_ANIM_FRAME_SET_H
#define TPR_ANIM_FRAME_SET_H
//=== *** glad FIRST, glfw SECEND *** ===
#include <glad/glad.h> 

//-------------------- CPP --------------------//
#include <string>
#include <vector>
#include <unordered_map>
#include <memory>

//------------------- Libs --------------------//
#include "tprDataType.h"

//-------------------- Engine --------------------//
#include "tprAssert.h"
#include "IntVec.h" 
#include "RGBA.h" 
#include "FramePos.h"
#include "AnimAction.h"
#include "ColliderType.h"
#include "ID_Manager.h" 
#include "AnimSubspecies.h"

#include "tprDebug.h"



class AnimFrameSet{
public:
    AnimFrameSet( const std::string &name_ ):
        name(name_)
        {}

    void insert_a_png(
                const std::string &lpath_pic_, 
                IntVec2             frameNum_,
                size_t              totalFrameNum_,
                bool                isHaveShadow_,
                bool                isPjtSingleFrame_,
                bool                isShadowSingleFrame_,
                ColliderType        colliderType_,
                const std::vector<std::shared_ptr<AnimActionParam>> &animActionParams_ );

    inline const std::vector<GLuint> *get_texNames_pic_ptr() const noexcept{
        return &(this->texNames_pic);
    }
    inline const std::vector<GLuint> *get_texNames_shadow_ptr() const noexcept{
        return &(this->texNames_shadow);
    }
    //-- 仅被 animAction 调用 --
    inline const std::vector<FramePos> *get_framePoses() const noexcept{
        return &(this->framePoses);
    }

    inline animSubspeciesId_t apply_a_random_animSubspeciesId(const std::vector<AnimLabel> &labels_, 
                                                              double             randVal_ )noexcept{
        return this->subGroup.apply_a_random_animSubspeciesId( labels_, randVal_ );
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

    AnimSubspeciesGroup subGroup {};

};


#endif


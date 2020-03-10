/*
 * =================== AnimFrameSet.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2018.11.23
 *                                        MODIFY -- 
 * ----------------------------------------------------------
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
// Don't include glfw3.h ALONE!!!
#include <glad/glad.h> 

#include "pch.h"

//-------------------- Engine --------------------//
#include "RGBA.h" 
#include "AnimActionPos.h"
#include "AnimAction.h"
#include "AnimSubspecies.h"


using afsId_t = uint32_t; //- animFrameSet id type


class AnimFrameSet{
public:
    explicit AnimFrameSet( const std::string &name_ ):
        name(name_)
        {}

    void insert_a_png(
                const std::string  &path_pic_, 
                IntVec2             frameNum_,
                size_t              totalFrameNum_,
                bool                isHaveShadow_,
                bool                isPjtSingleFrame_,
                bool                isShadowSingleFrame_,
                const std::vector<std::shared_ptr<AnimActionParam>> &animActionParams_ );

    inline const std::vector<GLuint> *get_texNames_pic_ptr() const noexcept{
        return &(this->texNames_pic);
    }
    inline const std::vector<GLuint> *get_texNames_shadow_ptr() const noexcept{
        return &(this->texNames_shadow);
    }

    inline animSubspeciesId_t apply_a_random_animSubspeciesId(const std::string &label_, size_t uWeight_ )noexcept{
        return this->subGroup.apply_a_random_animSubspeciesId( label_, uWeight_ );
    }


private:
    void handle_pjt( const std::vector<std::shared_ptr<AnimActionParam>> &animActionParams_ );
    void handle_shadow();

    //======== vals ========//

    //-- 通常为 图元所属的 obj name --
    std::string  name; //-- 目前来看 用处不大

    //- 动画中的每一帧图都会被 存储为 一个 texture实例。
    //- 具体数据存储在 gl内。 此处存储其 textel names 
    //- 帧排序 符合 左上坐标系（也就是我们排列动画帧的坐标系） --
    std::vector<GLuint> texNames_pic    {}; 
    std::vector<GLuint> texNames_shadow {}; //- 就算没有 shadow数据，也要填写0 来占位

    //-- each animaction --
    std::unordered_map<animActionPosId_t, std::unique_ptr<AnimActionPos>> animActionPosUPtrs {};

    AnimSubspeciesGroup subGroup {};


    //======== flags ========//
    bool isPJTSingleFrame {false}; //- Jpng 是否只有一帧
                        //- 若为 true，本afs实例下属的所有 animAction，
                        // 都直接绑定这唯一的一份 AnimActionPos 数据
    bool isShadowSingleFrame {false};  //- Spng 是否只有一帧
};



namespace json{//------------- namespace json ----------------
    void parse_animFrameSetJsonFile();
                // go 相关的 afs数据，应该被 各个 go 独立管理，
                // 不再由此函数 统一加载

    void parse_single_animFrameSetJsonFile( const std::string &path_file_,
                                        std::unordered_set<std::string> *out_afsNamesPtr_=nullptr );

}//------------- namespace json: end ----------------s

#endif


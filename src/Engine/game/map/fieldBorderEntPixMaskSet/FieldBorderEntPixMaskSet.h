/*
 * ==================== FieldBorderEntPixMaskSet.h =======================
 *                          -- tpr --
 *                                        CREATE -- 2019.03.09
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *  部分 mapent 身处 field边缘。
 *  这些 mapent里的 pixels 需要表现得破碎些
 * --------
 *  预制了很多 "破碎像素分割mask"
 *  通过一个 函数来获得 访问权
 * ----------------------------
 */
#ifndef _TPR_FIELD_BORDER_ENT_PIX_MASK_SET_H_
#define _TPR_FIELD_BORDER_ENT_PIX_MASK_SET_H_

//-------------------- CPP --------------------//
#include <vector>
#include <string>

//------------------- Libs --------------------//
#include "tprDataType.h" 


//------------------- Engine --------------------//
#include "IntVec.h"
#include "FieldBorderType.h"



//--- singleton ---//
// 
//
class FieldBorderEntPixMaskSet{
    using maskSet_t = std::vector<std::vector<IntVec2>>;
public:
    FieldBorderEntPixMaskSet() = default;

    void init(  const std::string &_lpath,
                IntVec2  _frameNum,
                int      _totalFrameNum );

    const std::vector<IntVec2> &get_rand_maskSet( FieldBorderType _borderType );

private:

    //--- 主体数据,存储的是 一组点的 相对mapent左下角的 ppos偏移 ---
    // 公 4个档次，
    maskSet_t  maskSets_1_pix {}; 
    maskSet_t  maskSets_2_pix {}; 
    maskSet_t  maskSets_4_pix {}; 
    maskSet_t  maskSets_7_pix {}; 

    std::string  lpath {};
    IntVec2      pixNum_per_frame {};   //- 单帧画面 的 长宽 像素值
    IntVec2      frameNum {};         //- 画面中，横排可分为几帧，纵向可分为几帧
    int          totalFrameNum {};    //- 总 图元帧 个数                            
};

#endif


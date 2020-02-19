/*
 * =================== animSubspeciesId.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.09.10
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#ifndef TPR_ANIM_SUB_SPEC_ID_H
#define TPR_ANIM_SUB_SPEC_ID_H

//------------------- CPP --------------------//
#include <cstdint> // uint8_t


using animSubspeciesId_t = uint32_t;

// 一个空的 id 值， 表示生成此值的代码，本来就不打算生成一个 实际的id
// 用于 蓝图分配
// 有些 go，会在 具象go class 中，自行分配 sudId 
inline constexpr animSubspeciesId_t EMPTY_animSubspeciesId { static_cast<animSubspeciesId_t>(-1) };

inline bool is_empty_animSubspeciesId( animSubspeciesId_t id_ )noexcept{
    return (id_ == EMPTY_animSubspeciesId);
}


#endif 


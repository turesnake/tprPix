/*
 * ========================== LandWaterEnt.h =======================
 *                          -- tpr --
 *                                        CREATE -- 2019.03.16
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *  land-water ent信息。
 * -1- 在section 生成阶段统一生成，并将数据，以chunk为单元，存储在全局容器中
 * -2- 每个chunk实例创建时，取出自己的这份数据，转存到 每一个 chunk.mapent 中
 * ----------------------------
 */
#ifndef _TPR_LAND_WATER_ENT_H_
#define _TPR_LAND_WATER_ENT_H_


//------------------- Libs --------------------//
#include "tprDataType.h" 


// idx:0 -- isLand
// idx:1 -- isBorder （水陆边界）
// idx:2
// idx:3
// idx:4
// idx:5
// idx:6
// idx:7
class LandWaterEnt{
public:
    LandWaterEnt() = default;

    inline void set_isLand( bool _isLand ){
        ( _isLand ) ?
            val = val |   1  :
            val = val & ~(1);
    }

    inline void set_isBorder( bool _isBorder ){
        ( _isBorder ) ?
            val = val |  (1<<1) :
            val = val & ~(1<<1); 
    }

    inline bool get_isLand() const {
        return ((val & 1) == 1);
    }

    inline bool get_isBorder() const {
        return (((val>>1)&1) == 1);
    }

    inline u8_t get_val() const{
        return val;
    }

private:
    u8_t val {0};
};



#endif 

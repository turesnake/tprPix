/*
 * ======================= UIAnchor.h ========================
 *                          -- tpr --
 *                                        创建 -- 2019.08.28
 *                                        修改 -- 
 * ----------------------------------------------------------
 *  based on ViewingBox::gameSZ 
 *  a point used in UI sys
 * ---------
 */
#ifndef TPR_UI_ANCHOR_H
#define TPR_UI_ANCHOR_H

//--- glm - 0.9.9.5 ---
#include "glm_no_warnings.h"


//-------------------- Engine --------------------//
#include "ViewingBox.h"
#include "tprAssert.h"


class UIAnchor{
public:
    //- often used in GameObj class. init() will be delay
    UIAnchor() = default;

    //- often used to create a single instance. 
    UIAnchor(   const glm::dvec2 &basePointProportion_,  // [-1.0,1.0]
                const glm::dvec2 &offDPos_ ):
        basePointProportion(basePointProportion_),
        offDPos(offDPos_)
        {
            this->check_basePointProportion( basePointProportion_ );
            this->init( basePointProportion_, offDPos_ );
        }

    //--- 基于一个 基点，做一个偏移，计算出 本点的 currentDPos
    //- param_1: window 上的 比例点[-1.0,1.0],此为偏移 基点
    //- param_2: 绝对偏移值，可写零
    void init(  const glm::dvec2 &basePointProportion_,  // [-1.0,1.0]
                const glm::dvec2 &offDPos_  )noexcept{
            this->check_basePointProportion( basePointProportion_ );
            glm::dvec2 basePointDPos {  
                    static_cast<double>(ViewingBox::gameSZ.x) * 0.5 * basePointProportion_.x,
                    static_cast<double>(ViewingBox::gameSZ.y) * 0.5 * basePointProportion_.y };
            this->currentDPos = basePointDPos + offDPos_;
        }

    //----- set -----//
    inline void set_alti( double alti_ )noexcept{
        this->alti = alti_;
    }
    inline void accum_dpos( const glm::dvec2 &addDPos_ )noexcept{
        this->currentDPos += addDPos_;
    }

    //----- get -----//
    inline const glm::dvec2 &get_dpos() const noexcept{ return this->currentDPos; }
    inline double           get_alti()const noexcept{ return this->alti; }
    inline const glm::dvec2 &get_basePointProportion() const noexcept{ return this->basePointProportion; }
    inline const glm::dvec2 &get_offDPos() const noexcept{ return this->offDPos; }

private:
    inline void check_basePointProportion( const glm::dvec2 &basePointProportion_ )noexcept{
        tprAssert(  (basePointProportion_.x>=-1.0) && (basePointProportion_.x<=1.0) &&  
                    (basePointProportion_.y>=-1.0) && (basePointProportion_.y<=1.0) );
    }

    //---- vals -----//
    glm::dvec2 currentDPos {}; //- base on window_center.
    double  alti {}; //- 腾空高度。
    //--
    glm::dvec2 basePointProportion {}; 
                    // 将 window 看成一张 [-1.0,1.0] 的坐标图
                    // 想要定位一个 uigo，就要先确定一个 basePoint （基于以上坐标系的一个值）
                    // 最常用的有 中心点，四角点 和 四边中点

    glm::dvec2 offDPos {};
                    // 在 basePointProportion 基础上，
                    // 再累加一个 像素偏移值
                    // 以此获得，最终的 uigo 坐标值

};


#endif 


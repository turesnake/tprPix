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
                const glm::dvec2 &offDPos_  ){
            this->check_basePointProportion( basePointProportion_ );
            glm::dvec2 basePointDPos {  
                    static_cast<double>(ViewingBox::gameSZ.x) * 0.5 * basePointProportion_.x,
                    static_cast<double>(ViewingBox::gameSZ.y) * 0.5 * basePointProportion_.y };
            this->currentDPos = basePointDPos + offDPos_;
        }

    //----- set -----//
    inline void set_alti( double alti_ ){
        this->alti = alti_;
    }
    inline void accum_currentDPos( const glm::dvec2 &addDPos_ ){
        this->currentDPos += addDPos_;
    }

    //----- get -----//
    inline const glm::dvec2 &get_currentDPos() const{
        return this->currentDPos;
    }
    inline const double &get_alti() const {
        return this->alti;
    }
    inline const glm::dvec2 &get_basePointProportion() const {
        return this->basePointProportion;
    }
    inline const glm::dvec2 &get_offDPos() const {
        return this->offDPos;
    }

private:
    inline void check_basePointProportion( const glm::dvec2 &basePointProportion_ ){
        tprAssert(  (basePointProportion_.x>=-1.0) && (basePointProportion_.x<=1.0) &&  
                    (basePointProportion_.y>=-1.0) && (basePointProportion_.y<=1.0) );
    }

    //---- vals -----//
    glm::dvec2 currentDPos {}; //- base on window_center.
    double  alti {}; //- 腾空高度。
    //--
    glm::dvec2 basePointProportion {}; 
    glm::dvec2 offDPos {};
};


#endif 


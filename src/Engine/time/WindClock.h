/*
 * ========================= WindClock.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.10.18
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *   control the grass anim
 * ----------------------------
 */
#ifndef TPR_WIND_CLOCK_H
#define TPR_WIND_CLOCK_H
//--- glm - 0.9.9.5 ---
#include "glm_no_warnings.h"

//------------------- CPP --------------------// 
#include <cmath>
#include <vector>

//------------------- Engine --------------------// 
#include "tprAssert.h"


class WindClock{
public:
    WindClock()=default;
    void init()noexcept;

    inline void update()noexcept{
        //--- clockStep ---//
        this->clockStep--;
        if( this->clockStep <= 0 ){
            this->reflesh_clockStep();
            this->clockCount++;
        }
        //--- playSpeedScale ---//
        this->playSpeedScalePoolIdx++;
        if( this->playSpeedScalePoolIdx >= this->playSpeedScalePool.size() ){
            this->playSpeedScalePoolIdx = 0;
        }
    }

    //----- get -----//
    inline size_t get_clockCount()const noexcept{ return this->clockCount; }

    inline double get_playSpeedScale( size_t offIdx_=0 )const noexcept{ 
        size_t idx = (this->playSpeedScalePoolIdx + offIdx_) % this->playSpeedScalePool.size();
        return this->playSpeedScalePool.at(idx);
    }


private:
    inline void reflesh_clockStep()noexcept{
        this->clockStep = this->clockStepPool.at( this->clockStepPoolIdx );
        this->clockStepPoolIdx++;
        if( this->clockStepPoolIdx >= this->clockStepPool.size() ){
            this->clockStepPoolIdx = 0;
        }
    }


    //--- vals ---//
    size_t clockCount {0};
    size_t clockStep {};
    std::vector<size_t> clockStepPool {}; // 100 dynamic clockStep-vals
    size_t              clockStepPoolIdx {0};


    //---
    std::vector<double> playSpeedScalePool {};
    size_t              playSpeedScalePoolIdx {0};

};


size_t calc_goMesh_windDelayIdx( const glm::dvec2 &dpos_ )noexcept;


#endif 


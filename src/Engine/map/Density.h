/*
 * ====================== Density.h =======================
 *                          -- tpr --
 *                                        CREATE -- 2019.03.23
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *  密度 一个 世界区域划分规则。
 *  密度将游戏空间划分为 数段等高线（>=0 为 高地，<0 为低地）
 *  只有 field 才需要记录自己的 density 值
 * ----------------------------
 */
#ifndef TPR_DENSITY_H
#define TPR_DENSITY_H

//------------------- CPP --------------------//
#include <vector>

//------------------- Engine --------------------//
#include "tprAssert.h"
#include "IntVec.h" 


class EcoObj;

//--- [mem] ---
class Density{
public:
    Density() = default;
    
    //-- 手动指定 lvl值，仅用于特殊场合 --
    explicit Density( int lvl_ ):
        lvl(lvl_)
        {
            tprAssert( (lvl_>=Density::minLvl) && (lvl_<=Density::maxLvl) );
        }

    Density(IntVec2 fieldMPos_, 
            double ecoObj_densitySeaLvlOff_,
            const std::vector<double> *ecoObj_densityDivideValsPtr_ )
        {
            this->init( fieldMPos_, ecoObj_densitySeaLvlOff_, ecoObj_densityDivideValsPtr_ );
        }

    inline constexpr int get_lvl() const noexcept{ return this->lvl; }

    //-- 主要用于 遍历一些容器 --
    inline size_t get_idx() const noexcept{
        switch( this->lvl ){
            case -3: return 0;
            case -2: return 1;
            case -1: return 2;
            case 0:  return 3;
            case 1:  return 4;
            case 2:  return 5;
            case 3:  return 6;
            default:
                tprAssert(0);
                return 0; //- never reach
        }
    }

    static size_t get_idxNum()noexcept{
        return 7; //- 一共7档
    }

    static int get_minLvl()noexcept{
        return Density::minLvl;
    }
    static int get_maxLvl()noexcept{
        return Density::maxLvl;
    }

    static size_t lvl_2_idx( int lvl_ )noexcept{
        switch( lvl_ ){
            case -3: return 0;
            case -2: return 1;
            case -1: return 2;
            case 0:  return 3;
            case 1:  return 4;
            case 2:  return 5;
            case 3:  return 6;
            default:
                tprAssert(0);
                return 0; //- never reach
        }
    }   


private:
    void init(   IntVec2 fieldMPos_, 
                double ecoObj_densitySeaLvlOff_,
                const std::vector<double> *ecoObj_densityDivideValsPtr_ );
    //===== vals =====//
    int lvl {0}; // [-3, 3] 共7档

    //===== static =====//
    static int  minLvl;  // -3
    static int  maxLvl;  //  3
};




inline constexpr bool operator < ( Density a_, Density b_ ) noexcept {
    return (a_.get_lvl() < b_.get_lvl());
}
inline constexpr bool operator == ( Density a_, Density b_ ) noexcept {
    return (a_.get_lvl() == b_.get_lvl());
}
inline constexpr bool operator != ( Density a_, Density b_ ) noexcept {
    return (a_.get_lvl() != b_.get_lvl());
}





#endif 


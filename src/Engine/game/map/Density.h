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
#ifndef _TPR_DENSITY_H_
#define _TPR_DENSITY_H_

//------------------- C --------------------//
#include <cassert>


//------------------- Engine --------------------//
#include "IntVec.h" 


//--- [mem] ---
class Density{
public:
    Density() = default;
    
    //-- 手动指定 lvl值，仅用于特殊场合 --
    explicit Density( int _lvl ):
        lvl(_lvl)
        {
            assert( (_lvl>=Density::minVal) && (_lvl<=Density::maxVal) );
        }

    void set( const IntVec2 &_fieldMPos );

    inline int get_lvl() const {
        return this->lvl;
    }

    //-- 主要用于 遍历一些容器 --
    inline size_t get_idx() const {
        switch( this->lvl ){
            case -3: return 0;
            case -2: return 1;
            case -1: return 2;
            case 0:  return 3;
            case 1:  return 4;
            case 2:  return 5;
            case 3:  return 6;
            default:
                assert(0);
                return 0; //- never reach
        }
    }

    static size_t get_idxNum(){
        return 7; //- 一共7档
    }

    static int get_minVal(){
        return Density::minVal;
    }
    static int get_maxVal(){
        return Density::maxVal;
    }

    static int lvl_2_idx( int _lvl ){
        switch( _lvl ){
            case -3: return 0;
            case -2: return 1;
            case -1: return 2;
            case 0:  return 3;
            case 1:  return 4;
            case 2:  return 5;
            case 3:  return 6;
            default:
                assert(0);
                return 0; //- never reach
        }
    }   


private:
    //===== vals =====//
    int lvl {0}; // [-3, 3] 共7档

    //===== static =====//
    static int  minVal;  // -3
    static int  maxVal;  //  3
};



 

#endif 


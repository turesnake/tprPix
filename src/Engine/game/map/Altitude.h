/*
 * ====================== Altitude.h =======================
 *                          -- tpr --
 *                                        CREATE -- 2019.03.11
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *   每个 pix / mapEnt／field 都拥有自身的 高度信息。
 *  
 * ----------------------------
 */
#ifndef _TPR_ALTITUDE_H_
#define _TPR_ALTITUDE_H_


//------------------- Libs --------------------//
//#include "tprDataType.h" 

//------------------- Engine --------------------//
#include "IntVec.h" 
#include "EcoSys.h"



//-- [mem]
class Altitude{
public:
    Altitude() = default;
    

    void set( const IntVec2 &_ppos, const EcoSys *_ecosysPtr );


    //===== vals =====//
    int  val {0}; //- [-100,100]
                    // 静态值，最简单的alti，不应该直接用此值。
                    //
    int  lvl {0}; //- [-5,2]
                    // "高度等级" 基于 val，单受到更多因素的影响
                    // 推荐 主用此值来表达 “高度”
                    // --------
                    // -5: 水下最深处，go无法通行
                    // -1 ~ -4: 水下，根据深度，逐渐降低 go 的移动速度
                    // 0: 水上，“临水区” 颜色特别。比如有沙滩
                    // 1: “次临水区” 一个过渡区
                    // 2: 彻底的陆地
    
    bool  isSand {false}; //- tmp
};




#endif 


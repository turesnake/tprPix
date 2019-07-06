/*
 * ====================== MapAltitude.h =======================
 *                          -- tpr --
 *                                        CREATE -- 2019.03.11
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *   每个 pix / mapEnt／field 都拥有自身的 高度信息。
 * ----------------------------
 */
#ifndef TPR_MAP_ALTITUDE_H
#define TPR_MAP_ALTITUDE_H

//------------------- Engine --------------------//
#include "IntVec.h" 


//-- [mem]
class MapAltitude{
public:
    MapAltitude() = default;

    explicit MapAltitude( double val_ ){
        this->set( val_ );
    }
    
    void set( double altiVal_from_gpgpu_ );

    //-- 仅表达是否为land，很可能为 临水区域 --
    inline bool is_land() const {
        return (this->val >= 0);
    }

    //-- 是否为内陆，排除掉了 领水陆地 --
    /*
    inline bool is_inland() const {
        return (this->val >= 15);
    }
    */

    //-- tmp 测试用
    inline bool is_inland_2() const {
        return (this->val >= 10);
    }

    //===== vals =====//
    int  val {0}; //- [-100,100]
                    // 静态值，最简单的alti，不应该直接用此值。
                    // >=0 时 为 land
                    
    int  lvl {0}; //- [-5,2]
                    // "高度等级" 基于 val，单受到更多因素的影响
                    // 推荐 主用此值来表达 “高度”
                    // --------
                    // -5: 水下最深处，go无法通行
                    // -1 ~ -4: 水下，根据深度，逐渐降低 go 的移动速度
                    // 0: 水上，“临水区” 颜色特别。比如有沙滩
                    // 1: “次临水区” 一个过渡区
                    // 2: 彻底的陆地
};


/* ===========================================================
 *                   operator <
 * -----------------------------------------------------------
 * -- 通过这个 "<" 运算符重载，IntVec2 类型将支持 set.find() 
 */
inline bool operator < ( const MapAltitude &a_, const MapAltitude &b_ ){
    return ( a_.val < b_.val );
}

inline bool operator > ( const MapAltitude &a_, const MapAltitude &b_ ){
    return ( a_.val > b_.val );
}


#endif 


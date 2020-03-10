/*
 * ====================== CircuitBoard.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2020.01.04
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#ifndef TPR_CIRCUIT_BOARD_H
#define TPR_CIRCUIT_BOARD_H

#include "pch.h"

//------------------- Engine --------------------//
#include "GameObjType.h"
#include "mapEntKey.h"



/* "电路板"系统
 * 所有 artifact-go/mpgo 实例，可在自己被创建之时，在 "电路板"系统中，
 * 在自身附近的某些 mapent位置上，留一组函数
 * 当目标 mapent 在未来出生一个 mpgo 时，此mpgo 会自动拾取 之前登记的这组函数，并在条件符合时执行它
 * === 数据的卸载
 *     唯独当 数据的登记者 dogo 被释放时，才会将与dogo 关联的所有 数据，逐个删除（最耗时，但干净的办法）
 * === 登记函数的作用周期
 * -1- 只要 登记者/dogo 存在，与它关联的 登记函数会始终存在。
 *     哪怕 目标 mapent 所属的 chunk 已经被释放了
 * 
 * 一个 dogo，只能登记一份 functor。
 *     若存在细分需求，则要在 这个函数 体内完成
 * 
 * ====== 用法 =======
 * 由于 关联只发送在 mpgo 被创建的一瞬间，所以，登记的函数，最常用来实现的功能，就是 连接数个 mpgo 之间的父子关系
 */
class CircuitBoard{
public:
    //================== nested ==================//
    // 调用优先级
    enum class MessageWeight{
        V_0 = 0, // last call
        V_1,
        V_2,
        V_3,
        V_4,
        V_5, // first call
    };

private:

    // 当一个 dogo 被释放时，会直接释放 dogoids容器中 所有关联数据 （最彻底的做法）
    // 同时，若发现 dogoids 已为空，将直接删除 本 mapEntMessage 实例
    class MapEntMessage{
    public:
        MapEntMessage()=default;
        std::multimap<MessageWeight, goid_t> dogoids {};
    };

public:
    //================== static ==================//
    static void init_for_static()noexcept;// MUST CALL IN MAIN !!! 

    static void signUp( goid_t dogoid_, F_AFFECT functor_,
                        const std::map<mapEntKey_t, CircuitBoard::MessageWeight> &mpDatas_ )noexcept;

    static void check_and_call_messages( IntVec2 mpos_, GameObj &begoRef_ )noexcept;
    
    static void erase_dogoMessages( goid_t dogoid_, const std::map<mapEntKey_t, CircuitBoard::MessageWeight> &mpDatas_ )noexcept;

private:
    static std::unordered_map<mapEntKey_t, std::unique_ptr<MapEntMessage>> messages;
    static std::unordered_map<goid_t, F_AFFECT> functors; // 实际 函数对象 存储地
};



#endif 


/*
 * ========================= Dog_A.h ==========================
 *                          -- tpr --
 *                                        创建 -- 2018.12.23
 *                                        修改 -- 2018.12.23
 * ----------------------------------------------------------
 *   具象GameObj类 示范。
 * 
 * ----------------------------
 */
#ifndef _TPR_DOG_A_H_
#define _TPR_DOG_A_H_


//-------------------- CPP --------------------//
#include <string>
#include <vector>
#include <functional>


//-------------------- SELF --------------------//
#include "GameObj.h"
#include "Mesh.h"
#include "Action_SRC.h"



//--- 具象GameObj类 示范 --
//   如何使用 动态绑定
//-------
//  具象go类 不仅仅是个 “装配工厂”。也容纳实实在在的数据
//  所以，每一个 具象go类实例，都需要被 动态存储于 mem
//--- 目前选定的方案是：
//   阔气地为 每一种 具象go类型，准备一个 stl容器（vector / unordered_map）
//   这些容器会持续存在，直到游戏终止。
//--- 在这种策略下，全局资源 memGameObjs 也将不再存储 go实例，改为存储 goid 
class Dog_A{
public:
    explicit Dog_A() = default;

    //--- 延迟init ---//
    void init();



    GameObj go {}; //- go实例，全程都在装配它。


    std::vector<Mesh> meshes; //- mesh实例 实际存储地
                            //- 将被绑定到 go 中


    std::vector<std::string> action_names; //- 所有 action实例 都存储在 统一容器中
                            // 其他代码只通过 name／id 来调用它







private:
};













#endif

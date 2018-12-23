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
//  如何使用 动态绑定
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

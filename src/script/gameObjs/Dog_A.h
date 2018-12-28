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
#include "Action.h"


//-- 定义了 go.binary 的数据格式 --
struct Dog_A_Binary{

    int   HP;
    int   MP;
};


//--- 具象GameObj类 示范 --
//   如何使用 动态绑定
//-------
//  具象go类 仅仅是个 “装配工厂”。并不容纳数据
//  具象go类 所需的 特殊数据，存储在 go.binary 这个 二进制块中。
//  所以，我们永远 不创建 具象go类实例
class Dog_A{
public:
    explicit Dog_A() = default;

    //--- 延迟init ---//
    void init( GameObj *_goPtr );
    void bind( GameObj *_goPtr );

    void Update(); 
    void BeAffect();


    //--------------- vals -----------------//


    GameObj *goPtr {nullptr}; //- go实例指针，所有的操作都是为了 服务于它
                            //- 具象go类 被彻底 工厂化，它甚至不再存储 go实例。
                            //- 这大幅度降低了 具象go类实例 创建的成本
                            //（多数时间作为 临时对象，创建在一个 函数内）

    Dog_A_Binary  *binaryPtr {nullptr}; //- 指向 goPtr->binary 
                                        //- 通过这个指针来 简化调用


    //std::vector<std::string> action_names; //- 所有 action实例 都存储在 统一容器中
                            // 其他代码只通过 name／id 来调用它







private:
};













#endif

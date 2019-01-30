/*
 * ========================= Norman.h ==========================
 *                          -- tpr --
 *                                        创建 -- 2019.01.30
 *                                        修改 -- 2019.01.30
 * ----------------------------------------------------------
 *   诺曼人
 * 
 * ----------------------------
 */
#ifndef _TPR_NORMAN_H_
#define _TPR_NORMAN_H_

//-------------------- CPP --------------------//
#include <string>
#include <vector>
#include <functional>

//-------------------- Engine --------------------//
#include "GameObj.h"
#include "GameObjMesh.h"
#include "Action.h"


namespace gameObjs{//------------- namespace gameObjs ----------------


//-- 定义了 go.binary 的数据格式 --
struct Norman_Binary{
    int   HP;
};


class Norman{
public:
    Norman() = default;

    //--- 延迟init ---//
    void init( GameObj *_goPtr );
    void bind( GameObj *_goPtr );

    //--- 从硬盘读取到 go实例数据后，重bind callback
    void rebind( GameObj *_goPtr );

    //--- callback ---//
    void RenderUpdate( GameObj *_goPtr ); 
    void LogicUpdate( GameObj *_goPtr ); 
    void BeAffect( GameObj *_goPtr );

    //--------------- tmp vals -----------------//
    GameObj *goPtr {nullptr}; //- go实例指针，所有的操作都是为了 服务于它
                            //- 具象go类 被彻底 工厂化，它甚至不再存储 go实例。
                            //- 这大幅度降低了 具象go类实例 创建的成本
                            //（多数时间作为 临时对象，创建在一个 函数内）

    Norman_Binary  *bp {nullptr}; //- 指向 goPtr->binary 
                            //- 通过这个指针来 简化调用
                            //  由于 具象go类实例的 生命周期很短（通常活不过一个函数）
                            //  所以，这个指针也是临时的

    //-------------- static --------------//
    static  u32  specId; //- 在 onGoSpecIds_SignUp() 中手动设置...
};

//---------- static ----------//
inline u32  Norman::specId {3};

//=====< Norman类 唯一的 保留实例 >=====
inline Norman  norman {};



}//------------- namespace gameObjs: end ----------------
#endif 


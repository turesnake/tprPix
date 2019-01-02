/*
 * ========================= ActionHandler.h ==========================
 *                          -- tpr --
 *                                        创建 -- 2018.12.28
 *                                        修改 -- 2018.12.28
 * ----------------------------------------------------------
 *    action 处理器
 *    这是一个 非常疯狂的 计划...
 * ----------------------------
 */
#ifndef _TPR_ACTION_HANDLER_H_
#define _TPR_ACTION_HANDLER_H_


//-------------------- CPP --------------------//
#include <string>
#include <vector>
#include <functional>
#include <unordered_map> 


//------------------- Libs --------------------//
#include "tprDataType.h" 

//------------------- Engine --------------------//
#include "ScriptBuf.h"


//-- 类似 GameObj，一个“伪接口类” --
//  存储 数据 和 接口
//  被 具象AH类 装配
//  一切 AH类 以及 具象AH类 数据，都只存储在 mem态
class ActionHandler{

    //- 万能函数接口, 依赖 scriptBuf 传递 参数／返回值
    using F_GENERAL = std::function<int(ActionHandler*,int)>; 
public:
    ActionHandler() = default;

    //----------- interface -------------//
    std::unordered_map<std::string, F_GENERAL> funcs; 

    //----------- self vals -------------//
    u32  typeId;       //- 具象AH类 类型id。

    //----------- binary chunk -------------//
    std::vector<u8>  binary; //- 具象AH类 定义的 二进制数据块。真实存储地
                            //- binary 本质是一个 C struct。 由 具象AH类方法 使用。
};


#endif


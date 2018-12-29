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



//-- 类似 GameObj，一个“伪接口类” --
//  存储 数据 和 接口
//  被 具象AH类 装配
//  一切 AH类 以及 具象AH类 数据，都只存储在 mem态
class ActionHandler{

    using F_GENERAL = std::function<void*(void*,int*)>; //- 万能函数接口
public:
    ActionHandler() = default;

    //----------- interface -------------//
    std::unordered_map<std::string, F_GENERAL> funcs; 

    //----------- vals -------------//
    u32  type;       //- 具象AH类 类型id。

    int  frames;     //- action实例 的 总画面帧数
    int  enterIdx;   //- 入口帧序号. 这个值永不变
    int  currentIdx; //- 当前指向的 画面帧序号（基于0）
    int  step;       //- 每切换一帧画面，停留多少 时间帧

    //----------- binary chunk -------------//
    std::vector<u8>  binary; //- 具象AH类 定义的 二进制数据块。真实存储地
                            //- binary 本质是一个 C struct。 由 具象AH类方法 使用。
private:
};


#endif


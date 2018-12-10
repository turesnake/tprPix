/*
 * ========================= ID_Manager.h ==========================
 *                          -- tpr --
 *                                        创建 -- 2018.12.10
 *                                        修改 -- 2018.12.10
 * ----------------------------------------------------------
 *   通用的 id 管理器
 * 
 * ----------------------------
 */
#ifndef _TPR_ID_MANAGER_H_
#define _TPR_ID_MANAGER_H_

//------------------- Libs --------------------//
#include "tprDataType.h" 


//----- id 管理器 -----
// 通常作为 其他类的 静态成员 存在。
// 0 号 id 就是空id
class ID_Manager{
public:
    explicit ID_Manager( u64 _max_id=1 ):
        max_id(_max_id)
        {}

    //-- 申请分配一个 新id --
    inline u64 apply_a_id(){
        //-- 2^64 个id，永远也用不完。
        max_id++;
        return max_id;
    }

    //---- vals ----
    u64  max_id; //- 当前 使用的 数值最大的 id号
    
};

#endif


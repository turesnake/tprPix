/*
 * ========================= Mesh.h ==========================
 *                          -- tpr --
 *                                        创建 -- 2018.11.24
 *                                        修改 -- 2018.11.24
 * ----------------------------------------------------------
 *   Mesh 类, 可以看作 图元类。
 *   一个 mesh，负责管理 一张 矩形图元
 *  ----------
 * Mesh 和 Action_SRC 的区别：
 *	  mesh 的主体部分 就是原本的 model类
 *	  mesh 需要 图片资源，
 *	  而大部分 图片资源，以 Action_SRC 的形式存储。
 *  ----------
 *   一个 mesh 拥有：
 *     一个 根锚点 ／ root anchor   -- 代表 mesh 本身
 *     数个 子锚点 ／ child anchor  -- 用来绑定其他 meshes
 *  
 * 
 * 
 * ----------------------------
 */
#ifndef _TPR_MESH_H_
#define _TPR_MESH_H_


//------------------- Libs --------------------//
#include "tprDataType.h" 

//------------------- SELF --------------------//
#include "Action_SRC.h"
//#include "srcs_manager.h" //- 会引发一个 引用bug


//-- Mesh 实例 只能在 GameObj 中创建 -- 
class Mesh{

public:
    explicit Mesh() = default;
        //:
        //{}


    void init();



private:
    u64   id;     //- go实例 在程序中的 主要搜索依据。


};




#endif


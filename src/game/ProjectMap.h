/*
 * ====================== ProjectMap.h =======================
 *                          -- tpr --
 *                                        创建 -- 2018.11.24
 *                                        修改 -- 2018.11.24
 * ----------------------------------------------------------
 *    投影地图。 游戏的碰撞检测 核心组件
 *    ----------
 *    目前版本，默认制作一个 2048 * 2048 像素的 投影地图
 * ----------------------------
 */
#ifndef _TPR_PROJECT_MAP_H_
#define _TPR_PROJECT_MAP_H_

//--- glm - 0.9.8 ---
#include <glm/glm.hpp>
            //-- glm::vec3
            //-- glm::vec4
            //-- glm::mat4
#include <glm/gtc/matrix_transform.hpp>
            //-- glm::translate
            //-- glm::rotate
            //-- glm::scale
            //-- glm::perspective
#include <glm/gtc/type_ptr.hpp> 
            //-- glm::value_ptr

//-------------------- CPP --------------------//
#include <string>
#include <vector>

//------------------- Libs --------------------//
#include "tprDataType.h" 


//------------------- SELF --------------------//
#include "GameObj.h"



//-- 投影地图单位的 一级信息 --//
//- 优先查看此信息，如有需要，再通过 id 查找 二级信息。
struct Fst_MapEnt{
    u16   fst_data;    //- 一级信息 本体
    u16   sec_data_id; //- 二级信息 id号
};


//-- 投影地图单位的 二级信息 --//
//- 存储在一个 内存池中，数量有限。
struct Sec_MapEnt{
    u64  surface_go_id; //- 地面类 go id
    u64  item_go_id;    //- 道具 go id
    u64  reg_go_id;     //- 常规 go id

};



class ProjectMap{


public:


    void init();


private:
    std::vector<Fst_MapEnt>  fst_mapEnts; //- 地图单位 一级信息集

    //- 一个 内存池 实例，里面有申请到的 内存 --//
    //- 二级信息
    //...




};











#endif


/*
 * ====================== ProjectMap.h =======================
 *                          -- tpr --
 *                                        创建 -- 2018.11.24
 *                                        修改 -- 2018.11.24
 * ----------------------------------------------------------
 *    投影地图。 游戏的碰撞检测 核心组件
 *    ----------
 *    目前版本，默认制作一个 512 * 512 地面单位的 投影地图
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

//-------------------- Engine --------------------//
#include "GameObj.h" 
#include "MapEnt.h"








class ProjectMap{


public:


    void init();


private:
    //std::vector<Fst_MapEnt>  fst_mapEnts; //- 地图单位 一级信息集

    //- 一个 内存池 实例，里面有申请到的 内存 --//
    //- 二级信息
    //- 二级信息 将被外部的 一个大 内存池 统一管理 。。
    //...




};












#endif


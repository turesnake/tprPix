/*
 * ================== GameWorldCoord.h =======================
 *                          -- tpr --
 *                                        创建 -- 2018.11.24
 *                                        修改 -- 2018.11.24
 * ----------------------------------------------------------
 *    游戏世界坐标系
 *    游戏中的 主坐标系。
 *    每个 gameobj，都以此为 基础去完成 逻辑运算
 *    通过它，再转换为 gl世界坐标系，然后完成图形渲染。
 * ----------------------------
 */
#ifndef _TPR_GAME_WORLD_COORD_H_
#define _TPR_GAME_WORLD_COORD_H_


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



class GameWorldCoord{

public:


    void init();



private:






};


#endif


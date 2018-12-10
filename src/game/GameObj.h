/*
 * ========================= GameObj.h ==========================
 *                          -- tpr --
 *                                        创建 -- 2018.11.24
 *                                        修改 -- 2018.11.24
 * ----------------------------------------------------------
 *    GameObj 是游戏中的 一等公民。
 *    可以作为一个 独立的单位，存在于 游戏中
 * ----------------------------
 */
#ifndef _TPR_GAME_OBJ_H_
#define _TPR_GAME_OBJ_H_

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
#include "Mesh.h" 
#include "ID_Manager.h" 
//#include "GameWorldCoord.h" 


//-- 一种 go 需要的全部参数 ---
struct GameObjParams{
	std::string  type; //- go type_name
    //std::string  path;
    //...

}; 



class GameObj{

public:

    explicit GameObj( const std::string &_typename )
        :
        type(_typename)
        {}


    void init();



    //------ vals --------//
    u64          id;     //- go实例 在程序中的 主要搜索依据。
    std::string  type;   //- 代表类型的 字符串


    glm::vec2    pos {};    //- go实例 在游戏世界中的 pos


    static ID_Manager  id_manager;

private:
    
    

    //GameWorldCoord  gwCoord {0.0f, 0.0f}; //- 游戏世界坐标值
    
    std::vector<Mesh> meshes;



};














#endif


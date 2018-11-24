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

//-------------------- CPP --------------------//
#include <string>
#include <vector>

//------------------- Libs --------------------//
#include "tprDataType.h" 


//------------------- SELF --------------------//
#include "Mesh.h" 
#include "GameWorldCoord.h" 





class GameObj{

public:

    explicit GameObj( const std::string &_name )
        :
        name(_name)
        {}



    void init();



private:
    u64          id;     //- go实例 在程序中的 主要搜索依据。
    std::string  name;

    GameWorldCoord  gwCoord; //- 游戏世界坐标值


    std::vector<Mesh> meshes;



};














#endif


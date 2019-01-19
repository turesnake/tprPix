/*
 * ========================= ColliEntSet.h ==========================
 *                          -- tpr --
 *                                        创建 -- 2019.01.13
 *                                        修改 -- 2019.01.13
 * ----------------------------------------------------------
 *    碰撞区预制件 （目前这个名字不够好）
 *    所有的 go 都不在具备 独立的 碰撞区，而是选用 现成的预制件 来有机组合。
 * ----------------------------
 */
#ifndef _TPR_COLLI_ENT_SET_H_
#define _TPR_COLLI_ENT_SET_H_

//--- glm - 0.9.8 ---
#include <glm/glm.hpp>
            //-- glm::vec2
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
#include <vector>
#include <set>

//-------------------- Engine --------------------//
#include "config.h"
#include "MapCoord.h"
#include "PixVec.h"
#include "NineBox.h"


//-- 一个 碰撞区预制件，创建一个实例 --
class ColliEntSet{
public:

    //---- set ----//
    inline void clear_all(){
        colliEnts.clear();
        colliEntCenters.clear();
        colliEnt_adds.clear();
        colliEnt_dels.clear();
        center.clear_all();
        radius = 0;
    }

    inline void add_colliEnt( const PixVec2 &_ppos ){
        MapCoord pos;
        pos.set_by_ppos( _ppos );
        colliEnts.insert( pos ); //- copy 
        colliEntCenters.push_back(glm::vec2{ (float)(_ppos.x + 1.5f),
                                          (float)(_ppos.y + 1.5f) }); //- copy
    }

    inline void set_center( const PixVec2 &_ppos ){
        center = _ppos;
        centerCompass = get_ppos_compass( center );
    }

    //- 注意，传入的 _r 是 (radius * 10)
    inline void set_radius( int _r_10 ){
        radius = (float)_r_10 * 0.1f;
    }

    void create_adds_dels();
    //---- get ----//    
    
    inline const PixVec2 &get_center() const {
        return center;
    }

    void debug();

private:
    std::set<MapCoord>  colliEnts {};       //- mapEnt坐标(左下) 
    std::vector<glm::vec2> colliEntCenters {}; //- mapEnt中心（用于 fly）
                                    //-- glm::vec2 暂时无法放入 set 容器
                                    //-- 先存储在 vector 中

    //-- 在 crawl 模式中，朝8个方向移动时，固定的 新增集／减少集 --
    // set -- 类内自动生成
    // get -- 通过 NineBoxIdx 下标来访问。 
    // idx==4 的 子容器 是空的  
    std::vector< std::set<MapCoord> > colliEnt_adds {};
    std::vector< std::set<MapCoord> > colliEnt_dels {};


    PixVec2   center        {}; //- 碰撞区中心 ppos
    NineBox   centerCompass {0,0}; //- center 在其 mapent 中的位置 
    float     radius        {}; //- 半径（通常是个手动设置的 粗略值，多用于 fly移动 ）
                                //- 以像素为单位

    //----- funcs -------
    void create_adds_dels_by_nineBox( NineBoxIdx _idx );
};


#endif 

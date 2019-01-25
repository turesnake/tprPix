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
#include "IntVec.h"
#include "NineBox.h"
#include "MapEntCompass.h"


//-- 一个 碰撞区预制件，创建一个实例 --
class ColliEntSet{
public:

    //---- set ----//
    inline void clear_all(){
        colliEnts.clear();
        colliEntMidFPoses.clear();
        addEntOffss.clear();
        delEntOffss.clear();
        centerPPos.clear_all();
        radius = 0;
    }

    inline void add_colliEnt( const IntVec2 &_ppos ){
        MapCoord pos;
        pos.set_by_ppos( _ppos );
        colliEnts.insert( pos ); //- copy 
        colliEntMidFPoses.push_back(glm::vec2{ ((float)_ppos.x + HALF_PIXES_PER_MAPENT),
                                               ((float)_ppos.y + HALF_PIXES_PER_MAPENT) }); //- copy
    }

    inline void set_centerPPos( const IntVec2 &_ppos ){
        centerPPos = _ppos;
        centerCompass = calc_ppos_compass( centerPPos );
    }

    //- 注意，传入的 _r 是 (radius * 10)
    inline void set_radius( int _r_10 ){
        radius = (float)_r_10 * 0.1f;
    }

    void create_adds_dels();
    //---- get ----//    
    
    inline const IntVec2 &get_centerPPos() const {
        return  centerPPos;
    }

    inline const MapEntCompass &get_centerCompass() const {
        return  centerCompass;
    }

    //--- IMPORTANT !!! ---//
    //  直接返回指定 adds/dels 子容器的 访问权（引用）
    inline const std::set<MapCoord> &get_addEntOffs( NineBoxIdx _nbIdx ) const {
        return addEntOffss.at( (int)_nbIdx );
    }
    inline const std::set<MapCoord> &get_delEntOffs( NineBoxIdx _nbIdx ) const {
        return delEntOffss.at( (int)_nbIdx );
    }


    void debug();


private:
    //-- 此处的 set，依赖 MapCoord类型的 "<" 运算符
    std::set<MapCoord>     colliEnts {};         //- mapEnt坐标( 基于 ces左下点 ) 
    std::vector<glm::vec2> colliEntMidFPoses {}; //- mapEnt中心（用于 fly）
                                    //-- glm::vec2 暂时无法放入 set 容器
                                    //-- 先存储在 vector 中

    //-- 在 crawl 模式中，朝8个方向移动时，固定的 新增集／减少集 --
    // set -- 类内自动生成
    // get -- 通过 NineBoxIdx 下标来访问。 
    // idx==4 的 子容器 是空的  
    std::vector< std::set<MapCoord> > addEntOffss {};
    std::vector< std::set<MapCoord> > delEntOffss {};


    IntVec2         centerPPos    {};    //- 碰撞区中心 ppos
    MapEntCompass   centerCompass {0,0}; //- center 在其 mapent 中的位置 
    float           radius        {};    //- 半径（通常是个手动设置的 粗略值，多用于 fly移动 ）
                                         //- 以像素为单位

    //----- funcs -------
    void create_adds_dels_by_nineBox( NineBoxIdx _idx );
};


#endif 


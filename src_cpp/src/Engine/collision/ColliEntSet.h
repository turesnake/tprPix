/*
 * ========================= ColliEntSet.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.01.13
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *    碰撞区预制件 （目前这个名字不够好）
 *    所有的 go 都不在具备 独立的 碰撞区，而是选用 现成的预制件 来有机组合。
 * ----------------------------
 */
#ifndef TPR_COLLI_ENT_SET_H_
#define TPR_COLLI_ENT_SET_H_

//--- glm - 0.9.9.5 ---
#include "glm_no_warnings.h"

//-------------------- CPP --------------------//
#include <vector>
#include <set>

//-------------------- Engine --------------------//
#include "config.h"
#include "MapCoord.h"
#include "IntVec.h"
#include "NineBox.h"
#include "MapEntCompass.h"


//-- 一个 碰撞区预制件，CREATE一个实例 --
class ColliEntSet{
    using addEntOffs_size_type = std::vector<std::set<MapCoord>>::size_type;
public:

    //---- set ----//
    inline void clear_all(){
        this->colliEnts.clear();
        this->addEntOffss.clear();
        this->delEntOffss.clear();
        this->centerPPos.clear_all();
        this->centerMPos.clear_all();
    }

    inline void add_colliEnt( const IntVec2 &_ppos ){
        MapCoord pos {};
        pos.set_by_ppos_( _ppos ); //-- 必须使用严谨版
        this->colliEnts.insert( pos ); //- copy 
    }

    inline void set_centerPPos( const IntVec2 &_ppos ){
        this->centerPPos = _ppos;
        this->centerMPos = anyPPos_2_mpos( _ppos );
        this->centerCompass = calc_ppos_compass( this->centerPPos );
    }

    void create_adds_dels();
    //---- get ----//    

    inline const IntVec2 &get_centerPPos() const {
        return  this->centerPPos;
    }
    //-- 主用于 碰撞检测，暂存于 ceh 中
    //  用它 可以快速计算 ces 左下角 mpos
    inline const IntVec2 &get_centerMPos() const {
        return  this->centerMPos;
    }
    inline const MapEntCompass &get_centerCompass() const {
        return  this->centerCompass;
    }

    //--- IMPORTANT !!! ---//
    //  直接返回指定 adds/dels 子容器的 访问权（引用）
    inline const std::set<MapCoord> &get_colliEnts() const {
        return this->colliEnts;
    }
    inline const std::set<MapCoord> &get_addEntOffs( NineBoxIdx _nbIdx ) const {
        return this->addEntOffss.at( static_cast<addEntOffs_size_type>(_nbIdx) );
    }
    inline const std::set<MapCoord> &get_delEntOffs( NineBoxIdx _nbIdx ) const {
        return this->delEntOffss.at( static_cast<addEntOffs_size_type>(_nbIdx) );
    }

private:
    void create_adds_dels_by_nineBox( NineBoxIdx _idx );

    //======== vals ========//
    //-- 此处的 set，依赖 MapCoord类型的 "<" 运算符
    std::set<MapCoord>     colliEnts {};         //- mapEnt坐标( 基于 ces左下点 ) 

    //-- 在 crawl 模式中，朝8个方向移动时，固定的 新增集／减少集 --
    // set -- 类内自动生成
    // get -- 通过 NineBoxIdx 下标来访问。 
    // idx==4 的 子容器 是空的  
    std::vector< std::set<MapCoord> > addEntOffss {};
    std::vector< std::set<MapCoord> > delEntOffss {};

    IntVec2         centerPPos    {};    //- 碰撞区中心 ppos
    IntVec2         centerMPos    {};    //- centerPPos 所在 mapent 的mpos
                                         //  主要用于 碰撞检测
                                         //  用它 可以快速计算 ces 左下角 mpos
    MapEntCompass   centerCompass {0,0}; //- center 在其 mapent 中的位置 

};


#endif 


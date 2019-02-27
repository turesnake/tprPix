/*
 * ==================== landOrWaters.cpp =======================
 *                          -- tpr --
 *                                        CREATE -- 2019.02.23
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *    each mapEnt.isLand 
 *    ----------
 * ----------------------------
 */
#include "Section.h"

//-------------------- CPP --------------------//

//------------------- Libs --------------------//
#include "tprDataType.h" 

//------------------- Engine --------------------//
#include "Section.h"
#include "sectionBuild.h"
#include "MapEntInBuild.h"


namespace sectionBuild { //------- namespace: sectionBuild ----------//

namespace{//----------- namespace ---------------//

    void poisson_distr();
    void merge_ent( u8_t _valA, u8_t _valB );

    void independent_erase();
    void lonely_point_erase( int _w, int _h );


}//-------------- namespace : end ---------------//


/* ===========================================================
 *                build_landOrWaters
 * -----------------------------------------------------------
 * -- 1: land
 * -- 0: waters
 * ---------
 * 不同的生态，其水域分布存在区别。
 * 当前版本是最简模式，“无视生态差异”
 */
void build_landOrWaters(){

    size_t idx;

    //--------------------------//
    //  一，为每个像素随机分配 0/1
    //--------------------------//


    for( int h=0; h<entSideLen_; h++ ){
        for( int w=0; w<entSideLen_; w++ ){
            idx = h*entSideLen_ + w;
            //-- 靠边的一圈像素，统统设置为 LAND
            //   此处有误：
            //  靠边的一圈，应该继承相邻section 的边缘设置 
            //  只有在 相邻section 为空时，才 自行设置为 LAND 
            //  未实现... 

            mapEnts.at(idx).isLand = LAND;
            
            /*
            (uDistribution(randEngine)>landPercent) ?
                    mapEnts.at(idx).isLand = LAND :
                    mapEnts.at(idx).isLand = WATERS;
            */

        }
    }



    //--------------------------//
    //    泊松分布 land-water点
    //--------------------------//
    //poisson_distr();


    //---------------------------------------//
    //  二，       合并 land点 
    //---------------------------------------//
    /*
    for( int i=0; i<numMergeLand; i++ ){
        merge_ent( LAND, WATERS );
    }
    */

    //---------------------------------------//
    //     删除 孤立的 land点  [优化] 
    //---------------------------------------//
    //independent_erase();


    //---------------------------------------//
    //     修改  section 数据:
    //   -1- mapents
    //   -2- mapTex  （不在此处...）
    //---------------------------------------//
    for( size_t i=0; i<mapEnts.size(); i++ ){
        (mapEnts.at(i).isLand==LAND) ?
                sectionPtr->memMapEnts.at(i).isLand = true :
                sectionPtr->memMapEnts.at(i).isLand = false;
    }

    //--- end ---
}


namespace{//----------- namespace ---------------//


/* ===========================================================
 *                  poisson_distr
 * -----------------------------------------------------------
 * 设置 泊松分部 
 * ------
 * 待实现：
 *   根据每个 ent 属于哪个生态群落，来配置 landPoissonTimes 等...
 */
void poisson_distr(){

    int w; //- tmp
    int h; //- tmp
    int times;
    size_t idx;

    //-- 特别边缘的区域，不生成
    std::uniform_int_distribution<int> di_w(  5, entSideLen_-5  );
    std::uniform_int_distribution<int> di_h(  5, entSideLen_-5  );

    //----- 生成 n个 land 随机点 -----
    land_poisson_pts.clear();
    for( int i=0; i<landPoissonTimes; i++ ){
        land_poisson_pts.push_back( IntVec2{di_w(randEngine), di_h(randEngine)} );
    }

    for( const auto& p : land_poisson_pts ){
        //-- 针对每个点，做柏松分布 --
        std::poisson_distribution<int> poisDi_w( p.x );
        std::poisson_distribution<int> poisDi_h( p.y );
        //----
        times = poisDistribution(randEngine);
        //----
        for( int i=0; i<times; i++ ){
            w = poisDi_w(randEngine);
            h = poisDi_h(randEngine);
            idx = h*entSideLen_ + w;
            if( (w>0) && (w<entSideLen_-1) && (h>0) && (h<entSideLen_-1)  ){
                mapEnts.at(idx).isLand = LAND; 
            }
        }
    }

    //----- 生成 n个 waters 随机点 -----
    waters_poisson_pts.clear();
    for( int i=0; i<watersPoissonTimes; i++ ){
        waters_poisson_pts.push_back( IntVec2{di_w(randEngine), di_h(randEngine)} );
    }

    for( const auto& p : waters_poisson_pts ){
        //-- 针对每个点，做柏松分布 --
        std::poisson_distribution<int> poisDi_w( p.x );
        std::poisson_distribution<int> poisDi_h( p.y );
        //----
        times = poisDistribution(randEngine);
        //----
        for( int i=0; i<times; i++ ){
            w = poisDi_w(randEngine);
            h = poisDi_h(randEngine);
            idx = h*entSideLen_ + w;
            if( (w>0) && (w<entSideLen_-1) && (h>0) && (h<entSideLen_-1)  ){
                mapEnts.at(idx).isLand = WATERS; 
            }
        }
    }
}




/* ===========================================================
 *                      merge_ent
 * -----------------------------------------------------------
 * -- 仅用于 land-waters 数据
 * -- 对一张 随机像素图，合并 某个值的 区域。
 * -- 根据 九宫格策略，如果一个ent的周围 8格中，有4格为 目标值，则设置此ent为 目标值
 * -- param: _valA -- 目标值，进行 merge
 * -- param: _valB -- 对应值，不要进行merge 的ent 改成此值
 * -- param: _map  -- 要改写哪张 map
 */
void merge_ent( u8_t _valA, u8_t _valB ){

    //---- 重排 map 全体单位的序号 顺序 --------
    //   速度会慢些，但可以接受
    randEngine.discard(7); //- 后跳7个状态
    std::shuffle( randWH.begin(), randWH.end(), randEngine );

    //----- 遍历 每个 map单位 -----
    int count; //-- 统计 单个像素，四周8格的 同色 的个数
    size_t idx;
    for( const auto &p : randWH ){
        count = 0;
        //-- 遍历 9宫格 --
        for( int h=(p.y-1); h<=(p.y+1); h++ ){
            for( int w=(p.x-1); w<=(p.x+1); w++ ){
                idx = h*entSideLen_ + w;
                //- 只处理周围 8 格 --
                if( !((w==p.x)&&(h==p.y)) ){
                    //- 防止 边界外访问
                    if( (w>=0) && (w<entSideLen_) && (h>=0) && (h<entSideLen_) ){
                        //- 只记录 要求色
                        if( mapEnts.at(idx).isLand == _valA ){
                            count++;
                        }
                    }
                }
            }
        }
        //-- 改写 目标ent的颜色
        idx = p.y*entSideLen_ + p.x;
        (count >= 4) ? 
            mapEnts.at(idx).isLand = _valA :
            mapEnts.at(idx).isLand = _valB;
    }
}


/* ===========================================================
 *                 independent_erase
 * -----------------------------------------------------------
 * -- 删除 孤立的 黑白点
 */
void independent_erase(){
    for( int h=0; h<entSideLen_; h++ ){
        for( int w=0; w<entSideLen_; w++ ){
            lonely_point_erase( w, h );
        }
    }

}


/* ===========================================================
 *                 lonely_point_erase
 * -----------------------------------------------------------
 * -- 消除 孤立的 land-waters点。只消除一个 
 * -- 当一个点的上下左右都 为异色，这个点就被同化
 */
void lonely_point_erase( int _w, int _h ){

    u8_t same; //- 同色
    u8_t anti; //- 异色
    if( mapEnts.at(_h*entSideLen_ + _w).isLand == LAND ){
        same = LAND;
        anti = WATERS;
    }else{
        same = WATERS;
        anti = LAND;
    }

    //-- 检查左边 --
    if( (_w>0) && (mapEnts.at(_h*entSideLen_ + (_w-1)).isLand == same) ){
        return;
    }
    //-- 检查右边 --
    if( (_w<entSideLen_-1) && (mapEnts.at(_h*entSideLen_ + (_w+1)).isLand == same) ){
        return;
    }
    //-- 检查下边 --
    if( (_h>0) && (mapEnts.at((_h-1)*entSideLen_ + _w).isLand == same) ){
        return;
    }
    //-- 检查上边 --
    if( (_h<entSideLen_-1) && (mapEnts.at((_h+1)*entSideLen_ + _w).isLand == same) ){
        return;
    }

    //-- 确定 目标像素被孤立，将其同化 --
    mapEnts.at(_h*entSideLen_ + _w).isLand = anti;
}




}//-------------- namespace : end ---------------//
}//-------------- namespace: sectionBuild: end -------------------//


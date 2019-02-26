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
#include "MapSection.h"

//-------------------- CPP --------------------//

//------------------- Libs --------------------//
#include "tprDataType.h" 

//------------------- Engine --------------------//
#include "sectionBuilderTools.h"



namespace{//----------- namespace ---------------//

    void poisson_distr();
    void merge_ent( u8_t _colorA, u8_t _colorB, std::vector<u8_t> &_map );

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
void MapSection::build_landOrWaters(){

    size_t idx;
    //------
    pointMap.clear();
    pointMap.resize( SECTION_SIDE_ENTS*SECTION_SIDE_ENTS );

    //--------------------------//
    //  一，为每个像素随机分配 0/1
    //--------------------------//
    
    for( int h=0; h<entWH.y; h++ ){
        for( int w=0; w<entWH.x; w++ ){
            idx = h*entWH.x + w;
            //-- 靠边的一圈像素，统统设置为 LAND
            //   此处有误：
            //  靠边的一圈，应该继承相邻section 的边缘设置 
            //  只有在 相邻section 为空时，才 自行设置为 LAND 
            //  未实现... 
            if( (h==0) || (h==entWH.y-1) || (w==0) || (w==entWH.x-1) ){
                pointMap.at(idx) = LAND;
            }else{
                (uDistribution(randEngine)>landPercent) ?
                    pointMap.at(idx) = LAND :
                    pointMap.at(idx) = WATERS;
            }
        }
    }

    //--------------------------//
    //    泊松分布 land-water点
    //--------------------------//
    poisson_distr();


    //---------------------------------------//
    //  二，       合并 land点 
    //---------------------------------------//
    for( int i=0; i<numMergeLand; i++ ){
        merge_ent( LAND, WATERS, pointMap );
    }

    //---------------------------------------//
    //     删除 孤立的 land点  [优化] 
    //---------------------------------------//
    independent_erase();


    //---------------------------------------//
    //     修改  section 数据:
    //   -1- mapents
    //   -2- mapTex  （不在此处...）
    //---------------------------------------//
    for( size_t i=0; i<pointMap.size(); i++ ){
        (pointMap.at(i)==LAND) ?
                this->memMapEnts.at(i).isLand = true :
                this->memMapEnts.at(i).isLand = false;
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
    std::uniform_int_distribution<int> di_w(  5, entWH.x-5  );
    std::uniform_int_distribution<int> di_h(  5, entWH.y-5  );

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
            idx = h*entWH.x + w;
            if( (w>0) && (w<entWH.x-1) && (h>0) && (h<entWH.y-1)  ){
                pointMap.at(idx) = LAND; 
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
            idx = h*entWH.x + w;
            if( (w>0) && (w<entWH.x-1) && (h>0) && (h<entWH.y-1)  ){
                pointMap.at(idx) = WATERS; 
            }
        }
    }
}




/* ===========================================================
 *                      merge_ent
 * -----------------------------------------------------------
 * -- 通用
 * -- 对一张 随机像素图，合并 某个值的 区域。
 * -- 根据 九宫格策略，如果一个ent的周围 8格中，有4格为 目标值，则设置此ent为 目标值
 * -- param: _colorA -- 目标色，进行 merge
 * -- param: _colorB -- 对应色，不要进行merge 的ent 改成此值
 * -- param: _map  -- 要改写哪张 map
 */
void merge_ent( u8_t _colorA, u8_t _colorB, std::vector<u8_t> &_map ){

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
                idx = h*entWH.x + w;
                //- 只处理周围 8 格 --
                if( !((w==p.x)&&(h==p.y)) ){
                    //- 防止 边界外访问
                    if( (w>=0) && (w<entWH.x) && (h>=0) && (h<entWH.y) ){
                        //- 只记录 要求色
                        if( _map.at(idx) == _colorA ){
                            count++;
                        }
                    }
                }
            }
        }
        //-- 改写 目标ent的颜色
        idx = p.y*entWH.x + p.x;
        (count >= 4) ? 
            _map.at(idx) = _colorA :
            _map.at(idx) = _colorB;
    }
}


/* ===========================================================
 *                 independent_erase
 * -----------------------------------------------------------
 * -- 删除 孤立的 黑白点
 */
void independent_erase(){
    for( int h=0; h<entWH.y; h++ ){
        for( int w=0; w<entWH.x; w++ ){
            lonely_point_erase( w, h );
        }
    }

}


/* ===========================================================
 *                 lonely_point_erase
 * -----------------------------------------------------------
 * -- 消除 孤立的 黑白点。只消除一个 
 * -- 当一个点的上下左右都 为异色，这个点就被同化
 */
void lonely_point_erase( int _w, int _h ){

    bool isLand_; //- 当前处理的点是 land or waters
    int same; //- 同色
    int anti; //- 异色
    if( pointMap[_h*entWH.x + _w] == LAND ){
        isLand_ = true;
        same = LAND;
        anti = WATERS;
    }else{
        isLand_ = false;
        same = WATERS;
        anti = LAND;
    }

    //-- 检查左边 --
    if( (_w>0) && (pointMap[_h*entWH.x + (_w-1)] == same) ){
        return;
    }
    //-- 检查右边 --
    if( (_w<entWH.x-1) && (pointMap[_h*entWH.x + (_w+1)] == same) ){
        return;
    }
    //-- 检查下边 --
    if( (_h>0) && (pointMap[(_h-1)*entWH.x + _w] == same) ){
        return;
    }
    //-- 检查上边 --
    if( (_h<entWH.y-1) && (pointMap[(_h+1)*entWH.x + _w] == same) ){
        return;
    }

    //-- 确定 目标像素被孤立，将其同化 --
    pointMap[_h*entWH.x + _w] = anti;
}




}//-------------- namespace : end ---------------//



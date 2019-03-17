/*
 * ==================== landWater_build.cpp =======================
 *                          -- tpr --
 *                                        CREATE -- 2019.03.16
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *    each mapEnt.isLand 
 *    被取代 ...
 * ----------------------------
 */

//-------------------- C --------------------//
#include <cassert>

//-------------------- Engine --------------------//
#include "sectionBuild_innerDatas.h"



namespace sectionBuild { //------- namespace: sectionBuild ----------//

namespace{//----------- namespace ---------------//

    //- 有关landwater 数据，一次只处理一个 section
    sectionKey_t landWater_sectionKey;
    IntVec2      landWater_sectionMPos;

    void poisson_distr();
    void get_landWaterPrefabDatas();
    void transfer_tmpDatas_2_esrc();
    void merge_ent( bool _isLand );


}//-------------- namespace : end ---------------//



/* ===========================================================
 *                 build_landWaterEnts
 * -----------------------------------------------------------
 */
void build_landWaterEnts( sectionKey_t _sectionKey ){

    //--------------------------//
    //  查看目标 section 是否已经创建过 本组数据
    //--------------------------//
    Section  *sectionPtr_ = esrc::get_sectionPtr( _sectionKey ); 
    if( sectionPtr_->is_landWaterEntSets_set ){
        return;
    }


    landWater_sectionKey = _sectionKey;
    landWater_sectionMPos = sectionKey_2_mpos( _sectionKey );

    //--------------------------//
    //  初始化临时容器
    //  为每个 ent 分配 随机 LandOrWater 0/1
    //--------------------------//
    landWaterEnts_in_section.clear();
    landWaterEnts_in_section.resize( ENTS_PER_SECTION * ENTS_PER_SECTION );
    for( auto &entRef : landWaterEnts_in_section ){
        
        (uDistribution_regular(randEngine)<landPercent) ?
                entRef.set_isLand(true) :
                entRef.set_isLand(false);
        
        

        //entRef.set_isLand(true); //- 先全部设置为 land

    }

    //--------------------------//
    //    泊松分布 land-water点
    //--------------------------//
    poisson_distr();

    //--------------------------//
    //   取得section corners/edges landWater预制件，
    //   覆盖边缘的随机数据
    //--------------------------//
    //get_landWaterPrefabDatas();

    //--------------------------//
    //     合并 land点 
    //--------------------------//
    for( int i=0; i<numMergeLand; i++ ){
        merge_ent( true );
    }

    //for( int i=0; i<numMergeLand; i++ ){
    //    merge_ent( false );
    //}

    get_landWaterPrefabDatas();

    //--------------------------//
    //     删除 孤立的 land点  [优化] 
    //--------------------------//
    //...


    //--------------------------//
    //   遍历每个ent，计算其 idBorder
    //--------------------------//
    //...


    //--------------------------//
    //   数据制作完毕
    // 以 chunk 为单位，存储到 全局容器中
    //--------------------------//
    transfer_tmpDatas_2_esrc();

    //-------
    sectionPtr_->is_landWaterEntSets_set = true;
}


namespace{//----------- namespace ---------------//

/* ===========================================================
 *                  poisson_distr
 * -----------------------------------------------------------
 * 设置 泊松分部 land or water
 */
void poisson_distr(){

    size_t  w;
    size_t  h;
    size_t  idx;
    size_t  times;
    
    //-- 泊松节点 不会出现在 section 边缘区域 --
    std::uniform_int_distribution<size_t> di_w(  5, ENTS_PER_SECTION-5  );
    std::uniform_int_distribution<size_t> di_h(  5, ENTS_PER_SECTION-5  );

    //-------------------------//
    //  生成数组 Land 泊松点群
    //-------------------------//
    size_t landPoissonTimes = uDistribution_landPoissonTimes(randEngine);
    for( int i=0; i<landPoissonTimes; i++ ){
        //-- 随机生成一个 泊松节点，并以此为基础，制作一对 泊松分布器 --
        std::poisson_distribution<size_t> poisDi_w( di_w(randEngine) );
        std::poisson_distribution<size_t> poisDi_h( di_h(randEngine) );
        //-----------
        times = uDistribution_poissonLand(randEngine);
        for( int i=0; i<times; i++ ){
            w = poisDi_w(randEngine);
            h = poisDi_h(randEngine);
            idx = h*ENTS_PER_SECTION + w;
            //- section 边缘2ent 不设置 --
            if( (w>1) && (w<ENTS_PER_SECTION-2) && (h>1) && (h<ENTS_PER_SECTION-2)  ){
                landWaterEnts_in_section.at(idx).set_isLand( true );
            }
        }
    }

    //-------------------------//
    //  生成数组 Water 泊松点群
    //-------------------------//
    size_t waterPoissonTimes = uDistribution_waterPoissonTimes(randEngine);
    for( int i=0; i<waterPoissonTimes; i++ ){
        //-- 随机生成一个 泊松中点，并以此为基础，制作一对 泊松分布器 --
        std::poisson_distribution<int> poisDi_w( di_w(randEngine) );
        std::poisson_distribution<int> poisDi_h( di_h(randEngine) );
        //-----------
        times = uDistribution_poissonWater(randEngine);
        for( int i=0; i<times; i++ ){
            w = poisDi_w(randEngine);
            h = poisDi_h(randEngine);
            idx = h*ENTS_PER_SECTION + w;
            //- section 边缘2ent 不设置 --
            if( (w>1) && (w<ENTS_PER_SECTION-2) && (h>1) && (h<ENTS_PER_SECTION-2)  ){
                landWaterEnts_in_section.at(idx).set_isLand( false );
            }
        }
    }
}


/* ===========================================================
 *              get_landWaterPrefabDatas
 * -----------------------------------------------------------
 *  从 全局容器中，取出 本section corners/edges id，
 *  再根据id，取出 对应的 预制件 数据块
 */
void get_landWaterPrefabDatas(){

    IntVec2       tmpCornerMpos;
    IntVec2       tmpEdgeMpos;
    IntVec2       tmpMPos;
    IntVec2       entMposOff; //- 每个ent，相对 landWater_sectionMPos 的 mpos偏移。

    size_t        idx;

    chunkKey_t    tmpChunkKey;

    landWaterPrefabEdgeId_t    edgeId;
    landWaterPrefabCornerId_t  cornerId;

    //-- 访问 section 4个corner --
    for( const auto &cornerDataRef : allCornerDatas_in_section ){ //- each corner
        tmpCornerMpos = cornerDataRef.mposOff + landWater_sectionMPos;
        cornerId = esrc::get_landWaterPrefabCornerId( anyMPos_2_sectionKey(tmpCornerMpos) );

        //-- 获得 预制件 对应区块的数据 --
        for( auto &cornerPrefabEnt : get_landWaterPrefabCorner(cornerId, cornerDataRef.quadType) ){

            tmpMPos = tmpCornerMpos + cornerPrefabEnt.lMPosOff;
            entMposOff = tmpMPos - landWater_sectionMPos;
                assert( (entMposOff.x>=0) && (entMposOff.y>=0) ); //- tmp

            idx = entMposOff.y*ENTS_PER_SECTION + entMposOff.x;
                assert( idx < landWaterEnts_in_section.size() );
            //-- 假定将 预制件 主色配给 land [固定] --
            ( cornerPrefabEnt.is_major ) ?
                landWaterEnts_in_section.at(idx).set_isLand( true ) :
                landWaterEnts_in_section.at(idx).set_isLand( false );
        }
    } //--- each corner end ---

    
    //-- 访问 section 4个侧边节点 --
    for( const auto &edgeDataRef : allEdgeDatas_in_section ){ //- each edge

        tmpEdgeMpos = edgeDataRef.mposOff + landWater_sectionMPos;
        edgeId = esrc::get_landWaterPrefabEdgeId( anyMPos_2_chunkKey(tmpEdgeMpos) );

        //-- 获得 预制件 对应区块的数据 --
        for( auto &edgePrefabEnt : get_landWaterPrefabEdge( edgeId, edgeDataRef.quadType ) ){

            tmpMPos = tmpEdgeMpos + edgePrefabEnt.lMPosOff;
            entMposOff = tmpMPos - landWater_sectionMPos;
                assert( (entMposOff.x>=0) && (entMposOff.y>=0) ); //- tmp

            idx = entMposOff.y*ENTS_PER_SECTION + entMposOff.x;
                assert( idx < landWaterEnts_in_section.size() );
            //-- 假定将 预制件 主色配给 land [固定] --
            ( edgePrefabEnt.is_major ) ?
                landWaterEnts_in_section.at(idx).set_isLand( true ) :
                landWaterEnts_in_section.at(idx).set_isLand( false );
        }
    } //- each edge end --
    

}



/* ===========================================================
 *              transfer_tmpDatas_2_esrc
 * -----------------------------------------------------------
 * 将临时容器 landWaterEnts_in_section 中的数据，以 chunk 为单位
 * 转存到 全局容器中
 */
void transfer_tmpDatas_2_esrc(){

    chunkKey_t   tmpChunkKey;
    size_t       entW; //- ent 在 section 中 wh mpos
    size_t       entH; //- ent 在 section 中 wh mpos
    size_t       entIdx_in_section;
    size_t       entIdx_in_chunk;

    for( int ch=0; ch<CHUNKS_PER_SECTION; ch++ ){
        for( int cw=0; cw<CHUNKS_PER_SECTION; cw++ ){ //- each chunk in section

            tmpChunkKey = anyMPos_2_chunkKey( IntVec2{  landWater_sectionMPos.x + cw*ENTS_PER_CHUNK,
                                                        landWater_sectionMPos.y + ch*ENTS_PER_CHUNK });

            //-- 在全局容器中 创建 目标chunk 子容器，并扩容 --
            std::vector<LandWaterEnt> &containerRef = esrc::insert_new_landWaterEntSet( tmpChunkKey );
            containerRef.resize( ENTS_PER_CHUNK * ENTS_PER_CHUNK );

            //-- 复制每个 ent 数据 --
            for( int eh=0; eh<ENTS_PER_CHUNK; eh++ ){
                for( int ew=0; ew<ENTS_PER_CHUNK; ew++ ){ //- each mapent in chunk

                    entW = cw*ENTS_PER_CHUNK + ew;
                    entH = ch*ENTS_PER_CHUNK + eh;

                    entIdx_in_section = entH*ENTS_PER_SECTION + entW;
                    entIdx_in_chunk = eh*ENTS_PER_CHUNK + ew;
    
                    containerRef.at(entIdx_in_chunk) = landWaterEnts_in_section.at(entIdx_in_section);
                }
            } //- each mapent in chunk end --
        }
    } //- each chunk in section end --

    //-- 不用清理 临时容器 landWaterEnts_in_section --
    //  下次再生成 section 时，会被清理
}



/* ===========================================================
 *                     merge_ent
 * -----------------------------------------------------------
 *  临时版...
 */
void merge_ent( bool _isLand ){

    //---- 重排 section 全体单位的序号 顺序 --------
    //   速度会慢些，但可以接受
    std::shuffle( randWH.begin(), randWH.end(), randEngine );

    //----- 遍历 section 中 每个 ent -----
    int     count; //-- 统计 单个像素，四周8格的 同色 的个数
    size_t  idx;

    for( const auto &p : randWH ){
        count = 0;
        //-- 遍历 9宫格 --
        for( int h=(p.y-1); h<=(p.y+1); h++ ){
            for( int w=(p.x-1); w<=(p.x+1); w++ ){
                idx = h*ENTS_PER_SECTION + w;

                //- 只处理周围 8 格 --
                if( !((w==p.x)&&(h==p.y)) ){

                    //- 防止 边界外访问 -
                    // 此处实现有误。应该是，边界 2ent，通过访问 预制件来获得数据...

                    if( (w>=0) && (w<ENTS_PER_SECTION) && (h>=0) && (h<ENTS_PER_SECTION) ){
                        //- 只记录 要求色
                        if( landWaterEnts_in_section.at(idx).get_isLand() == _isLand ){
                            count++;
                        }
                    }
                }
            }
        }
        //-- 改写 目标ent的颜色
        idx = p.y*ENTS_PER_SECTION + p.x;
        (count >= 4) ? 
            landWaterEnts_in_section.at(idx).set_isLand( _isLand ) :
            landWaterEnts_in_section.at(idx).set_isLand( !_isLand );
    }

}





}//-------------- namespace : end ---------------//

}//----------------- namespace: sectionBuild: end -------------------//


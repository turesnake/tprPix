/*
 * ========================== Chunk.h =======================
 *                          -- tpr --
 *                                        CREATE -- 2018.12.09
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *    Chunk  64*64_mapents, [left-bottom]
 * ----------------------------
 */
#ifndef TPR_CHUNK_H
#define TPR_CHUNK_H

//--- glm - 0.9.9.5 ---
#include "glm_no_warnings.h"

//-------------------- CPP --------------------//
#include <vector>
#include <set>
#include <memory>

//-------------------- Engine --------------------//
#include "tprAssert.h"
#include "IntVec.h" 
#include "MapEnt.h"
#include "config.h" 
//#include "Mesh.h"
//#include "MapTexture.h" 
#include "chunkKey.h"
#include "MapCoord.h" 
#include "sectionKey.h"

#include "tprCast.h"

//-- 64*64 个 Fst_diskMapEnt 元素.[硬盘态] --
//-- 下面这段，暂时没想起来 它是用来做什么的 ... ---
struct Fst_diskChunk{
    Fst_diskMapEnt data[ ENTS_PER_CHUNK * ENTS_PER_CHUNK ]; //- 512KB
};


//-- 64*64 个 mapEnt, 组成一张 chunk  [mem] --
//  chunk 作为一个整体被存储到硬盘，就像 mc 中的 Field
class Chunk{
public:
    Chunk() = default;

    void init();
    

    inline void insert_2_goIds( const goid_t &id_ ){
        this->goIds.insert(id_);
    }
    inline size_t erase_from_goIds( const goid_t &id_ ){
        return this->goIds.erase(id_);
    }
    inline void insert_2_edgeGoIds( const goid_t &id_ ){
        this->edgeGoIds.insert(id_);
    }
    inline size_t erase_from_edgeGoIds( const goid_t &id_ ){
        return this->edgeGoIds.erase(id_);
    }

    //------- set -------//
    inline void set_by_chunkKey( chunkKey_t chunkKey_ ){
        this->chunkKey = chunkKey_;
        this->mcpos.set_by_mpos( chunkKey_2_mpos(chunkKey_) );
    }

    //------- get -------//
    inline const IntVec2 &get_mpos() const {
        return this->mcpos.get_mpos();
    }
    inline const MapCoord &get_mcpos() const {
        return this->mcpos;
    }
    inline const chunkKey_t &get_key() const {
        return this->chunkKey;
    }
    inline const std::vector<fieldKey_t> &get_fieldKeys() const {
        return this->fieldKeys;
    }
    inline const std::set<goid_t> &get_goIds() const {
        return this->goIds;
    }
    inline const std::set<goid_t> &get_edgeGoIds() const {
        return this->edgeGoIds;
    }
    //-- 确保 参数为 基于chunk左下ent 的 相对mpos
    inline MemMapEnt &getnc_mapEntRef_by_lMPosOff( const IntVec2 &lMPosOff_ ){
        size_t idx = cast_2_size_t( lMPosOff_.y*ENTS_PER_CHUNK + lMPosOff_.x );
            tprAssert( idx < memMapEnts.size() ); //- tmp
        return *(memMapEnts.at(idx).get());
    }
    
    //======== flags ========//
    bool     is_memMapEnts_set              {false};

    bool     is_dirty  {false};//- 如果目标chunk上的 伪随机go数据，发生变化
                               //- 则被释放为 dirty.
                               //  下次生成此chunk时，不能直接使用 伪随机数，
                               //  还要参考 改动表 中的数据
                               //  此部分功能未实现

private:
    void init_memMapEnts();
    size_t get_mapEntIdx_in_chunk( const IntVec2 &anyMPos_ );
    size_t get_pixIdx_in_chunk( const IntVec2 &anyPPos_ );

    chunkKey_t  chunkKey {};
    MapCoord    mcpos    {}; //- [left-bottom]

    std::vector<std::unique_ptr<MemMapEnt>> memMapEnts {}; 

    std::set<goid_t>  goIds {}; //- 动态存储 本chunk 拥有的所有 go id。
                                //  部分元素会与 edgeGoIds 重合
                                //  添加和 释放工作要做干净。

    std::set<goid_t>  edgeGoIds {}; //- "临界go"／“边缘go” 容器
                                    // 有些 go 位于 chunk边缘，其 collients 可能在隔壁 chunk
                                    // 这种go 在 创建／销毁 阶段往往很麻烦
                                    // 使用一个容器来 动态保管它们。

    std::vector<fieldKey_t> fieldKeys {}; //- 8*8 fieldKeys,仅用来辅助 chunk创建
                                          

    double  zOff {}; //- chunk间存在前后层次，
};


#endif


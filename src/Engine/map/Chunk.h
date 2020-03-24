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

#include "pch.h"

//-------------------- Engine --------------------//
#include "MapEnt.h"
#include "GameObjType.h"
#include "fieldKey.h"
#include "chunkKey.h"
#include "MapCoord.h" 
#include "sectionKey.h"


//-- 64*64 个 mapEnt, 组成一张 chunk  [mem] --
//  chunk 作为一个整体被存储到硬盘，就像 mc 中的 Field
class Chunk{
public:
    explicit Chunk( chunkKey_t chunkKey_ ):
        chunkKey(chunkKey_),
        mpos(chunkKey_2_mpos(chunkKey_))
        {
            this->init();
        }

    inline void     insert_2_goIds( goid_t id_ )noexcept{ this->goIds.insert(id_); }
    inline size_t   erase_from_goIds( goid_t id_ )noexcept{ return this->goIds.erase(id_); }
    inline void     insert_2_edgeGoIds( goid_t id_ )noexcept{ this->edgeGoIds.insert(id_); }
    inline size_t   erase_from_edgeGoIds( goid_t id_ )noexcept{ return this->edgeGoIds.erase(id_); }

    //------- get -------//
    inline IntVec2                  get_mpos() const noexcept{ return this->mpos; }
    inline chunkKey_t               get_key() const noexcept{ return this->chunkKey; }
    inline const std::set<goid_t>   &get_goIds() const noexcept{ return this->goIds; }
    inline const std::set<goid_t>   &get_edgeGoIds() const noexcept{ return this->edgeGoIds; }
    
    
    //-- 确保 参数为 基于chunk左下ent 的 相对mpos
    inline MemMapEnt *getnc_mapEntPtr( const IntVec2 &lMPosOff_ )noexcept{
        tprAssert( (lMPosOff_.x>=0) && (lMPosOff_.y>=0) );
        size_t idx = cast_2_size_t( lMPosOff_.y * ENTS_PER_CHUNK<> + lMPosOff_.x );
        tprAssert( idx < this->memMapEnts.size() ); //- tmp
        return this->memMapEnts.at(idx).get();
    }
    
private:
    void init();

    //========== vals ==========//
    chunkKey_t      chunkKey;
    IntVec2         mpos; // [left-bottom]
    size_t          uWeight  {}; // [0, 9999]

    //--------------//
    std::vector<std::unique_ptr<MemMapEnt>> memMapEnts {}; 

    std::set<goid_t>  goIds {}; //- 动态存储 本chunk 拥有的所有 go id。
                                //  部分元素会与 edgeGoIds 重合
                                //  添加和 释放工作要做干净。

    std::set<goid_t>  edgeGoIds {}; //- "临界go"／“边缘go” 容器
                                    // 有些 go 位于 chunk边缘，其 collients 可能在隔壁 chunk
                                    // 这种go 在 创建／销毁 阶段往往很麻烦
                                    // 使用一个容器来 动态保管它们。

    //======== flags ========//
    bool     is_dirty  {false};//- 如果目标chunk上的 伪随机go数据，发生变化
                               //- 则被释放为 dirty.
                               //  下次生成此chunk时，不能直接使用 伪随机数，
                               //  还要参考 改动表 中的数据
                               //  此部分功能未实现    
};


#endif


/*
 * ========================== Section.h =======================
 *                          -- tpr --
 *                                        CREATE -- 2019.03.02
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 * ----------------------------
 */
#ifndef _TPR_SECTION_H_
#define _TPR_SECTION_H_

//-------------------- C --------------------//
#include <cassert>

//-------------------- CPP --------------------//
#include <vector>

//-------------------- Engine --------------------//
#include "sectionKey.h"
#include "IntVec.h"
#include "Quad.h"
#include "chunkKey.h"

//--- need ---//
class EcoSysInMap;


//- 为了生成地图 而设置的 数据结构
//  section 确实代表了 游戏世界中 实实在在的一个区域。
//  但这部分数据 已经被存储到了 Chunk / EcoSysInMap 等结构中
//  -----
//  现在的 section数据，更多是用来 辅助地图生成的
//
class Section{
public:
    Section() = default;

    void init();

    void bind_ecoSysInMapPtrs();
    void assign_chunks_to_ecoSysInMap(); //- 核心函数

    //-- param: _anyMPos - 本section 中的任意 mpos
    inline void set_by_anyMPos( const IntVec2 &_anyMPos ){
        this->sectionKey = anyMPos_2_sectionKey( _anyMPos );
        mcpos.set_by_mpos( sectionKey_2_mpos(this->sectionKey) );        
    }

    inline void set_by_sectionKey( sectionKey_t _sectionKey ){
        this->sectionKey = _sectionKey;
        mcpos.set_by_mpos( sectionKey_2_mpos(this->sectionKey) );        
    }

    //-- get --
    inline const std::vector<sectionKey_t> &get_nearbySectionKeys() const {
        return nearbySectionKeys;
    }
    inline const std::vector<sectionKey_t> &get_quadSectionKeys() const {
        return quadSectionKeys;
    }
    inline sectionKey_t get_quadSectionKey_by_quadType( QuadType _type ) const {
        int quadIdx = QuadType_2_Idx( _type );
        return quadSectionKeys.at( quadIdx );
    }

    //- 获得 目标chunk 在 本section 容器中的 序号 [0,8]
    //- param: _chunkMPos - 必须是 chunk mpos   [未做检测]
    inline size_t get_chunk_idx( const IntVec2 &_chunkMPos ){
            assert( is_a_chunkMPos(_chunkMPos) ); //- tmp
        IntVec2 mposOff = _chunkMPos - mcpos.get_mpos();
        int w = mposOff.x/ENTS_PER_CHUNK;
        int h = mposOff.y/ENTS_PER_CHUNK;
        return (h*CHUNKS_PER_SECTION + w);
    }

    //- param: _w_chunk - 以 chunk 为单位的 横向序号
    //- param: _h_chunk - 以 chunk 为单位的 纵向序号
    /*    暂时未被使用...
    inline const IntVec2 &get_chunkNodePPos( int _w_chunk, int _h_chunk ) const {
        size_t idx = _h_chunk*CHUNKS_PER_SECTION + _w_chunk;
            assert( (idx>=0) && (idx<chunkNodePPoses.size()) ); //- tmp
        return chunkNodePPoses.at(idx);
    }
    */
    inline const IntVec2 &get_chunkNodePPos( size_t _idx ) const {
            assert( (_idx>=0) && (_idx<chunkNodePPoses.size()) ); //- tmp
        return chunkNodePPoses.at(_idx);
    }
    
    /*    暂时未被使用...
    inline const sectionKey_t get_chunkEcoSysInMapKey( int _w_chunk, int _h_chunk ) const {
        size_t idx = _h_chunk*CHUNKS_PER_SECTION + _w_chunk;
            assert( (idx>=0) && (idx<chunkEcoSysInMapKeys.size()) ); //- tmp
        return chunkEcoSysInMapKeys.at(idx);
    }
    */
    inline const sectionKey_t get_chunkEcoSysInMapKey( size_t _idx ) const {
            assert( (_idx>=0) && (_idx<chunkEcoSysInMapKeys.size()) ); //- tmp
        return chunkEcoSysInMapKeys.at(_idx);
    }

    


    //======== vals ========//
    sectionKey_t  sectionKey {};
    MapCoord      mcpos  {}; //- [left-bottom]

    //----- 一阶数据 / first order data ------//
    std::vector<sectionKey_t>  nearbySectionKeys {}; //- 周边9个section 的 keys
    std::vector<sectionKey_t>  quadSectionKeys {};   //- 本section四个端点的 keys

    //-- 属于 chunk 的 planData --
    std::vector<sectionKey_t>  chunkEcoSysInMapKeys {};  //- 每个 chunk，属于哪个 ecoSysInMap - 4*4
                                        //- 临时存储处，等 chunk正式被创建时，这份数据会被搬运过去。

    std::vector<IntVec2>  chunkNodePPoses {}; //- 本section 含有的 每个 chunk 的 距离场点ppos - 4*4
                                        //- 临时存储处，等 chunk正式被创建时，这份数据会被搬运过去。


    //----- 二阶数据 / second order data ------//
    std::vector<EcoSysInMap*>  ecoSysInMapPtrs {}; //- 4个端点的 ecosys 指针。
                                    // 也许不需要...


    //======== flags ========//

private:

    void init_nearbySectionKeys();
    void init_quadSectionKeys();
    void init_chunkNodePPoses();

    inline void init_chunkEcoSysInMapKeys(){
        chunkEcoSysInMapKeys.resize( CHUNKS_PER_SECTION * CHUNKS_PER_SECTION );
    }
    

};




#endif 


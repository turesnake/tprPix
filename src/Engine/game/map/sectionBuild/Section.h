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

    //- 获得 目标chunk 在 本section 容器中的 序号 [0,15]
    //- param: _chunkMPos - 必须是 chunk mpos   [未做检测]
    size_t get_chunk_idx( const IntVec2 &_chunkMPos );

    //-- param: _anyMPos - 本section 中的任意 mpos
    inline void set_by_anyMPos( const IntVec2 &_anyMPos ){
        this->sectionKey = anyMPos_2_sectionKey( _anyMPos );
        this->mcpos.set_by_mpos( sectionKey_2_mpos(this->sectionKey) );        
    }

    inline void set_by_sectionKey( sectionKey_t _sectionKey ){
        this->sectionKey = _sectionKey;
        mcpos.set_by_mpos( sectionKey_2_mpos(this->sectionKey) );        
    }

    //-- get --
    inline const std::vector<sectionKey_t> &get_nearbySectionKeys() const {
        assert( this->is_nearbySectionKeys_set ); //- tmp
        return nearbySectionKeys;
    }
    inline const std::vector<sectionKey_t> &get_quadSectionKeys() const {
        assert( this->is_quadSectionKeys_set ); //- tmp
        return quadSectionKeys;
    }
    inline sectionKey_t get_quadSectionKey_by_quadType( QuadType _type ) const {
        assert( this->is_quadSectionKeys_set ); //- tmp
        int quadIdx = QuadType_2_Idx( _type );
        return quadSectionKeys.at( quadIdx );
    }
    inline const IntVec2 &get_mpos() const {
        return this->mcpos.get_mpos();
    }

    
    inline const IntVec2 &get_chunkNodeMPos( size_t _idx ) const {
            assert( this->is_chunkNodeMPoses_set ); //- tmp
            assert( (_idx>=0) && (_idx<chunkNodeMPoses.size()) ); //- tmp
        return chunkNodeMPoses.at(_idx);
    }
    
    inline const sectionKey_t get_chunkEcoSysInMapKey( size_t _idx ) const {
            assert( this->is_chunkEcoSysInMapKeys_set ); //- tmp
            assert( (_idx>=0) && (_idx<chunkEcoSysInMapKeys.size()) ); //- tmp
        return chunkEcoSysInMapKeys.at(_idx);
    }

    
    //======== vals ========//
    sectionKey_t  sectionKey {};
    MapCoord      mcpos  {}; //- [left-bottom]

    //----- 一阶数据 / first order data ------//
    std::vector<sectionKey_t>  nearbySectionKeys {}; //- 周边 9个section 的 keys
    std::vector<sectionKey_t>  quadSectionKeys {};   //- 本section四个端点的 keys

    //-- 属于 chunk 的 planData --
    std::vector<sectionKey_t>  chunkEcoSysInMapKeys {};  //- 每个 chunk，属于哪个 ecoSysInMap - 4*4
                                        //- 临时存储处，等 chunk正式被创建时，这份数据会被搬运过去。

    std::vector<IntVec2>  chunkNodeMPoses {}; //- 本section 含有的 每个 chunk 的 距离场点 mpos - 4*4 个
                                        //- 临时存储处，等 chunk正式被创建时，这份数据会被搬运过去。


    //----- 二阶数据 / second order data ------//
    std::vector<EcoSysInMap*>  ecoSysInMapPtrs {}; //- 4个端点的 ecosys 指针。
                                    // 也许不需要...


    //======== flags ========//
    bool  is_nearbySectionKeys_set    {false};
    bool  is_quadSectionKeys_set      {false}; 
    bool  is_chunkEcoSysInMapKeys_set {false};
    bool  is_chunkNodeMPoses_set      {false};
    bool  is_ecoSysInMapPtrs_set      {false};

    bool  is_assign_chunks_to_ecoSysInMap_done {false};


private:

    void init_nearbySectionKeys();
    void init_quadSectionKeys();
    void init_chunkNodeMPoses();

    inline void init_chunkEcoSysInMapKeys(){
        if( this->is_chunkEcoSysInMapKeys_set ){
            return;
        }
        chunkEcoSysInMapKeys.resize( CHUNKS_PER_SECTION * CHUNKS_PER_SECTION );
        this->is_chunkEcoSysInMapKeys_set = true;
    }
    

};




#endif 


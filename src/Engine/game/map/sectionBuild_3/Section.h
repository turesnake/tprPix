/*
 * ========================== Section.h =======================
 *                          -- tpr --
 *                                        CREATE -- 2019.03.02
 *                                        MODIFY -- 
 * ----------------------------------------------------------
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

    
    //======== vals ========//
    sectionKey_t  sectionKey {};
    MapCoord      mcpos  {}; //- [left-bottom]

    //----- 一阶数据 / first order data ------//
    std::vector<sectionKey_t>  quadSectionKeys {}; //- 本section四个端点的 keys

    //----- 二阶数据 / second order data ------//
    std::vector<EcoSysInMap*>  ecoSysInMapPtrs {}; //- 4个端点的 ecosys 指针。
                                    
    //======== flags ========//
    bool  is_quadSectionKeys_set      {false}; 
    bool  is_ecoSysInMapPtrs_set      {false};

private:

    void init_quadSectionKeys();
};


#endif 


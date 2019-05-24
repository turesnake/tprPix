/*
 * ========================== Chunk.h =======================
 *                          -- tpr --
 *                                        CREATE -- 2018.12.09
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *    Chunk  64*64_mapents, [left-bottom]
 * ----------------------------
 */
#ifndef _TPR_CHUNK_H_
#define _TPR_CHUNK_H_

//--- glm - 0.9.8 ---
#include <glm/glm.hpp>
            //-- glm::vec2
            //-- glm::vec3
            //-- glm::vec4
            //-- glm::mat4

//-------------------- C --------------------//
#include <cassert> //- tmp

//-------------------- CPP --------------------//
#include <vector>
#include <set>

//-------------------- Engine --------------------//
#include "IntVec.h" 
#include "MapEnt.h"
#include "config.h" 
#include "Mesh.h"
#include "MapTexture.h" 
#include "chunkKey.h"
#include "MapCoord.h" 
#include "sectionKey.h"


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
    
    //-- 每1渲染帧，都要根据 camera，重设 mesh.translate
    void refresh_translate_auto();


    inline void insert_2_goIds( const goid_t &_id ){
        this->goIds.insert(_id);
    }
    inline size_t erase_from_goIds( const goid_t &_id ){
        return this->goIds.erase(_id);
    }
    inline void insert_2_edgeGoIds( const goid_t &_id ){
        this->edgeGoIds.insert(_id);
    }
    inline size_t erase_from_edgeGoIds( const goid_t &_id ){
        return this->edgeGoIds.erase(_id);
    }

    //------- set -------//
    //-- 参数 _mpos 是任意 mapent 的 mpos值。
    inline void set_by_anyMPos( const IntVec2 &_anyMPos ){
        this->chunkKey = anyMPos_2_chunkKey( _anyMPos );
        mcpos.set_by_mpos( chunkKey_2_mpos( this->chunkKey ) );             
    }
    inline void set_mesh_shader_program( ShaderProgram *_sp ){
        this->mesh.set_shader_program( _sp );
    }
    inline void set_mesh_isVisible( bool _isVisible ){
        this->mesh.isVisible = _isVisible;
    }

    //------- get -------//
    inline const glm::vec2 get_fpos() const {
        return mcpos.get_fpos();
    }
    inline const IntVec2 &get_mpos() const {
        return this->mcpos.get_mpos();
    }
    inline const MapCoord &get_mcpos() const {
        return this->mcpos;
    }
    inline const chunkKey_t &get_key() const {
        return this->chunkKey;
    }
    inline const Mesh &get_mesh() const {
        return this->mesh;
    }
    const std::vector<fieldKey_t> &get_fieldKeys() const {
        return this->fieldKeys;
    }
    //-- 确保 参数为 基于chunk左下ent 的 相对mpos
    inline MemMapEnt *getnc_mapEntPtr_by_lMPosOff( const IntVec2 &_lMPosOff ){
        int idx = _lMPosOff.y*ENTS_PER_CHUNK + _lMPosOff.x;
            assert( (idx>=0) && (idx<memMapEnts.size()) ); //- tmp
        return &(memMapEnts.at(idx));
    }
    
    //======== flags ========//
    bool     is_memMapEnts_set              {false};

private:
    void init_memMapEnts();
    size_t get_mapEntIdx_in_chunk( const IntVec2 &_anyMPos );
    size_t get_pixIdx_in_chunk( const IntVec2 &_anyPPos );

    //======== vals ========//
    //------- chunk 自己的 图形 ---
    MapTexture  mapTex {};
    Mesh        mesh   {}; 

    chunkKey_t  chunkKey {};
    MapCoord    mcpos    {}; //- [left-bottom]

    std::vector<MemMapEnt> memMapEnts {}; 

    std::set<goid_t>  goIds {}; //- 动态存储 本chunk 拥有的所有 go id。
                                //  部分元素会与 edgeGoIds 重合
                                //  添加和 释放工作要做干净。

    std::set<goid_t>  edgeGoIds {}; //- "临界go"／“边缘go” 容器
                                    // 有些 go 位于 chunk边缘，其 collients 可能在隔壁 chunk
                                    // 这种go 在 创建／销毁 阶段往往很麻烦
                                    // 使用一个容器来 动态保管它们。

    std::vector<fieldKey_t> fieldKeys {}; //- 8*8 fieldKeys,仅用来辅助 chunk创建
                                          

    float  zOff {}; //- chunk间存在前后层次，
};





//--- 临时放这里 ---





#endif


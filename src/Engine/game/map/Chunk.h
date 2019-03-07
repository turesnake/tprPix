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
#include <glm/gtc/matrix_transform.hpp>
            //-- glm::translate
            //-- glm::rotate
            //-- glm::scale
            //-- glm::perspective
#include <glm/gtc/type_ptr.hpp> 
            //-- glm::value_ptr

//-------------------- C --------------------//
#include <cassert> //- tmp

//-------------------- CPP --------------------//
#include <vector>

//-------------------- Engine --------------------//
#include "IntVec.h" 
#include "GameObj.h" 
#include "MapEnt.h"
#include "config.h" 
#include "Mesh.h"
#include "MapTexture.h" 
#include "chunkKey.h"
#include "MapCoord.h" 
#include "ChunkFieldSet.h"
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

    void init_memMapEnts();

    void assign_mapEnts_2_field();
   
    //-- 参数 _mpos 是任意 mapent 的 mpos值。
    inline void set_by_anyMPos( const IntVec2 &_anyMPos ){
        this->chunkKey = anyMPos_2_chunkKey( _anyMPos );
        mcpos.set_by_mpos( chunkKey_2_mpos( this->chunkKey ) );             
    }

    //--- get ---
    inline const glm::vec2 get_fpos() const {
        return mcpos.get_fpos(); //- return a tmp val
    }
    inline const IntVec2& get_mpos() const {
        return mcpos.get_mpos();
    }
    inline const MapCoord& get_mcpos() const {
        return mcpos;
    }

    inline const chunkKey_t get_key() const {
        return chunkKey;
    }

    //-- 每1渲染帧，都要根据 camera，从设 mesh.translate
    void refresh_translate_auto();

    
    //-- 确保 参数为 基于chunk左下ent 的 相对mpos
    inline MemMapEnt* get_memMapEnt_by_lMPosOff( const IntVec2 &_lMPosOff ){
        int idx = _lMPosOff.y*ENTS_PER_CHUNK + _lMPosOff.x;
            assert( (idx>=0) && (idx<memMapEnts.size()) ); //- tmp
        return (MemMapEnt*)&(memMapEnts.at(idx));
    }
    

    //======== vals ========//
    //------- chunk 自己的 图形 ---
    MapTexture  mapTex {};
    Mesh        mesh   {}; 

    chunkKey_t  chunkKey {};
    MapCoord    mcpos    {}; //- [left-bottom]

    IntVec2     nodePPos {}; //- 距离场点 ppos (320*320pix 中的一个点) （均匀距离场）
                            //- 绝对 ppos 坐标。
                            //  （此值在 section中已经提前生成好了，只需要搬运到此处）


    ChunkFieldSet  *fieldSetPtr  {nullptr}; 
                        //- fieldSet 数据 往往先于 chunk 数据被创建。

    sectionKey_t  ecoSysInMapKey {}; 


    std::vector<MemMapEnt> memMapEnts; 

    //======== flags ========//
    //bool   is_ecoSysInMap_set  {false};

private:

    size_t get_mapEntIdx_in_chunk( const IntVec2 &_anyMPos );

   
};





//--- 临时放这里 ---





#endif


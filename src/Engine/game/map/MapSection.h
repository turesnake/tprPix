/*
 * ====================== MapSection.h =======================
 *                          -- tpr --
 *                                        创建 -- 2018.12.09
 *                                        修改 -- 2018.12.09
 * ----------------------------------------------------------
 *    map section 一个地图区域。 左下坐标系
 * ----------------------------
 */
#ifndef _TPR_MAP_SECTION_H_
#define _TPR_MAP_SECTION_H_

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

//-------------------- CPP --------------------//
#include <vector>

//-------------------- Engine --------------------//
#include "IntVec.h" 
#include "GameObj.h" 
#include "MapEnt.h"
#include "config.h" 
#include "Mesh.h"
#include "MapTexture.h" 
#include "SectionKey.h"
#include "MapCoord.h" 

 
//-- 256*256 个 Fst_diskMapEnt 元素.[硬盘态] --
//-- 下面这段，暂时没想起来 它是用来做什么的 ... ---

struct Fst_diskMapSection{
    Fst_diskMapEnt data[ SECTION_W_ENTS * SECTION_H_ENTS ]; //- 512KB
};



//-- 256*256 个 mapEnt, 组成一张 section  [mem] --
//  section 作为一个整体被存储到硬盘，就像 mc 中的 chunk
class MapSection{
public:
    MapSection() = default;

    void init();


    //----------- pos / key ------------
    inline void set_by_mpos( const IntVec2 &_mpos ){
        pos.set_by_mpos( _mpos );
        sectionKey.init_by_mpos( pos.get_mpos() );
    }

    inline void set_by_ppos( const glm::vec2 &_ppos ){
        pos.set_by_ppos( (int)_ppos.x, (int)_ppos.y ); //- ?
        sectionKey.init_by_mpos( pos.get_mpos() );
    }

    inline void set_by_ppos( const IntVec2 &_ppos ){
        pos.set_by_ppos( _ppos ); 
        sectionKey.init_by_mpos( pos.get_mpos() );
    }

    inline const glm::vec2 get_fpos() const {
        return pos.get_fpos(); //- return a tmp val
    }
    inline const IntVec2& get_mpos() const {
        return pos.get_mpos();
    }

    inline const u64 get_key() const {
        return sectionKey.get_key();
    }

    //-- 每1渲染帧，都要根据 camera，从设 mesh.translate
    void refresh_translate_auto();




    //------- section 自己的 图形 ---
    MapTexture  mapTex {};
    Mesh        mesh   {}; 


    std::vector<MemMapEnt> memMapEnts; 

private:
    //---------- pos & key ------------//
    //-- once init, never change.
    SectionKey  sectionKey {};
    //-- [left-bottom] --
    MapCoord      pos  {}; //- mpos/ppos  

};




#endif


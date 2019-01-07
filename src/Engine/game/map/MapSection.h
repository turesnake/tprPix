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
#include "PixVec.h" 
#include "GameObj.h" 
#include "MapEnt.h"
#include "config.h" 
#include "Mesh.h"
#include "MapTexture.h" 
#include "SectionKey.h"

 
//-- 256*256 个 Fst_diskMapEnt 元素.[硬盘态] --
struct Fst_diskMapSection{

    Fst_diskMapEnt data[ SECTION_W_ENTS * SECTION_H_ENTS ]; //- 512KB
};


//-- 256*256 个 mapEnt, 组成一张 section --
//  section 作为一个整体被存储到硬盘，就像 mc 中的 chunk
class MapSection{
public:
    MapSection() = default;

    void init();


    //----------- pos / key ------------
    inline void set_pos( const PixVec2 &_mepos ){
        mapEntPos = _mepos;
        pixPos = glm::vec2{ (float)(_mepos.x*PIXES_PER_MAPENT), 
                            (float)(_mepos.y*PIXES_PER_MAPENT) };
        sectionKey.init_by_pos( mapEntPos );
    }
    inline void set_pos( const glm::vec2 &_ppos ){
        pixPos = _ppos;
        mapEntPos = PixVec2{ (int)(_ppos.x/PIXES_PER_MAPENT), 
                             (int)(_ppos.y/PIXES_PER_MAPENT) };
        sectionKey.init_by_pos( mapEntPos );
    }

    inline const glm::vec2& get_pixPos() const {
        return pixPos;
    }
    inline const PixVec2& get_mapEntPos() const {
        return mapEntPos;
    }

    inline const u64 get_key() const {
        return sectionKey.get_key();
    }


    //-----------------
    MapTexture  mapTex {};
    Mesh        mesh   {}; 


    std::vector<MemMapEnt> memMapEnts; 

private:

    //---------- pos & key ------------//
    //-- once init, never change.
    SectionKey  sectionKey {};
    //-- [left-bottom] --
    glm::vec2   pixPos     {0.0f, 0.0f}; //- based on game pixel
    PixVec2     mapEntPos  {0, 0};       //- based on mapEnt


};




#endif


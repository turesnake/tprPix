/*
 * ====================== Section.h =======================
 *                          -- tpr --
 *                                        CREATE -- 2018.12.09
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *    section 一个地图区域。 左下坐标系
 * ----------------------------
 */
#ifndef _TPR_SECTION_H_
#define _TPR_SECTION_H_

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
#include "SectionKey.h"
#include "MapCoord.h" 
#include "SectionFieldSet.h"

 
//-- 256*256 个 Fst_diskMapEnt 元素.[硬盘态] --
//-- 下面这段，暂时没想起来 它是用来做什么的 ... ---
struct Fst_diskMapSection{
    Fst_diskMapEnt data[ SECTION_SIDE_ENTS * SECTION_SIDE_ENTS ]; //- 512KB
};



//-- 256*256 个 mapEnt, 组成一张 section  [mem] --
//  section 作为一个整体被存储到硬盘，就像 mc 中的 Field
class Section{
public:
    Section() = default;

    void init();

    //----------- mcpos / key ------------    
    //-- 参数 _mpos 是任意 mapent 的 mpos值。
    inline void set_by_mapEnt_mpos( const IntVec2 &_mpos ){
        //-- “地板除法，向低取节点值”, 再乘回 节点间距。
        //   获得 所在section 左下ent mpos
        IntVec2 mpos = get_section_mpos( _mpos );
        mcpos.set_by_mpos( mpos );
        sectionKey.init_by_mapEnt_mpos( mcpos.get_mpos() ); 
                        //-- 这里的计算重复了，但问题不大。
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

    inline const u64_t get_key() const {
        return sectionKey.get_key();
    }

    inline const std::vector<u64_t> &get_near_9_sectionKeys() const {
        return sectionKey.get_near_9_sectionKeys();
    }

    //-- 每1渲染帧，都要根据 camera，从设 mesh.translate
    void refresh_translate_auto();

    
    //-- 确保 参数为 基于section左下ent 的 相对mpos
    inline MemMapEnt* get_memMapEnt_by_lMPosOff( const IntVec2 &_lMPosOff ){
        int idx = _lMPosOff.y*SECTION_SIDE_ENTS + _lMPosOff.x;
            assert( (idx>=0) && (idx<memMapEnts.size()) ); //- tmp
        return (MemMapEnt*)&(memMapEnts.at(idx));
    }
    

    //======== vals ========//
    //------- section 自己的 图形 ---
    MapTexture  mapTex {};
    Mesh        mesh   {}; 

    //-- 也许要放到 private 中
    std::vector<MemMapEnt> memMapEnts; 

    SectionFieldSet  *fieldSetPtr  {nullptr}; 
                        //- fieldSet 数据 往往先于 section 数据被创建。

    //======== static vals ========//
    // 仅仅便于 快速访问
    static  int entSideLen; //- 256*256 mapEnts  (只记录单边)
    static  int pixSideLen; //- 1280*1280 pixels (只记录单边)
 
private:

    //======== vals ========//
    //-- once init, never change.
    SectionKey    sectionKey {};
    //-- [left-bottom] --
    MapCoord      mcpos  {}; //- mpos/ppos 

};



//--- 临时放这里 ---
void build_first_section( const IntVec2 &_mpos );
void build_nearby_sections( const IntVec2 &_mpos );
void check_and_build_nearby_sections();




#endif


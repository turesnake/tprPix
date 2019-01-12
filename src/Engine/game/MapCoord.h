/*
 * ====================== MapCoord.h =======================
 *                          -- tpr --
 *                                        创建 -- 2019.01.09
 *                                        修改 -- 2019.01.09
 * ----------------------------------------------------------
 *   mapEntPos / pixPos  2-axis
 * ----------------------------
 */
#ifndef _TPR_MAP_COORD_H_
#define _TPR_MAP_COORD_H_

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
#include <cassert>

//-------------------- Engine --------------------//
#include "PixVec.h" 
#include "config.h" 


//-- [left-bottom] --
class MapCoord{
public:

    //--- set ----
    inline void set_by_mpos( const PixVec2 &_mpos ){
        mpos = _mpos;
        ppos.x = mpos.x * PIXES_PER_MAPENT;
        ppos.y = mpos.y * PIXES_PER_MAPENT;
    }

    inline void set_by_mpos( int _x, int _y ){
        mpos.x = _x;
        mpos.y = _y;
        ppos.x = mpos.x * PIXES_PER_MAPENT;
        ppos.y = mpos.y * PIXES_PER_MAPENT;
    }


    inline void set_by_ppos( const PixVec2 &_ppos ){
        assert( (_ppos.x%PIXES_PER_MAPENT==0) && (_ppos.y%PIXES_PER_MAPENT==0) );
        ppos = _ppos;
        mpos.x = ppos.x/PIXES_PER_MAPENT;
        mpos.y = ppos.y/PIXES_PER_MAPENT;
    }

    inline void set_by_ppos( int _x, int _y ){
        assert( (_x%PIXES_PER_MAPENT==0) && (_y%PIXES_PER_MAPENT==0) );
        ppos.x = _x;
        ppos.y = _y;
        mpos.x = ppos.x/PIXES_PER_MAPENT;
        mpos.y = ppos.y/PIXES_PER_MAPENT;
    }

    //--- get ---
    inline const PixVec2& get_mpos() const {
        return mpos;
    }

    inline const PixVec2& get_ppos() const{
        return ppos;
    }

    inline glm::vec2 get_ppos_glm() const {
        return glm::vec2( (float)ppos.x, (float)ppos.y );
    }

    //--- add ---
    //inline 

private:
    PixVec2   mpos  {0, 0}; //- based on mapEnt
    PixVec2   ppos  {0, 0}; //- based on pixel
};


#endif 


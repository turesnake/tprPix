/*
 * ====================== MapTexture.h =======================
 *                          -- tpr --
 *                                        CREATE -- 2019.01.06
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *    texture for map sys
 * ----------------------------
 */
#ifndef _TPR_MAP_TEXTURE_H_
#define _TPR_MAP_TEXTURE_H_
//=== *** glad FIRST, glfw SECEND *** ===
#include <glad/glad.h> 

//-------------------- C --------------------//
#include <cassert>

//-------------------- CPP --------------------//
#include <vector>
#include <functional>

//------------------- Libs --------------------//
#include "tprDataType.h" 


//------------------- Engine --------------------//
#include "IntVec.h"
#include "config.h" 
#include "RGBA.h"



//-1- bind MapTexture::mapBuilder
//-2- create MapTexture instance: mapTex
//-3- call mapTex.init();
class MapTexture{
public:

    void creat_texName();

    inline GLuint get_texName() const {
        return texName;
    }

    inline RGBA *get_texBufHeadPtr() {
        assert( this->is_resize_done );
        return (RGBA*)&(texBuf.at(0));
    }

    inline void resize_texBuf(){
        if( this->is_resize_done ){
            return;
        }
        texBuf.resize( PIXES_PER_CHUNK * PIXES_PER_CHUNK );
        this->is_resize_done = true;
    }

private:
    GLuint              texName {0}; //- texture textel_name
    std::vector<RGBA>   texBuf;      //- 本模块的重心，一张合成的 texture
                                    // 在生成 texName 之后，此数据可以被释放 
                                    // 若不选择释放，则存储在 本实例中，直到实例被销毁

    //===== flags =====//
    bool  is_resize_done {false};

};


#endif 


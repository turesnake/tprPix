/*
 * ====================== MapTexture.h =======================
 *                          -- tpr --
 *                                        CREATE -- 2019.01.06
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *    texture for map sys
 * ----------------------------
 */
#ifndef TPR_MAP_TEXTURE_H
#define TPR_MAP_TEXTURE_H
//=== *** glad FIRST, glfw SECEND *** ===
// Don't include glfw3.h ALONE!!!
#include <glad/glad.h> 

//-------------------- CPP --------------------//
#include <vector>
#include <functional>


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

    inline void delete_texture()noexcept{
        glDeleteTextures( 1, &texName );
    }

    inline GLuint get_texName()const noexcept{ return texName; }

    //- texBuf 数据 是在 job线程中生成的，传递到 主线程后，只需 swap 到此处 --
    inline void swap_texBuf_from( std::vector<RGBA> &src_ )noexcept{
        this->texBuf.swap(src_);
    }
    
private:
    GLuint              texName {0}; //- texture textel_name
    std::vector<RGBA>   texBuf {};      //- 本模块的重心，一张合成的 texture
                                    // 在生成 texName 之后，此数据可以被释放 
                                    // 若不选择释放，则存储在 本实例中，直到实例被销毁
};


#endif 


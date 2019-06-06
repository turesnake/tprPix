/*
 * ====================== MapTexture.h =======================
 *                          -- tpr --
 *                                        CREATE -- 2019.01.06
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *    texture for map sys
 * ----------------------------
 */
#ifndef TPR_MAP_TEXTURE_H_
#define TPR_MAP_TEXTURE_H_
//=== *** glad FIRST, glfw SECEND *** ===
#include <glad/glad.h> 

//-------------------- C --------------------//
//#include <cassert>

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

    inline const GLuint &get_texName() const {
        return texName;
    }

    //- texBuf 数据 是在 job线程中生成的，传递到 主线程后，只需 复制到此处 --
    inline void copy_texBuf_from( const std::vector<RGBA> &_src ){
        this->texBuf.clear();
        this->texBuf.insert( this->texBuf.end(), _src.begin(), _src.end() ); //- copy
    }
    
private:
    GLuint              texName {0}; //- texture textel_name
    std::vector<RGBA>   texBuf;      //- 本模块的重心，一张合成的 texture
                                    // 在生成 texName 之后，此数据可以被释放 
                                    // 若不选择释放，则存储在 本实例中，直到实例被销毁
};


#endif 


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
    //using  F_MAP_BUILDER = std::function<void(RGBA*, int, int)>;
public:

    //void init();

    void creat_texName();

    inline void set_pixSize( IntVec2 _newSize ){
        pixSize = _newSize;
    }

    inline GLuint get_texName() const {
        return texName;
    }

    inline RGBA *get_texBufHeadPtr() {
        return (RGBA*)&(texBuf.at(0));
    }

    inline void resize_texBuf(){
        texBuf.resize( pixSize.x * pixSize.y );
    }

    //===== static =====//
    static  IntVec2  pixSize; //- mapTex 长宽像素值

private:
    GLuint              texName {0}; //- texture textel_name
    std::vector<RGBA>   texBuf;      //- 本模块的重心，一张合成的 texture
                                    // 在生成 texName 之后，此数据可以被释放 
                                    // 若不选择释放，则存储在 本实例中，直到实例被销毁

};


#endif 


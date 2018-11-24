//
//========================= RectVertics.h ==========================
//                       -- OpenGL --
//                                        创建 -- 2018.11.21
//                                        修改 -- 2018.11.21
//----------------------------------------------------------
//    图元（一个简单的矩形）的  顶点数据 生成器
//
//----------------------------
#ifndef _TPR_RECTVERTICS_H_
#define _TPR_RECTVERTICS_H_

/* -- 确保 glad GLFW 两个库 的引用顺序 ---
 * --    glad.h 包含了正确的OpenGL头文件（如GL/gl.h），
 * --    所以需要在其它依赖于OpenGL的头文件之前 包含 glad.h
 */
#include <glad/glad.h>

//------------------- Libs --------------------//
#include "tprDataType.h" 

//-------------------- CPP --------------------//
#include <vector>



//---------------------//
// 图元 定点数据 模版.
// 左下坐标系
inline std::vector<float> rect_base {
    //      坐标            textuel 
     0.0f,  0.0f, 0.0f,  0.0f, 0.0f,
     1.0f,  0.0f, 0.0f,  1.0f, 0.0f,
     1.0f,  1.0f, 0.0f,  1.0f, 1.0f,
     1.0f,  1.0f, 0.0f,  1.0f, 1.0f,
     0.0f,  1.0f, 0.0f,  0.0f, 1.0f,
     0.0f,  0.0f, 0.0f,  0.0f, 0.0f
};


class RectVertics{

public:
    explicit RectVertics( u32 _w_pix, u32 _h_pix )
        :
        w_pix(_w_pix),
        h_pix(_h_pix)
        {
            //-- 一步到位地制作好 顶点数据 --//
            data = rect_base;
            for( auto i=0; i<data.size(); i++ ){
                if( (i%5) == 0 ){
                    data[i] *= w_pix;
                }else if( (i%5) == 1 ){
                    data[i] *= h_pix;
                }
            }
        }

    //- 获得 顶点数据 指针 --
    inline GLvoid *get_data() const {
        return (GLvoid*)&(data[0]);
    }
    //- 获得 顶点数据 字节总数 --
    inline GLsizeiptr get_size() const {
        return (GLsizeiptr)(sizeof(float) * data.size());
    }
    //- 获得 顶点数据 步长 --
    inline GLsizei get_stride() const{
        return (GLsizei)( sizeof(float) * 5 );
    }

private:
    u32  w_pix; //- 目标图元，像素宽度
    u32  h_pix; //- 目标图元，像素高度

    std::vector<float> data; //- 图元顶端数据存储区
};







#endif


/*
 * ========================= Action_SRC.cpp ==========================
 *                          -- tpr --
 *                                        创建 -- 2018.11.23
 *                                        修改 -- 2018.11.23
 * ----------------------------------------------------------
 * 
 * ----------------------------
 */
#include "Action_SRC.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h" //-- 加载图片数据用

//-------------------- C --------------------//
//#include <errno.h> //- errno
#include <cassert> //- assert

//-------------------- CPP --------------------//
#include <algorithm> //- find
#include <iostream>

//------------------- Libs --------------------//
#include "tprFileSys.h" 

//------------------- SELF --------------------//
#include "global.h"


using std::cout;
using std::endl;
using std::string;
using std::vector;


/* ===========================================================
 *                       init
 * -----------------------------------------------------------
 */
void Action_SRC::init(){
    //- 注释 以 lpath_pic = "/animal/dog_ack_01.P.png" 为例

    //-- tmp --
    string lst; //- tmp, 尾部字符串，不停地被截断

    //--------------------//
    //      生成 name
    //--------------------//
    auto lst_slash_idx = lpath_pic.rfind( '/' ); //- 指向最后一个 '/'
    assert( lst_slash_idx != string::npos ); 
    //- lst 等于 "dog_ack_01.P.png"
    lst.assign( (lpath_pic.begin()+lst_slash_idx+1), lpath_pic.end() );

    auto point_idx = lst.find( '.', 0 ); //- 指向第一个 '.'
    assert( point_idx != string::npos );
    name.assign( lst.begin(), (lst.begin()+point_idx) );

    //--------------------//
    //    生成 lpath_pjt
    //--------------------//
    point_idx = lpath_pic.find( '.', 0 ); //- 指向第一个 '.'
    //- lpath_pjt 暂时等于 "/animal/dog_ack_01"
    lpath_pjt.assign( lpath_pic.begin(), (lpath_pic.begin()+point_idx) );
    lpath_pjt += ".J.png";


    //----------------------------------------//
    //  load & divide png数据，存入每个 帧容器中
    //----------------------------------------//
    int frames = frames_w * frames_h; //- 总帧数

    //-- 帧数据容器组。这里的帧排序 符合 左上坐标系 --
    vector< vector<PngPix> > P_frame_data_ary {}; 
    vector< vector<PngPix> > J_frame_data_ary {}; 

    load_and_divide_png( true,  P_frame_data_ary );
    load_and_divide_png( false, J_frame_data_ary );

    //----------------------------//
    //      读取 pjt 投影信息
    //----------------------------//
    //...
    //..
    //.

    //---------------------------------//
    //  依次 制作每一动画帧 的 texture 实例
    //---------------------------------//
    texNames.resize( frames );
    //-- 申请 n个 tex实例，并获得其 names
    glGenTextures( frames, &texNames[0] );

    
    for( int i=0; i<frames; i++ ){

        glBindTexture( GL_TEXTURE_2D, texNames[i] );

        //-- 为 GL_TEXTURE_2D 设置环绕、过滤方式
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);  //-- 设置 S轴的 环绕方式
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);  //-- 设置 T轴的 环绕方式
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); // 缩小时 纹理过滤 的策略
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); // 放大时 纹理过滤 的策略
                                                        //-- GL_NEAREST 临近过滤，8-bit 专用
                                                        //-- GL_LINEAR  线性过滤，

        u8 *dptr = (u8*)&P_frame_data_ary[i][0];

        //-- 通过之前的 png图片数据，生成 一个 纹理。
        glTexImage2D( GL_TEXTURE_2D,       //-- 指定纹理目标／target，
                        0,                 //-- 多级渐远纹理的级别: 0: 基本级别
                        GL_RGBA,           //-- 希望把纹理储存为何种格式
                        pixes_per_frame_w, //-- 纹理的宽度
                        pixes_per_frame_h, //-- 纹理的高度
                        0,                 //-- 总是被设为0（历史遗留问题
                        GL_RGBA,           //-- 源图的 格式
                        GL_UNSIGNED_BYTE,  //-- 源图的 数据类型
                        dptr               //-- 图像数据
                        );
        //-- 8-bit 游戏不需要 多级渐远 
        //glGenerateMipmap(GL_TEXTURE_2D); //-- 生成多级渐远纹理
    }
}


/* ===========================================================
 *                load_and_divide_png
 * -----------------------------------------------------------
 * -- param: _is_pic
 * -- param: _frame_data_ary -- 将每一帧的图形数据，存入这组 帧容器中
 */
void Action_SRC::load_and_divide_png( bool _is_pic,
        std::vector< std::vector<PngPix>> &_frame_data_ary ){

    //----------------------------//
    //      合成 文件的 绝对路径名
    //----------------------------//
    string path;
    if( _is_pic == true ){
        path = tpr::path_combine( path_textures, lpath_pic );
    }else{
        path = tpr::path_combine( path_textures, lpath_pjt );
    }

    //------------------------------//
    //   加载 png图片，获得其 原始数据
    //------------------------------//
    int width;
    int height;
    int nrChannels;
    unsigned char *data; //-- 临时，图片数据的指针。
                        //- 真实数据存储在 stbi 模块自己创建的 内存中。
                        //- 我们只获得一个 调用指针。

    stbi_set_flip_vertically_on_load( 1 ); //-- 防止 图片 y轴颠倒。
    data = stbi_load( path.c_str(), &width, &height, &nrChannels, 0 );
    assert( data != nullptr );

    //------------------------------------//
    //   获得 每一帧的数据, 存入各自 帧容器中
    //------------------------------------// 
    //-- 填入 相应数量的 帧容器 --
    for( int i=0; i<(frames_w*frames_h); i++ ){
        vector<PngPix> v {};
        _frame_data_ary.push_back( v );
    }

    auto fit = _frame_data_ary.begin(); //- 指向某个 帧容器

    int wf; //-- 以帧为单位，目标像素在横排中 的序号
    int hf; //-- 以帧为单位，目标像素的 纵向 序号
    int nrf; //-- 像素 属于的 帧序号

    PngPix *pixhead = (PngPix*)data;
    PngPix *pixp; //- tmp

    //--- 遍历 png 中的 每一像素 ---
    for( int h=0; h<height; h++  ){
        for( int w=0; w<width; w++  ){

            //-- 计算 本像素 所属帧的容器 的迭代器 fit --
            wf = w/pixes_per_frame_w;
            hf = h/pixes_per_frame_h;
            hf = frames_h - hf; //- 关键步骤！修正帧排序，
                        //- 现在，帧排序从 左下 修正为 左上角坐标系
            nrf = hf*frames_w + wf;
            fit = _frame_data_ary.begin() + nrf;

            //-- 获得 本像素 的数据 --
            pixp = pixhead + (h*width + w);

            //-- 将数据 压入 对应的 帧容器 中 --
            fit->push_back( *pixp ); //-copy
        }
    }
    //-- png图片 原始数据已经没用了，释放掉 ---
    stbi_image_free( data );
}








/*
 * ========================= Action.cpp ==========================
 *                          -- tpr --
 *                                        创建 -- 2018.11.23
 *                                        修改 -- 2018.11.28
 * ----------------------------------------------------------
 * 
 * ----------------------------
 */
#include "Action.h"

/*
#ifndef STB_IMAGE_IMPLEMENTATION
    #define STB_IMAGE_IMPLEMENTATION
#endif
*/
#include "stb_image.h" //-- 加载图片数据用

//-------------------- C --------------------//
#include <cassert> //- assert

//-------------------- CPP --------------------//
#include <algorithm> //- find
#include <iostream>
//#include <iterator> //- distance

//------------------- Libs --------------------//
#include "tprFileSys.h" 

//------------------- SELF --------------------//
#include "global.h"


using std::cout;
using std::endl;
using std::string;
using std::vector;

namespace{//----------------- namespace ------------------//

    RGBA color_pjtMask    { 0,0,0,255 };   //- 投影单位 颜色 
    RGBA color_rootAnchor { 255,0,0,255 }; //- 根锚点 颜色

}//-------------------- namespace: end ------------------//


/* ===========================================================
 *                       init
 * -----------------------------------------------------------
 */
void Action::init(){
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
    int frames_total = frames.x * frames.y; //- 总帧数

    //-- 帧数据容器组。这里的帧排序 符合 左上坐标系 --
    vector< vector<RGBA> > P_frame_data_ary {}; 
    vector< vector<RGBA> > J_frame_data_ary {}; 

    load_and_divide_png( true,  P_frame_data_ary );
    load_and_divide_png( false, J_frame_data_ary );

    //----------------------------//
    //      读取 pjt 投影信息
    //----------------------------//
    int pixNums = pixes_per_frame.x * pixes_per_frame.y; //- 一帧有几个像素点

    for( int j=0; j<frames_total; j++ ){

        std::vector<PixVec2> pjtmask;
        PixVec2 pix; //- tmp 

        for( int i=0; i<pixNums; i++ ){

            if( is_equal( J_frame_data_ary[j][i], color_pjtMask ) ){

                pix.y = i/pixes_per_frame.x;
                pix.x = i%pixes_per_frame.x;
                pjtmask.push_back(pix);
            }            
            if( is_equal( J_frame_data_ary[j][i], color_rootAnchor ) ){
                
                pix.y = i/pixes_per_frame.x;
                pix.x = i%pixes_per_frame.x;
                anchors_root.push_back(pix);
            }
        }

        pjtMasks.push_back( pjtmask ); //- copy, 低效...
    }
    //--- 基础查错 -----
    assert( anchors_root.size() == frames_total );
    assert( pjtMasks.size() == frames_total );


    //---------------------------------//
    //  依次 制作每一动画帧 的 texture 实例
    //---------------------------------//
    texNames.resize( frames_total );
    //-- 申请 n个 tex实例，并获得其 names
    glGenTextures( frames_total, &texNames[0] );

    for( int i=0; i<frames_total; i++ ){

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
                        pixes_per_frame.x, //-- 纹理的宽度
                        pixes_per_frame.y, //-- 纹理的高度
                        0,                 //-- 总是被设为0（历史遗留问题
                        GL_RGBA,           //-- 源图的 格式
                        GL_UNSIGNED_BYTE,  //-- 源图的 数据类型
                        dptr               //-- 图像数据
                        );
        //-- 本游戏不需要 多级渐远 
        //glGenerateMipmap(GL_TEXTURE_2D); //-- 生成多级渐远纹理
    }
}


/* ===========================================================
 *                load_and_divide_png
 * -----------------------------------------------------------
 * -- param: _is_pic
 * -- param: _frame_data_ary -- 将每一帧的图形数据，存入这组 帧容器中
 */
void Action::load_and_divide_png( bool _is_pic,
        std::vector< std::vector<RGBA>> &_frame_data_ary ){

    //----------------------------//
    //      合成 文件的 绝对路径名
    //----------------------------//
    string path;
    if( _is_pic == true ){
        path = tpr::path_combine( path_actions, lpath_pic );
    }else{
        path = tpr::path_combine( path_actions, lpath_pjt );
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
    int frames_total = frames.x * frames.y; //- 总帧数

    //-- 事先准备好 每一帧的容器 --
    _frame_data_ary.clear();
    for( int i=0; i<frames_total; i++ ){
        vector<RGBA> v {};
        _frame_data_ary.push_back( v );//- copy
    }

    auto fit = _frame_data_ary.begin(); //- 指向某个 帧容器

    int wf; //-- 以帧为单位，目标像素在横排中 的序号
    int hf; //-- 以帧为单位，目标像素的 纵向 序号 (左下坐标系)
    int antihf; //-- 以帧为单位，目标像素的 纵向 序号 (左上坐标系，我们要的)
    int nrf; //-- 像素 属于的 帧序号

    RGBA *pixhead = (RGBA*)data;
    RGBA *pixp; //- tmp

    //--- 遍历 png 中的 每一像素 ---
    for( int h=0; h<height; h++  ){
        for( int w=0; w<width; w++  ){

            //-- 计算 本像素 所属帧的容器 的迭代器 fit --
            wf = w/pixes_per_frame.x;
            hf = h/pixes_per_frame.y;
            hf = frames.y - 1 - hf; 
                        //- 关键步骤！修正帧排序，(注意必须先减1，可画图验证)
                        //- 现在，帧排序从 左下 修正为 左上角坐标系
            nrf = hf*frames.x + wf;
            fit = _frame_data_ary.begin() + nrf;
            assert( nrf < frames_total ); //- 避免迭代器越界

            //-- 获得 本像素 的数据 --
            pixp = pixhead + (h*width + w);

            //-- 将数据 压入 对应的 帧容器 中 --
            fit->push_back( *pixp ); //-copy
        }
    }
    //-- png图片 原始数据已经没用了，释放掉 ---
    stbi_image_free( data );
}



/* ===========================================================
 *                    debug
 * -----------------------------------------------------------
 * -- 向终端输出 本Action的信息，用来 debug
 */
void Action::debug(){

        cout<< "\nname = " << name
            << "\nlpath_pic = " << lpath_pic
            << "\nlpath_pjt = " << lpath_pjt
            << "\npixes_per_frame.x = " << pixes_per_frame.x
            << "\npixes_per_frame.y = " << pixes_per_frame.y
            << "\nframes.x = " << frames.x
            << "\nframes.y = " << frames.y
            << endl;

        cout << "anchors_root: " << endl;
        for( auto i : anchors_root ){
            cout << "  " << i.x
                << ",  " << i.y
                << endl;
        }

        cout << "texNames: " << endl;
        for( auto i : texNames ){
            cout << "  " << i << endl;
        }

        cout << "pjtMasks: " << endl;
        for( int j=0; j<pjtMasks.size(); j++ ){
            cout << "  " << j << " frame: " << endl;;

            for( int i=0; i<pjtMasks[j].size(); i++ ){

                PixVec2 pix = pjtMasks[j][i];
                cout << "    " << pix.x
                    << ", " << pix.y
                    << endl; 
            }
            cout << endl;
        }
}





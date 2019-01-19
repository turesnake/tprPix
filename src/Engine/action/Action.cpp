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
#include "stb_image.h" //-- 加载图片数据用
*/

//-------------------- C --------------------//
#include <cassert> //- assert

//-------------------- CPP --------------------//
#include <algorithm> //- find

//------------------- Libs --------------------//
#include "tprFileSys.h" 

//------------------- Engine --------------------//
#include "global.h"
#include "PjtRGBAHandle.h"

using std::string;
using std::vector;

#include "debug.h" //- tmp


extern void load_and_divide_png( const std::string &_path,
                                const PixVec2 &_pixes_per_frame,
                                const PixVec2 &_frameNum,
                                int            _totalFrameNum,
        std::vector< std::vector<RGBA>> &_frame_data_ary );


//namespace{//----------------- namespace ------------------//
//}//-------------------- namespace: end ------------------//


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
    //-- 图元帧 数据容器组。帧排序为 [left-top] --
    vector< vector<RGBA> > P_frame_data_ary {}; 
    vector< vector<RGBA> > J_frame_data_ary {}; 

    load_and_divide_png( tpr::path_combine( path_actions, lpath_pic ),
                        pixes_per_frame,
                        frameNum,
                        totalFrameNum,
                        P_frame_data_ary );

    load_and_divide_png( tpr::path_combine( path_actions, lpath_pjt ),
                        pixes_per_frame,
                        frameNum,
                        totalFrameNum,
                        J_frame_data_ary );

    //----------------------------//
    //      读取 pjt 投影信息
    //----------------------------//
    int pixNum = pixes_per_frame.x * pixes_per_frame.y; //- 一帧有几个像素点
    PjtRGBAHandle  jh {5};
    framePoses.resize( totalFrameNum );

    for( int f=0; f<totalFrameNum; f++ ){ //--- each frame ---

        PixVec2 pixPos; //- tmp. current pix pos

        for( int p=0; p<pixNum; p++ ){ //- each frame.pix [left-bottom]

            pixPos.set( p%pixes_per_frame.x,
                        p/pixes_per_frame.x );

            jh.set_rgba( J_frame_data_ary.at(f).at(p), pixPos );
            if( jh.is_emply() == true ){
                continue; //- next frame.pix
            }


            //--- 一张 action.frame 只有一个 rootAnchor ---
            if( jh.is_rootAnchor() == true ){
                framePoses.at(f).set_rootAnchorOff( pixPos );
            }

            //if( jh.is_childAnchor() == true ){
                //-- 暂时什么也不做...
            //}


            if( jh.is_rootColliEntHead() == true ){
                framePoses.at(f).pushBack_the_rootColliEntHead( jh.get_colliEntHead() );
            }
            if( jh.is_colliEntHead() == true ){
                framePoses.at(f).pushBack_new_colliEntHead( jh.get_colliEntHead() );
            }
        }//------ each frame.pix ------

        //-- 检测 root_ceh / regular_ceh 是否对齐 --//
        framePoses.at(f).check(); //-- MUST --//

    }//-------- each frame -------


    //---------------------------------//
    //  依次 制作每一动画帧 的 texture 实例
    //---------------------------------//
    texNames.resize( totalFrameNum );
    //-- 申请 n个 tex实例，并获得其 names
    glGenTextures( totalFrameNum, &texNames.at(0) );

    for( int i=0; i<totalFrameNum; i++ ){

        glBindTexture( GL_TEXTURE_2D, texNames.at(i) );

        //-- 为 GL_TEXTURE_2D 设置环绕、过滤方式
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);  //-- 设置 S轴的 环绕方式
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);  //-- 设置 T轴的 环绕方式
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); // 缩小时 纹理过滤 的策略
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); // 放大时 纹理过滤 的策略
                                                        //-- GL_NEAREST 临近过滤，8-bit 专用
                                                        //-- GL_LINEAR  线性过滤，

        u8 *dptr = (u8*)&P_frame_data_ary.at(i).at(0);

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
        //-- 本游戏不需要 多级渐远 --
        //glGenerateMipmap(GL_TEXTURE_2D); //-- 生成多级渐远纹理
    }
}


/* ===========================================================
 *                    debug
 * -----------------------------------------------------------
 * -- 向终端输出 本Action的信息，用来 debug
 */
/*
void Action::debug() const{

        cout<< "\nname = " << name
            << "\nlpath_pic = " << lpath_pic
            << "\nlpath_pjt = " << lpath_pjt
            << "\npixes_per_frame.x = " << pixes_per_frame.x
            << "\npixes_per_frame.y = " << pixes_per_frame.y
            << "\nframeNum.x = " << frameNum.x
            << "\nframeNum.y = " << frameNum.y
            << endl;

        cout << "rootAnchors: " << endl;
        for( auto i : rootAnchors ){
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
*/


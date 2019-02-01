/*
 * ================= AnimFrameSet.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2018.11.23
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#include "AnimFrameSet.h"

//-------------------- C --------------------//
#include <cassert> //- assert

//-------------------- CPP --------------------//
#include <algorithm> //- find

//------------------- Libs --------------------//
#include "tprFileSys.h" 

//------------------- Engine --------------------//
#include "global.h"
#include "Pjt_RGBAHandle.h"
#include "create_texNames.h"

#include "debug.h" //- tmp

extern IntVec2 load_and_divide_png( const std::string &_path,
                          const IntVec2 &_frameNum,
                          int            _totalFrameNum,
        std::vector< std::vector<RGBA>> &_frame_data_ary );


namespace{//----------------- namespace ------------------//

    //-- 图元帧 数据容器组。帧排序为 [left-top] --
    std::vector<std::vector<RGBA>> P_frame_data_ary {}; 
    std::vector<std::vector<RGBA>> J_frame_data_ary {}; 
    std::vector<std::vector<RGBA>> S_frame_data_ary {};

}//-------------------- namespace: end ------------------//


/* ===========================================================
 *                       init
 * -----------------------------------------------------------
 */
void AnimFrameSet::init(){
    //- 注释 以 lpath_pic = "/animal/dog_ack_01.P.png" 为例

    //-- tmp --
    std::string lst; //- tmp, 尾部字符串，不停地被截断

    //--------------------//
    //      生成 name
    //--------------------//
    auto lst_slash_idx = lpath_pic.rfind( '/' ); //- 指向最后一个 '/'
    assert( lst_slash_idx != std::string::npos ); 
    //- lst 等于 "dog_ack_01.P.png"
    lst.assign( (lpath_pic.begin()+lst_slash_idx+1), lpath_pic.end() );

    auto point_idx = lst.find( '.', 0 ); //- 指向第一个 '.'
    assert( point_idx != std::string::npos );
    name.assign( lst.begin(), (lst.begin()+point_idx) );

    //--------------------//
    //    生成 lpath_pjt
    //--------------------//
    point_idx = lpath_pic.find( '.', 0 ); //- 指向第一个 '.'
    //- lpath_pjt 暂时等于 "/animal/dog_ack_01"
    lpath_pjt.assign( lpath_pic.begin(), (lpath_pic.begin()+point_idx) );
    lpath_pjt += ".J.png";

    //--------------------//
    //    生成 lpath_shadow
    //--------------------//
    //- lpath_shadow 暂时等于 "/animal/dog_ack_01"
    lpath_shadow.assign( lpath_pic.begin(), (lpath_pic.begin()+point_idx) );
    lpath_shadow += ".S.png";


    //----------------------------------------//
    //  load & divide png数据，存入每个 帧容器中
    //----------------------------------------//
    P_frame_data_ary.clear();
    J_frame_data_ary.clear();
    S_frame_data_ary.clear();

    IntVec2 tmpv2; //-tmp
    pixNum_per_frame = load_and_divide_png( tpr::path_combine( path_animFrameSets, lpath_pic ),
                                            frameNum,
                                            totalFrameNum,
                                            P_frame_data_ary );

    tmpv2 = load_and_divide_png( tpr::path_combine( path_animFrameSets, lpath_pjt ),
                                frameNum,
                                totalFrameNum,
                                J_frame_data_ary );
    assert( tmpv2 == pixNum_per_frame );
    
    tmpv2 = load_and_divide_png( tpr::path_combine( path_animFrameSets, lpath_shadow ),
                                frameNum,
                                totalFrameNum,
                                S_frame_data_ary );
    assert( tmpv2 == pixNum_per_frame );

    //---------------------------------//
    handle_pjt();
    handle_shadow();
    
    //---------------------------------//
    //       create GL.texNames
    //---------------------------------//
    create_texNames( totalFrameNum,
                     pixNum_per_frame,
                     P_frame_data_ary,
                     texNames_pic );
    
    create_texNames( totalFrameNum,
                     pixNum_per_frame,
                     S_frame_data_ary,
                     texNames_shadow );
}


/* ===========================================================
 *                   handle_pjt
 * -----------------------------------------------------------
 */
void AnimFrameSet::handle_pjt(){

    int pixNum = pixNum_per_frame.x * pixNum_per_frame.y; //- 一帧有几个像素点
    Pjt_RGBAHandle  jh {5};
    framePoses.resize( totalFrameNum );

    IntVec2  pixPPos; //- tmp. current pix ppos
    IntVec2  rootAnchorOff;       //- tmp
    IntVec2  rootColliEntHeadOff; //- tmp

    for( int f=0; f<totalFrameNum; f++ ){ //--- each frame ---
        for( int p=0; p<pixNum; p++ ){ //--- each frame.pix [left-bottom]

            pixPPos.set( p%pixNum_per_frame.x,
                         p/pixNum_per_frame.x );

            jh.set_rgba( J_frame_data_ary.at(f).at(p), pixPPos );
            if( jh.is_emply() == true ){
                continue; //- next frame.pix
            }

            //--- 一张 animFrameSet.frame 只有一个 rootAnchor ---
            if( jh.is_rootAnchor() == true ){
                rootAnchorOff = pixPPos; //- 并不直接设置，而是先缓存起来。
            }

            //if( jh.is_childAnchor() == true ){
                //-- 暂时什么也不做...
            //}


            if( jh.is_rootColliEntHead() == true ){
                rootColliEntHeadOff = pixPPos;
                framePoses.at(f).pushBack_the_rootColliEntHead( jh.get_colliEntHead() );
            }
            if( jh.is_colliEntHead() == true ){
                framePoses.at(f).pushBack_new_colliEntHead( jh.get_colliEntHead() );
            }
        }//------ each frame.pix ------

        //-- 注意，顺序不能错!!! --//
        framePoses.at(f).set_rootAnchorPos( rootAnchorOff, rootColliEntHeadOff );
        framePoses.at(f).calc_ceh_pposOff_fromRootAnchor();
        framePoses.at(f).check();                 //-- MUST --//

    }//-------- each frame -------
}


/* ===========================================================
 *                   handle_shadow
 * -----------------------------------------------------------
 */
void AnimFrameSet::handle_shadow(){

    int pixNum = pixNum_per_frame.x * pixNum_per_frame.y; //- 一帧有几个像素点
    RGBA  color_shadow { 0,0,0, 255 };

    for( int f=0; f<totalFrameNum; f++ ){ //--- each frame ---
        for( int p=0; p<pixNum; p++ ){ //--- each frame.pix [left-bottom]

        RGBA &rgbaRef = S_frame_data_ary.at(f).at(p);
        if( rgbaRef.is_near(color_shadow, 5) == false ){
            continue; //- next frame.pix
        }
        
        //-- 将 shadow pix 改为需要的颜色 --//
        //  （一个临时的简陋的方案）
        rgbaRef.r = 0;
        rgbaRef.g = 5;
        rgbaRef.b = 10;
        rgbaRef.a = 70;
        }
    }
}


/* ===========================================================
 *                    debug
 * -----------------------------------------------------------
 * -- 向终端输出 本 AnimFrameSet 的信息，用来 debug
 */
/*
void AnimFrameSet::debug() const{

        cout<< "\nname = " << name
            << "\nlpath_pic = " << lpath_pic
            << "\nlpath_pjt = " << lpath_pjt
            << "\npixNum_per_frame.x = " << pixNum_per_frame.x
            << "\npixNum_per_frame.y = " << pixNum_per_frame.y
            << "\nframeNum.x = " << frameNum.x
            << "\nframeNum.y = " << frameNum.y
            << endl;

        cout << "rootAnchors: " << endl;
        for( auto i : rootAnchors ){
            cout << "  " << i.x
                << ",  " << i.y
                << endl;
        }

        cout << "texNames_pic: " << endl;
        for( auto i : texNames_pic ){
            cout << "  " << i << endl;
        }

        cout << "pjtMasks: " << endl;
        for( int j=0; j<pjtMasks.size(); j++ ){
            cout << "  " << j << " frame: " << endl;;

            for( int i=0; i<pjtMasks[j].size(); i++ ){

                IntVec2 pix = pjtMasks[j][i];
                cout << "    " << pix.x
                    << ", " << pix.y
                    << endl; 
            }
            cout << endl;
        }
}
*/


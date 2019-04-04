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
    auto lst_slash_idx = this->lpath_pic.rfind( '/' ); //- 指向最后一个 '/'
    assert( lst_slash_idx != std::string::npos ); 
    //- lst 等于 "dog_ack_01.P.png"
    lst.assign( (this->lpath_pic.begin()+lst_slash_idx+1), this->lpath_pic.end() );

    auto point_idx = lst.find( '.', 0 ); //- 指向第一个 '.'
    assert( point_idx != std::string::npos );
    this->name.assign( lst.begin(), (lst.begin()+point_idx) );

    //--------------------//
    //    生成 lpath_pjt
    //--------------------//
    point_idx = this->lpath_pic.find( '.', 0 ); //- 指向第一个 '.'
    //- lpath_pjt 暂时等于 "/animal/dog_ack_01"
    this->lpath_pjt.assign( this->lpath_pic.begin(), (this->lpath_pic.begin()+point_idx) );
    this->lpath_pjt += ".J.png";

    //--------------------//
    //    生成 lpath_shadow
    //--------------------//
    //- lpath_shadow 暂时等于 "/animal/dog_ack_01"
    this->lpath_shadow.assign( this->lpath_pic.begin(), (this->lpath_pic.begin()+point_idx) );
    this->lpath_shadow += ".S.png";


    //----------------------------------------//
    //  load & divide png数据，存入每个 帧容器中
    //----------------------------------------//
    P_frame_data_ary.clear();
    J_frame_data_ary.clear();
    S_frame_data_ary.clear();

    IntVec2 tmpv2; //-tmp
    this->pixNum_per_frame = load_and_divide_png( tpr::path_combine( path_animFrameSets, this->lpath_pic ),
                                            this->frameNum,
                                            this->totalFrameNum,
                                            P_frame_data_ary );

    tmpv2 = load_and_divide_png( tpr::path_combine( path_animFrameSets, this->lpath_pjt ),
                                this->frameNum,
                                this->totalFrameNum,
                                J_frame_data_ary );
    assert( tmpv2 == this->pixNum_per_frame );
    
    tmpv2 = load_and_divide_png( tpr::path_combine( path_animFrameSets, this->lpath_shadow ),
                                this->frameNum,
                                this->totalFrameNum,
                                S_frame_data_ary );
    assert( tmpv2 == this->pixNum_per_frame );

    //---------------------------------//
    handle_pjt();
    handle_shadow();
    
    //---------------------------------//
    //       create GL.texNames
    //---------------------------------//
    create_texNames( this->totalFrameNum,
                     this->pixNum_per_frame,
                     P_frame_data_ary,
                     this->texNames_pic );
    
    create_texNames( this->totalFrameNum,
                     this->pixNum_per_frame,
                     S_frame_data_ary,
                     this->texNames_shadow );
}


/* ===========================================================
 *                   handle_pjt
 * -----------------------------------------------------------
 */
void AnimFrameSet::handle_pjt(){

    int pixNum = this->pixNum_per_frame.x * 
                 this->pixNum_per_frame.y; //- 一帧有几个像素点
    Pjt_RGBAHandle  jh {5};
    this->framePoses.resize( this->totalFrameNum );

    IntVec2  pixPPos; //- tmp. current pix ppos
    IntVec2  rootAnchorOff;       //- tmp
    IntVec2  rootColliEntHeadOff; //- tmp

    for( int f=0; f<this->totalFrameNum; f++ ){ //--- each frame ---
        for( int p=0; p<pixNum; p++ ){ //--- each frame.pix [left-bottom]

            pixPPos.set( p%this->pixNum_per_frame.x,
                         p/this->pixNum_per_frame.x );

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
                this->framePoses.at(f).pushBack_the_rootColliEntHead( jh.get_colliEntHead() );
            }
            if( jh.is_colliEntHead() == true ){
                this->framePoses.at(f).pushBack_new_colliEntHead( jh.get_colliEntHead() );
            }
        }//------ each frame.pix ------

        //-- 注意，顺序不能错!!! --//
        this->framePoses.at(f).set_rootAnchorPos( rootAnchorOff, rootColliEntHeadOff );
        this->framePoses.at(f).calc_ceh_pposOff_fromRootAnchor();
        this->framePoses.at(f).check();                 //-- MUST --//

    }//-------- each frame -------
}


/* ===========================================================
 *                   handle_shadow
 * -----------------------------------------------------------
 */
void AnimFrameSet::handle_shadow(){

    int pixNum = this->pixNum_per_frame.x * 
                 this->pixNum_per_frame.y; //- 一帧有几个像素点
    RGBA  color_shadow { 0,0,0, 255 };

    for( int f=0; f<this->totalFrameNum; f++ ){ //--- each frame ---
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


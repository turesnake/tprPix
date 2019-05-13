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
#include "load_and_divide_png.h"

//#include "debug.h" //- tmp


namespace{//----------------- namespace ------------------//

    //-- 每次 insert png数据时，以下这些数据都会被复用 --
    size_t headIdx; //- 本次 png数据insert，起始idx
                    //- 在此之前，容器中以及有数据了。

    //-- 图元帧 数据容器组。帧排序为 [left-top] --
    std::vector<std::vector<RGBA>> P_frame_data_ary {}; 
    std::vector<std::vector<RGBA>> J_frame_data_ary {}; 
    std::vector<std::vector<RGBA>> S_frame_data_ary {};

    //- 画面 贴图集的 相对路径名。一个动作的所有帧图片，存储为一张 png图。
    //- 这个 路径名 只在游戏启动阶段被使用，之后 预存于此
    //- local path，based on path_animFrameSet_srcs
    std::string  lpath_pic;    //-- picture
    std::string  lpath_pjt;    //-- collients
    std::string  lpath_shadow; //-- shadow  (此文件也许可为空...)

    IntVec2  pixNum_per_frame {};  //- 单帧画面 的 长宽 像素值 （会被存到 animAction 实例中）
    IntVec2  frameNum {};          //- 画面中，横排可分为几帧，纵向可分为几帧
    int      totalFrameNum {};     //- 目标png文件中，总 图元帧 个数

    bool  isPJTSingleFrame    {};  //- pjt 每帧数据都是一样的，png也只记录了一帧
    bool  isShadowSingleFrame {};  //- shadow 每帧数据都是一样的，png也只记录了一帧

    std::vector<GLuint> tmpTexNames; //- 用于 create_texNames()

    //===== funcs =====//
    void build_three_lpaths( const std::string &_lpath_pic );


}//-------------------- namespace: end ------------------//


/* ===========================================================
 *                  insert_a_png
 * -----------------------------------------------------------
 */
void AnimFrameSet::insert_a_png(  const std::string &_lpath_pic, 
                            IntVec2             _frameNum,
                            int                 _totalFrameNum,
                            bool                _isHaveShadow, //-- 将被记录到 animAction 数据中去
                            bool                _isPjtSingleFrame,
                            bool                _isShadowSingleFrame,
                            const std::vector<AnimActionParam> &_animActionParams
                            ){

    build_three_lpaths( _lpath_pic );
    frameNum = _frameNum;
    totalFrameNum = _totalFrameNum;
    isPJTSingleFrame = _isPjtSingleFrame;
    isShadowSingleFrame = _isShadowSingleFrame;
    
    //-------------------//
    //
    //-------------------//
    //-- 获得本次 insert 的 起始idx
    size_t lastNums = this->framePoses.size();
    assert( this->texNames_pic.size() == lastNums );
    assert( this->texNames_shadow.size() == lastNums );
    headIdx = lastNums;

    //----------------------------------------//
    //  load & divide png数据，存入每个 帧容器中
    //----------------------------------------//
    P_frame_data_ary.clear();
    J_frame_data_ary.clear();
    S_frame_data_ary.clear();

    IntVec2 tmpv2; //-tmp

    //-------------------//
    //       Pic
    //-------------------//
    //-- 目前暂不支持 没有 pic数据的 AnimFrameSet 实例 ---
    pixNum_per_frame = load_and_divide_png( tpr::path_combine( path_animFrameSets, lpath_pic ),
                                            frameNum,
                                            totalFrameNum,
                                            P_frame_data_ary );

    tmpTexNames.clear();
    create_texNames( totalFrameNum,
                     pixNum_per_frame,
                     P_frame_data_ary,
                     tmpTexNames );
    this->texNames_pic.insert( this->texNames_pic.end(), tmpTexNames.begin(), tmpTexNames.end() ); //- copy

    //-------------------//
    //       Pjt
    //-------------------//
    if( isPJTSingleFrame ){
        //--- J.png 只有一帧 --//
        tmpv2 = load_and_divide_png( tpr::path_combine( path_animFrameSets, lpath_pjt ),
                                    IntVec2{1,1},
                                    1,
                                    J_frame_data_ary );
    }else{
        tmpv2 = load_and_divide_png( tpr::path_combine( path_animFrameSets, lpath_pjt ),
                                    frameNum,
                                    totalFrameNum,
                                    J_frame_data_ary );
    }
    assert( tmpv2 == pixNum_per_frame );
    this->handle_pjt();

    //-------------------//
    //      Shadow
    //-------------------//
    if( _isHaveShadow ){
        if( isShadowSingleFrame ){
            //--- S.png 只有一帧 --//
            tmpv2 = load_and_divide_png( tpr::path_combine( path_animFrameSets, lpath_shadow ),
                                        IntVec2{1,1},
                                        1,
                                        S_frame_data_ary );
        }else{
            tmpv2 = load_and_divide_png( tpr::path_combine( path_animFrameSets, lpath_shadow ),
                                        frameNum,
                                        totalFrameNum,
                                        S_frame_data_ary );
        }

        assert( tmpv2 == pixNum_per_frame );
        this->handle_shadow();
        tmpTexNames.clear();
        create_texNames( totalFrameNum,
                     pixNum_per_frame,
                     S_frame_data_ary,
                     tmpTexNames );
        this->texNames_shadow.insert( this->texNames_shadow.end(), tmpTexNames.begin(), tmpTexNames.end() );
        
    }else{
        //-- 没有 shadow数据时，填写 0 来占位 --
        this->texNames_shadow.insert( this->texNames_shadow.end(), totalFrameNum, 0 );
    }


    //-------------------//
    //    animActions
    //-------------------//
    for( const auto &param : _animActionParams ){

        this->animActions.insert({ param.actionName, AnimAction{} }); 
        AnimAction &actionRef = this->animActions.at(param.actionName);

        actionRef.init( const_cast<const AnimFrameSet*>(this),
                        param,
                        pixNum_per_frame,
                        headIdx,
                        _isHaveShadow );
    }
}



/* ===========================================================
 *                   handle_pjt
 * -----------------------------------------------------------
 */
void AnimFrameSet::handle_pjt(){

    int pixNum = pixNum_per_frame.x * 
                 pixNum_per_frame.y; //- 一帧有几个像素点
    Pjt_RGBAHandle  jh {5};

    this->framePoses.insert( this->framePoses.end(), totalFrameNum, FramePos{} );

    IntVec2  pixPPos; //- tmp. current pix ppos
    IntVec2  rootAnchorOff;       //- tmp
    IntVec2  rootColliEntHeadOff; //- tmp
    size_t   idx_for_J_frame_data_ary;
    size_t   idx_framePoses;

    for( size_t f=0; f<totalFrameNum; f++ ){ //--- each frame ---

        idx_framePoses = headIdx + f;
        assert( this->framePoses.size() > idx_framePoses );

        for( int p=0; p<pixNum; p++ ){ //--- each frame.pix [left-bottom]

            pixPPos.set( p%pixNum_per_frame.x,
                         p/pixNum_per_frame.x );

            //-- 通过此装置，实现了 “相同的 pjt数据 只用记录一帧” --
            //   并不是最优解，会做很多次重复运算，但是是对原有代码改动最少的。
            isPJTSingleFrame ?
                    idx_for_J_frame_data_ary=0 :
                    idx_for_J_frame_data_ary=f;

            jh.set_rgba( J_frame_data_ary.at(idx_for_J_frame_data_ary).at(p), pixPPos );
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

            //- 新版中，一个 mesh 只有一个 ces／ceh
            if( jh.is_rootColliEntHead() == true ){
                rootColliEntHeadOff = pixPPos;
                this->framePoses.at(idx_framePoses).set_colliEntHead( jh.get_colliEntHead() );
            }

        }//------ each frame.pix ------

        //-- 注意，顺序不能错!!! --//
        this->framePoses.at(idx_framePoses).set_rootAnchorPPosOff( rootAnchorOff );
        this->framePoses.at(idx_framePoses).calc_ceh_mposOff_from_cesLB_2_centerMPos();
        this->framePoses.at(idx_framePoses).calc_ceh_rootAnchorCompass_and_off_from_rootAnchor_2_mapEntMid();
        this->framePoses.at(idx_framePoses).check();                 //-- MUST --//

    }//-------- each frame -------
}


/* ===========================================================
 *                   handle_shadow
 * -----------------------------------------------------------
 */
void AnimFrameSet::handle_shadow(){

    int pixNum = pixNum_per_frame.x * 
                 pixNum_per_frame.y; //- 一帧有几个像素点
    RGBA    color_shadow { 0,0,0, 255 };
    size_t  idx_for_S_frame_data_ary;

    for( size_t f=0; f<totalFrameNum; f++ ){ //--- each frame ---
        for( int p=0; p<pixNum; p++ ){ //--- each frame.pix [left-bottom]

        //-- 通过此装置，实现了 “相同的 pjt数据 只用记录一帧” --
        //   并不是最优解，会做很多次重复运算，但是是对原有代码改动最少的。
        isShadowSingleFrame ?
                idx_for_S_frame_data_ary=0 :
                idx_for_S_frame_data_ary=f;

        RGBA &rgbaRef = S_frame_data_ary.at(idx_for_S_frame_data_ary).at(p);
        if( rgbaRef.is_near(color_shadow, 5) == false ){
            continue; //- next frame.pix
        }
        
        //-- 将 shadow pix 改为需要的颜色 --//

            // 一个临时的简陋的方案...
            // 在推荐方案中，为 shadow 专门配备一个 fs着色器，具体颜色在 着色器程序中调整

        rgbaRef.r = 0;
        rgbaRef.g = 5;
        rgbaRef.b = 10;
        rgbaRef.a = 70;
        }
    }
}



namespace{//----------------- namespace ------------------//


/* ===========================================================
 *                 build_three_lpaths
 * -----------------------------------------------------------
 */
void build_three_lpaths( const std::string &_lpath_pic ){
    //- 注释 以 lpath_pic = "/animal/dog_ack_01.P.png" 为例

    std::string lst; //- tmp, 尾部字符串，不停地被截断

    lpath_pic = _lpath_pic;

    //--------------------//
    //    生成 lpath_pjt
    //--------------------//
    auto point_idx = lpath_pic.find( '.', 0 ); //- 指向第一个 '.'
    //- lpath_pjt 暂时等于 "/animal/dog_ack_01"
    lpath_pjt.assign( lpath_pic.begin(), (lpath_pic.begin()+point_idx) );
    lpath_pjt += ".J.png";

    //--------------------//
    //    生成 lpath_shadow
    //--------------------//
    //- lpath_shadow 暂时等于 "/animal/dog_ack_01"
    lpath_shadow.assign( lpath_pic.begin(), (lpath_pic.begin()+point_idx) );
    lpath_shadow += ".S.png";
}

}//-------------------- namespace: end ------------------//

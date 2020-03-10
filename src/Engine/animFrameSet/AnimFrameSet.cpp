/*
 * ================= AnimFrameSet.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2018.11.23
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#include "pch.h"
#include "AnimFrameSet.h"

//-------------------- CPP --------------------//
#include <algorithm> //- find
#include <iterator>
#include <utility> //- move

//------------------- Engine --------------------//
#include "global.h"
#include "create_texNames.h"
#include "load_and_divide_png.h"


#include "esrc_animFrameSet.h"


namespace afs_inn {//----------------- namespace: afs_inn ------------------//

    //-- 每次 insert png数据时，以下这些数据都会被复用 --
    size_t headIdx {}; //- 本次 png数据insert，起始idx
                       //- 在此之前，容器中以及有数据了。

    //-- 图元帧 数据容器组。帧排序为 [left-top] --
    std::vector<std::vector<RGBA>> P_frame_data_ary {}; 
    std::vector<std::vector<RGBA>> J_frame_data_ary {}; 
    std::vector<std::vector<RGBA>> S_frame_data_ary {};

    //- 画面 贴图集的 绝对路径名。一个动作的所有帧图片，存储为一张 png图。
    //- 这个 路径名 只在游戏启动阶段被使用，之后 预存于此
    //- local path，based on path_animFrameSet_srcs
    std::string  path_pic    {}; //-- picture
    std::string  path_pjt    {}; //-- collients
    std::string  path_shadow {}; //-- shadow  (此文件可为空...)

    IntVec2  pixNum_per_frame {};  //- 单帧画面 的 长宽 像素值 （会被存到 animAction 实例中）
    IntVec2  frameNum         {};  //- 画面中，横排可分为几帧，纵向可分为几帧
    size_t   totalFrameNum    {};  //- 目标png文件中，总 图元帧 个数

    std::vector<GLuint> tmpTexNames {}; //- 用于 create_texNames()

    std::unordered_map<size_t, animActionPosId_t> lAnimActionPosIds {}; //- key: animActionParams_.endIdx

    //===== funcs =====//
    void build_three_paths( const std::string &lpath_pic_ );

    bool find_rootAnchor_in_Jpng( const RGBA &color_ )noexcept;


}//-------------------- namespace: afs_inn end ------------------//





/* ===========================================================
 *                  insert_a_png
 * -----------------------------------------------------------
 */
void AnimFrameSet::insert_a_png(  const std::string &path_pic_, 
                            IntVec2             frameNum_,
                            size_t              totalFrameNum_,
                            bool                isHaveShadow_, //-- 将被记录到 animAction 数据中去
                            bool                isPjtSingleFrame_,
                            bool                isShadowSingleFrame_,
                            const std::vector<std::shared_ptr<AnimActionParam>> &animActionParams_ ){

    afs_inn::build_three_paths( path_pic_ );
    afs_inn::frameNum = frameNum_;
    afs_inn::totalFrameNum = totalFrameNum_;

    this->isPJTSingleFrame = isPjtSingleFrame_;
    this->isShadowSingleFrame = isShadowSingleFrame_;
    
    //-------------------//
    //-- 获得本次 insert 的 起始idx
    afs_inn::headIdx = this->texNames_pic.size();

    //----------------------------------------//
    //          lAnimActionPosUPtrs
    //----------------------------------------//
    afs_inn::lAnimActionPosIds.clear();
    if( this->isPJTSingleFrame ){
        //--- 所有 actionName,绑定同一个 id ---
        auto aaPosId = AnimActionPos::id_manager.apply_a_u32_id();
        this->animActionPosUPtrs.insert({ aaPosId, std::make_unique<AnimActionPos>() });
        //---
        for( size_t i=0; i<animActionParams_.size(); i++ ){
            const AnimActionParam *paramPtr = animActionParams_.at(i).get();
            auto [insertIt, insertBool] = afs_inn::lAnimActionPosIds.emplace( i, aaPosId );
            tprAssert( insertBool );
        }

    }else{
        //--- 每个 actionName, 绑定各自的 id ---
        for( size_t i=0; i<animActionParams_.size(); i++ ){
            const AnimActionParam *paramPtr = animActionParams_.at(i).get();
            //---
            auto aaPosId = AnimActionPos::id_manager.apply_a_u32_id();
            this->animActionPosUPtrs.insert({ aaPosId, std::make_unique<AnimActionPos>() });
            //---
            auto [insertIt, insertBool] = afs_inn::lAnimActionPosIds.emplace( i, aaPosId );
            tprAssert( insertBool );
        }
    }

    //----------------------------------------//
    //  load & divide png数据，存入每个 帧容器中
    //----------------------------------------//
    afs_inn::P_frame_data_ary.clear();
    afs_inn::J_frame_data_ary.clear();
    afs_inn::S_frame_data_ary.clear();

    IntVec2 tmpv2 {}; //-tmp

    //-------------------//
    //       Pic
    //-------------------//
    //-- 目前暂不支持 没有 pic数据的 AnimFrameSet 实例 ---
    afs_inn::pixNum_per_frame = load_and_divide_png(afs_inn::path_pic,
                                                    afs_inn::frameNum,
                                                    afs_inn::totalFrameNum,
                                                    afs_inn::P_frame_data_ary );

    afs_inn::tmpTexNames.clear();
    create_texNames( afs_inn::totalFrameNum,
                     afs_inn::pixNum_per_frame,
                     afs_inn::P_frame_data_ary,
                     afs_inn::tmpTexNames );
    this->texNames_pic.insert( this->texNames_pic.end(), afs_inn::tmpTexNames.begin(), afs_inn::tmpTexNames.end() ); //- copy

    //-------------------//
    //       Pjt
    //-------------------//
    if( this->isPJTSingleFrame ){
        //--- J.png 只有一帧 --//
        tmpv2 = load_and_divide_png(afs_inn::path_pjt,
                                    IntVec2{1,1},
                                    1,
                                    afs_inn::J_frame_data_ary );

    }else{
        tmpv2 = load_and_divide_png(afs_inn::path_pjt,
                                    afs_inn::frameNum,
                                    afs_inn::totalFrameNum,
                                    afs_inn::J_frame_data_ary );
    }
    tprAssert( tmpv2 == afs_inn::pixNum_per_frame );
    this->handle_pjt( animActionParams_ );

    //-------------------//
    //      Shadow
    //-------------------//
    if( isHaveShadow_ ){

        if( this->isShadowSingleFrame ){
            //--- S.png 只有一帧 --//
            tmpv2 = load_and_divide_png(afs_inn::path_shadow,
                                        IntVec2{1,1},
                                        1,
                                        afs_inn::S_frame_data_ary );

            tprAssert( tmpv2 == afs_inn::pixNum_per_frame );
            this->handle_shadow();
            afs_inn::tmpTexNames.clear();
            create_texNames( 1,
                            afs_inn::pixNum_per_frame,
                            afs_inn::S_frame_data_ary,
                            afs_inn::tmpTexNames );

            // 就算只有 一张 tex，也要重复 insert，确保总体数量是 和 P.png 帧数相同的
            // 降低后续代码的 复杂度
            this->texNames_shadow.insert( this->texNames_shadow.end(), afs_inn::totalFrameNum, *afs_inn::tmpTexNames.begin() );

        }else{
            tmpv2 = load_and_divide_png(afs_inn::path_shadow,
                                        afs_inn::frameNum,
                                        afs_inn::totalFrameNum,
                                        afs_inn::S_frame_data_ary );

            tprAssert( tmpv2 == afs_inn::pixNum_per_frame );
            this->handle_shadow();
            afs_inn::tmpTexNames.clear();
            create_texNames( afs_inn::totalFrameNum,
                            afs_inn::pixNum_per_frame,
                            afs_inn::S_frame_data_ary,
                            afs_inn::tmpTexNames );

            this->texNames_shadow.insert( this->texNames_shadow.end(), afs_inn::tmpTexNames.begin(), afs_inn::tmpTexNames.end() );
        }
        
    }else{
        //-- 没有 shadow数据时，填写 0 来占位 --
        this->texNames_shadow.insert(   this->texNames_shadow.end(), 
                                        afs_inn::totalFrameNum, 
                                        0 );
    }

    //-------------------//
    //    animActions
    //-------------------//
    for( size_t i=0; i<animActionParams_.size(); i++ ){
        const AnimActionParam *paramPtr = animActionParams_.at(i).get();
        //---
        animSubspeciesId_t subId = this->subGroup.find_or_create_a_animSubspeciesId(paramPtr->animLabel,
                                                                                    paramPtr->subspeciesIdx );
        AnimSubspecies &subRef = esrc::find_or_insert_new_animSubspecies( subId );
        AnimAction &actionRef = subRef.insert_new_animAction( paramPtr->actionDir, paramPtr->actionBrokenLvl, paramPtr->actionEName );


        auto aaposId = afs_inn::lAnimActionPosIds.at( i );

        actionRef.init( *this,
                        *paramPtr,
                        this->animActionPosUPtrs.at(aaposId).get(),
                        afs_inn::pixNum_per_frame,
                        afs_inn::headIdx,
                        isHaveShadow_ );

    }
    this->subGroup.check(); //- MUST !!!    
}



/* ===========================================================
 *                   handle_pjt
 * -----------------------------------------------------------
 */
void AnimFrameSet::handle_pjt( const std::vector<std::shared_ptr<AnimActionParam>> &animActionParams_ ){

    size_t pixNum = cast_2_size_t( afs_inn::pixNum_per_frame.x * 
                                    afs_inn::pixNum_per_frame.y); //- 一帧有几个像素点

    glm::dvec2  pixDPos {}; //- current pix dpos

    if( this->isPJTSingleFrame ){
        //-- 此时，所有 actionName 都指向同一个 id --
        auto aaposId = afs_inn::lAnimActionPosIds.begin()->second;
        std::optional<glm::dvec2> rootAnchorDPosOff {std::nullopt};

        for( size_t p=0; p<pixNum; p++ ){ //--- each frame.pix [left-bottom]

            pixDPos.x = static_cast<double>( static_cast<int>(p)%afs_inn::pixNum_per_frame.x );
            pixDPos.y = static_cast<double>( static_cast<int>(p)/afs_inn::pixNum_per_frame.x );

            if( afs_inn::find_rootAnchor_in_Jpng( afs_inn::J_frame_data_ary.at(0).at(p) ) ){
                tprAssert( !rootAnchorDPosOff.has_value() ); // not set yet
                rootAnchorDPosOff = { pixDPos }; // set only once
                break;
            }
        }//------ each frame.pix ------
        tprAssert( rootAnchorDPosOff.has_value() );

        //--- 执行所有 补充性设置， IMPORTANT!!! --- 
        this->animActionPosUPtrs.at(aaposId)->set_rootAnchorDPosOff( rootAnchorDPosOff.value() );

    }else{

        for( size_t i=0; i<animActionParams_.size(); i++ ){
            const AnimActionParam *paramPtr = animActionParams_.at(i).get();
            //---
            auto aaposId = afs_inn::lAnimActionPosIds.at(i);
            std::optional<glm::dvec2> rootAnchorDPosOff {std::nullopt};

            for( size_t p=0; p<pixNum; p++ ){ //--- each frame.pix [left-bottom]

                pixDPos.x = static_cast<double>( static_cast<int>(p)%afs_inn::pixNum_per_frame.x );
                pixDPos.y = static_cast<double>( static_cast<int>(p)/afs_inn::pixNum_per_frame.x );

                if( afs_inn::find_rootAnchor_in_Jpng( afs_inn::J_frame_data_ary.at(paramPtr->jFrameIdx).at(p) ) ){
                    tprAssert( !rootAnchorDPosOff.has_value() ); // not set yet
                    rootAnchorDPosOff = { pixDPos }; // set only once
                    break;
                }

            }//------ each frame.pix ------
            tprAssert( rootAnchorDPosOff.has_value() );

            //--- 执行所有 补充性设置， IMPORTANT!!! --- 
            this->animActionPosUPtrs.at(aaposId)->set_rootAnchorDPosOff( rootAnchorDPosOff.value() );
        }
    }
}


/* ===========================================================
 *                   handle_shadow
 * -----------------------------------------------------------
 */
void AnimFrameSet::handle_shadow(){

    size_t pixNum = cast_2_size_t( afs_inn::pixNum_per_frame.x * 
                                    afs_inn::pixNum_per_frame.y); //- 一帧有几个像素点
    RGBA    color_shadow { 0,0,0, 255 };
    size_t  idx_for_S_frame_data_ary {};

    for( size_t f=0; f<afs_inn::totalFrameNum; f++ ){ //--- each frame ---
        for( size_t p=0; p<pixNum; p++ ){ //--- each frame.pix [left-bottom]

        //-- 通过此装置，实现了 “相同的 pjt数据 只用记录一帧” --
        //   并不是最优解，会做很多次重复运算，但是是对原有代码改动最少的。
        this->isShadowSingleFrame ?
                idx_for_S_frame_data_ary=0 :
                idx_for_S_frame_data_ary=f;

        RGBA &rgbaRef = afs_inn::S_frame_data_ary.at(idx_for_S_frame_data_ary).at(p);

        //if( rgbaRef.is_near(color_shadow, 5) == false ){
        //    continue; //- next frame.pix
        //}

        //- 折中版，只要达到一定透明度，一律算作 shadow pix
        if( rgbaRef.a < 200 ){
            continue;
        }
        
        //-- 将 shadow pix 改为需要的颜色 --//
        rgbaRef.r = 0;
        rgbaRef.g = 5;
        rgbaRef.b = 10;
        rgbaRef.a = 255; // opaque
        }
    }
}


namespace afs_inn {//----------------- namespace: afs_inn ------------------//



void build_three_paths( const std::string &path_pic_ ){
    //- 注释 以 path_pic = "/animal/dog_ack_01.P.png" 为例

    //--------------------//
    //    path_pic
    //--------------------//
    path_pic = path_pic_;

    //--------------------//
    //    path_pjt
    //--------------------//
    size_t point_idx = path_pic.find( '.', 0 ); //- 指向第一个 '.'
    auto lastIt = path_pic.begin();
    std::advance( lastIt, point_idx ); //- advance 并不防止 溢出
    //- lpath_pjt 暂时等于 "/animal/dog_ack_01"
    path_pjt.assign( path_pic.begin(), lastIt );
    path_pjt += ".J.png";

    //--------------------//
    //    path_shadow
    //--------------------//
    //- lpath_shadow 暂时等于 "/animal/dog_ack_01"
    path_shadow.assign( path_pic.begin(), lastIt );
    path_shadow += ".S.png";
}



bool find_rootAnchor_in_Jpng( const RGBA &color_ )noexcept{
    if( color_.a < 250 ){
        return false;
    }
    return ( is_closeEnough<uint8_t>( color_.r, 255, 10 ) );
}



}//-------------------- namespace: afs_inn end ------------------//

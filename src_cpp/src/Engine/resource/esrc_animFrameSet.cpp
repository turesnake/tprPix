/*
 * ================= esrc_animFrameSet.cpp =====================
 *                          -- tpr --
 *                                        CREATE -- 2018.11.23
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *  资源管理:  animFrameSeta
 * ----------------------------
 */
//-------------------- CPP --------------------//
#include <string>
#include <sstream>
#include <memory>

//-------------------- tpr --------------------//
#include "tprGeneral.h"

//-------------------- Engine --------------------//
#include "tprAssert.h"
#include "esrc_animFrameSet.h"


//#include "tprDebug.h" //- tmp

namespace esrc {//------------------ namespace: esrc -------------------------//


namespace animFrameSet_inn {//----------- namespace: animFrameSet_inn -------------//

    std::unordered_map<std::string, std::unique_ptr<AnimFrameSet>> animFrameSets {};

    std::vector<AnimActionParam> animActionParams {};

    //===== funcs =====//
    AnimFrameSet &insert_new_animFrameSet( const std::string &name_ );

    void build_animActionParams(const std::string &prefix_,
                                const std::string &suffix_,
                                size_t size_ );

}//--------------- namespace: animFrameSet_inn end -----------------//


/* ===========================================================
 *                      getnc_animActionPtr
 * -----------------------------------------------------------
 * -- 用法：
 *    getnc_animActionPtr( "dog", "jump" );
 */
AnimAction *getnc_animActionPtr(  const std::string &animFrameSetName_,
                                const std::string &actionName_ ){

    tprAssert( animFrameSet_inn::animFrameSets.find(animFrameSetName_) != animFrameSet_inn::animFrameSets.end() );
    return  animFrameSet_inn::animFrameSets.at(animFrameSetName_)->getnc_animActionPtr( actionName_ );
}



/* ===========================================================
 *                     load_animFrameSets
 * -----------------------------------------------------------
 * -- 在游戏初始化阶段，被调用。
 * -- 统一加载并初始化 所有 AnimFrameSet 资源
 */
void load_animFrameSets(){

    animFrameSet_inn::animFrameSets.clear();

    //---------------------------------------//
    //                Button
    //---------------------------------------//
    {
        auto &animFrameSetRef = animFrameSet_inn::insert_new_animFrameSet( "button_beginScene" );
        animFrameSetRef.insert_a_png(  
                "/button/beginScene/button_beginScene.P.png",
                IntVec2{1, 3}, 3,
                false, //- 无 shadow 数据
                false, false, //- J,S 数据是否只有 1 帧
                std::vector<AnimActionParam>{      
                    AnimActionParam{    "pointer", 0 },
                    AnimActionParam{    "new",     1 }, 
                    AnimActionParam{    "data",    2 }
                });
    }

    //---------------------------------------//
    //               GO: Major
    //---------------------------------------//
    //-----------//
    //   norman
    //-----------//
    {
        auto &animFrameSetRef = animFrameSet_inn::insert_new_animFrameSet( "norman" );
        animFrameSetRef.insert_a_png(  
                "/norman/norman.P.png",
                IntVec2{6, 2}, 12,
                true, //- 有 shadow 数据
                false, false, //- J,S 数据是否只有 1 帧
                std::vector<AnimActionParam>{
                    AnimActionParam{    "move_idle", 
                                        AnimActionType::Cycle,
                                        true,  //- isOrder
                                        std::vector<size_t>{ 0,  1, 2,  3, 4, 5 },
                                        std::vector<size_t>{ 12, 8, 8, 12, 8, 8 } },
                    AnimActionParam{    "move_walk", 
                                        AnimActionType::Cycle,
                                        true,  //- isOrder
                                        std::vector<size_t>{ 6, 7, 8, 9, 10, 11 },
                                        4 }
                });
    }

    //-----------//
    //   bigMan
    //-----------//
    {
        auto &animFrameSetRef = animFrameSet_inn::insert_new_animFrameSet( "bigMan" );
        animFrameSetRef.insert_a_png(  
                "/bigMan/bigMan.P.png",
                IntVec2{6, 2}, 12,
                true, //- 有 shadow 数据
                false, false, //- J,S 数据是否只有 1 帧
                std::vector<AnimActionParam>{
                    AnimActionParam{    "move_idle", 
                                        AnimActionType::Cycle,
                                        true,  //- isOrder
                                        std::vector<size_t>{ 0,  1, 2,  3, 4,  5 },
                                        std::vector<size_t>{ 10, 8, 8,  8, 8, 16 } },
                    AnimActionParam{    "move_walk", 
                                        AnimActionType::Cycle,
                                        true,  //- isOrder
                                        std::vector<size_t>{ 6, 7, 8, 9, 10, 11 },
                                        std::vector<size_t>{ 3, 6, 3, 3,  6,  3 } }
                });
    }

    //-----------//
    //    crab
    //-----------//
    {
        auto &animFrameSetRef = animFrameSet_inn::insert_new_animFrameSet( "crab" );
        animFrameSetRef.insert_a_png(  
                "/crab/crab.P.png",
                IntVec2{5, 4}, 19,
                true, //- 有 shadow 数据
                true, false, //- J,S 数据是否只有 1 帧
                std::vector<AnimActionParam>{

                    AnimActionParam{    "move_idle", 0 },                     
                    
                    AnimActionParam{    "move_walk", 
                                        AnimActionType::Cycle,
                                        true,  //- isOrder
                                        std::vector<size_t>{ 0,  4,  5,  6 },
                                        std::vector<size_t>{ 4,  4,  4,  4 } },

                    AnimActionParam{    "panic", 
                                        AnimActionType::Once,
                                        true,  //- isOrder
                                        std::vector<size_t>{ 0,  1, 2,  3 },
                                        std::vector<size_t>{ 8,  4, 4,  8 } },

                    AnimActionParam{    "appear", 
                                        AnimActionType::Once,
                                        false,  //- isOrder
                                        std::vector<size_t>{ 7,8,9,10,11,12,13,14, 15,16,17,18 },
                                        std::vector<size_t>{ 4,4,4, 4, 4, 4, 4, 4, 20, 4, 4, 4 } },

                    AnimActionParam{    "disappear", 
                                        AnimActionType::Once,
                                        true,  //- isOrder
                                        std::vector<size_t>{ 7,8,9,10,  11,12,13,14,15,16,17,18 },
                                        std::vector<size_t>{ 4,4,4, 4,  16, 4, 4, 4, 6, 4, 4, 4 } },

                    AnimActionParam{    "half_appear", 
                                        AnimActionType::Once,
                                        false,  //- isOrder
                                        std::vector<size_t>{ 7,8,9,10 },
                                        std::vector<size_t>{ 4,4,4, 4 } },

                    AnimActionParam{    "half_disappear", 
                                        AnimActionType::Once,
                                        true,  //- isOrder
                                        std::vector<size_t>{ 7,8,9,10 },
                                        std::vector<size_t>{ 4,4,4, 4 } },
                    //... 未完 ...
                });
    }


    //---------------------------------------//
    //               GO: Tree
    //---------------------------------------//
    

    //-----------//
    //  oakTree
    //-----------//
    {
        auto &animFrameSetRef = animFrameSet_inn::insert_new_animFrameSet( "oakTree" );
        animFrameSet_inn::build_animActionParams( "", "_idle", 17 );
        animFrameSetRef.insert_a_png(  
                "/oakTree/OakTree_2.P.png",
                IntVec2{5, 4}, 17,
                true, //- 有 shadow 数据
                false, false, //- J,S 数据是否只有 1 帧
                animFrameSet_inn::animActionParams );
    }

    //-----------//
    //  pineTree
    //-----------//
    {
        auto &animFrameSetRef = animFrameSet_inn::insert_new_animFrameSet( "pineTree" );
        animFrameSet_inn::build_animActionParams( "", "_idle", 8 );
        animFrameSetRef.insert_a_png(  
                "/pineTree/pineTree.P.png", 
                IntVec2{4, 2}, 8,
                true, //- 有 shadow 数据
                false, false, //- J,S 数据是否只有 1 帧
                animFrameSet_inn::animActionParams );
    }


    //---------------------------------------//
    //               GO: Bush
    //---------------------------------------//
    
    //-----------//
    //  wheat
    //-----------//
    {
        auto &animFrameSetRef = animFrameSet_inn::insert_new_animFrameSet( "wheat" );
        animFrameSet_inn::build_animActionParams( "front_", "_idle", 4 );
        animFrameSetRef.insert_a_png(  
                "/wheat/wheat_Front.P.png",
                IntVec2{4, 1}, 4,
                false, //- 没有 shadow 数据
                false, false, //- J,S 数据是否只有 1 帧
                animFrameSet_inn::animActionParams );
        animFrameSet_inn::build_animActionParams( "back_", "_idle", 4 );
        animFrameSetRef.insert_a_png(  
                "/wheat/wheat_Back.P.png",
                IntVec2{4, 1}, 4,
                false, //- 没有 shadow 数据
                false, false, //- J,S 数据是否只有 1 帧
                animFrameSet_inn::animActionParams );
    }
    
    //-----------//
    // singleStone_Desert 
    //-----------//
    {
        auto &animFrameSetRef = animFrameSet_inn::insert_new_animFrameSet( "singleStone_Desert" );
        animFrameSet_inn::build_animActionParams( "", "_idle", 8 );
        animFrameSetRef.insert_a_png(  
                "/singleStone_Desert/singleStone_Desert.P.png", 
                IntVec2{4, 2}, 8,
                false, //- 没有 shadow 数据
                false, false, //- J,S 数据是否只有 1 帧
                animFrameSet_inn::animActionParams
                );
    }

    //-----------//
    //  leaf_DForest
    //-----------//
    {
        auto &animFrameSetRef = animFrameSet_inn::insert_new_animFrameSet( "leaf_DForest" );
        animFrameSet_inn::build_animActionParams( "", "_idle", 8 );
        animFrameSetRef.insert_a_png(  
                "/leaf_DForest/leaf_DForest.P.png", 
                IntVec2{4, 2}, 8,
                false, //- 没有 shadow 数据
                false, false, //- J,S 数据是否只有 1 帧
                animFrameSet_inn::animActionParams
                );
    }

    //---------------------------------------//
    //               GO: MapSurface
    //---------------------------------------//
    
    //-----------//
    // lichen_Forest 
    //-----------//
    {
        auto &animFrameSetRef = animFrameSet_inn::insert_new_animFrameSet( "lichen_Forest" );
        animFrameSet_inn::build_animActionParams( "", "_idle", 32 );
        animFrameSetRef.insert_a_png(  
                "/lichen_Forest/lichen_Forest.P.png", 
                IntVec2{8, 4}, 32,
                false, //- 没有 shadow 数据
                false, false, //- J,S 数据是否只有 1 帧
                animFrameSet_inn::animActionParams
                );
    }


    //-----------//
    // lichen_DForest 
    //-----------//
    {
        auto &animFrameSetRef = animFrameSet_inn::insert_new_animFrameSet( "lichen_DForest" );
        animFrameSet_inn::build_animActionParams( "", "_idle", 24 );
        animFrameSetRef.insert_a_png(  
                "/lichen_DForest/lichen_DForest.P.png", 
                IntVec2{8, 3}, 24,
                false, //- 没有 shadow 数据
                false, false, //- J,S 数据是否只有 1 帧
                animFrameSet_inn::animActionParams
                );
    }

}



namespace animFrameSet_inn {//----------- namespace: animFrameSet_inn -------------//


/* ===========================================================
 *               insert_new_animFrameSet
 * -----------------------------------------------------------
 */
AnimFrameSet &insert_new_animFrameSet( const std::string &name_ ){
    animFrameSet_inn::animFrameSets.insert({ name_, std::make_unique<AnimFrameSet>(name_) });
    return *(animFrameSet_inn::animFrameSets.at(name_).get());
}



/* ===========================================================
 *               build_animActionParams
 * -----------------------------------------------------------
 */
void build_animActionParams(const std::string &prefix_,
                            const std::string &suffix_,
                            size_t size_ ){
    animFrameSet_inn::animActionParams.clear();
    for( size_t i=0; i<size_; i++ ){
        animFrameSet_inn::animActionParams.push_back( AnimActionParam{ tprGeneral::nameString_combine( prefix_, i, suffix_ ), i } );
    }
}


}//--------------- namespace: animFrameSet_inn end -----------------//
}//---------------------- namespace: esrc end -------------------------//


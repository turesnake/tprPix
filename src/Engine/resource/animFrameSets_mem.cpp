/*
 * ================= animFrameSets_mem.cpp =====================
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

//-------------------- tpr --------------------//
#include "tprFileSys.h"

//-------------------- Engine --------------------//
#include "esrc_animFrameSet.h"


//#include "tprDebug.h" //- tmp

namespace esrc {//------------------ namespace: esrc -------------------------//


namespace{//--------------- namespace -----------------//

    std::unordered_map<std::string, AnimFrameSet> animFrameSets {};

    std::stringstream  ss;

    std::vector<AnimActionParam> animActionParams {};


    //===== funcs =====//
    AnimFrameSet *insert_new_animFrameSet( const std::string &_name );

    void build_animActionParams(const std::string &_prefix,
                            const std::string &_suffix,
                            size_t _size );

}//--------------- namespace end -----------------//


/* ===========================================================
 *                      getnc_animActionPtr
 * -----------------------------------------------------------
 * -- 用法：
 *    esrc::getnc_animActionPtr( "dog", "jump" );
 */
AnimAction *getnc_animActionPtr(  const std::string &_animFrameSetName,
                                const std::string &_actionName ){

    assert( esrc::animFrameSets.find(_animFrameSetName) != esrc::animFrameSets.end() );
    return  esrc::animFrameSets.at(_animFrameSetName).getnc_animActionPtr( _actionName );
}



/* ===========================================================
 *                     load_animFrameSets
 * -----------------------------------------------------------
 * -- 在游戏初始化阶段，被调用。
 * -- 统一加载并初始化 所有 AnimFrameSet 资源
 */
void load_animFrameSets(){

    esrc::animFrameSets.clear();

    AnimFrameSet *animFrameSetPtr;


    //---------------------------------------//
    //                Button
    //---------------------------------------//
    animFrameSetPtr = insert_new_animFrameSet( "button_beginScene" );
    animFrameSetPtr->insert_a_png(  
            "/button/beginScene/button_beginScene.P.png",
            IntVec2{1, 3}, 3,
            false, //- 无 shadow 数据
            false, false, //- J,S 数据是否只有 1 帧
            std::vector<AnimActionParam>{      
                AnimActionParam{    "pointer", 0 },
                AnimActionParam{    "new",     1 }, 
                AnimActionParam{    "data",    2 }
            });


    //---------------------------------------//
    //               GO: Major
    //---------------------------------------//
    //-----------//
    //   norman
    //-----------//
    animFrameSetPtr = insert_new_animFrameSet( "norman" );
    animFrameSetPtr->insert_a_png(  
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

    
    //-----------//
    //   bigMan
    //-----------//
    animFrameSetPtr = insert_new_animFrameSet( "bigMan" );
    animFrameSetPtr->insert_a_png(  
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


    
    //-----------//
    //    crab
    //-----------//
    animFrameSetPtr = insert_new_animFrameSet( "crab" );
    animFrameSetPtr->insert_a_png(  
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


    //---------------------------------------//
    //               GO: Tree
    //---------------------------------------//
    

    //-----------//
    //  oakTree
    //-----------//
    animFrameSetPtr = insert_new_animFrameSet( "oakTree" );
    build_animActionParams( "", "_idle", 17 );
    animFrameSetPtr->insert_a_png(  
            "/oakTree/OakTree_2.P.png",
            IntVec2{5, 4}, 17,
            true, //- 无 shadow 数据
            false, false, //- J,S 数据是否只有 1 帧
            animActionParams );


    //-----------//
    //  pineTree
    //-----------//
    animFrameSetPtr = insert_new_animFrameSet( "pineTree" );
    build_animActionParams( "", "_idle", 8 );
    animFrameSetPtr->insert_a_png(  
            "/pineTree/pineTree.P.png", 
            IntVec2{4, 2}, 8,
            true, //- 有 shadow 数据
            false, false, //- J,S 数据是否只有 1 帧
            animActionParams );


    //---------------------------------------//
    //               GO: Bush
    //---------------------------------------//
    
    //-----------//
    //  wheat
    //-----------//
    animFrameSetPtr = insert_new_animFrameSet( "wheat" );
    build_animActionParams( "front_", "_idle", 4 );
    animFrameSetPtr->insert_a_png(  
            "/wheat/wheat_Front.P.png",
            IntVec2{4, 1}, 4,
            false, //- 没有 shadow 数据
            false, false, //- J,S 数据是否只有 1 帧
            animActionParams );
    build_animActionParams( "back_", "_idle", 4 );
    animFrameSetPtr->insert_a_png(  
            "/wheat/wheat_Back.P.png",
            IntVec2{4, 1}, 4,
            false, //- 没有 shadow 数据
            false, false, //- J,S 数据是否只有 1 帧
            animActionParams );

    
    //-----------//
    // singleStone_Desert 
    //-----------//
    animFrameSetPtr = insert_new_animFrameSet( "singleStone_Desert" );
    build_animActionParams( "", "_idle", 8 );
    animFrameSetPtr->insert_a_png(  
            "/singleStone_Desert/singleStone_Desert.P.png", 
            IntVec2{4, 2}, 8,
            false, //- 没有 shadow 数据
            false, false, //- J,S 数据是否只有 1 帧
            animActionParams
            );

    //-----------//
    //  leaf_DForest
    //-----------//
    animFrameSetPtr = insert_new_animFrameSet( "leaf_DForest" );
    build_animActionParams( "", "_idle", 8 );
    animFrameSetPtr->insert_a_png(  
            "/leaf_DForest/leaf_DForest.P.png", 
            IntVec2{4, 2}, 8,
            false, //- 没有 shadow 数据
            false, false, //- J,S 数据是否只有 1 帧
            animActionParams
            );



    //---------------------------------------//
    //               GO: MapSurface
    //---------------------------------------//
    
    //-----------//
    // lichen_Forest 
    //-----------//
    animFrameSetPtr = insert_new_animFrameSet( "lichen_Forest" );
    build_animActionParams( "", "_idle", 32 );
    animFrameSetPtr->insert_a_png(  
            "/lichen_Forest/lichen_Forest.P.png", 
            IntVec2{8, 4}, 32,
            false, //- 没有 shadow 数据
            false, false, //- J,S 数据是否只有 1 帧
            animActionParams
            );



    //-----------//
    // lichen_DForest 
    //-----------//
    animFrameSetPtr = insert_new_animFrameSet( "lichen_DForest" );
    build_animActionParams( "", "_idle", 24 );
    animFrameSetPtr->insert_a_png(  
            "/lichen_DForest/lichen_DForest.P.png", 
            IntVec2{8, 3}, 24,
            false, //- 没有 shadow 数据
            false, false, //- J,S 数据是否只有 1 帧
            animActionParams
            );

}



namespace{//--------------- namespace -----------------//


/* ===========================================================
 *               insert_new_animFrameSet
 * -----------------------------------------------------------
 */
AnimFrameSet *insert_new_animFrameSet( const std::string &_name ){
    esrc::animFrameSets.insert({ _name, AnimFrameSet { _name } });
    return &esrc::animFrameSets.at(_name);
}


/* ===========================================================
 *               build_animActionParams
 * -----------------------------------------------------------
 */
void build_animActionParams(const std::string &_prefix,
                            const std::string &_suffix,
                            size_t _size ){
    animActionParams.clear();
    for( size_t i=0; i<_size; i++ ){
        animActionParams.push_back( AnimActionParam{ tpr::nameString_combine( _prefix, i, _suffix ), i } );
    }
}


}//--------------- namespace end -----------------//
}//---------------------- namespace: esrc -------------------------//


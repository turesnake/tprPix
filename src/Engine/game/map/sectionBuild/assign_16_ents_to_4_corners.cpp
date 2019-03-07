/*
 * =============== assign_16_ents_to_4_corners.cpp ==========
 *                          -- tpr --
 *                                        CREATE -- 2019.03.02
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *  一个独立的模块，分配 4*4 个节点，给 4个端点。
 *  最终结果是一张 vector, 根据它，来分配 section 中的 chunks
 *  ------
 *  由于这个模块中的数据会被 反复使用，所以要做好 使用前 clear 工作
 * ----------------------------
 */
#include "Section.h"

//-------------------- C --------------------//
#include <cassert>


//-------------------- Engine --------------------//
#include "random.h"
#include "config.h"
#include "chunkKey.h"
#include "Chunk.h"
#include "srcs_engine.h"
#include "Quad.h"

#include "debug.h"


namespace{//-------- namespace: --------------//

    std::default_random_engine  randEngine; //-- 通用引擎
    std::uniform_int_distribution<int> uDistribution_two(0,1);   //- 二选一
    std::uniform_int_distribution<int> uDistribution_four(0,3);  //- 四选一
    //-- 用于生成 ecosys early weights --
    // 注意，随机数不是平均分布的，[0,6]->[1,4]
    std::uniform_int_distribution<int> uDistribution_earlyWeights(0,6);
    //---

    std::vector<int> earlyWeights {}; //- 为 4个端点 随机分配一个 权重值 [1,4] 
                                      //  根据这个值，为一个 端点分配最初几个的 ents

    class Ent{
    public:
        QuadType type   {QuadType::Left_Bottom};
        bool       is_set {false};
    };
    std::vector<Ent>  ents {}; //- 本模块处理的主数据 4*4


    //-- early weights 分配表 ---
    //  记录了 4个corner 在 weights 分别等于 1，2，3，4 时
    //  依次分配到的 ents 的 坐标 
    std::vector<std::vector<IntVec2>> earlyWeightsTable {
        std::vector<IntVec2>{ //- 左下 corner
            IntVec2{ 0, 0 },
            IntVec2{ 0, 1 },
            IntVec2{ 1, 0 },
            IntVec2{ 1, 1 }
        },
        std::vector<IntVec2>{ //- 右下 corner
            IntVec2{ 3, 0 },
            IntVec2{ 2, 0 },
            IntVec2{ 3, 1 },
            IntVec2{ 2, 1 }
        },
        std::vector<IntVec2>{ //- 左上 corner
            IntVec2{ 0, 3 },
            IntVec2{ 1, 3 },
            IntVec2{ 0, 2 },
            IntVec2{ 1, 2 }
        },
        std::vector<IntVec2>{ //- 右上 corner
            IntVec2{ 3, 3 },
            IntVec2{ 3, 2 },
            IntVec2{ 2, 3 },
            IntVec2{ 2, 2 }
        }
    };

    //- 临时容器，根据 ecosys early weights，分配到数量不定的 ents
    std::vector<IntVec2> earlyEntPoses {};


    //-- 4组 侧边 ent pos --
    std::vector<IntVec2> Left_Side_EntPoses{
        IntVec2{ 0, 1 },
        IntVec2{ 0, 2 },
    };
    std::vector<IntVec2> Right_Side_EntPoses{
        IntVec2{ 3, 1 },
        IntVec2{ 3, 2 },
    };
    std::vector<IntVec2> Top_Side_EntPoses{
        IntVec2{ 1, 3 },
        IntVec2{ 2, 3 },
    };
    std::vector<IntVec2> Bottom_Side_EntPoses{
        IntVec2{ 1, 0 },
        IntVec2{ 2, 0 },
    };

    //-- 4个 中心区 ent pos --
    IntVec2 Mid_Left_Bottom_EntPos  { 1, 1 };
    IntVec2 Mid_Right_Bottom_EntPos { 2, 1 };
    IntVec2 Mid_Left_Top_EntPos     { 1, 2 };
    IntVec2 Mid_Right_Top_EntPos    { 2, 2 };

    //-- 四个 ecoSysInMap 指针 --
    std::vector<EcoSysInMap*> fourEcoSysInMapPtrs {};
                //-- 这个数据 和 Section.ecoSysInMapPtrs 是完全重复的
                //   未来考虑做修改...


    //---------- funcs ---------//
    void  main_assign(); 
    void init_earlyWeights();
    void assign_side_ents( const std::vector<IntVec2> &_poses, QuadType _valA, QuadType _valB );
    void assign_mid_ent( const IntVec2 &_pos, QuadType _valA, QuadType _valB, QuadType _valC );

}//------------- namespace: end --------------//



/* ===========================================================
 *                assign_chunks_to_ecoSysInMap
 * -----------------------------------------------------------
 * -- 主函数，将本 section 拥有的每个 chunk， 分配给 4个 ecosys 中的一个 
 * -- 并不真的分配 chunk数据，而是分配 section 中的 暂存数据
 */
void Section::assign_chunks_to_ecoSysInMap(){

        //return;

    //-----------------------------//
    //         perpare
    //-----------------------------//
    //-- 绑定 四个 ecosys 指针 --//
    fourEcoSysInMapPtrs.clear();
    for( const auto &key : quadSectionKeys ){
        fourEcoSysInMapPtrs.push_back( esrc::get_ecoSysInMapPtr(key) );

                /*
                EcoSysInMap *ptr = esrc::get_ecoSysInMapPtr(key);
                cout << "++ mpos:" << ptr->mcpos.get_mpos().x
                    << ", " << ptr->mcpos.get_mpos().y << "; \n"
                    << "key = " << ptr->sectionKey.get_key() << "; \n"
                    << "   " << ptr->is_all_sections_done.is_left_bottom
                    << ", " << ptr->is_all_sections_done.is_right_bottom
                    << ", " << ptr->is_all_sections_done.is_left_top
                    << ", " << ptr->is_all_sections_done.is_right_top
                    << endl;
                */
    }


        //cout << "-1- fourEcoSysInMapPtrs.size() = " << fourEcoSysInMapPtrs.size() << endl;


    //-----------------------------//
    //       正式分配
    //-----------------------------//
    main_assign();


    //-----------------------------//
    //  根据 ents 数据，正式 设置 4*4 chunks
    //-----------------------------//
    size_t        idx;
    IntVec2       sectionMPos = this->mcpos.get_mpos();
    for( int h=0; h<CHUNKS_PER_SECTION; h++ ){ //- each chunk
        for( int w=0; w<CHUNKS_PER_SECTION; w++ ){
            idx = h*CHUNKS_PER_SECTION + w;
                assert( (idx>=0) && (idx<chunkEcoSysInMapKeys.size()) );
            chunkEcoSysInMapKeys.at(idx) = get_quadSectionKey_by_quadType( ents.at(idx).type );
        }
    }

    //-----------------------------//
    //  改写 4 个 ecosys 的 flag
    //  表示 对应 象限section 已经写入完毕
    //-----------------------------//
    //- 注意，这里改写的 flag 是反向的。
    fourEcoSysInMapPtrs.at(QuadType_2_Idx(QuadType::Left_Bottom))->is_all_sections_done.is_right_top = true;
    fourEcoSysInMapPtrs.at(QuadType_2_Idx(QuadType::Right_Bottom))->is_all_sections_done.is_left_top = true;
    fourEcoSysInMapPtrs.at(QuadType_2_Idx(QuadType::Left_Top))->is_all_sections_done.is_right_bottom = true;
    fourEcoSysInMapPtrs.at(QuadType_2_Idx(QuadType::Right_Top))->is_all_sections_done.is_left_bottom = true;

    //...
}



namespace{//-------- namespace: --------------//



/* ===========================================================
 *                   main_assign
 * -----------------------------------------------------------
 * -- 本模块 主函数
 */
void main_assign(){

    randEngine.seed( get_new_seed() ); //- tmp
    //-------------------------//
    //          init 
    //-------------------------//
    init_earlyWeights();
    ents.clear();
    ents.resize( CHUNKS_PER_SECTION * CHUNKS_PER_SECTION );
        assert( CHUNKS_PER_SECTION == 4 ); //- 当此值被改变时，本模块就失效了
    //--------

    size_t  idx;
    QuadType  tmpCornerType;

    //-------------------------//
    //   根据 early weight 分配第一批 ents
    //-------------------------//
    for( int i=0; i<QUAD_NUM; i++ ){ //- each corner

        tmpCornerType = QuadIdx_2_Type(i);
        //-- 填好 earlyEntPoses --
        earlyEntPoses.clear();
        const auto &table = earlyWeightsTable.at(i);
        switch( earlyWeights.at(i) ){
            case 1:
                earlyEntPoses.push_back( table.at(0) );
                break;
            case 2:
                earlyEntPoses.push_back( table.at(0) );
                (uDistribution_two(randEngine) == 0) ?
                    earlyEntPoses.push_back( table.at(1) ) :
                    earlyEntPoses.push_back( table.at(2) );
                break;
            case 3:
                earlyEntPoses.push_back( table.at(0) );
                earlyEntPoses.push_back( table.at(1) );
                earlyEntPoses.push_back( table.at(2) );
                break;
            case 4:
                earlyEntPoses.push_back( table.at(0) );
                earlyEntPoses.push_back( table.at(1) );
                earlyEntPoses.push_back( table.at(2) );
                earlyEntPoses.push_back( table.at(3) );
                break;
            default:
                assert(0);
        }

        //-- 根据 earlyEntPoses 中的元素，设置 对应的 ent --
        for( const auto &pos : earlyEntPoses ){
            idx = pos.y*CHUNKS_PER_SECTION + pos.x;
            Ent &entRef = ents.at(idx);
            entRef.type = tmpCornerType;
            entRef.is_set = true;
        }
    }
        //return;

    //-------------------------//
    //    处理剩下的 ents
    //-------------------------//
    //-- 4组侧边 ents --
    assign_side_ents(   Left_Side_EntPoses,
                        QuadType::Left_Bottom,
                        QuadType::Left_Top );

    assign_side_ents(   Right_Side_EntPoses,
                        QuadType::Right_Bottom,
                        QuadType::Right_Top );
    
    assign_side_ents(   Top_Side_EntPoses,
                        QuadType::Left_Top,
                        QuadType::Right_Top );

    assign_side_ents(   Bottom_Side_EntPoses,
                        QuadType::Left_Bottom,
                        QuadType::Right_Bottom );

    //-- 4个 中心区 ents --
    assign_mid_ent( Mid_Left_Bottom_EntPos,
                    QuadType::Left_Bottom,
                    QuadType::Left_Top,
                    QuadType::Right_Bottom );
    
    assign_mid_ent( Mid_Right_Bottom_EntPos,
                    QuadType::Right_Bottom,
                    QuadType::Left_Bottom,
                    QuadType::Right_Top );

    assign_mid_ent( Mid_Left_Top_EntPos,
                    QuadType::Left_Top,
                    QuadType::Left_Bottom,
                    QuadType::Right_Top );

    assign_mid_ent( Mid_Right_Top_EntPos,
                    QuadType::Right_Top,
                    QuadType::Left_Top,
                    QuadType::Right_Bottom );

        //return;

    //-------------------------//
    //    检测，统计
    //-------------------------//
    EcoSysInMap  *ecoSysPtr;
    int  tmpIdx; //- tmp
    for( const auto &entRef : ents ){
        assert( entRef.is_set == true );
        tmpIdx = QuadType_2_Idx(entRef.type);

            
            //cout << "fourEcoSysInMapPtrs.size() = " << fourEcoSysInMapPtrs.size() << endl;
            //if( (tmpIdx<0) || (tmpIdx>=fourEcoSysInMapPtrs.size()) ){
            //    cout << "tmpIdx = " << tmpIdx << endl;
            //}

            assert( (tmpIdx>=0) && (tmpIdx<fourEcoSysInMapPtrs.size()) );
            

        ecoSysPtr = fourEcoSysInMapPtrs.at( tmpIdx );
        ecoSysPtr->accum_chunkNum( entRef.type ); //- chunk 计数器 累加
    }

}


/* ===========================================================
 *               init_earlyWeights
 * -----------------------------------------------------------
 */
void init_earlyWeights(){
    int  tmpWeight;
    int  randVal;
    earlyWeights.clear();
    for( int i=0; i<QUAD_NUM; i++ ){ //- each ecosys
        randVal = uDistribution_earlyWeights(randEngine);
        if( randVal == 0 ){
            tmpWeight = 1;
        }else if( randVal <= 3 ){
            tmpWeight = 2;
        }else if( randVal <= 5 ){
            tmpWeight = 3;
        }else{
            tmpWeight = 4;
        }
        //-------
        earlyWeights.push_back( tmpWeight );//- copy
    }
}


/* ===========================================================
 *              assign_side_ents
 * -----------------------------------------------------------
 */
void assign_side_ents( const std::vector<IntVec2> &_poses, QuadType _valA, QuadType _valB ){
    size_t idx;
    for( const auto &pos : _poses ){
        idx = pos.y*CHUNKS_PER_SECTION + pos.x;
        Ent &entRef = ents.at(idx);
        if( entRef.is_set == false ){
            entRef.is_set = true;
            (uDistribution_two(randEngine) == 0) ?
                entRef.type = _valA :
                entRef.type = _valB;
        }
    }
}


/* ===========================================================
 *              assign_mid_ent
 * -----------------------------------------------------------
 * 
 */
void assign_mid_ent( const IntVec2 &_pos, QuadType _valA, QuadType _valB, QuadType _valC ){

    size_t idx = _pos.y*CHUNKS_PER_SECTION + _pos.x;
    Ent &entRef = ents.at(idx);
    int randVal;
    if( entRef.is_set == false ){
        entRef.is_set = true;
        randVal = uDistribution_four(randEngine);
        if( randVal <= 1 ){
            entRef.type = _valA;  //- 50%
        }else if( randVal == 2 ){
            entRef.type = _valB;  //- 25%
        }else{
            entRef.type = _valC;  //- 25%
        }
    }
}



}//------------- namespace: end --------------//


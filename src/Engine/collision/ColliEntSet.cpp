/*
 * ========================= ColliEntSet.cpp ==========================
 *                          -- tpr --
 *                                        创建 -- 2019.01.15
 *                                        修改 -- 2019.01.15
 * ----------------------------------------------------------
 *    
 * ----------------------------
 */
#include "ColliEntSet.h"


#include "debug.h"


/* ===========================================================
 *             create_adds_dels_by_nineBox
 * -----------------------------------------------------------
 * -- 生成 所有方向的  adds / dels colliEnts集
 * -- 务必在 本图元帧的 所有数据都加载完毕后，再调用本函数
 */
void ColliEntSet::create_adds_dels(){

    //-- 9个 子容器 --
    addEntOffss.resize( NineBoxIdxSize );
    delEntOffss.resize( NineBoxIdxSize );

    //- 填充每一个 子容器 --
    create_adds_dels_by_nineBox( NineBoxIdx::Left_Bottom );
    create_adds_dels_by_nineBox( NineBoxIdx::Mid_Bottom );
    create_adds_dels_by_nineBox( NineBoxIdx::Right_Bottom );

    create_adds_dels_by_nineBox( NineBoxIdx::Left_Mid );
    create_adds_dels_by_nineBox( NineBoxIdx::Mid_Mid );  //- 空的
    create_adds_dels_by_nineBox( NineBoxIdx::Right_Mid );

    create_adds_dels_by_nineBox( NineBoxIdx::Left_Top );
    create_adds_dels_by_nineBox( NineBoxIdx::Mid_Top );
    create_adds_dels_by_nineBox( NineBoxIdx::Right_Top );
}


/* ===========================================================
 *             create_adds_dels_by_nineBox
 * -----------------------------------------------------------
 * -- 根据 NineBox 提供的 crawl方向，生成 对应的 adds / dels 集
 * -- 这个函数 将被 调用 9 次 
 */
void ColliEntSet::create_adds_dels_by_nineBox( NineBoxIdx _idx ){

    //-- idx==4 号 子容器为空，直接跳过 --
    if( _idx == NineBoxIdx::Mid_Mid ){
        return;
    }

    //-- adds/dels 子容器idx --
    int idx = (int)_idx;
    NineBox nb = NineBox_Idx_2_XY( _idx );

    //-- 每个 collient 需要的 位移向量 --
    MapCoord  mcOff { IntVec2{ nb.x, nb.y } };

    //-- 生成 位移后的 collient集 --
    std::set<MapCoord> neo {};
    for( const auto &i : colliEnts ){
        neo.insert( i + mcOff ); //- copy
    }

    //-- 事先把 子容器 清空 --
    addEntOffss.at(idx).clear();
    delEntOffss.at(idx).clear();
    
    //-- 填充 addEntOffss 子容器 --
    for( const auto &i : neo ){
        if( colliEnts.find(i) == colliEnts.end() ){
            addEntOffss.at(idx).insert(i);
        }
    }
    //-- 填充 delEntOffss 子容器 --
    for( const auto &i : colliEnts ){
        if( neo.find(i) == neo.end() ){
            delEntOffss.at(idx).insert(i);
        }
    }
}


/* ===========================================================
 *                   debug
 * -----------------------------------------------------------
 */

void ColliEntSet::debug(){

    /*
    cout << "centerPPos: " << centerPPos.x 
        << ", " << centerPPos.y
        << endl; //- check --
    cout << "centerCompass: " << centerCompass.x
        << ", " << centerCompass.y
        << endl; //- check --
    cout << "radius = " << radius 
        << endl; //-- check --
    */
    

    /*
    cout << "colliEnts.size() = "           << colliEnts.size()
         << "\ncolliEntMidFPoses.size() = " << colliEntMidFPoses.size()
         << "\naddEntOffss.size() = "       << addEntOffss.size()
         << "\ndelEntOffss.size() = "       << delEntOffss.size()
         << endl; //-- check --
    */

    cout << "midFPos: " << endl;
    for( auto &i : colliEntMidFPoses ){
        
        cout << "    " << i.x
            << ", " << i.y 
            << endl;
    }
    




    /*
    for( const auto &v : addEntOffss ){

        cout << "  addEntOffs.size() = " << v.size()
            << endl;
    }

    
    for( int i=0; i<9; i++ ){
        //cout << " adds.size() = " << addEntOffss.at(i).size() << endl;
        //cout << " dels.size() = " << delEntOffss.at(i).size() << endl;
        //cout << endl;
        size_t a = addEntOffss.at(i).size();
        size_t d = delEntOffss.at(i).size();
        assert( a == d );
    }
    */
    
    

}





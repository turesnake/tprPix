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
    colliEnt_adds.resize( NineBoxIdxSize );
    colliEnt_dels.resize( NineBoxIdxSize );

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
    colliEnt_adds.at(idx).clear();
    colliEnt_dels.at(idx).clear();
    
    //-- 填充 colliEnt_adds 子容器 --
    for( const auto &i : neo ){
        if( colliEnts.find(i) == colliEnts.end() ){
            colliEnt_adds.at(idx).insert(i);
        }
    }
    //-- 填充 colliEnt_dels 子容器 --
    for( const auto &i : colliEnts ){
        if( neo.find(i) == neo.end() ){
            colliEnt_dels.at(idx).insert(i);
        }
    }
}


/* ===========================================================
 *                   debug
 * -----------------------------------------------------------
 */

void ColliEntSet::debug(){

    
    cout << "centerPPos: " << centerPPos.x 
        << ", " << centerPPos.y
        << endl; //- check --
    cout << "centerCompass: " << centerCompass.x
        << ", " << centerCompass.y
        << endl; //- check --
    cout << "radius = " << radius 
        << endl; //-- check --
    

    /*
    cout << "colliEnts.size() = " << colliEnts.size()
         << "\ncolliEntCenters.size() = " << colliEntCenters.size()
         << "\ncolliEnt_adds.size() = " << colliEnt_adds.size()
         << "\ncolliEnt_dels.size() = " << colliEnt_dels.size()
         << endl; //-- check --
    
    
    for( const auto &v : colliEnt_adds ){

        cout << "  colliEnt_adds.size() = " << v.size()
            << endl;
    }

    
    for( int i=0; i<9; i++ ){
        //cout << " adds.size() = " << colliEnt_adds.at(i).size() << endl;
        //cout << " dels.size() = " << colliEnt_dels.at(i).size() << endl;
        //cout << endl;
        size_t a = colliEnt_adds.at(i).size();
        size_t d = colliEnt_dels.at(i).size();
        assert( a == d );
    }
    */

}





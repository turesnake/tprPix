/*
 * ========================= RcyPool.cpp ==========================
 *                          -- tpr --
 *                                        创建 -- 2018.11.05
 *                                        修改 -- 2018.11.05
 * ----------------------------------------------------------
 * 
 *   一个 文件地址偏移 回收池，可分别支持 定长数据 和 变长数据 的回收
 * 
 * 
 * ----------------------------
 * 依赖的其它库：
 * ----------------------------
 */
#include "RcyPool.h" 

//------------ C -------------//
#include <cassert> //- assert

//------------ CPP -------------//
#include <iomanip> //- setw
#include <iostream> //- tmp

using std::cout;
using std::endl;
using std::string;
using std::vector;
using std::deque;

using std::setw;
using std::left;
using std::right;

namespace tpr { //--------------- namespace: tpr -------------------//
namespace DB {//--------------- namespace: DB -------------------//


/* ===========================================================
 *                        init
 * -----------------------------------------------------------
 */
void RcyPool::init( bool _is_fixed, len_t _fix_blk_bytes ){

    is_pool_fixed = _is_fixed;
    if( is_pool_fixed == true ){
        //---- FIX ----//
        fix_blk_bytes = _fix_blk_bytes;

    }else{
        //---- VAR ----//
        //---- *** 注意，此处为了 对齐，我们还额外 保留了 block_0 *** ---
        //-- B_32 之前的 0，1，2，3，4 共 5个 pool 都是不使用的。
        for( int i=0; i<(BLOCK_T_NUMS + 1); i++ ){
            deque<base_t> pool;
            varPools.push_back( pool ); 
                //-- push_back 是 值复制. 效率不高
        }
    }
}

/* ===========================================================
 *                     is_empty  
 * -----------------------------------------------------------
 * -- 兼容 FIX / VAR 
 * -- 目标 rcy pools 是否为空
 * -- 目前的 varPools 下标，和 blk_idx 是完全 一一对应的
 */
bool RcyPool::is_empty( len_t _blk_bytes ){

    if( is_pool_fixed == true ){
        //---- FIX ----//
        if( fixPool.empty() == true ){
            return true;
        }else{
            return false;
        }

    }else{
        //---- VAR ----//
        u8_t blk_t_idx = Blk_bytes_2_idx( _blk_bytes );

        deque<base_t> *deq;
        //--------------------------------
        //  允许 blk_t_idx == 4 的存在，
        //  此时 strData为空，只有 strent头部
        //--------------------------------
        assert( (blk_t_idx>=4) && (blk_t_idx<=14) );
        deq = &(varPools[blk_t_idx]);
        //--------------
        if( deq->size() == 0 ){
            return true;
        }else{
            return false;
        }
    }
}

/* ===========================================================
 *                      push   [2]
 * -----------------------------------------------------------
 * -- 兼容 FIX / VAR
 * -- 向 目标 rcy pools 压入一个 元素
 */
void RcyPool::push( base_t _base, len_t _blk_bytes ){

    if( is_pool_fixed == true ){
        //---- FIX ----//
        fixPool.push_back( _base );
        all_pool_bytes += (u64_t)fix_blk_bytes;

    }else{
        //---- VAR ----//
        u8_t blk_t_idx = Blk_bytes_2_idx(_blk_bytes);
        assert( (blk_t_idx>=4) && (blk_t_idx<=14) );

        varPools[blk_t_idx].push_back( _base );
        all_pool_bytes += _blk_bytes;
    }
}


/* ===========================================================
 *                      pop   [1]
 * -----------------------------------------------------------
 * -- 兼容 FIX / VAR
 * -- 从 目标 rcy pools 提取一个元素 
 */
base_t RcyPool::pop( len_t _blk_bytes ){

    base_t base;
    if( is_pool_fixed == true ){
        //---- FIX ----//
        //-- 确保 目标 回收池 非空 --//
        assert( fixPool.empty() == false );

        all_pool_bytes -= (u64_t)fix_blk_bytes;
        //-------
        base = fixPool.front();
        fixPool.pop_front();
        return base;

    }else{
        //---- VAR ----//
        u8_t blk_t_idx = Blk_bytes_2_idx( _blk_bytes );
        assert( (blk_t_idx>=4) && (blk_t_idx<=14) );

        deque<base_t> *deq;
        deq = &(varPools[blk_t_idx]);

        //-- 确保 目标 回收池 非空 --//
        assert( deq->size() != 0 );

        all_pool_bytes -= _blk_bytes;
        //-------
        base = deq->front();
        deq->pop_front();
        return base;
    }
}

/* ===========================================================
 *                      info_var
 * -----------------------------------------------------------
 * -- 直接打印 var pool 的信息
 */
void RcyPool::info_var(){

    assert( is_pool_fixed == false );

    //-------- vals --------//
    u32_t step_1 = 4; //-- 列间距
    u32_t step_2 = 7; 
    u32_t step_3 = 8; 
    u32_t step_4 = 10; 
    u32_t step_5 = 12;

        cout << "\n--------------------- varPool ------------------------\n";

        cout << "\n";
        cout << "varPools: " << varPools.size() << "_pools, "
            << "total " << sizeof(varPools) << "-bytes, "
            << endl;
        
        cout << "\n";
        cout << "all_pool_bytes:   " << all_pool_bytes << "-bytes, "
            << endl;
        
        cout << "pool_limit:       " << pool_limit << "-bytes, "
            << endl;

        vector<string> rcyPool_heads;
        rcyPool_heads.push_back( "[_0_]" ); //-- 闲置 pool --
        rcyPool_heads.push_back( "[_2_]" ); //-- 闲置 pool --
        rcyPool_heads.push_back( "[_4_]" ); //-- 闲置 pool --
        rcyPool_heads.push_back( "[_8_]" ); //-- 闲置 pool --
        rcyPool_heads.push_back( "[_16_]" ); 
        rcyPool_heads.push_back( "[_32_]" );
        rcyPool_heads.push_back( "[_64_]" );
        rcyPool_heads.push_back( "[_128_]" );
        rcyPool_heads.push_back( "[_256_]" );
        rcyPool_heads.push_back( "[_512_]" );
        rcyPool_heads.push_back( "[_1024_]" );
        rcyPool_heads.push_back( "[_2048_]" );
        rcyPool_heads.push_back( "[_4096_]" );
        rcyPool_heads.push_back( "[_8192_]" );
        rcyPool_heads.push_back( "[_16384_]" );

        cout << "\n";
        cout << "  " 
            << left   
            << setw(step_4) << "rcyPool"
            << right
            << setw(step_3) << "Nums"
            << "  "
            << left
            << "Base"
            << endl;

        for( size_t i=0; i<(size_t)(varPools.size()); i++ ){

            deque<base_t> *deqp;
            deqp = &varPools[i];

            cout << "  " 
                << left   
                << setw(step_4) << rcyPool_heads[i]
                << right
                << setw(step_3) << deqp->size()
                << "  ";
            
            for( size_t j=0; j< (size_t)(deqp->size()); j++ ){
                cout << (*deqp)[j] << ", ";
            }

            cout << endl;
        }
}


/* ===========================================================
 *                      info_fix
 * -----------------------------------------------------------
 * -- 直接打印 fix pool 的信息
 */
void RcyPool::info_fix(){

    assert( is_pool_fixed == true );

    //-------- vals --------//
    u32_t step_1 = 4; //-- 列间距
    u32_t step_2 = 7; 
    u32_t step_3 = 8; 
    u32_t step_4 = 10; 
    u32_t step_5 = 12;

        cout << "\n--------------------- fixPool ------------------------\n";

    cout << "\n";
    cout << "all_pool_bytes:   " << all_pool_bytes << "-bytes, "
        << endl;
        
    cout << "pool_limit:       " << pool_limit << "-bytes, "
        << endl;


    cout << "\n";
    cout << "  " 
        << left   
        << setw(step_4) << "blk_bytes"
        << right
        << setw(step_3) << "Nums"
        << "  "
        << left
        << "Base"
        << endl;


    cout << "  " 
        << left   
        << setw(step_4) << fix_blk_bytes
        << right
        << setw(step_3) << fixPool.size()
        << "  ";


    for( size_t j=0; j< (size_t)(fixPool.size()); j++ ){
        cout << fixPool[j] << ", ";
    }

    cout << endl;

    

}



}//------------------- namespace: DB ------------------------//
}//------------------- namespace: tpr ------------------------//

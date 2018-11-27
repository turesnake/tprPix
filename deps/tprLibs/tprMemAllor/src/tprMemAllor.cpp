/*
 * ====================== tprMemAllor.cpp =======================
 *                          -- tpr --
 *                                        创建 -- 2018.11.24
 *                                        修改 -- 2018.11.24
 * ----------------------------------------------------------
 *    简易的 内存分配器
 *    --------
 *    暂时只适合 定长数据集 的分配
 * ----------------------------
 */
#include "tprMemAllor.h" 

//------------ C -------------//
#include <string.h> //- memcpy
#include <cassert> //- assert
#include <errno.h> //- errno

//-------------------- CPP --------------------//
#include <iostream>


using std::string;
using std::cout;
using std::endl;


namespace tpr { //-------------- namespace : tpr ------------//

using MemAllor::eid_t;
using MemAllor::len_t;
using MemAllor::ptr_t;

using MemAllor::MemEnt;

/* ===========================================================
 *                         init
 * -----------------------------------------------------------
 */
void tprMemAllor::init(){

    mem.clear();
    busy_hash.clear();
    idle_pool.clear();

    //---------------------//
    //     分配 mem
    //---------------------//
    mem.resize( ent_bytes * ent_nums );

    //------------------------------------//
    //  将 mem 中所有ent，存入 idle pool 中
    //------------------------------------//
    ptr_t headp = (void*)&mem[0];
    MemEnt  ment; //- tmp

    if( is_limit_id_mod == true ){
        
        for( eid_t i = 0; i<ent_nums; i++ ){
            ment.id = i+1; //- id号 从 1 分配起
            ment.ptr = (void*)&mem[ i * ent_bytes ];
            idle_pool.push_back( ment );
        }

    }else{
        //...
        assert(0);
    }
}

/* ===========================================================
 *                      insert
 * -----------------------------------------------------------
 */
MemAllor::eid_t tprMemAllor::insert( const void *_buf, MemAllor::len_t _len ){

    //--- 内存池用满即报错。暂不设置处理方式 ---
    assert( idle_pool.size() > 0 );

    assert( _len == ent_bytes );

    //--- 从 idle_pool 头部取出一个 ent，存入 busy_pool ---
    MemEnt ment;
    ment = idle_pool.front();
    idle_pool.pop_front();
    //-------
    busy_hash.insert( {  ment.id, ment.ptr } );

    //--- 正式存入 ent data ---//
    memcpy( ment.ptr,
            _buf,
            ent_bytes
            );

    return ment.id;
}

/* ===========================================================
 *                      erase
 * -----------------------------------------------------------
 * -- 并不清除 ent 中的 旧数据
 */
int tprMemAllor::erase( MemAllor::eid_t _id ){

    assert( check_id(_id) == true );

    MemEnt ment;
    ment.id = _id;
    ment.ptr = busy_hash_find( _id );

    //-- 存入 idle_pool --
    idle_pool.push_back( ment );

    //-- 从 busy_pool 中删除 --
    busy_hash.erase( _id );

    return 0; //- 表示清除成功，暂无意义
}


/* ===========================================================
 *                      read
 * -----------------------------------------------------------
 */
ssize_t tprMemAllor::read( MemAllor::eid_t _id, void *_buf, size_t _len ){

    assert( check_id(_id) == true );
    assert( _len >= ent_bytes );

    //--- 正式读取 ent data ---
    memcpy( _buf,
            busy_hash_find( _id ),
            ent_bytes
            );

    return (ssize_t)ent_bytes; //- 暂无大用
}

/* ===========================================================
 *                      write
 * -----------------------------------------------------------
 */
ssize_t tprMemAllor::write( MemAllor::eid_t _id, const void *_buf, MemAllor::len_t _len ){

    assert( check_id(_id) == true );
    assert( _len == ent_bytes );

    //--- 正式写入 新 ent data ---
    memcpy( busy_hash_find( _id ),
            (void*)_buf,
            ent_bytes
            );

    return (ssize_t)ent_bytes; //- 暂无大用
}


/* ===========================================================
 *                      check_id
 * -----------------------------------------------------------
 */
MemAllor::ptr_t tprMemAllor::busy_hash_find( MemAllor::eid_t _id ){

    auto pair = busy_hash.find( _id );
    assert( pair != busy_hash.end() );
    return pair->second;
}

/* ===========================================================
 *                      check_id
 * -----------------------------------------------------------
 */
bool tprMemAllor::check_id( MemAllor::eid_t _id ){

    if( is_limit_id_mod == true ){

        if( (_id>=1) && (_id<=ent_nums+1) ){
            return true;
        }else{
            return false;
        }

    }else{
        //...
        assert(0);
        return false;
    }
}

/* ===========================================================
 *                      info
 * -----------------------------------------------------------
 */
void tprMemAllor::info(){

    cout << "========== tprMemAllor.info() ==========" << endl;

    cout << "\nmem.size()       = " << mem.size() << " -bytes"
         << "\nbusy_hash.size() = " << busy_hash.size()
         << "\nidle_pool.size() = " << idle_pool.size() 
         << endl;

    cout << "\nent_bytes = " << ent_bytes << " -bytes"
         << "\nent_nums  = " << ent_nums 
         << endl;

    cout << "is_limit_id_mod: ";
    if( is_limit_id_mod == true ){
        cout << "true";
    }else{
        cout << "false";
    }
    cout << endl;

}




} //-------------- namespace : tpr end --------------//


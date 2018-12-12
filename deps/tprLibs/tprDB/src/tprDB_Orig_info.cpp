/*
 * ==================== tprDB_Orig_info.cpp =================
 *                          -- tpr --
 *                                        创建 -- 2018.11.20
 *                                        修改 -- 2018.11.20
 * ----------------------------------------------------------
 * 
 *   tprDB_Orig 数据库 信息打印
 * 
 * ----------------------------
 * 依赖的其它库：
 *       wrapFuncs 
 *       tprFileSys
 *       tprFileModeT
 *       tprCrypto
 *       tprDataType
 *       tprBinary (临时)
 * ----------------------------
 */
#include "tprDB_Orig.h"

//------------ C -------------//
#include <string.h> //- memcpy
#include <cassert> //- assert
#include <errno.h> //- errno

//------------ CPP -------------//
#include <sstream> //- stringstream
#include <deque>
#include <vector>
#include <unordered_map>
#include <sstream>
#include <iomanip> //- setw

using std::cout;
using std::endl;
using std::string;
using std::deque;
using std::vector;
using std::unordered_map;
using std::stringstream;

using std::to_string;

using std::setw;
using std::left;
using std::right;

namespace tpr { //--------------- namespace: tpr -------------------//

using DB::BlockState;

using DB::BlkState_2_idx;
using DB::idx_2_BlkState;

using DB::varEntHead_f2m;
using DB::varEntHead_m2f;
using DB::fixEntHead_f2m;
using DB::fixEntHead_m2f;

using DB::mem_Block;
using DB::mem_Ent;

using DB::find_suited_Blk_bytes;

using DB::aligns_2_Blk_bytes;

using DB::file_EntHead;
using DB::file_FixOptions;

using DB::eid_t;
using DB::len_t;
using DB::base_t;
using DB::FILE_ENT_HEAD_LEN;
using DB::FILE_FIXOPTION_LEN;

using DB::tprDB_File_Head;

using DB::mk_options;


namespace{//------ namespace [static] ----------//

    //-------- vals --------//
    unsigned int step_1 = 8; //-- 列间距
    unsigned int step_2 = 8; 
    unsigned int step_3 = 10; 
    unsigned int step_4 = 10; 
    unsigned int step_5 = 12; 

    deque<eid_t> ids;


    //-------- funcs -------//
    std::string _memblk_2_str( DB::mem_Block _mb, u32 _step_1, u32 _step_2, u32 _step_3 );
    std::string _bool_2_str( bool _b );

    void _build_ids( unordered_map<eid_t, DB::mem_Ent> *_hashp );



}//--------- namespace [static] ------------//


/* ===========================================================
 *                     info
 * -----------------------------------------------------------
 * -- 测试用。 打印 数据库实例的 一切信息。
 */
void tprDB_Orig::info(){

    bool is_print_file = false;
    bool is_print_mem = true;
    bool is_print_rcyPool = true;
    bool is_print_cache = true;

    //-------------------------//
    //  原有的 id vector 已经没了
    //  临时制作一个 deque 来代替 
    //-------------------------//
    _build_ids( &mem_hash );

    //-------------------------
    if( is_print_file ){
        cout << "\n--------------------- FILE ------------------------\n";
        cout << "\nDB_PATH = " << path_file 
             << "\nDB_FD   = " << fd_file
             << endl;
    }

    if( is_print_mem ){
        cout << "\n---------------------- MEM ------------------------\n";

        mem_Ent *mEntp;
        char str[9]; //-- 预览模式只显示字符串的 头 8 字节
        string str_tmp;
        //-----------

        cout << "mem_hash: " << mem_hash.size() << "_ent,  "
            << "total " << sizeof(mem_hash) << "-bytes, "
            << endl;

        cout << "\n";
        cout << "max_id:   " << max_id 
            << endl;

        cout << "next_base:    " << next_base 
            << endl;

        cout << "all_data_bytes: " << all_data_bytes << "-bytes, "
            << endl;

        cout << "all_blk_bytes: " << all_blk_bytes << "-bytes, "
            << endl;

        //------

        cout << "\n  "
            << left
            << setw(step_1) << "ID"
            << right
            << setw(step_2) << "Len"
            << setw(step_4) << "Base"
            << setw(step_3) << "Block"
            << setw(step_2) << "State"
            << setw(step_3) << "rd/wr"
            << setw(step_2) << "inMem"
            << setw(step_2) << "Dirty"
            << endl;

        if( is_fix == false ){
            cout << "   "        << "VAR" 
                << endl;
        }
        
        string rdwr_times;
        for( auto id : ids ){
            mEntp = mem_hash_find( id ); 
            //--------
            rdwr_times = to_string(mEntp->rd_times);
            rdwr_times += '/';
            rdwr_times += to_string(mEntp->wr_times);

            //--- 打印 基础信息 ---
            cout << "  "
                << left
                << setw(step_1) << id
                << right
                << setw(step_2) <<mEntp->len
                << _memblk_2_str(mEntp->memblk, step_4, step_3, step_2)
                << setw(step_3) << rdwr_times
                << setw(step_2) << _bool_2_str(mEntp->is_data_in_mem)
                << setw(step_2) << _bool_2_str(mEntp->is_dirty)
                << "   ";
            //--- 打印 字符串 本体 --

            if( (mEntp->is_data_in_mem == false) || (is_fix == true) ){
                cout << "\"\"";

            }else{
                if( mEntp->len <= 8 ){
                    memcpy( (void*)str,
                            (void*)&(mEntp->data[0]),
                            (size_t)(mEntp->len)
                            );
                    str[mEntp->len] = 0; //-- 手动补上 尾后0
                    str_tmp = str;
                    cout << "\"" << str_tmp << "\"";
                }else{
                    memcpy( (void*)str,
                            (void*)&(mEntp->data[0]),
                            8
                            );
                    str[8] = 0; //-- 手动补上 尾后0
                    str_tmp = str;
                    cout << "\"" << str_tmp << "...";
                }
            }
            
            cout << endl;
        }
    }

    if( is_print_rcyPool ){
        if( is_fix == true ){
            rcy_pools.info_fix();
        }else{
            rcy_pools.info_var();
        }
    }


    if( is_print_cache ){
        cout << "\n--------------------- cache ------------------------\n";

        cout << "\n"
            << "cache_history:       " << cache_history.size() << "_ent, "
            << "total " << sizeof(cache_history) << "-bytes, "
            << endl;  

        cout << "cache_history_iters: " << cache_history_iters.size() << "_ent, "
            << "total " << sizeof(cache_history_iters) << "-bytes, \n"
            << endl;

        cout << "all_cache_bytes:   " << all_cache_bytes << "-bytes, "
            << endl; 

        cout << "cache_limit:       " << cache_limit << "-bytes, "
            << endl;

        //--------
        cout << "\n";
        cout << "[old] --> [new]:" << endl;

        size_t nums = 0;
        if( cache_history.size() == 0 ){
            cout << "... empty ..." 
                << endl;

        }else{
            for( auto i : cache_history ){
                cout << i;
                nums++;
                if( nums < cache_history.size() ){
                    cout << " --> ";
                }
            }
            cout << endl;
        }
    }


    cout << endl;
}



namespace{//------ namespace [static] ----------//


/* ===========================================================
 *                     _build_ids
 * -----------------------------------------------------------
 * -- 根据 文件地址偏移，对 数据库 所有 id 进行排序
 */
void _build_ids( unordered_map<eid_t, DB::mem_Ent> *_hashp ){

    //-- 将所有 id 存入 ids ---
    ids.clear();
    for( auto it=_hashp->begin(); it!=_hashp->end(); ++it ){
        ids.push_front( it->first );
    }

    //-- 根据 每个id 的 base，重排序 ids ---
    std::sort( ids.begin(), ids.end(),
        [_hashp]( eid_t _a, eid_t _b )->bool
        {
            mem_Ent *ap = &(*_hashp)[_a];
            mem_Ent *bp = &(*_hashp)[_b];
            return ( ap->memblk.base < bp->memblk.base );
        }
    );
}

/* ===========================================================
 *                    _memblk_2_str
 * -----------------------------------------------------------
 * -- 陈列 memblk 的信息
 */
string _memblk_2_str( DB::mem_Block _mb, u32 _step_1, u32 _step_2, u32 _step_3 ){

    stringstream ss;
    //------
    ss << setw(_step_1) << _mb.base;
    ss << setw(_step_2) << _mb.blk_bytes; 
    
    if( (u32)_mb.blk_state_idx == 1 ){
        ss << setw(_step_3) << "Idle";
    }else{
        ss << setw(_step_3) << "Busy";
    }
    return ss.str();
}

/* ===========================================================
 *                    _bool_2_str
 * -----------------------------------------------------------
 */
string _bool_2_str( bool _b ){
    if( _b == true ){
        return "true";
    }else{
        return "false";
    }
}



}//--------- namespace [static] ------------//
}//------------------- namespace: tpr ------------------------//

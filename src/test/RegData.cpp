#include "RegData.h"


using std::cout;
using std::endl;

using tpr::DB::len_t;
using tpr::DB::base_t;
using tpr::DB::eid_t;
using tpr::DB::DATA_T;


/* ===========================================================
 *                     insert
 * -----------------------------------------------------------
 */
eid_t RegData::insert( const RegData_User& _data ){

    //-- insert name ---
    eid_t sid;
    sid = dbs.insert( false, _data.name.c_str(), _data.name.size() );

    //-- build rdb ---
    RegData_DB rdb;
    rdb.name = sid;
    rdb.id   = _data.id;

    //-- insert rdb ---
    return dbs.insert( true, &rdb, sizeof(rdb) );
}


/* ===========================================================
 *                     read 
 * -----------------------------------------------------------
 */
ssize_t RegData::read( eid_t _id, RegData_User *_rdu ){

    ssize_t rs;

    //--- 读取 fix --
    RegData_DB rdb;
    rs = dbs.read( true, _id, &rdb, sizeof(rdb), false );
    assert( rs != -1 );

    //--- 读取 var --
    ssize_t name_len;
    rs = dbs.get_entData_len( false, rdb.name );
    assert( rs != -1 );
    name_len = rs;

    _rdu->name.resize( name_len );
    rs = dbs.read( false, rdb.name, (void*)(_rdu->name.c_str()), name_len, false );
    assert( rs != -1 );

    //--- 合成 RegData_User --
    _rdu->id = rdb.id;

    return 0; //-- 暂无用
}


/* ===========================================================
 *                     write 
 * -----------------------------------------------------------
 * -- return
 *      成功，返回 0
 *      出错，返回 -1
 */
ssize_t RegData::write( eid_t _id, const RegData_User& _data, const RegData_Mask& _mask ){

    ssize_t rs;

    if( (!_mask.is_name) && (!_mask.is_id) ){
        return 0; //-- 暂时无用
    }

    //-- read fix --
    RegData_DB rdb;
    rs = dbs.read( true, _id, &rdb, sizeof(rdb), true ); //-- 记得开启 fast 位
    assert( rs != -1 );

    //-- write str --
    if( _mask.is_name == true ){
        rs = dbs.write( false, rdb.name, _data.name.c_str(), _data.name.size()  );
        assert( rs ==  _data.name.size() );
    }

    //-- write fix --
    if( _mask.is_id == true ){
        rdb.id = _data.id;
        rs = dbs.write( true, _id, &rdb, sizeof(rdb) );
    }   
    return 0; //-- 暂无用
}





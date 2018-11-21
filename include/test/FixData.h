/*
 * ========================= FixData.h ===================
 *                          -- tpr --
 *                                        创建 -- 2018.11.20
 *                                        修改 -- 2018.11.20
 * ----------------------------------------------------------
 * 
 *   Fix DB 的包裹类。
 *   很简单，甚至不需要 cpp 文件
 * 
 * ----------------------------
 */
#ifndef _TPR_FIXDATA_H_
#define _TPR_FIXDATA_H_

#include <iostream>
#include <string>

//-------------------- tpr --------------------//
#include "tprDB.h" 
#include "nocopyable.h" 

//-------------------- SELF --------------------//
#include "global.h" 



//-- 在 Fix DB 中，只需要一个 数据结构 -- 
//-- 一个数据结构要做两版 struct，一版给 db, 一版给用户
struct FixData_DB{
    tpr::DB::T_U32   id   {0};  //- DATA_T::U32
    tpr::DB::T_I32   type {0};  //- DATA_T::I32
}; 

//-- 掩码，write 专用
//  true   表示要写入
//  false  表示不要写入
struct FixData_Mask{
    bool is_id;
    bool is_type;
};


class FixData : tpr::nocopyable{
public:
    //--- 构造函数 ---//
    FixData()
        {
            dbs.init( path_data, "2018.1120.Fix",
                        {   tpr::DB::DATA_T::U32,
                            tpr::DB::DATA_T::U32,
                        },
                        sizeof(FixData_DB)
                        );
        }

    //--- 析构函数 ---//
    ~FixData() = default;


    // -< 增 >-
    inline tpr::DB::eid_t insert( const FixData_DB& _data ){
        return dbs.insert( true, &_data, sizeof(_data) );
    }

    // -< 删 >-
    inline int erase( tpr::DB::eid_t _id ){
        return dbs.erase( _id );
    }

    // -< 查 >-
    inline ssize_t read( tpr::DB::eid_t _id, FixData_DB *_rdu ){
        return dbs.read( true, _id, _rdu, sizeof(_rdu) );
    }

    // -< 改 >-
    inline ssize_t write( tpr::DB::eid_t _id, 
                    const FixData_DB& _data, 
                    const FixData_Mask& _mask ){

        if( (!_mask.is_id) && (!_mask.is_type) ){
            return 0; //-- 暂时无用
        }
        return dbs.write( true, _id, &_data, sizeof(_data) );
    }

    // -< 冲 >-
    inline void flush(){
        dbs.flush();
    }

    inline void info( bool _fix, bool _var ){
        dbs.info( _fix, _var );
    }


private:
    tpr::tprDB  dbs{ false };
    
};



#endif

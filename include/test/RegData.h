/*
 * ========================= RegData.h ===================
 *                          -- tpr --
 *                                        创建 -- 2018.11.20
 *                                        修改 -- 2018.11.20
 * ----------------------------------------------------------
 * 
 *   Reg DB 的包裹类。
 *   需要实现者 手动 制作几个 包裹函数。
 *   但是 工作量不大。
 * 
 * ----------------------------
 */
#ifndef _TPR_REGDATA_H_
#define _TPR_REGDATA_H_

#include <iostream>
#include <string>

//-------------------- tpr --------------------//
#include "tprDB.h" 
#include "nocopyable.h" 

//-------------------- SELF --------------------//
#include "global.h" 

//-- 一个数据结构要做两版 struct，一版给 db, 一版给用户
struct RegData_DB{
    tpr::DB::T_STRID name {0};  //- DATA_T::STRID
    tpr::DB::T_U64   id   {0};  //- DATA_T::I64    
}; 

struct RegData_User{
    std::string  name {0};
    u64          id   {0};
};

//-- 掩码，write 专用
//  true   表示要写入
//  false  表示不要写入
struct RegData_Mask{
    bool is_name;
    bool is_id;
};


class RegData : tpr::nocopyable{

public:
    //--- 构造函数 ---//
    RegData()
        {
            dbs.init( path_data, "2018.1120",
                        {   tpr::DB::DATA_T::STRID,
                            tpr::DB::DATA_T::U64,
                        },
                        sizeof(RegData_DB)
                        );
        }

    //--- 析构函数 ---//
    ~RegData() = default;


    // -< 增 >-
    tpr::DB::eid_t insert( const RegData_User& _data );

    // -< 删 >-
    inline int erase( tpr::DB::eid_t _id ){
        return dbs.erase( _id );
    }

    // -< 查 >-
    ssize_t read( tpr::DB::eid_t _id, RegData_User *_rdu );

    // -< 改 >-
    ssize_t write( tpr::DB::eid_t _id, 
                    const RegData_User& _data, 
                    const RegData_Mask& _mask );

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

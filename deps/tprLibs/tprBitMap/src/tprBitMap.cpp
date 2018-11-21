/*
 * ========================= tprBitMap.cpp ==========================
 *                          -- tpr --
 *                                        创建 -- 2018.10.11
 *                                        修改 -- 2018.10.11
 * ----------------------------------------------------------
 * 
 * ----------------------------
 */
#include "tprBitMap.h" 

//------------ C -------------//
#include <string.h> //- memcpy

#include <cassert> //- assert

//------------ CPP -------------//

//------------ self -------------//


using std::cout;
using std::endl;
using std::string;

namespace tpr { //--------------- namespace: tpr -------------------//

/* ===========================================================
 *                       creat
 * -----------------------------------------------------------
 * return:
 *     若返回值 == _len, 表示 载入成功 (目前暂无 其他可能)
 */
size_t BitMap::creat( size_t _len, u8 _bit ){

    bitmap.clear();      //- 清空所有元素
    bitmap.resize(_len, _bit); //- 扩容为 目标大小,顺带将所有元素 初始化

    return bitmap.size();
}


/* ===========================================================
 *                       load
 * -----------------------------------------------------------
 * return:
 *     若返回值 == _len, 表示 载入成功 (目前暂无 其他可能)
 */
size_t BitMap::load( u8 *_buf, size_t _len ){

    this->creat( _len, 0); 
    memcpy( (void*)&(bitmap[0]),
            (void*)_buf,
            _len 
            );

    return bitmap.size();
}

/* ===========================================================
 *                       save
 * -----------------------------------------------------------
 * param[in,out]: _lenp -- "值-引用“类型，调用者需要传入 目标buf 的可用字节数
 *                               函数返回时，函数再传回 实际写入的 字节数
 * return:
 *     若返回 0, 表示 运行正确 （目前暂无错误）
 */
int BitMap::save( u8 *_buf, size_t *_lenp ){

    string err_info = "BitMap::save(): ";

    //---------------------------//
    //   检测 参数 _lenp 指向的值 是否足够大
    if( *_lenp < bitmap.size() ){
        cout << err_info << "ERROR. _len < bitmap.size() " 
            << endl;
        assert(0); //-- 调用者代码问题，直接报错终止
    }

    //---------------------------//
    memcpy( (void*)_buf,
            (void*)&(bitmap[0]),
            bitmap.size()
            );
    
    *_lenp = bitmap.size(); //-- 回传 实际存入 字节数
    return 0;
}


/* ===========================================================
 *                       setbit
 * -----------------------------------------------------------
 * return:
 *     若返回 0, 表示 运行正确 （目前暂无错误）
 */
int BitMap::setbit( size_t _idx, u8 _bit ){

    string err_info = "BitMap::setbit(): ";

    size_t idx_lst = bitmap.size() - 1; //- 最后一位 单元的 下标

    //---------------------------//
    //   检测 参数 _idx 是否越界
    if( _idx > idx_lst ){
        cout << err_info << "ERROR. "
            << "_idx = " << _idx << ". _idx > bitmap.size() " 
            << endl;
        assert(0); //-- 调用者代码问题，直接报错终止
    }

    //---------------------------//
    bitmap[_idx] = _bit; 

    return 0;
}


/* ===========================================================
 *                       setbits
 * -----------------------------------------------------------
 * return:
 *     若返回 0, 表示 运行正确 （目前暂无错误）
 */
int BitMap::setbits( size_t _idx_fst, size_t _idx_lst, u8 _bit ){

    string err_info = "BitMap::setbit(): ";

    size_t idx_lst = bitmap.size() - 1; //- 最后一位 单元的 下标

    //---------------------------//
    //         检测 参数 
    //---------------------------//
    if( _idx_fst > idx_lst ){
        cout << err_info << "ERROR. "
            << "_idx_fst = " << _idx_fst << ". _idx_fst > bitmap.size() " 
            << endl;
        assert(0); //-- 调用者代码问题，直接报错终止
    }

    if( _idx_lst > idx_lst ){
        cout << err_info << "ERROR. "
            << "_idx_lst = " << _idx_lst << ". _idx_lst > bitmap.size() " 
            << endl;
        assert(0); //-- 调用者代码问题，直接报错终止
    }

    if( _idx_fst > _idx_lst ){
        cout << err_info << "ERROR. "
            << "_idx_fst > _idx_lst " 
            << endl;
        assert(0); //-- 调用者代码问题，直接报错终止
    }

    //---------------------------//
    //       正式 set 数据
    //---------------------------//
    for( size_t i=_idx_fst; i<=_idx_lst; i++ ){
        bitmap[i] = _bit;
    }

    return 0;
}


/* ===========================================================
 *                       next
 * -----------------------------------------------------------
 * -- 从 下标 _idx_corrent开始，向后查找 第一个 匹配 _bitmask 的单元
 * -- _bitmask 比配原则：只要某一 单元的 _bitmask 位 都置1， 这个单元 就是 符合的
 * 
 * param: _idx_corrent -- 当前已查阅的 下标号。
 *                         若 值为 -1, 表示 需要从 bitmap 0号下标开始查阅
 *                         若 值 = idx_lst, 此时已指向 最后一元素，
 *                                              函数将 直接终止。
 *                         若 (值 >= 0)||(值 < idx_lst), 表示 从 (_idx_corrent+1) 开始查阅
 *                         
 * 
 * return:
 *     若 返回值 >= 0, 说明找到了 匹配单元，并返回了其 下标号／idx
 *     若 返回值 == -1，说明 没找到 匹配单元
 */
size_t BitMap::next( ssize_t _idx_corrent, u8 _bitmask ){

    string err_info = "BitMap::next(): ";

    size_t idx; //-- 需要查阅的 bitmap 下标
    size_t idx_lst = bitmap.size() - 1; //- 最后一位 单元的 下标

    //---------------------------//
    //         检测 参数 
    //---------------------------//
    if( _idx_corrent == -1 ){
        idx = 0; //-- 从 0 开始查阅
    }else if( _idx_corrent == idx_lst ){
        return -1; //-- 已经是 最后一个单元了，返回 “没找到”
    }else if( (_idx_corrent >= 0) || (_idx_corrent < idx_lst) ){
        idx = _idx_corrent + 1;
    }else{
        cout << err_info << "ERROR. "
            << "_idx_corrent out of range " 
            << endl;
        assert(0); //-- 调用者代码问题，直接报错终止
    }

    //---------------------------//
    //       开始 查找
    //---------------------------//
    bool is_find = false; 
    for( ; idx <= idx_lst; idx++ ){
        if( (bitmap[idx] & _bitmask) == _bitmask ){
            is_find = true;
            break; //-- 找到 第一个 匹配单元，退出循环
        }
    }

    if( is_find == false ){
        return -1;  //-- 没找到 匹配单位
    }

    return idx; //-- 返回 匹配单元 的 下标
}











}//------------------- namespace: tpr ------------------------//


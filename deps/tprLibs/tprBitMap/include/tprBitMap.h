/*
 * ========================= tprBitMap.h ==========================
 *                          -- tpr --
 *                                        创建 -- 2018.10.11
 *                                        修改 -- 2018.10.11
 * ----------------------------------------------------------
 * 
 *   位图的 创建／修改／查阅／清空 
 *   通常使用 位图来 表示 大型文件的 数据块 状态
 *   目前版本 以一种很粗旷的方式去实现 位图映射：
 *       1-byte(位图单元) : 1KB(数据)
 *   也就是说，本BitMap 中所说的 一个单元 ,其实 是一个 字节（可以容纳 8位信息）
 * ----------------------------
 * 依赖的其它库：
 *       tprDataType
 * ----------------------------
 */

#ifndef _TPR_BIT_MAP_H_
#define _TPR_BIT_MAP_H_

//------------ C -------------//
#include <unistd.h> //- size_t


//------------ CPP -------------//
#include <string>
#include <vector> 
//#include <iostream> 
//------------ SELF -------------//
#include "tprDataType.h" 


namespace tpr { //--------------- namespace: tpr -------------------//

//------------ self -------------//


//--------------------------------//
// bitmap 中，"1-bit／1个单元" 实为 1-byte,所以可以容纳 8位信息
// 可以用 以下 宏 来 合成 bitMask, 并以此来 访问 单位 bit 的内容
//
//      u8_t bitMask = (BIT_SEQ_1 | BIT_SEQ_7);
//       
#define BIT_SEQ_0 0x01
#define BIT_SEQ_1 0x02
#define BIT_SEQ_2 0x04
#define BIT_SEQ_3 0x08
#define BIT_SEQ_4 0x10
#define BIT_SEQ_5 0x20
#define BIT_SEQ_6 0x40
#define BIT_SEQ_7 0x80
//-------
// 由于 本库中，单元 上 每一位的数据 代表的含义是 未定义的。
// 所以推荐，调用者 自行包裹一个 h文件。


/* ===========================================================
 *                        MsgIO 
 * -----------------------------------------------------------
 */
class BitMap{
    //============== friend ==============
public:
    //-- 构造函数 --//
    BitMap( size_t _block_size = 1024 )
        :
        block_size(_block_size)
        {}


    inline void set_block_size( size_t _block_size )
    {
        block_size = _block_size;
    }

    inline size_t get_block_size() const
    {
        return block_size;
    }


    size_t creat( size_t _len, u8_t _bit );    //-- 新建 对象的 bitmap，每个 单元 都初始化为 _bit  
    size_t load( u8_t *_buf, size_t _len );    //-- 从外部读取一个 bitmap，存入本地 bitmap 中
    int save( u8_t *_buf, size_t *_lenp ); //-- 将本地 bitmap，存储到 外部 某 buf 中。

    int setbit( size_t _idx, u8_t _bit );   //-- 设置 下标_idx 对应的 bit 值为 _bit
    int setbits( size_t _idx_fst, size_t _idx_lst, u8_t _bit ); //-- 设置 下标 [fst, lst] 区间的 bits，值为 _bit

    size_t next( ssize_t _idx_corrent, u8_t _bitmask ); //-- 从 下标 _idx_corrent开始，向后查找 第一个 匹配 _bitmask 的单元
                                                            




private:

    size_t block_size; //-- 位图颗粒度，位图中，一个单元 代表 数据中多少 字节的数据。
                       //-- 默认为 1024-字节

    std::vector<u8_t> bitmap; //-- 目标位图的 本地缓存。



};





}//------------------- namespace: tpr ------------------------//

#endif

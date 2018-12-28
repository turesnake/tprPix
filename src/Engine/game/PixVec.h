/*
 * ========================= PixVec.h ==========================
 *                          -- tpr --
 *                                        创建 -- 2018.11.28
 *                                        修改 -- 2018.11.28
 * ----------------------------------------------------------
 *    以像素为单位的 vec 向量。
 * ----------------------------
 */
#ifndef _TPR_PIX_VEC2_H_
#define _TPR_PIX_VEC2_H_


struct PixVec2{
    int  x {0}; 
    int  y {0};
};
inline bool is_equal( const PixVec2 &_a, const PixVec2 &_b ){
    return ( ((_a.x==_b.x)&&(_a.y==_b.y)) );
}



struct PixVec3{
    int  x  {0}; 
    int  y  {0};
    int  z  {0};
};
inline bool is_equal( const PixVec3 &_a, const PixVec3 &_b ){
    return ( ((_a.x==_b.x)&&(_a.y==_b.y)&&(_a.z==_b.z)) );
}




#endif


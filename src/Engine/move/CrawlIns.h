/*
 * ========================= CrawlIns.h ==========================
 *                          -- tpr --
 *                                        创建 -- 2019.01.05
 *                                        修改 -- 2019.01.05
 * ----------------------------------------------------------
 *    像 Crawl模块 发送的 移动指令
 *    旧名为 CrossState,基于十字键的  位移运动状态
 * ----------------------------
 */
#ifndef _TPR_CRAWL_INS_H_
#define _TPR_CRAWL_INS_H_

//- 十字键 x／y轴 状态：
//  -1 -- 左／下
//   0 -- 无
//   1 -- 右／上
struct CrawlIns{
    int  x  {0};
    int  y  {0};
};


inline CrawlIns ci_right      {  1,  0 };
inline CrawlIns ci_right_up   {  1,  1 };
inline CrawlIns ci_up         {  0,  1 };
inline CrawlIns ci_up_left    { -1, 1 };
inline CrawlIns ci_left       { -1, 0 };
inline CrawlIns ci_left_down  { -1, -1 };
inline CrawlIns ci_down       {  0, -1 };
inline CrawlIns ci_down_right {  1, -1 };


enum class DIRECTION{
    Idle, //- 无方向
    Left,
    Right,
    Up,
    Down
};


#endif 


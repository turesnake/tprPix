/*
 * ========================= windowConfig.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.03.15
 *                                        MODIFY --
 * ----------------------------------------------------------
 *   配置 参数 [视窗]
 *   分离出来，以便快速切换
 * ----------------------------
 */
#ifndef _TPR_WINDOW_CONFIG_H_
#define _TPR_WINDOW_CONFIG_H_

//-- window 显示尺寸（像素）--
//   拉升窗口将会改变这个值
//#define SCR_WIDTH  880 //- 早期版，游戏标配窗口大小
//#define SCR_HEIGHT  660

#define SCR_WIDTH  1600
#define SCR_HEIGHT 1200


//#define SCR_WIDTH  1280  //- 为了配合生态建设 设置的窗口大小。
//#define SCR_HEIGHT  1280

//-- 游戏工作尺寸（游戏像素）--
//   由于是 pixel游戏，所以这个值会很小。
//   这个值 也是 Camera 的尺寸
//-- 目前版本中，拉升窗口并不会改变这个值。（后期应实现 自动匹配）
//-- 请尽量确保此变量 与 SCR_WIDTH／SCR_HEIGHT 等比例
//--:
//#define WORK_WIDTH  220 //- 早期版，游戏标配窗口大小
//#define WORK_HEIGHT 165

#define WORK_WIDTH  400
#define WORK_HEIGHT 300


//#define WORK_WIDTH  1280  //- 在 1280*1280 尺寸下，能显示一张完整的 section(256,256)
//#define WORK_HEIGHT 1280  //- 仅用在 生态群落 建设阶段


//-- 是否开启 全屏模式 --
inline bool IS_FULL_SCREEN = false; 



#endif 


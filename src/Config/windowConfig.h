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


//  SCR_WIDTH
//  SCR_HEIGHT
//--- window 显示尺寸（像素）---
//   拉升窗口将会改变这个值
// ----- z注意 ------
// 在 mac 这种 高分屏中， 实际分配的 window像素尺寸 可能和 这组值不同
// 比如，是这组值的两倍。
// 这在 改写 glViewport() 时会体现出来。
// 一个推荐的做法是，先通过 glfwGetFramebufferSize(), 检查出当前的 真实视口尺寸
// 再用这个 尺寸去 改写 glViewport()


//  WORK_WIDTH
//  WORK_HEIGHT
//-- 游戏工作尺寸（游戏像素）--
//   由于是 pixel游戏，所以这个值会很小。
//   这个值 也是 Camera 的尺寸
//-- 目前版本中，拉升窗口并不会改变这个值。（后期应实现 自动匹配）
//-- 请尽量确保此变量 与 SCR_WIDTH／SCR_HEIGHT 等比例



//----------------- 1920 * 1080 -----------------------//
/*
#define SCR_WIDTH  1920
#define SCR_HEIGHT 1080
//-------
#define WORK_WIDTH  480
#define WORK_HEIGHT 270
*/

//----------------- 1920 * 1200 ----------------------//

#define SCR_WIDTH  1920
#define SCR_HEIGHT 1200
//-------
#define WORK_WIDTH  480
#define WORK_HEIGHT 300



//----------------- 大尺寸测试版 ----------------------//
//- 在 1280*1280 尺寸下，能显示一张完整的 section(256,256)
//- 仅用在 生态群落 建设阶段
/*
#define SCR_WIDTH  1280
#define SCR_HEIGHT 1280
//-------
#define WORK_WIDTH  1280
#define WORK_HEIGHT 1280
*/








//-- 是否开启 全屏模式 --
inline bool IS_FULL_SCREEN = false; 



#endif 


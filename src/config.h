/*
 * ========================= config.h ==========================
 *                          -- tpr --
 *                                        创建 -- 2018.11.21
 *                                        修改 -- 2018.11.21
 * ----------------------------------------------------------
 *   配置 参数
 *   临时，后期会更换为 lua 来实现
 * ----------------------------
 */

#ifndef _TPR_CONFIG_H_
#define _TPR_CONFIG_H_

//-- window 显示尺寸（像素）--
//   拉升窗口将会改变这个值
#define SCR_WIDTH  880
#define SCR_HEIGHT  660

//-- 游戏工作尺寸（游戏像素）--
//   由于是 pixel游戏，所以这个值会很小。
//   这个值 也是 Camera 的尺寸
//-- 目前版本中，拉升窗口并不会改变这个值。（后期应实现 自动匹配）
#define WORK_WIDTH  160
#define WORK_HEIGHT 120

//-- 是否开启 全屏模式 --
inline bool IS_FULL_SCREEN = false; 



#endif







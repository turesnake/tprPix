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
#ifndef TPR_WINDOW_CONFIG_H_
#define TPR_WINDOW_CONFIG_H_


//------- 屏幕长宽(像素) --------//
// 在高分屏中，比如 mac，当我们申请一个 800*600 的window
// 系统可能会分配 1600*1200 像素的window
// 所以，这组值是一个 基于感官视觉的值。
// -----
// 目前游戏只支持 窗口模式（不支持全屏模式）
// 必须在编译前设置 window尺寸，游戏运行时，无法修改 window 尺寸
#define SCR_WIDTH_  900
#define SCR_HEIGHT_ 600
            // 在未来，考虑用 lua 配置表 来替代...


//-- 一个 "游戏像素" ，占用 n*n 个 显示屏像素 --
//   推荐值: {3,4} 
#define PIXES_PER_GAMEPIX  3


//-- 是否开启 全屏模式 --
//  尚未完工，严谨开启 full sreen
inline bool IS_FULL_SCREEN  {false}; 



#endif 


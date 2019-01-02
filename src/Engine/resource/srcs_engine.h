/*
 * ========================= srcs_engine.h ==========================
 *                          -- tpr --
 *                                        创建 -- 2018.11.21
 *                                        修改 -- 2018.11.21
 * ----------------------------------------------------------
 *   全游戏的 资源，都存储在此 头文件中
 *   这个文件中的资源 很可能会被拆分。因为容易引发 循环include
 * ----------------------------
 */
#ifndef _TPR_SRCS_MANAGER_H_
#define _TPR_SRCS_MANAGER_H_

//-------------------- CPP --------------------//
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set> 
#include <functional> 
#include <map>

//-------------------- Engine --------------------//
#include "GameObj.h"
#include "Action.h"
#include "Player.h" 
#include "TimeBase.h" 
#include "TimeCircle.h" 
#include "Camera.h"
#include "ShaderProgram.h"
#include "Mesh.h" 
#include "Behaviour.h" 


namespace esrc{ //------------------ namespace: esrc -------------------------//


using FUNC_V_V = std::function<void()>;



//-------------------------//
//       time 资源 
//-------------------------//
inline TimeBase   timer {}; //-- 全局时间
inline TimeCircle logicTimeCircle { &timer, 5 }; //- 逻辑时间循环 实例（5帧1周期）


//-------------------------//
//       Camera 资源 
//-------------------------//
inline Camera camera {}; //-- 本游戏暂时只有 一个 摄像机


//-------------------------//
//       Shader 资源 
//-------------------------//
inline ShaderProgram rect_shader { "/shaders/base.vs", "/shaders/base.fs" };
void shaders_init();


//-------------------------//
//     Action 资源（动画动作图集）
//-------------------------//
inline std::unordered_map<std::string, Action> actions {};
void actions_load();


//-------------------------//
//       GameObj 资源
//-------------------------//
//--- mem ---//
inline std::unordered_map<goid_t, GameObj> memGameObjs {}; //- 所有载入内存的 go实例 实际存储区。
                                    
inline std::unordered_set<goid_t> goids_active   {}; //- 激活组 (身处 激活圈 之内)
inline std::unordered_set<goid_t> goids_inactive {}; //- 未激活组 (身处 激活圈 之外)

inline FUNC_V_V  goSpecIds_SignUp  {nullptr}; //- goSpecIds 注册函数对象


void insert_new_gameObj( GameObj &_go );
void realloc_active_goes();
void realloc_inactive_goes();

//--- db ---//


//-------------------------//
//   renderPool / 渲染池
//-------------------------//
//--- mem ---//
inline std::map<float, Mesh*> renderPool {}; 
            //- key 是 图元的 z值。map会自动排序(负无穷在前，正无穷在后，符合我们要的顺序)
            //- 遍历 渲染池，就能从远到近地 渲染每一个 图元


//-------------------------//
//     globState 资源
//-------------------------//
//--- db ---//
void globState_srcs_init();
void globState_srcs_save();


//-------------------------//
//      Player 资源
//-------------------------//
//--- mem ---//
inline Player  player {}; //- 全游戏唯一 Player 实例  

//--- db ---//
void player_srcs_init();
void player_srcs_save();



//-------------------------//
//     Behaviour
//-------------------------//
inline Behaviour behaviour {};  //- 全游戏唯一 Behaviour 实例


void call_scriptMain(); //- 调用 脚本层 入口函数



}//---------------------- namespace: esrc -------------------------//
#endif


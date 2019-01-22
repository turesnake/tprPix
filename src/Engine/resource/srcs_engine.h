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
//=== *** glad FIRST, glfw SECEND *** ===
#include<glad/glad.h>  
#include<GLFW/glfw3.h>

//-------------------- CPP --------------------//
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set> 
#include <functional> 
#include <map>

//-------------------- Engine --------------------//
//     不应该在 这个文件中 加载如此多的  Engine_h文件 
#include "GameObj.h"
#include "Action.h"
#include "Player.h" 
#include "TimeBase.h" 
#include "TimeCircle.h" 
#include "Camera.h"
#include "ShaderProgram.h"
//#include "GameMesh.h" 
#include "ChildMesh.h"
#include "Behaviour.h" 
#include "MapSection.h" 
#include "ColliEntSet.h"
#include "MapCoord.h"


namespace esrc{ //------------------ namespace: esrc -------------------------//


using FUNC_V_V = std::function<void()>;


//-------------------------//
//       window 资源 
//-------------------------//
//-- 一个 类实例 维护一个 window。
//-- window 数据结构的 具体内容由 glfw库管理。
//-- 用户只需保存一个指针。用来访问这个 window。
inline GLFWwindow  *windowPtr {};

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
//     colliEntSet 资源
//-------------------------//
inline std::unordered_map<int, ColliEntSet> colliEntSets {};
void colliEntSets_load();
void colliEntSets_debug(); //- debug

//-- 正反表 --
inline std::unordered_map<std::string, int> colliEntSet_name_idx {};
inline std::unordered_map<int, std::string> colliEntSet_idx_name {};
void colliEntSet_tables_init(); 


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

using F_GOID_GOPTR = std::function<void(goid_t,GameObj*)>;
void foreach_memGameObjs( F_GOID_GOPTR _fp );
void foreach_goids_active( F_GOID_GOPTR _fp );
void foreach_goids_inactive( F_GOID_GOPTR _fp );

goid_t insert_new_gameObj();
void realloc_active_goes();
void realloc_inactive_goes();

inline GameObj *find_memGameObjs( goid_t _goid ){
    return (GameObj*)&(memGameObjs.at(_goid));
}


//--- db ---//


//-------------------------//
//   renderPool_picMeshs
//-------------------------//
//--- mem ---//
inline std::multimap<float, ChildMesh*> renderPool_picMeshs {}; 
inline std::multimap<float, ChildMesh*> renderPool_shadowMeshs {}; 
            //- key 是 图元的 z值。map会自动排序(负无穷在前，正无穷在后，符合我们要的顺序)
            //- 遍历 渲染池，就能从远到近地 渲染每一个 图元
            //- 有的 go.pos.z 值可能相同，所以要使用 multimap !
void draw_renderPool_picMeshs();
void draw_renderPool_shadowMeshs();



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


//-------------------------//
//     MapSection 资源
//-------------------------//
//-- 可能在 mem态，加载很多张 mapsection
//-- 但每一渲染帧，只会有 1／2／4 张 map，被渲染。
inline std::unordered_map<u64, MapSection> mapSections {};

MapSection *insert_new_mapSection( const MapCoord &_sectionPos );
MemMapEnt *get_memMapEnt( const MapCoord &_mc ); //- 临时放这 


//-------------------------//
//   renderPool_meshs
//-------------------------//
//-- 一切以 Mesh为标准的 图元，都可以丢进这个 容器中
//-- 比如：
//    - mapSection
//    - go脚下的阴影
//    - UI图元等
inline std::multimap<float, Mesh*> renderPool_meshs {};

void draw_renderPool_meshs();



}//---------------------- namespace: esrc -------------------------//
#endif


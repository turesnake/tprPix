/*
 * ========================= srcs_engine.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2018.11.21
 *                                        MODIFY --
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
#include "AnimFrameSet.h"
#include "Player.h" 
#include "TimeBase.h" 
#include "TimeCircle.h" 
#include "Camera.h"
#include "ShaderProgram.h"
#include "ChildMesh.h"
#include "Behaviour.h" 
#include "Section.h" 
#include "ColliEntSet.h"
#include "MapCoord.h"
#include "EcoSys.h"
#include "GameSeed.h" //- tmp
#include "SectionChunkSet.h"


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
//       gameSeed 资源 
//-------------------------//
inline GameSeed  gameSeed {}; //- tmp, 游戏种子，
                            //- 暂设为：只有一个种子，且每次运行都重置

//-------------------------//
//       Camera 资源 
//-------------------------//
inline Camera camera {}; //-- 本游戏暂时只有 一个 摄像机


//-------------------------//
//       Shader 资源 
//-------------------------//
inline ShaderProgram rect_shader { "/shaders/base.vs", "/shaders/base.fs" };
void init_shaders();


//-------------------------//
//     colliEntSet 资源
//-------------------------//
inline std::unordered_map<int, ColliEntSet> colliEntSets {};
void load_colliEntSets();
void debug_colliEntSets(); //- debug

//-- 正反表 --
inline std::unordered_map<std::string, int> colliEntSet_name_idx {};
inline std::unordered_map<int, std::string> colliEntSet_idx_name {};
void init_colliEntSet_tables(); 


//-------------------------//
//     AnimFrameSet 资源（动画动作图集）
//-------------------------//
inline std::unordered_map<std::string, AnimFrameSet> animFrameSets {};
void load_animFrameSets();


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
void signUp_newGO_to_mapEnt( GameObj *_goPtr );

inline GameObj *find_memGameObjs( goid_t _goid ){
    return (GameObj*)&(memGameObjs.at(_goid));
}


//--- db ---//


//-------------------------//
//   renderPool goMeshs
//-------------------------//
//--- mem ---//
inline std::multimap<float, ChildMesh*> renderPool_goMeshs_pic {}; 
            //- key 是 图元的 z值。map会自动排序(负无穷在前，正无穷在后，符合我们要的顺序)
            //- 遍历 渲染池，就能从远到近地 渲染每一个 图元
            //- 有的 go.pos.z 值可能相同，所以要使用 multimap !
inline std::vector<ChildMesh*> renderPool_goMeshs_shadow {};
            //- 所有 shadow 拥有相同的 z-deep. 对渲染次序 无要求
            
void draw_renderPool_goMeshs_pic();
void draw_renderPool_goMeshs_shadow();



//-------------------------//
//     globState 资源
//-------------------------//
//--- db ---//
void init_globState_srcs();
void save_globState_srcs();


//-------------------------//
//      Player 资源
//-------------------------//
//--- mem ---//
inline Player  player {}; //- 全游戏唯一 Player 实例  

//--- db ---//
void init_player_srcs();
void player_srcs_save();


//-------------------------//
//     ecosyses
//-------------------------//
inline std::unordered_map<std::string, EcoSys> ecoSyses {};
//EcoSys *insert_new_ecoSys(const std::string &_name);
void init_ecoSyses();


//-------------------------//
//     Behaviour
//-------------------------//
inline Behaviour behaviour {};  //- 全游戏唯一 Behaviour 实例

void call_scriptMain(); //- 调用 脚本层 入口函数


//-------------------------//
//     Section 资源
//-------------------------//
//-- 可能在 mem态，加载很多张 section
//-- 但每一渲染帧，只会有 1／2／4 张 map，被渲染。
// key 为 section.sectionKey.key;
inline std::unordered_map<u64_t, Section> sections {};

Section *insert_new_section( const MapCoord &_sectionMCPos );
MemMapEnt *get_memMapEnt( const MapCoord &_mcpos ); //- 临时放这 

//-- chunk集数据 一定先于 sections 数据被创建 --
//  这两个结构间 存在大量数据重复，未来可以优化之...
inline std::unordered_map<u64_t, SectionChunkSet> sectionChunkSets {};

SectionChunkSet *insert_new_sectionChunkSet( u64_t _sectionKeyVal );


//-------------------------//
//   renderPool_meshs
//-------------------------//
//-- 一切以 Mesh为标准的 图元，都可以丢进这个 容器中
//-- 比如：
//    - Section
//    - go脚下的阴影
//    - UI图元等
inline std::multimap<float, Mesh*> renderPool_meshs {};

void draw_renderPool_meshs();



}//---------------------- namespace: esrc -------------------------//
#endif


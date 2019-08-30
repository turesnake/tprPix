/*
 * ======================= ColliPoint.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.08.29
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#ifndef TPR_COLLI_POINT_H
#define TPR_COLLI_POINT_H


// 用于 技能碰撞检测
// 如果一个 animAction 内发动了技能，那么其 anim.J.png 中，有那么几帧，会携带 ColliPoint 信息
// colliPoint 是一个 点（ppos），程序记录这个点，到 rootAnchor 的偏移距离
// ---
// 一组 colliPoint 点（比如五六个）可以表达一个 技能的碰撞区域。
// ---
//  每次执行碰撞检测时：
// --1-- 遍历本帧的 所有 colliPoints，计算出其所在的 mapent。将这些 mapent，收集到一个 临时容器中
//       如果一个 colliPoint，在 mapent 中的位置 过于边缘，此 mapent 将不会被命中
//       这反过来要求， colliPoint 在 J.png 中的布置必须合理，技能碰撞区 最好大一些
// --2-- 遍历上文收集的 mapent集，逐个执行 碰撞检测
//
class ColliPoint{
public:







};


#endif 


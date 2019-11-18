/*
 * ========================= config.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2018.11.21
 *                                        MODIFY --
 * ----------------------------------------------------------
 *   配置 参数
 *   临时，后期会更换为 lua 来实现
 * ----------------------------
 */
#ifndef TPR_CONFIG_H
#define TPR_CONFIG_H


//-- 一个 section，占有 4*4 chunks
#define CHUNKS_PER_SECTION 4

//-- 一个 chunk，占有 8*8 fields
#define FIELDS_PER_CHUNK  8

//-- 一个 field，占有 4*4 mapents
#define ENTS_PER_FIELD      4
#define HALF_ENTS_PER_FIELD 2

//-- 一个 mapent 占用 8*8像素
//#define PIXES_PER_MAPENT  8
//#define HALF_PIXES_PER_MAPENT 4
//-- 求 mpos 中间pixel的 ppos 时用到
//#define MID_PPOS_IDX_IN_MAPENT 4


#define PIXES_PER_MAPENT  64
#define HALF_PIXES_PER_MAPENT 32
//-- 求 mpos 中间pixel的 ppos 时用到
#define MID_PPOS_IDX_IN_MAPENT 32


//-- 一个 chunk，占有 32*32 mapents
#define ENTS_PER_CHUNK (FIELDS_PER_CHUNK*ENTS_PER_FIELD)

//-- 一个 section，占有 128*128 mapEnts
#define ENTS_PER_SECTION  (ENTS_PER_CHUNK*CHUNKS_PER_SECTION)

//-- 一个 field，占有 32*32 pixel
#define PIXES_PER_FIELD (ENTS_PER_FIELD*PIXES_PER_MAPENT)

//-- 一个 chunk，占有 256*256 pixel
#define PIXES_PER_CHUNK (ENTS_PER_CHUNK*PIXES_PER_MAPENT)

//-- 为避免渲染时，chunk间出现白线，而做的 补偿措施 “每个chunk 多渲一排pix，相互叠加” --
#define PIXES_PER_CHUNK_IN_TEXTURE (PIXES_PER_CHUNK + 1)

//-- 一个 section，占有 1024*1024 pixel
#define PIXES_PER_SECTION (CHUNKS_PER_SECTION*PIXES_PER_CHUNK)

//-- camera.viewingBox z_deep
#define VIEWING_BOX_Z_DEEP  10000



#endif

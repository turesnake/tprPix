/*
 * ========================= config.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2018.11.21
 *                                        MODIFY --
 * ----------------------------------------------------------
 *  config_Vals about pixel and gameMapSize
 */
#ifndef TPR_CONFIG_H
#define TPR_CONFIG_H


//-- 一个 section，占有 4*4 chunks
constexpr int CHUNKS_PER_SECTION {4};

//-- 一个 chunk，占有 8*8 fields
constexpr int FIELDS_PER_CHUNK  {8};
constexpr int FIELDS_PER_SECTION { FIELDS_PER_CHUNK * CHUNKS_PER_SECTION };

//-- 一个 field，占有 4*4 mapents
constexpr int ENTS_PER_FIELD        {4};
constexpr int HALF_ENTS_PER_FIELD   { ENTS_PER_FIELD / 2 };

//-- 一个 mapent 占用 64 * 64 像素
constexpr int PIXES_PER_MAPENT      {64};
constexpr int HALF_PIXES_PER_MAPENT { PIXES_PER_MAPENT / 2 };

//-- 一个 chunk，占有 32*32 mapents
constexpr int ENTS_PER_CHUNK { FIELDS_PER_CHUNK * ENTS_PER_FIELD };

//-- 一个 section，占有 128*128 mapEnts
constexpr int ENTS_PER_SECTION  { ENTS_PER_CHUNK * CHUNKS_PER_SECTION };
constexpr int HALF_ENTS_PER_SECTION { ENTS_PER_SECTION / 2 };


//-- 一个 field，占有 32*32 pixel
constexpr int PIXES_PER_FIELD { ENTS_PER_FIELD * PIXES_PER_MAPENT };
constexpr int HALF_PIXES_PER_FIELD { PIXES_PER_FIELD / 2 };

//-- 一个 chunk，占有 256*256 pixel
constexpr int PIXES_PER_CHUNK { ENTS_PER_CHUNK*PIXES_PER_MAPENT };

//-- 为避免渲染时，chunk间出现白线，而做的 补偿措施 “每个chunk 多渲一排pix，相互叠加” --
constexpr int PIXES_PER_CHUNK_IN_TEXTURE { PIXES_PER_CHUNK + 1 };

//-- 一个 section，占有 1024*1024 pixel
constexpr int PIXES_PER_SECTION { CHUNKS_PER_SECTION*PIXES_PER_CHUNK };

//-- camera.viewingBox z_deep
constexpr int VIEWING_BOX_Z_DEEP  {10000};




//====== double ======
constexpr double PIXES_PER_MAPENT_D { static_cast<double>(PIXES_PER_MAPENT) };
constexpr double HALF_PIXES_PER_MAPENT_D { static_cast<double>(HALF_PIXES_PER_MAPENT) };

constexpr double PIXES_PER_FIELD_D { static_cast<double>(PIXES_PER_FIELD) };
constexpr double HALF_PIXES_PER_FIELD_D { static_cast<double>(HALF_PIXES_PER_FIELD) };

constexpr double ENTS_PER_CHUNK_D { static_cast<double>(ENTS_PER_CHUNK) };


constexpr double ENTS_PER_FIELD_D { static_cast<double>(ENTS_PER_FIELD) };
constexpr double HALF_ENTS_PER_FIELD_D { static_cast<double>(HALF_ENTS_PER_FIELD) };

constexpr double ENTS_PER_SECTION_D { static_cast<double>(ENTS_PER_SECTION) };


constexpr double PIXES_PER_CHUNK_D { static_cast<double>(PIXES_PER_CHUNK) };
constexpr float  PIXES_PER_CHUNK_F { static_cast<float>(PIXES_PER_CHUNK) };


#endif

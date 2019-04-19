/*
 * ====================== Chunk.cpp =======================
 *                          -- tpr --
 *                                        CREATE -- 2018.12.09
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#include "Chunk.h"

//-------------------- C --------------------//
#include <cassert>
#include <math.h>

//-------------------- CPP --------------------//
#include <map>

//-------------------- Engine --------------------//
#include "ViewingBox.h"
#include "srcs_engine.h"
//#include "MapEnt.h"
#include "EcoSysInMap.h"
#include "random.h"
#include "occupyWeight.h"
//#include "EcoSys.h"
#include "Altitude.h"
#include "Quad.h"
#include "FieldBorderSet.h"
#include "sectionBuild.h" //- tmp
#include "MapField.h"

//#include "debug.h"

//-------------------- Script --------------------//
#include "Script/gameObjs/create_goes.h"



namespace{//-------- namespace: --------------//

    std::uniform_int_distribution<int> uDistribution_regular(0,1000); // [0,1000]
    
    //--- 定值: chunk-mesh scale --
    glm::vec3  mesh_scaleVal {  static_cast<float>(PIXES_PER_CHUNK_IN_TEXTURE),
                                static_cast<float>(PIXES_PER_CHUNK_IN_TEXTURE),
                                1.0f };

    std::vector<fieldKey_t> fieldKeys {}; //- 8*8 fieldKeys

    class FieldData{
    public:
        explicit FieldData( MapField *_fieldPtr, QuadType _quadType ){
            this->fieldPtr = _fieldPtr;
            this->ecoInMapPtr = esrc::get_ecoSysInMapPtr( this->fieldPtr->get_ecoSysInMapKey() );
            this->quadContainerPtr = const_cast<FieldBorderSet::quadContainer_t*>( 
                                                &get_fieldBorderSet(this->fieldPtr->get_fieldBorderSetId(), 
                                                _quadType) );
        }
        //====== vals ======//
        MapField     *fieldPtr    {};
        EcoSysInMap  *ecoInMapPtr {};
        FieldBorderSet::quadContainer_t  *quadContainerPtr; //-- fieldBorderSet 子扇区容器 --
        //...
    };


    class PixData{
    public:
        inline void init( const IntVec2 &_ppos ){
            this->ppos = _ppos;
        }
        //====== vals ======//
        size_t     pixIdx_in_chunk    {};
        size_t     pixIdx_in_chunkTex {}; //- chunk-texture 会多出来几排pix
        size_t     pixIdx_in_field    {};
        IntVec2    ppos      {};
        RGBA      *texPixPtr {nullptr};
        FieldData *fieldDataPtr {nullptr}; 
        Altitude   alti {};
    };

    std::map<occupyWeight_t,FieldData> nearFour_fieldDatas {}; //- 一个 field 周边4个 field 数据
                                    // 按照 ecoSysInMap.occupyWeight 倒叙排列（值大的在前面）

    class FieldInfo{
    public:
        IntVec2   mposOff;
        QuadType  quad; //- 注意，这个值是反的，从 fieldBorderSet 角度去看 得到的 quad
    };

    //- 周边4个field 指示数据 --
    std::vector<FieldInfo> nearFour_fieldInfos {
        FieldInfo{ IntVec2{ 0, 0 },                           QuadType::Right_Top },
        FieldInfo{ IntVec2{ ENTS_PER_FIELD, 0 },              QuadType::Left_Top  },
        FieldInfo{ IntVec2{ 0, ENTS_PER_FIELD },              QuadType::Right_Bottom  },
        FieldInfo{ IntVec2{ ENTS_PER_FIELD, ENTS_PER_FIELD }, QuadType::Left_Bottom  }
    };

    bool  is_baseUniforms_transmited {false}; //- pixGpgpu 的几个 静态uniform值 是否被传输
                                        // 这些值是固定的，每次游戏只需传入一次...

    //-- 根据 奇偶性，来分配每个 chunk 的 zOff值 --
    std::vector<float> zOffs{
        0.1, 0.2, 0.3, 0.4
    };


    //===== funcs =====//
    MapField *colloect_and_creat_nearFour_fieldDatas( fieldKey_t _fieldKey );


}//------------- namespace: end --------------//


/* ===========================================================
 *                        init
 * -----------------------------------------------------------
 */
void Chunk::init(){
    
    //--- mesh.scale ---
    mesh.set_scale(mesh_scaleVal);

    //---
    this->init_memMapEnts();

    //--- 填充 mapTex buf
    this->mapTex.resize_texBuf();


    // 根据 本chunk 在 2*2 chunk 中的位置
    // 来分配 zoff 值。 
    //-- 本 chunk 在 世界坐标中的 奇偶性 --
    // 得到的值将会是 {0,0}; {1,0}; {0,1}; {1,1} 中的一种
    IntVec2 v = floorDiv( this->get_mpos(), ENTS_PER_CHUNK );
    IntVec2 oddEven = floorMod( v, 2 );
    this->zOff = zOffs.at( oddEven.y * 2 + oddEven.x );
        
}


/* ===========================================================
 *                  refresh_translate_auto
 * -----------------------------------------------------------
 */
void Chunk::refresh_translate_auto(){
    const IntVec2 &ppos = mcpos.get_ppos();
    mesh.set_translate(glm::vec3{   (float)ppos.x,
                                    (float)ppos.y,
                                    esrc::camera.get_zFar() + ViewingBox::chunks_zOff +
                                        this->zOff  //-- MUST --
                                    });
}


/* ===========================================================
 *                     init_memMapEnts
 * -----------------------------------------------------------
 * -- 向 memMapEnts 填入每个mapent，并设置它们的 mcpos
 * --- 除此之外，这些 mapent 数据都是空的
 */
void Chunk::init_memMapEnts(){
    if( this->is_memMapEnts_set ){
        return;
    }
    MemMapEnt mapEnt {};
    for( int h=0; h<ENTS_PER_CHUNK; h++ ){
        for( int w=0; w<ENTS_PER_CHUNK; w++ ){
            mapEnt.mcpos = mcpos + MapCoord{ w, h };
            this->memMapEnts.push_back( mapEnt ); //-copy
        }
    }
    this->is_memMapEnts_set = true;
}


/* ===========================================================
 *               get_mapEntIdx_in_chunk
 * -----------------------------------------------------------
 * -- 传入任意 mpos，获得其在 本chunk 中的 idx（访问 vector 用）
 */
size_t Chunk::get_mapEntIdx_in_chunk( const IntVec2 &_anyMPos ){
    IntVec2 mposOff = _anyMPos - this->mcpos.get_mpos();
    int w = mposOff.x;
    int h = mposOff.y;
        assert( (w>=0) && (w<ENTS_PER_CHUNK) &&
                (h>=0) && (h<ENTS_PER_CHUNK) ); //- tmp
    return static_cast<size_t>(h*ENTS_PER_CHUNK + w);
}


/* ===========================================================
 *               get_pixIdx_in_chunk
 * -----------------------------------------------------------
 * -- 传入任意 ppos 绝对值，获得 此pix 在 本chunk 中的 idx（访问 mapTex 用）
 */
size_t Chunk::get_pixIdx_in_chunk( const IntVec2 &_anyPPos ){
    IntVec2 pposOff = _anyPPos - this->mcpos.get_ppos();
    int w = pposOff.x;
    int h = pposOff.y;
        assert( (w>=0) && (w<PIXES_PER_CHUNK) &&
                (h>=0) && (h<PIXES_PER_CHUNK) ); //- tmp
    return static_cast<size_t>( h*PIXES_PER_CHUNK + w );
}



/* ===========================================================
 *           assign_ents_and_pixes_to_field
 * -----------------------------------------------------------
 */
void Chunk::assign_ents_and_pixes_to_field(){
    if( this->is_assign_ents_and_pixes_to_field_done ){
        return;
    }

    MapField  *tmpFieldPtr;
    RGBA      *texBufHeadPtr; //- mapTex
    RGBA       color;

    IntVec2    tmpEntMPos;
    IntVec2    pposOff;   //- 通用偏移向量
    IntVec2    mposOff;

    PixData   pixData;//- each pix

    size_t    entIdx_in_chunk;
    int       count;

    texBufHeadPtr = this->mapTex.get_texBufHeadPtr();

    //------------------------//
    // 委托 GPGPU 计算 pix数据
    //------------------------//

        //-- 在未来，这部分运算可能还是要交给 cpu 去做（多线程）
        //-- GPGPU 实际效果有点卡

    esrc::pixGpgpu.bind(); //--- MUST !!! ---

    IntVec2 chunkCPos = chunkMPos_2_chunkCPos( this->mcpos.get_mpos() );
    glUniform2f(esrc::pixGpgpu.get_uniform_location("chunkCFPos"), 
                static_cast<float>(chunkCPos.x), 
                static_cast<float>(chunkCPos.y) ); //- 2-float

    //-- 每个游戏存档的这组值 其实是固定的，游戏运行期间，只需传输一次 --
    if( is_baseUniforms_transmited == false ){
        is_baseUniforms_transmited = true;
        glUniform2f(esrc::pixGpgpu.get_uniform_location("altiSeed_pposOffSeaLvl"), 
                    esrc::gameSeed.altiSeed_pposOffSeaLvl.x,
                    esrc::gameSeed.altiSeed_pposOffSeaLvl.y ); //- 2-float
                    
        glUniform2f(esrc::pixGpgpu.get_uniform_location("altiSeed_pposOffBig"), 
                    esrc::gameSeed.altiSeed_pposOffBig.x,
                    esrc::gameSeed.altiSeed_pposOffBig.y ); //- 2-float
        
        glUniform2f(esrc::pixGpgpu.get_uniform_location("altiSeed_pposOffMid"), 
                    esrc::gameSeed.altiSeed_pposOffMid.x,
                    esrc::gameSeed.altiSeed_pposOffMid.y ); //- 2-float

        glUniform2f(esrc::pixGpgpu.get_uniform_location("altiSeed_pposOffSml"), 
                    esrc::gameSeed.altiSeed_pposOffSml.x,
                    esrc::gameSeed.altiSeed_pposOffSml.y ); //- 2-float
    }
                    
    esrc::pixGpgpu.let_gpgpuFBO_work();
    const std::vector<FRGB> &gpgpuDatas = esrc::pixGpgpu.get_texFBuf();
    esrc::pixGpgpu.release(); //--- MUST !!! ---
    
    //------------------------//
    //   reset fieldPtrs
    //------------------------//
    this->reset_fieldKeys();

    for( const auto &fieldKey : fieldKeys ){ //- each field key

        //-- 收集 目标field 周边4个 field 实例指针  --
        // 如果相关 field 不存在，就地创建之
        tmpFieldPtr = colloect_and_creat_nearFour_fieldDatas( fieldKey );

        
        for( int eh=0; eh<ENTS_PER_FIELD; eh++ ){
            for( int ew=0; ew<ENTS_PER_FIELD; ew++ ){ //- each ent in field

                tmpEntMPos = tmpFieldPtr->get_mpos() + IntVec2{ ew, eh };
                mposOff = tmpEntMPos - this->get_mpos();
                entIdx_in_chunk = mposOff.y * ENTS_PER_CHUNK + mposOff.x;
                MemMapEnt &mapEntRef = this->memMapEnts.at(entIdx_in_chunk);
                //...

                for( int ph=0; ph<PIXES_PER_MAPENT; ph++ ){
                    for( int pw=0; pw<PIXES_PER_MAPENT; pw++ ){ //------ each pix in mapent ------

                        pixData.init( mpos_2_ppos(tmpEntMPos) + IntVec2{pw,ph} );
                        pposOff = pixData.ppos - this->mcpos.get_ppos();
                        pixData.pixIdx_in_chunk = pposOff.y * PIXES_PER_CHUNK + pposOff.x;

                        pixData.pixIdx_in_chunkTex = pposOff.y * PIXES_PER_CHUNK_IN_TEXTURE + pposOff.x;
                        pixData.texPixPtr = texBufHeadPtr + pixData.pixIdx_in_chunkTex;

                        pposOff = pixData.ppos - tmpFieldPtr->get_ppos();
                        pixData.pixIdx_in_field = pposOff.y * PIXES_PER_FIELD + pposOff.x;

                        //--------------------------------//
                        // 确定 pix 属于 周边4个field 中的哪一个
                        //--------------------------------//
                        count = 0;
                        for( auto &fieldPair : nearFour_fieldDatas ){ //--- 周边4个 field 信息
                            count++;
                            const FieldData &fieldDataRef = fieldPair.second;
                            if( count != nearFour_fieldDatas.size() ){   //- 前3个 field
                                if( fieldDataRef.quadContainerPtr->at(pixData.pixIdx_in_field) == 1 ){
                                    pixData.fieldDataPtr = const_cast<FieldData*>( &fieldDataRef );
                                    break;
                                }
                            }else{     //- 第4个 field
                                pixData.fieldDataPtr = const_cast<FieldData*>( &fieldDataRef );
                            }
                        } //--- 周边4个 field 信息 end ---

                        //--------------------------------//
                        //  计算 本pix  alti
                        //--------------------------------//
                        pixData.alti.set( gpgpuDatas.at(pixData.pixIdx_in_chunk).r );

                        //--------------------------------//
                        // 数据收集完毕，将部分数据 传递给 ent
                        //--------------------------------//
                        if( (ph==HALF_PIXES_PER_MAPENT) && (pw==HALF_PIXES_PER_MAPENT) ){//- ent 中点 pix
                            mapEntRef.alti = pixData.alti;

                            //----- 记录 alti min/max ----//
                            if( pixData.alti < pixData.fieldDataPtr->fieldPtr->get_minAlti() ){ 
                                pixData.fieldDataPtr->fieldPtr->set_minAlti(pixData.alti);
                            }
                            if( pixData.alti > pixData.fieldDataPtr->fieldPtr->get_maxAlti() ){ 
                                pixData.fieldDataPtr->fieldPtr->set_maxAlti(pixData.alti);
                            }
                                            // 目前这个实现并不精确。对于 chunk 边缘的 field。它们的 min/max alti 
                                            // 要等到 隔壁 chunk 也生成后，才能补全。
                                            // 所以，现在还是会出现 “树木长在河里” 的现象
                            
                            //----- 记录 field.nodeAlti ----//
                            if( mapEntRef.get_mpos() == pixData.fieldDataPtr->fieldPtr->get_nodeMPos() ){
                                pixData.fieldDataPtr->fieldPtr->set_nodeAlti(pixData.alti);
                            }

                            //...
                        }

                        //--------------------------------//
                        //    正式给 pix 上色
                        //--------------------------------//
                        color = pixData.fieldDataPtr->ecoInMapPtr->get_landColor( pixData.fieldDataPtr->fieldPtr->get_density() );
                        color.a = 255;

                            //-- 当前版本，整个 chunk 都是实心的，water图层 被移动到了 chunk图层上方。

                        *pixData.texPixPtr = color;
                    }
                } //- each pix in mapent end ---
            }
        } //- each ent in field end --        
    } //-- each field key end --

    //---------------------------//
    //  为了解决游戏中 “chunk间白线” 问题
    //  将 chunk.tex 上方／右方 两条 pix 颜色补齐
    //  补齐方案很简单，直接复制 相邻pix 的颜色
    //---------------------------//
    RGBA    *pixPtr;
    RGBA    *nearPixPtr;
    //--- chunk 上方那条 额外 pix ---
    for( int pw=0; pw<PIXES_PER_CHUNK_IN_TEXTURE; pw++ ){
        nearPixPtr = texBufHeadPtr + 
                    ((PIXES_PER_CHUNK_IN_TEXTURE-2) * PIXES_PER_CHUNK_IN_TEXTURE + pw);
        pixPtr = texBufHeadPtr + 
                    ((PIXES_PER_CHUNK_IN_TEXTURE-1) * PIXES_PER_CHUNK_IN_TEXTURE + pw);
        //---
        *pixPtr = *nearPixPtr;
    }
    //--- chunk 右侧那条 额外 pix ---
    for( int ph=0; ph<PIXES_PER_CHUNK_IN_TEXTURE; ph++ ){
        nearPixPtr = texBufHeadPtr + 
                    (ph * PIXES_PER_CHUNK_IN_TEXTURE + (PIXES_PER_CHUNK_IN_TEXTURE-2));
        pixPtr = texBufHeadPtr + 
                    (ph * PIXES_PER_CHUNK_IN_TEXTURE + (PIXES_PER_CHUNK_IN_TEXTURE-1));
        //---
        *pixPtr = *nearPixPtr;
    }


    //--------------------------------//
    //          go 生成器  [tmp]
    //   为每个 field，分配一个 go实例
    //--------------------------------//
    for( const auto &fieldKey : fieldKeys ){ //- each field key
        sectionBuild::create_a_go_in_field( fieldKey );
    } //-- each field key end --

    
    //---------------------------//
    //   正式用 texture 生成 name
    //---------------------------//
    this->mapTex.creat_texName();

    this->is_assign_ents_and_pixes_to_field_done = true;
}



/* ===========================================================
 *                   reset_fieldKeys
 * -----------------------------------------------------------
 * -- fieldKeys 是个 局部公用容器，每次使用前，都要重装填
 */
void Chunk::reset_fieldKeys(){

    IntVec2    tmpFieldMpos;
    fieldKeys.clear();
    for( int h=0; h<FIELDS_PER_CHUNK; h++ ){
        for( int w=0; w<FIELDS_PER_CHUNK; w++ ){ //- each field

            tmpFieldMpos = this->get_mpos() + IntVec2{  w*ENTS_PER_FIELD,
                                                        h*ENTS_PER_FIELD };
            fieldKeys.push_back( fieldMPos_2_fieldKey(tmpFieldMpos) );
        }
    }
}


namespace{//-------- namespace: --------------//



/* ===========================================================
 *              colloect_and_creat_nearFour_fieldDatas
 * -----------------------------------------------------------
 * -- 收集 目标field 周边4个 field 数据
 *    如果相关 field 不存在，就地创建之
 * -- 返回 目标 field 指针
 */
MapField *colloect_and_creat_nearFour_fieldDatas( fieldKey_t _fieldKey ){

    MapField     *targetFieldPtr = esrc::find_or_insert_the_field_ptr( _fieldKey );
    MapField     *tmpFieldPtr;
    IntVec2       tmpFieldMPos;
    int           count = 0;
    //---
    nearFour_fieldDatas.clear();
    for( const auto &fieldInfo : nearFour_fieldInfos ){

        if( count == 0 ){
            tmpFieldPtr = targetFieldPtr;
        }else{
            tmpFieldMPos = targetFieldPtr->get_mpos() + fieldInfo.mposOff;
            tmpFieldPtr = esrc::find_or_insert_the_field_ptr( fieldMPos_2_fieldKey(tmpFieldMPos) );
        }

        nearFour_fieldDatas.insert({ -(tmpFieldPtr->get_occupyWeight()), FieldData{tmpFieldPtr,fieldInfo.quad} }); //- copy
                        //- 通过负数，来实现 倒叙排列，occupyWeight 值大的排前面
        count++;
    }
    return targetFieldPtr;
}


}//------------- namespace: end --------------//

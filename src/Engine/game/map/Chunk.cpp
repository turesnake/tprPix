/*
 * ====================== Chunk.cpp =======================
 *                          -- tpr --
 *                                        CREATE -- 2018.12.09
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *    map chunk 一个地图区域。 左下坐标系
 *    ----------
 *    
 * ----------------------------
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
#include "MapEnt.h"
#include "EcoSysInMap.h"
#include "random.h"
#include "occupyWeight.h"
#include "EcoSys.h"
#include "Altitude.h"
#include "Quad.h"
#include "FieldBorderSet.h"



namespace{//-------- namespace: --------------//

    std::default_random_engine  randEngine; //-通用 随机数引擎实例
    inline std::uniform_int_distribution<int> uDistribution_regular(0,1000); // [0,1000]
    bool  is_rand_init {false}; //- tmp
    
    //--- 定值: chunk-mesh scale --
    glm::vec3  mesh_scaleVal {  (float)(ENTS_PER_CHUNK * PIXES_PER_MAPENT),
                                (float)(ENTS_PER_CHUNK * PIXES_PER_MAPENT),
                                1.0f };

    std::vector<fieldKey_t> fieldKeys {}; //- 8*8 fieldKeys


    //--- 临时 沙滩色，水色 ---
    RGBA color_sand  {210, 195, 142, 255};
    RGBA color_water { 97, 125, 142, 255 };



    class FieldData{
    public:
        explicit FieldData( MapField *_fieldPtr, QuadType _quadType ){
            this->fieldPtr = _fieldPtr;
            this->ecoInMapPtr = esrc::get_ecoSysInMapPtr( this->fieldPtr->ecoSysInMapKey );
            this->ecoPtr = esrc::get_ecoSysPtr( this->ecoInMapPtr->ecoSysType );
            this->quadContainerPtr = (FieldBorderSet::quadContainer_t*)&get_fieldBorderSet( this->fieldPtr->fieldBorderSetId, _quadType );
        }
        //====== vals ======//
        MapField     *fieldPtr    {};
        EcoSysInMap  *ecoInMapPtr {};
        EcoSys       *ecoPtr      {};
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


    //===== funcs =====//
    MapField *colloect_and_creat_nearFour_fieldDatas( fieldKey_t _fieldKey );

    


}//------------- namespace: end --------------//


/* ===========================================================
 *                        init
 * -----------------------------------------------------------
 */
void Chunk::init(){
    
    if( is_rand_init == false ){
        is_rand_init = true;
        randEngine.seed( get_new_seed() );
    }
    

    //--- mesh.scale ---
    mesh.set_scale(mesh_scaleVal);

    //---
    this->init_memMapEnts();

    //--- 填充 mapTex buf
    this->mapTex.resize_texBuf();
}


/* ===========================================================
 *                  refresh_translate_auto
 * -----------------------------------------------------------
 */
void Chunk::refresh_translate_auto(){
    const IntVec2 &ppos = mcpos.get_ppos();
    mesh.set_translate(glm::vec3{   (float)ppos.x,
                                    (float)ppos.y,
                                    esrc::camera.get_zFar() + ViewingBox::chunks_zOff //-- MUST --
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
 *               assign_pixels_2_mapent
 * -----------------------------------------------------------
 * -- 最简模式，每个 mapent 根据所属field.color, 来涂颜色
 *    已被取代...
 */
void Chunk::assign_pixels_2_mapent(){
    if( this->is_assign_pixels_2_mapent_done ){
        return;
    }

    RGBA     color;
    RGBA     nearColor;

    RGBA    *pixBufHeadPtr;
    RGBA    *tmpPixPtr;

    IntVec2  tmpPixPPos;
    size_t   tmpPixIdx;

    size_t   maskIdx; 

    RGBA     water_color     { 102, 104, 122, 255 };
    RGBA     deepWater_color { 79, 81, 98, 255 };

    //------ 简易方案 -------//
    this->mapTex.resize_texBuf();
    pixBufHeadPtr = this->mapTex.get_texBufHeadPtr();
    
    for( const auto &mapEntRef : this->memMapEnts ){ //- each mapent

        //color = get_fieldPtr_by_key(mapEntRef.fieldKey)->color; //- 目标颜色
        //nearColor = get_fieldPtr_by_key(mapEntRef.nearbyFieldKey)->color; //- 副色

        //--- 如果本 mapent 是水域，直接涂水色 ---
        /*
        if( mapEntRef.landWater.get_isLand() == false ){

            for( int h=0; h<PIXES_PER_MAPENT; h++ ){
                for( int w=0; w<PIXES_PER_MAPENT; w++ ){ //- each pix in mapent

                    tmpPixPPos = mapEntRef.mcpos.get_ppos() + IntVec2{ w, h };
                    tmpPixIdx = this->get_pixIdx_in_chunk( tmpPixPPos );
                    tmpPixPtr = pixBufHeadPtr + tmpPixIdx;
                    *tmpPixPtr = deepWater_color; //- 水色。
                }
            } //- for each pix in mapent end ---

            // 还应该处理 border ent
            //...
            continue;
        }
        */

        //--- 先整体涂一遍 主色 ---//
        for( int h=0; h<PIXES_PER_MAPENT; h++ ){
            for( int w=0; w<PIXES_PER_MAPENT; w++ ){ //- each pix in mapent

                tmpPixPPos = mapEntRef.mcpos.get_ppos() + IntVec2{ w, h };
                tmpPixIdx = this->get_pixIdx_in_chunk( tmpPixPPos );
                tmpPixPtr = pixBufHeadPtr + tmpPixIdx;
                *tmpPixPtr = color;
            }
        } //- for each pix in mapent end ---

        //--- 再涂 副色 ---//
        for( const auto &pixWH : esrc::fieldBorderEntPixMaskSet.get_rand_maskSet( mapEntRef.fieldBorderType ) ){

            tmpPixPPos = mapEntRef.mcpos.get_ppos() + IntVec2{ pixWH.x, pixWH.y };
            tmpPixIdx = this->get_pixIdx_in_chunk( tmpPixPPos );
            tmpPixPtr = pixBufHeadPtr + tmpPixIdx;
            *tmpPixPtr = nearColor;
        }

        //--- 若为 border mapEnt -----//
        /*
        if( mapEntRef.is_fieldBorder ){
            color = get_fieldPtr_by_key(mapEntRef.fieldKey)->color; //- 目标颜色
            nearColor = get_fieldPtr_by_key(mapEntRef.nearbyFieldKey)->color; //- 副色
            maskIdx = apply_a_fieldBorderEntMask_idx();

            for( int h=0; h<PIXES_PER_MAPENT; h++ ){
                for( int w=0; w<PIXES_PER_MAPENT; w++ ){ //- each pix in mapent

                    tmpPixPPos = mapEntRef.mcpos.get_ppos() + IntVec2{ w, h };
                    tmpPixIdx = this->get_pixIdx_in_chunk( tmpPixPPos );
                    tmpPixPtr = pixBufHeadPtr + tmpPixIdx;

                    if( get_fieldBorderEntMask_val( maskIdx, w, h ) == true ){
                        *tmpPixPtr = nearColor;
                    }else{
                        *tmpPixPtr = color;
                    }
                    
                }
            } //- for each pix in mapent end ---


        //--- 若不是 border mapEnt -----//
        }else{
            color = get_fieldPtr_by_key(mapEntRef.fieldKey)->color; //- 目标颜色
            for( int h=0; h<PIXES_PER_MAPENT; h++ ){
                for( int w=0; w<PIXES_PER_MAPENT; w++ ){ //- each pix in mapent

                    tmpPixPPos = mapEntRef.mcpos.get_ppos() + IntVec2{ w, h };
                    tmpPixIdx = this->get_pixIdx_in_chunk( tmpPixPPos );
                    tmpPixPtr = pixBufHeadPtr + tmpPixIdx;
                    *tmpPixPtr = color;
                }
            } //- for each pix in mapent end ---
        }
        */
    } //- for each mapent end --


    //---------------------------//
    //   正式用 texture 生成 name
    //---------------------------//
    this->mapTex.creat_texName();

    this->is_assign_pixels_2_mapent_done = true;
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
    return (size_t)(h*ENTS_PER_CHUNK + w);
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
    return (size_t)( h*PIXES_PER_CHUNK + w );
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

    u8_t r;
    u8_t g;
    u8_t b;

    IntVec2    tmpEntMPos;
    IntVec2    pposOff;   //- 通用偏移向量

    float   off;
    float   freq = 1.7;

    PixData   pixData;//- each pix

    int    count;

    int    randVal;

    texBufHeadPtr = this->mapTex.get_texBufHeadPtr();
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
                //...

                for( int ph=0; ph<PIXES_PER_MAPENT; ph++ ){
                    for( int pw=0; pw<PIXES_PER_MAPENT; pw++ ){ //------ each pix in mapent ------

                        pixData.init( mpos_2_ppos(tmpEntMPos) + IntVec2{pw,ph} );
                        pposOff = pixData.ppos - this->mcpos.get_ppos();
                        pixData.pixIdx_in_chunk = pposOff.y * PIXES_PER_CHUNK + pposOff.x;
                        pixData.texPixPtr = texBufHeadPtr + pixData.pixIdx_in_chunk;

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
                                    pixData.fieldDataPtr = (FieldData*)&fieldDataRef;
                                    break;
                                }
                            }else{     //- 第4个 field
                                pixData.fieldDataPtr = (FieldData*)&fieldDataRef;
                            }
                        } //--- 周边4个 field 信息 end ---


                        //--------------------------------//
                        //  计算 本pix  alti
                        //--------------------------------//
                        pixData.alti.set( pixData.ppos, pixData.fieldDataPtr->ecoPtr );


                        //--------------------------------//
                        // 数据收集完毕，将部分数据 传递给 ent
                        //--------------------------------//
                        if( (ph==HALF_PIXES_PER_MAPENT) && (pw==HALF_PIXES_PER_MAPENT) ){//- ent 中点 pix

                            //...

                        }


                        //--------------------------------//
                        //    正式给 pix 上色
                        //--------------------------------//

                        if( pixData.alti.lvl < -1 ){ //- underwater

                            r = color_water.r - (-pixData.alti.lvl)*13;
                            g = color_water.g - (-pixData.alti.lvl)*13;
                            b = color_water.b - (-pixData.alti.lvl)*13;

                        }else if( pixData.alti.lvl == -1 ){ //- -1

                            r = color_water.r;
                            g = color_water.g;
                            b = color_water.b;

                        }else if( pixData.alti.lvl == 0 ){ //- 0
                            //-- 此时的 alti.val 一定是个 刚刚大于0的值

                            /*
                            if( pixData.alti.val == 0 ){
                                randVal = 0;
                            }else{
                                randVal = uDistribution_regular(randEngine) % (pixData.alti.val*2);
                            }

                            if( randVal < 3 ){ //- 1/n 几率
                                r = color_sand.r;
                                g = color_sand.g;
                                b = color_sand.b;

                            }else{
                                color = pixData.fieldDataPtr->ecoPtr->color_low;
                                r = (u8_t)(color.r + pixData.fieldDataPtr->fieldPtr->lColorOff_r);
                                g = (u8_t)(color.g + pixData.fieldDataPtr->fieldPtr->lColorOff_g);
                                b = (u8_t)(color.b + pixData.fieldDataPtr->fieldPtr->lColorOff_b);
                            }
                            */

                            r = color_sand.r;
                            g = color_sand.g;
                            b = color_sand.b;


                        }else{ //- land
                        
                            color = pixData.fieldDataPtr->ecoPtr->color_low;
                            r = (u8_t)(color.r + pixData.fieldDataPtr->fieldPtr->lColorOff_r);
                            g = (u8_t)(color.g + pixData.fieldDataPtr->fieldPtr->lColorOff_g);
                            b = (u8_t)(color.b + pixData.fieldDataPtr->fieldPtr->lColorOff_b);

                        }
                        
                        *pixData.texPixPtr = RGBA{ r,g,b,255 };


                    }
                } //- each pix in mapent end ---

            }
        } //- each ent in field end --


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

    MapField     *targetFieldPtr = esrc::find_or_insert_the_field( _fieldKey );
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
            tmpFieldPtr = esrc::find_or_insert_the_field( fieldMPos_2_fieldKey(tmpFieldMPos) );
        }

        nearFour_fieldDatas.insert({ -(tmpFieldPtr->occupyWeight), FieldData{tmpFieldPtr,fieldInfo.quad} }); //- copy
                        //- 通过负数，来实现 倒叙排列，occupyWeight 值大的排前面
        count++;
    }

    return targetFieldPtr;
}






}//------------- namespace: end --------------//

/*
 * ========================= GameObj.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2018.11.24
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#include "GameObj.h" 

//-------------------- CPP --------------------//
#include <functional>

//-------------------- Engine --------------------//
#include "Chunk.h"
#include "esrc_colliEntSet.h"
#include "esrc_chunk.h"
#include "esrc_shader.h"

using namespace std::placeholders;

//#include "tprDebug.h" //- tmp

/* ===========================================================
 *                   init_for_regularGo
 * -----------------------------------------------------------
 */
void GameObj::init_for_regularGo(   const IntVec2 mpos_,
                                    const IntVec2 pposOff_ ){
    
    //-----------------------//
    //    choose goPos
    //-----------------------//
    this->goPosUPtr = std::make_unique<GameObjPos>();
    this->goPosUPtr->init(   mpos_,
                        pposOff_,
                        std::bind( [this](){ return this->rootColliEntHeadPtr->rootAnchorCompass; }), 
                        std::bind( [this](){ return this->rootColliEntHeadPtr->off_from_rootAnchor_2_mapEntMid; }) );
                            // now, this->rootColliEntHeadPtr == nullptr;
                            // but this bind can work
    //-- Must Release Another One !!! --
    if( this->uiGoPosUPtr != nullptr ){
        this->uiGoPosUPtr = nullptr;
    } 

    GameObjPos *goPosPtr = this->goPosUPtr.get(); //- use for param 

    //-- bind functors --
    this->get_pos_currentDPos = std::bind( &GameObjPos::get_currentDPos, goPosPtr );
    this->set_pos_alti        = std::bind( &GameObjPos::set_alti, goPosPtr,  _1 );
    this->get_pos_alti        = std::bind( &GameObjPos::get_alti, goPosPtr );


    // only in goPos:
    this->get_goPos_currentMPos              = std::bind( &GameObjPos::get_currentMPos, goPosPtr );
    this->init_goPos_currentDPos             = std::bind( &GameObjPos::init_currentDPos, goPosPtr );
    this->calc_goPos_current_pposOff         = std::bind( &GameObjPos::calc_current_pposOff, goPosPtr );
    this->accum_goPos_current_dpos_and_mcpos = std::bind( &GameObjPos::accum_current_dpos_and_mcpos, goPosPtr, _1,_2,_3 );
    this->calc_goPos_rootAnchor_midDPos      = std::bind( &GameObjPos::calc_rootAnchor_midDPos, goPosPtr );

    // only in uiGoPos:
    this->accum_uiGoPos_currentDPos = nullptr;

    //-----------------------//
    //    collision
    //-----------------------//
    this->collisionUPtr = std::make_unique<Collision>(*this);
                            //-- 在未来，是否加载 collision 组件，可以由 go 的创建者 手动设置
                            //   目前先默认，所有 常规go，都加载 此 组件

    //-----------------------//
    //         oth
    //-----------------------//
    this->currentChunkKey = anyMPos_2_chunkKey( this->get_goPos_currentMPos() );
    //...
}

/* ===========================================================
 *                   init_for_uiGo
 * -----------------------------------------------------------
 */
void GameObj::init_for_uiGo(const glm::dvec2 &basePointProportion_,
                            const glm::dvec2 &offDPos_ ){

    //-----------------------//
    //    choose uiGoPos
    //-----------------------//
    this->uiGoPosUPtr = std::make_unique<UIAnchor>();
    this->uiGoPosUPtr->init( basePointProportion_, offDPos_ );

    //-- Must Release Another One !!! --
    if( this->goPosUPtr != nullptr ){
        this->goPosUPtr = nullptr;
    }

    UIAnchor *uiGoPosPtr = this->uiGoPosUPtr.get(); //- use for param 

    //-- bind functors --
    this->get_pos_currentDPos = std::bind( &UIAnchor::get_currentDPos, uiGoPosPtr );
    this->set_pos_alti        = std::bind( &UIAnchor::set_alti, uiGoPosPtr, _1 );
    this->get_pos_alti        = std::bind( &UIAnchor::get_alti, uiGoPosPtr );


    // only in goPos:
    this->get_goPos_currentMPos = nullptr;
    this->init_goPos_currentDPos = nullptr;
    this->calc_goPos_current_pposOff = nullptr;
    this->accum_goPos_current_dpos_and_mcpos = nullptr;
    this->calc_goPos_rootAnchor_midDPos = nullptr;

    // only in uiGoPos:
    this->accum_uiGoPos_currentDPos = std::bind( &UIAnchor::accum_currentDPos, uiGoPosPtr, _1 );

    //-----------------------//
    //    collision
    //-----------------------//
    this->collisionUPtr = nullptr;

    //-----------------------//
    //         oth
    //-----------------------//
    //...
}


/* ===========================================================
 *                     creat_new_goMesh
 * -----------------------------------------------------------
 * -- 通过一组参数来实现 gomesh 的初始化。
 * -- 在这个函数结束hou，仅剩下一件事要做： gomesh.bind_animAction( "god", "jump" );
 */
GameObjMesh &GameObj::creat_new_goMesh( const std::string &name_,
                                    const std::string &animFrameSetName_,
                                    const std::string &actionName_,
                                    RenderLayerType    layerType_,
                                    ShaderProgram     *pixShaderPtr_,
                                    const glm::vec2   pposOff_,
                                    double             off_z_,
                                    bool              isVisible_ ){

    tprAssert( this->goMeshs.find(name_) == this->goMeshs.end() );
    this->goMeshs.insert({ name_, std::make_unique<GameObjMesh>(*this) }); 
    GameObjMesh &gmesh = *(this->goMeshs.at(name_));

    //-- bind_animAction --//
    gmesh.bind_animAction( animFrameSetName_, actionName_ ); // Must Before Everything!!!

    //----- init -----//
    gmesh.set_pic_renderLayer( layerType_ ); 
    gmesh.set_pic_shader_program( pixShaderPtr_ );
    if( gmesh.isHaveShadow ){
        gmesh.set_shadow_shader_program( &esrc::get_rect_shader() ); //- 暂时自动选配 tmp
    }
    
    //-- goMesh pos in go --
    gmesh.set_pposOff(pposOff_);
    gmesh.set_off_z( static_cast<float>(off_z_));

    //-- flags --//
    gmesh.isVisible = isVisible_;

    //-- rootColliEntHeadPtr --//
    if( name_ == std::string{"root"} ){
        this->rootColliEntHeadPtr = &gmesh.get_currentFramePos().get_colliEntHead();
    }

    return gmesh;
}

/* ===========================================================
 *                    init_check
 * -----------------------------------------------------------
 */
void GameObj::init_check(){
    tprAssert( this->rootColliEntHeadPtr );
}


/* ===========================================================
 *                    reCollect_chunkKeys     [ IMPORTANT !!! ]
 * -----------------------------------------------------------
 * 遍历当前 rootCES 中每个 colliEnt，收集其所在 chunkKeys
 * ---
 * 此函数 只是单纯记录 本go相关的所有 chunk key 信息。
 * 此过程中并不访问 chunk实例 本事。所有，就算相关 chunk 尚未创建，也不影响本函数的执行。
 * ---
 * 目前只在2处位置执行：
 * -1- GameObj::signUp_newGO_to_mapEnt()
 * -2- Move::renderUpdate_inn()
 */
void GameObj::reCollect_chunkKeys(){

    if( this->isMoveCollide == false ){
        //return;
    }

    IntVec2      cesMPos = this->get_rootCES_leftBottom_MPos();
    IntVec2      tmpEntMPos  {};
    chunkKey_t   tmpChunkKey {};

    this->chunkKeys.clear();
    const ColliEntSet &doCesRef = esrc::get_colliEntSetRef(this->rootColliEntHeadPtr->colliEntSetIdx);

    for( const auto &mcpos : doCesRef.get_colliEnts() ){ //- each collient mcpos
        tmpEntMPos = mcpos.get_mpos() + cesMPos;
        tmpChunkKey = anyMPos_2_chunkKey( tmpEntMPos );
        this->chunkKeys.insert( tmpChunkKey ); //- copy
    } //- each collient mcpos end --
}



/* ===========================================================
 *                 signUp_newGO_to_mapEnt
 * -----------------------------------------------------------
 * -- 将 新建go 的 colliEnts 登记到所有对应的 mapent 上去。
 * 难点：
 *    有些身处 chunk边缘的 “临界go” 的 collient，可以位于 隔壁chunk
 *    而这个 隔壁 chunk，可能尚未创建。（此处会引发各种麻烦）
 *    目前解决方案：
 *      --- 新建go 跳过这个 collient 的登记工作
 *      --- 统计自己的 chunkeys,
 *      --- 一旦确认自己是 "临界go"，chunk容器 edgeGoIds 会动态记录这个数据
 *      --- 将 本goid，记录到 主chunk goids 容器中
 */
void GameObj::signUp_newGO_to_mapEnt(){

    //------------------------------//
    // --- 记录 go.currentChunkKey
    // --- 统计自己的 chunkeys
    // --- 一旦确认自己是 "临界go"，chunk容器 edgeGoIds 会动态记录这个数据
    // --- 将 本goid，记录到 主chunk goids 容器中
    //------------------------------//
    Chunk &currentChunkRef = esrc::get_chunkRef( this->currentChunkKey );

    this->reCollect_chunkKeys();
    if( this->chunkKeys.size() > 1 ){
        currentChunkRef.insert_2_edgeGoIds( this->id );
    }
    currentChunkRef.insert_2_goIds( this->id ); //- always

    //------------------------------//
    if( this->isMoveCollide == false ){
        return;
    }

    //------------------------------//
    //  signUp each collient to mapEnt
    //------------------------------//
    IntVec2     colliEntMPos {}; //- each collient mcpos [world-abs-pos]
    chunkKey_t  tmpChunkKey  {}; //- each collient current chunkKey

    const ColliEntHead &cehRef = this->get_rootColliEntHeadRef();
    const ColliEntSet &cesRef = esrc::get_colliEntSetRef( cehRef.colliEntSetIdx );
    IntVec2  cesMPos = this->get_rootCES_leftBottom_MPos(); //- rootCES left-bottom mcpos [world-abs-pos]
    
    for( const auto &i : cesRef.get_colliEnts() ){ //- each collient in rootCES

        colliEntMPos = i.get_mpos() + cesMPos;
        tmpChunkKey = anyMPos_2_chunkKey( colliEntMPos );

        //-- 如果 colliEnt所在 chunk 尚未创建，表明此 go 为 “临界go”。
        // 此时显然不能去调用 esrc::get_memMapEntRef_in_activeChunk(), 会出错。
        // 将会暂时 忽略掉这个 collient 的登记工作，
        // 这个工作，会等到 目标chunk 创建阶段，再补上: 
        // 在 signUp_nearby_chunks_edgeGo_2_mapEnt() 中
        auto chunkState = esrc::get_chunkMemState(tmpChunkKey);
        if( (chunkState==ChunkMemState::NotExist) || (chunkState==ChunkMemState::OnCreating) ){
            continue;
        }

        //---- 正式注册 collient 到 mapents 上 -----
        auto &mapEntRef = esrc::get_memMapEntRef_in_activeChunk( colliEntMPos );

        //-- 并不检测 当前 mapent 中是否有 重合的 go。而是直接 将数据 存入 mapent
        mapEntRef.insert_2_major_gos(   this->id, 
                                        cehRef.lGoAltiRange,
                                        cehRef.isCarryAffect );
    }
}


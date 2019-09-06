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
#include "esrc_chunk.h"
#include "esrc_shader.h"

using namespace std::placeholders;

#include "tprDebug.h" //- tmp

/* ===========================================================
 *                   init_for_regularGo
 * -----------------------------------------------------------
 */
void GameObj::init_for_regularGo( const glm::dvec2 &dpos_ ){
    
    //-----------------------//
    //    choose goPos
    //-----------------------//
    this->goPosUPtr = std::make_unique<GameObjPos>();      
    this->goPosUPtr->init( dpos_ );

    //-- Must Release Another One !!! --
    if( this->uiGoPosUPtr != nullptr ){
        this->uiGoPosUPtr = nullptr;
    } 

    GameObjPos *goPosPtr = this->goPosUPtr.get(); //- use for param 

    //-- bind functors --
    this->accum_currentDPos   = std::bind( &GameObjPos::accum_currentDPos, goPosPtr, _1 );
    this->set_pos_alti        = std::bind( &GameObjPos::set_alti, goPosPtr,  _1 );
    this->get_currentDPos     = std::bind( &GameObjPos::get_currentDPos, goPosPtr );
    this->get_pos_alti        = std::bind( &GameObjPos::get_alti, goPosPtr );
    

    //-----------------------//
    //    collision
    //-----------------------//
    this->collision2UPtr = std::make_unique<Collision2>(*this);

    //-----------------------//
    //         oth
    //-----------------------//
    this->currentChunkKey = anyMPos_2_chunkKey( dpos_2_mpos(this->get_currentDPos()) );

            
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
    this->accum_currentDPos   = std::bind( &UIAnchor::accum_currentDPos, uiGoPosPtr, _1 );
    this->set_pos_alti        = std::bind( &UIAnchor::set_alti, uiGoPosPtr, _1 );
    this->get_currentDPos     = std::bind( &UIAnchor::get_currentDPos, uiGoPosPtr );
    this->get_pos_alti        = std::bind( &UIAnchor::get_alti, uiGoPosPtr );

    //-----------------------//
    //    collision
    //-----------------------//
    this->collision2UPtr = nullptr;

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
        this->rootFramePos2Ptr = &gmesh.get_currentFramePos2();
    }

    return gmesh;
}

/* ===========================================================
 *                    init_check
 * -----------------------------------------------------------
 */
void GameObj::init_check(){

    tprAssert( this->rootFramePos2Ptr );

    //-- colliderType and isMoveCollide --
    // 只有支持 移动碰撞检测的 regularGo
    // 配合 携带了 非Nil碰撞体 的 rootGoMesh
    // 才会绑定 以下 functor
    if( this->isMoveCollide ){
        tprAssert( this->rootFramePos2Ptr->get_colliderType() != ColliderType::Nil );
        tprAssert( this->goPosUPtr != nullptr );

        //-- 主动调用，init signINMapEnts --- MUST!!!
        this->collision2UPtr->init_signInMapEnts( this->get_currentDPos(),
                    //std::bind( &FramePos2::get_colliPointDPosOffsRef, const_cast<FramePos2*>(this->rootFramePos2Ptr) ) 
                    std::bind( this->rootFramePos2Ptr->get_colliPointDPosOffsRef ) 
                    );

    }
    //-- 在检测完毕后，可以直接无视这些 flags 的混乱，仅用 go自身携带的 flag 来做状态判断 ...

}


/* ===========================================================
 *                    reCollect_chunkKeys     [ IMPORTANT !!! ]
 * -----------------------------------------------------------
 * 遍历当前 goPos 中每个 colliEnt，收集其所在 chunkKeys
 * ---
 * 此函数 只是单纯记录 本go相关的所有 chunk key 信息。
 * 此过程中并不访问 chunk实例 本事。所有，就算相关 chunk 尚未创建，也不影响本函数的执行。
 * ---
 * 目前只在2处位置执行：
 * -1- GameObj::signUp_newGO_to_mapEnt()
 * -2- Move::renderUpdate_inn()
 */
size_t GameObj::reCollect_chunkKeys(){

        tprAssert( this->isMoveCollide ); //- tmp

    this->chunkKeys.clear();
    for( const auto &mpos : this->get_currentSignINMapEntsRef() ){
        this->chunkKeys.insert( anyMPos_2_chunkKey(mpos) ); //- copy
    }
    return this->chunkKeys.size();
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
    currentChunkRef.insert_2_goIds( this->id ); //- always


    if( !this->isMoveCollide ){ return; }


    //------------------------------//
    size_t chunkKeySize = this->reCollect_chunkKeys();
    if( chunkKeySize > 1 ){
        currentChunkRef.insert_2_edgeGoIds( this->id );
    }
    //------------------------------//
    //  signUp each collient to mapEnt
    //------------------------------//
    chunkKey_t  tmpChunkKey  {}; //- each collient current chunkKey
        tprAssert( !this->get_currentSignINMapEntsRef().empty() ); //- tmp
    for( const auto &mpos : this->get_currentSignINMapEntsRef() ){

        tmpChunkKey = anyMPos_2_chunkKey( mpos );

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
        auto &mapEntRef = esrc::get_memMapEntRef_in_activeChunk( mpos );

        //-- 并不检测 当前 mapent 中是否有 重合的 go。而是直接 将数据 存入 mapent
        mapEntRef.insert_2_majorGos(   this->id );
    }

}



void GameObj::debug(){

    cout << "sizeof(go) = " << sizeof( *this )
        << "; sizeof(GO) = " << sizeof( GameObj )
        << endl;

}


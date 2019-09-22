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
    this->accum_dpos    = std::bind( &GameObjPos::accum_dpos, goPosPtr, _1 );
    this->set_pos_alti  = std::bind( &GameObjPos::set_alti, goPosPtr,  _1 );
    this->get_dpos      = std::bind( &GameObjPos::get_dpos, goPosPtr );
    this->get_pos_alti  = std::bind( &GameObjPos::get_alti, goPosPtr );
    

    //-----------------------//
    //    collision
    //-----------------------//
    this->collisionUPtr = std::make_unique<Collision>(*this);

                //- 如果是 isMoveCollide == false，是不是可以直接不初始化此 组件 ....

    //-----------------------//
    //         oth
    //-----------------------//
    this->currentChunkKey = anyMPos_2_chunkKey( dpos_2_mpos(this->get_dpos()) );

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
    this->accum_dpos   = std::bind( &UIAnchor::accum_dpos, uiGoPosPtr, _1 );
    this->set_pos_alti = std::bind( &UIAnchor::set_alti, uiGoPosPtr, _1 );
    this->get_dpos     = std::bind( &UIAnchor::get_dpos, uiGoPosPtr );
    this->get_pos_alti = std::bind( &UIAnchor::get_alti, uiGoPosPtr );

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
                                    animSubspeciesId_t subspeciesId_,
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
    //-- 确保提前设置好了 go.direction ！！！
    gmesh.bind_animAction( subspeciesId_, this->actionDirection, actionName_ ); // Must Before Everything!!!

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
        this->rootAnimActionPosPtr = &gmesh.get_currentAnimActionPos();
    }

    return gmesh;
}

/* ===========================================================
 *                    init_check
 * -----------------------------------------------------------
 */
void GameObj::init_check(){

    tprAssert( this->rootAnimActionPosPtr );

    //-- colliderType and isMoveCollide --
    this->colliDataFromJPtr = this->rootAnimActionPosPtr->get_colliDataFromJPtr();

    if( this->isMoveCollide ){
        tprAssert( this->get_colliderType() != ColliderType::Nil );
        tprAssert( this->goPosUPtr != nullptr );

        //-- 主动调用，init signINMapEnts --- MUST!!!
        this->collisionUPtr->init_signInMapEnts( this->get_dpos(),
                    std::bind( &ColliDataFromJ::get_colliPointDPosOffs, this->colliDataFromJPtr )
                    );
    }
    //-- 在检测完毕后，可以直接无视这些 flags 的混乱，仅用 go自身携带的 flag 来做状态判断 ...
}


/* ===========================================================
 *           rebind_rootAnimActionPosPtr
 * -----------------------------------------------------------
 *          
 * 
 *  未完工。。。
 */
void GameObj::rebind_rootAnimActionPosPtr(){
    //-- 仅在 root gomesh 切换 action 时才被调用 ！！！
    auto &gmesh = *(this->goMeshs.at("root"));
    this->rootAnimActionPosPtr = &gmesh.get_currentAnimActionPos();
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



void GameObj::debug(){

    cout << "sizeof(go) = " << sizeof( *this )
        << "; sizeof(GO) = " << sizeof( GameObj )
        << endl;

}


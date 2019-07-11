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
#include "esrc_colliEntSet.h"

//#include "tprDebug.h" //- tmp


/* ===========================================================
 *                         init
 * -----------------------------------------------------------
 */
void GameObj::init( const IntVec2 &mpos_ ){
    this->goPos.init(   mpos_,
                        std::bind( [this](){ return this->rootColliEntHeadPtr->rootAnchorCompass; }), 
                        std::bind( [this](){ return this->rootColliEntHeadPtr->off_from_rootAnchor_2_mapEntMid; }) );
                            // now, this->rootColliEntHeadPtr == nullptr;
                            // but this bind can work
    this->currentChunkKey = anyMPos_2_chunkKey( this->goPos.get_currentMPos() );
    //...
}


/* ===========================================================
 *                     creat_new_goMesh
 * -----------------------------------------------------------
 * -- 通过一组参数来实现 gomesh 的初始化。
 * -- 在这个函数结束hou，仅剩下一件事要做： gomesh.bind_animAction( "god", "jump" );
 */
void GameObj::creat_new_goMesh( const std::string &name_,
                                        const std::string &animFrameSetName_,
                                        const std::string &actionName_,
                                        RenderLayerType    layerType_,
                                        ShaderProgram     *pixShaderPtr_,
                                        ShaderProgram     *shadowShaderPtr_,
                                        const glm::vec2   pposOff_,
                                        double             off_z_,
                                        bool              isVisible_,
                                        bool              isCollide_,
                                        bool              isFlipOver_ ){

        tprAssert( this->goMeshs.find(name_) == this->goMeshs.end() );
    this->goMeshs.insert({ name_, std::make_unique<GameObjMesh>(*this) }); 
    GameObjMesh &gmesh = *(this->goMeshs.at(name_));

    //----- init -----//
    gmesh.set_pic_renderLayer( layerType_ ); 
    gmesh.set_pic_shader_program( pixShaderPtr_ );
    gmesh.set_shadow_shader_program( shadowShaderPtr_ );

    //-- goMesh pos in go --
    gmesh.set_pposOff(pposOff_);
    gmesh.set_off_z( static_cast<float>(off_z_));

    //-- flags --//
    gmesh.isVisible = isVisible_;
    gmesh.isCollide = isCollide_;
    gmesh.isFlipOver = isFlipOver_;

    //-- bind_animAction --//
    gmesh.bind_animAction( animFrameSetName_, actionName_ );

    //-- rootColliEntHeadPtr --//
    if( name_ == std::string{"root"} ){
        this->rootColliEntHeadPtr = &gmesh.get_currentFramePos().get_colliEntHead();
    }
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
 */
void GameObj::reCollect_chunkKeys(){
    //-- only check rootGoMesh --
    if( this->get_goMeshRef("root").isCollide == false ){
        return;
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
 *                       debug
 * -----------------------------------------------------------
 */
/*
void GameObj::debug(){

    cout << "------ GameObj: " << id << " --------" << endl;

    cout<< "\nsizeof(GameObj): "      << sizeof(GameObj)
        << "\nsizeof(GameObjPos): "   << sizeof(GameObjPos)
        << "\nsizeof(Move): "         << sizeof(Move)
        << "\nsizeof(GameObjMesh): "  << sizeof(GameObjMesh)
        << "\nsizeof(ChildMesh): "    << sizeof(ChildMesh)
        << "\nsizeof(ActionSwitch): " << sizeof(ActionSwitch)
        << "\nsizeof(Collision): "    << sizeof(Collision)
        << endl;

    
    cout<< "\nAwake:        " << ( Awake==nullptr ? "nullptr" : "not nullptr" )
        << "\nStart:        " << ( Start==nullptr ? "nullptr" : "not nullptr" )
        << "\nRenderUpdate: " << ( RenderUpdate==nullptr ? "nullptr" : "not nullptr" )
        << "\nLogicUpdate:  " << ( LogicUpdate==nullptr ? "nullptr" : "not nullptr" )
        << "\nBeAffect:     " << ( BeAffect==nullptr ? "nullptr" : "not nullptr" )
        << endl;

    cout<< "\nspecies = " << species
        << "\nid_parent = " << parentId
        << endl;


    cout << "weight = " << weight << endl;

    cout<< "\nisTopGo: "           << ( isTopGo ? "true" : "false" )
        << "\nisActive: "          << ( isActive ? "true" : "false" )
        << "\nisDirty: "           << ( isDirty ? "true" : "false" )
        << "\nisControlByPlayer: " << ( isControlByPlayer ? "true" : "false" )
        << "\nisFlipOver: "        << ( isFlipOver ? "true" : "false" )
        << endl;

    cout << "binary.size() = " << binary.size()
        << endl;

    cout << "\n\n" << endl;
    
}
*/




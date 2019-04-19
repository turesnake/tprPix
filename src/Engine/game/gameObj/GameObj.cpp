/*
 * ========================= GameObj.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2018.11.24
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#include "GameObj.h" 


//-------------------- Engine --------------------//
#include "esrc_colliEntSet.h"


//#include "debug.h" //- tmp


/* ===========================================================
 *                         init
 * -----------------------------------------------------------
 */
void GameObj::init(){
    this->collision.init( static_cast<GameObj*>(this) );
    this->goPos.init( static_cast<GameObj*>(this) ); //- MUST before move.init()
    this->move.init( static_cast<GameObj*>(this), &this->goPos, &this->collision );
    this->actionSwitch.init( static_cast<GameObj*>(this) );
    //...
}


/* ===========================================================
 *                     creat_new_goMesh
 * -----------------------------------------------------------
 * -- 通过一组参数来实现 gomesh 的初始化。
 * -- 在这个函数结束hou，仅剩下一件事要做： gomesh.animFrameIdxHandle.bind_xxx()
 */
GameObjMesh &GameObj::creat_new_goMesh( const std::string &_name,
                                        const std::string &_animFrameSetName,
                                        RenderLayerType    _layerType,
                                        ShaderProgram     *_pixShaderPtr,
                                        ShaderProgram     *_shadowShaderPtr,
                                        const glm::vec2   _pposOff,
                                        float             _off_z,
                                        bool              _isVisible,
                                        bool              _isCollide,
                                        bool              _isFlipOver ){

    // ***| INSERT FIRST, INIT LATER  |***
    GameObjMesh  goMesh; //- tmp 
    this->goMeshs.insert({ _name, goMesh }); //- copy
    GameObjMesh &gmesh = this->goMeshs.at(_name);

    //----- init -----//
    gmesh.bind_animFrameSet( _animFrameSetName );
    gmesh.init( const_cast<GameObj*>(this) );
    gmesh.set_pic_renderLayer( _layerType ); 
    gmesh.picMesh.set_shader_program( _pixShaderPtr );
    gmesh.shadowMesh.set_shader_program( _shadowShaderPtr );

    //-- goMesh pos in go --
    gmesh.pposOff = _pposOff;
    gmesh.off_z = _off_z;

    //-- flags --//
    gmesh.isVisible = _isVisible;
    gmesh.isCollide = _isCollide;
    gmesh.isFlipOver = _isFlipOver;

    //-----
    return gmesh;
}


/* ===========================================================
 *                    reset_chunkKeys
 * -----------------------------------------------------------
 * -- 重装填
 * 此函数 只是单纯记录 本go相关的所有 chunk key 信息。
 * 此过程中并不访问 chunk实例 本事。所有，就算相关 chunk 尚未创建，也不影响本函数的执行。
 */
void GameObj::reset_chunkKeys(){
    MapCoord     cesMCPos; //- 每个 ces左下角的 mcpos （世界绝对pos）
    MapCoord     tmpEntMCPos;
    chunkKey_t   tmpChunkKey;
    //-------
    this->chunkKeys.clear();
    //-------
    for( const auto &goMeshPair : this->goMeshs  ){  //- each goMesh
        const GameObjMesh &meshRef = goMeshPair.second;
        if( meshRef.isCollide == false ) continue; //- 不参与碰撞检测的 gomesh 直接跳过

        for( const auto &doCehRef : meshRef.get_currentFramePos().get_colliEntHeads() ){ //-- each do_colliEntHead
            cesMCPos.set_by_mpos(  anyPPos_2_mpos(  this->goPos.get_currentPPos() + 
                                                    doCehRef.pposOff_fromRootAnchor ) );
            const ColliEntSet &doCesRef = esrc::colliEntSets.at(doCehRef.colliEntSetIdx); //- get do_ces_ref

            for( const auto &mcpos : doCesRef.get_colliEnts() ){ //- each collient mcpos
                tmpEntMCPos = mcpos + cesMCPos;
                tmpChunkKey = anyMPos_2_chunkKey( tmpEntMCPos.get_mpos() );
                this->chunkKeys.insert( tmpChunkKey ); //- copy
            } //- each collient mcpos end --
        } //-- each do_colliEntHead end --
    } //- each goMesh end ---
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




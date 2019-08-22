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

//#include "tprDebug.h" //- tmp

/* ===========================================================
 *                         init
 * -----------------------------------------------------------
 */
void GameObj::init( const IntVec2 mpos_,
                    const IntVec2 pposOff_ ){
    this->goPos.init(   mpos_,
                        pposOff_,
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
GameObjMesh &GameObj::creat_new_goMesh( const std::string &name_,
                                    const std::string &animFrameSetName_,
                                    const std::string &actionName_,
                                    RenderLayerType    layerType_,
                                    ShaderProgram     *pixShaderPtr_,
                                    const glm::vec2   pposOff_,
                                    double             off_z_,
                                    bool              isVisible_,
                                    bool              isCollide_ ){

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
    gmesh.isCollide = isCollide_;

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
    //-- only check rootGoMesh --
    if( this->get_goMeshRef("root").isCollide == false ){
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
    //    only check rootGoMesh
    //------------------------------//
    if( this->get_goMeshRef("root").isCollide == false ){
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
        << endl;

    cout << "binary.size() = " << binary.size()
        << endl;

    cout << "\n\n" << endl;
    
}
*/




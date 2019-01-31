/*
 * ========================= GameObj.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2018.11.24
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *   
 * ----------------------------
 */
#include "GameObj.h" 

//-------------------- CPP --------------------//
#include <string>

using std::string;

//#include "debug.h" //- tmp


//============== 类静态成员 ===============//
ID_Manager  GameObj::id_manager { ID_TYPE::U64, 1};


/* ===========================================================
 *                         init
 * -----------------------------------------------------------
 */
void GameObj::init(){
    collision.init( (GameObj*)this );
    goPos.init( (GameObj*)this ); //- MUST before move.init()
    move.init( (GameObj*)this, &goPos, &collision );
    
    //...
}


/* ===========================================================
 *                     creat_new_goMesh
 * -----------------------------------------------------------
 * -- 在 goMeshs 容器中添加一个 新GoMesh实例，
 * -- 再返回这个 GoMesh实例的 指针
 */
GameObjMesh &GameObj::creat_new_goMesh( const std::string &_name ){

    // ***| INSERT FIRST, INIT LATER  |***
    GameObjMesh  goMesh; //- tmp 
    goMeshs.insert({ _name, goMesh }); //- copy
    return goMeshs.at(_name);
}





/* ===========================================================
 *                       debug
 * -----------------------------------------------------------
 */
/*
void GameObj::debug(){

    cout << "------ GameObj: " << id << " --------" << endl;

    cout<< "\nAwake:        " << ( Awake==nullptr ? "nullptr" : "not nullptr" )
        << "\nStart:        " << ( Start==nullptr ? "nullptr" : "not nullptr" )
        << "\nRenderUpdate: " << ( RenderUpdate==nullptr ? "nullptr" : "not nullptr" )
        << "\nLogicUpdate:  " << ( LogicUpdate==nullptr ? "nullptr" : "not nullptr" )
        << "\nBeAffect:     " << ( BeAffect==nullptr ? "nullptr" : "not nullptr" )
        << "\n"
        << "\nspecies = " << species
        << "\nis_top_go: " << ( is_top_go ? "true" : "false" )
        << "\nid_parent = " << parentId
        << "\nis_active: " << ( is_active ? "true" : "false" )
        << endl;

    cout << "velocity: " << velocity.x
        << ", " << velocity.y 
        << endl;

    cout << "weight = " << weight << endl;

    cout << "is_dirty: " << ( is_dirty ? "true" : "false" )
        << endl;

    //cout << "goMeshs.size() = " << goMeshs.size()
        //<< endl;

    cout << "binary.size() = " << binary.size()
        << endl;

    cout << "\n\n" << endl;
}
*/



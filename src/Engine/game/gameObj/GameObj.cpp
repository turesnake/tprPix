/*
 * ========================= GameObj.cpp ==========================
 *                          -- tpr --
 *                                        创建 -- 2018.11.24
 *                                        修改 -- 2018.11.24
 * ----------------------------------------------------------
 *   
 * ----------------------------
 */
#include "GameObj.h" 

//-------------------- CPP --------------------//
//#include <iostream> //-- cout
#include <string>

//using std::cout;
//using std::endl;
using std::string;


//============== 类静态成员 ===============//
ID_Manager  GameObj::id_manager { ID_TYPE::U64, 1};


/* ===========================================================
 *                         init
 * -----------------------------------------------------------
 */
void GameObj::init(){
    
}


/* ===========================================================
 *                     creat_new_mesh
 * -----------------------------------------------------------
 * -- 在 meshs 容器中添加一个 新mesh实例，
 * -- 再返回这个 mesh实例的 指针
 */
Mesh *GameObj::creat_new_mesh(){

    // ***| INSERT FIRST, INIT LATER  |***
    Mesh  mesh; //- tmp 
    meshs.push_back( mesh ); //- copy
    return  (Mesh*)&(meshs.back()); //- *** 也许有问题 ***
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
        << "\nid_parent = " << id_parent
        << "\nis_active: " << ( is_active ? "true" : "false" )
        << endl;

    cout << "targetPos: " << targetPos.x 
        << ", " << targetPos.y 
        << endl;

    cout << "currentPos: " << currentPos.x
        << ", " << currentPos.y 
        << endl;

    cout << "velocity: " << velocity.x
        << ", " << velocity.y 
        << endl;

    cout << "weight = " << weight << endl;

    cout << "is_dirty: " << ( is_dirty ? "true" : "false" )
        << endl;

    cout << "actionNames.size() = " << actionNames.size()
        << endl;

    //cout << "meshs.size() = " << meshs.size()
        //<< endl;

    cout << "binary.size() = " << binary.size()
        << endl;

    cout << "\n\n" << endl;
}
*/






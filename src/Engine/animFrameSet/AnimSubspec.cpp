/*
 * ===================== AnimSubspec.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.10.03
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#include "AnimSubspec.h"

#include "tprDebug.h"

//============== static ===============//
ID_Manager  AnimSubspec::id_manager { ID_TYPE::U32, 1};




/*
AnimAction *AnimSubspec::get_animActionPtr(   NineDirection   dir_,
                                            BrokenLvl       brokenLvl_,
                                            const std::string &actionName_ )const noexcept{


                if( this->animActions.find(dir_) == this->animActions.end() ){
                    cout << "dir = " <<  static_cast<int>(dir_) << endl;
                }


        tprAssert( this->animActions.find(dir_) != this->animActions.end() );
        auto &container_1 = this->animActions.at(dir_);
        //---
        tprAssert( container_1.find(brokenLvl_) != container_1.end() );
        auto &container_2 = container_1.at(brokenLvl_);
        //---
        tprAssert( container_2.find(actionName_) != container_2.end() );
        return container_2.at(actionName_).get();

}
*/






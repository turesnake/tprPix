/*
 * ====================== AnimActionPos.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.08.31
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#include "AnimActionPos.h"

//------------------- Engine --------------------//
#include "tprAssert.h"
#include "config.h" 
#include "tprMath.h"
#include "calc_colliPoints.h"

#include "tprDebug.h" 

//============== static ===============//
ID_Manager  AnimActionPos::id_manager { ID_TYPE::U32, 1};


/* ===========================================================
 *                  init_from_semiData
 * -----------------------------------------------------------
 */
void AnimActionPos::init_from_semiData( const AnimActionSemiData &semiData_ ){

        this->rootAnchorDPosOff = semiData_.get_rootAnchor();
        this->lGoAltiRange = semiData_.get_lGoAltiRange();

        auto colliderType = semiData_.get_colliderType();
        if( colliderType == ColliderType::Nil ){

            std::unique_ptr<ColliDataFromJ_Nil> nilUPtr = std::make_unique<ColliDataFromJ_Nil>();
            auto *nilPtr = nilUPtr.get();
            nilPtr->colliderType = ColliderType::Nil;
            this->colliDataFromJUPtr.reset( nilUPtr.release() );//- move uptr

        }else if( colliderType == ColliderType::Circular ){

            std::unique_ptr<ColliDataFromJ_Circular> cirUPtr = std::make_unique<ColliDataFromJ_Circular>();
            auto *cirPtr = cirUPtr.get();
            cirPtr->colliderType = ColliderType::Circular;
            cirPtr->moveColliRadius  = glm::length( semiData_.get_moveColliRadiusAnchor() - this->rootAnchorDPosOff );
            cirPtr->skillColliRadius = glm::length( semiData_.get_skillColliRadiusAnchor() - this->rootAnchorDPosOff );
            //-- colliPoints --
            calc_colliPoints_for_circular( cirPtr->colliPointDPosOffs, tprMax(cirPtr->moveColliRadius, cirPtr->skillColliRadius));

            cirPtr->makeSure_colliPointDPosOffs_isNotEmpty();
            this->colliDataFromJUPtr.reset( cirUPtr.release() );//- move uptr

        }else if( colliderType == ColliderType::Square ){
            
            std::unique_ptr<ColliDataFromJ_Square> squUPtr = std::make_unique<ColliDataFromJ_Square>();
            auto *squPtr = squUPtr.get();
            squPtr->colliderType = ColliderType::Square;
            this->colliDataFromJUPtr.reset( squUPtr.release() );//- move uptr

        }else{
            tprAssert(0);
        }
}


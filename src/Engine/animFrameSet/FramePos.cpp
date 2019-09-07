/*
 * ======================== FramePos.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.08.31
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#include "FramePos.h"

//------------------- Engine --------------------//
#include "tprAssert.h"
#include "config.h" 


#include "tprDebug.h" 


/* ===========================================================
 *                  init_from_semiData
 * -----------------------------------------------------------
 */
void FramePos::init_from_semiData( const FramePosSemiData &semiData_ ){

        this->colliderType = semiData_.get_colliderType();
        this->rootAnchorDPosOff = semiData_.get_rootAnchor();
        this->lGoAltiRange = semiData_.get_lGoAltiRange();

        if( this->colliderType == ColliderType::Nil ){
            this->circularUPtr = nullptr;
            this->capsuleUPtr  = nullptr;
            //---- functors -----//
            this->get_rootAnchor_2_tailAnchor = nullptr;
            this->get_moveColliRadius        = nullptr;
            this->get_skillColliRadius       = nullptr;
            this->get_colliPointDPosOffsRef  = nullptr;

        }else if( this->colliderType == ColliderType::Circular ){
            this->circularUPtr = std::make_unique<JData_InCircular>();
            this->capsuleUPtr  = nullptr;
            JData_InCircular *cirPtr = this->circularUPtr.get();
            //==
            cirPtr->moveColliRadius  = glm::length( semiData_.get_moveColliRadiusAnchor() - this->rootAnchorDPosOff );
            cirPtr->skillColliRadius = glm::length( semiData_.get_skillColliRadiusAnchor() - this->rootAnchorDPosOff );
            //-- colliPoints --
            for( const auto &i : semiData_.get_colliPointsRef() ){
                cirPtr->colliPointDPosOffs.push_back( i - this->rootAnchorDPosOff );
            }
            cirPtr->colliPointDPosOffs.push_back( glm::dvec2{0.0,0.0} );   //- rootAnchor

                cout<< "circular: moveColliRadius = " << cirPtr->moveColliRadius << "\n"
                    << "circular: skillColliRadius = " << cirPtr->skillColliRadius << "\n"
                    << "circular: colliPointDPosOffs.size() = " << cirPtr->colliPointDPosOffs.size() 
                    << endl;

            //---- functors -----//
            this->get_rootAnchor_2_tailAnchor = nullptr;
            this->get_moveColliRadius        = std::bind( &JData_InCircular::get_moveColliRadius, cirPtr );
            this->get_skillColliRadius       = std::bind( &JData_InCircular::get_skillColliRadius, cirPtr );
            this->get_colliPointDPosOffsRef  = std::bind( &JData_InCircular::get_colliPointDPosOffs, cirPtr );

                tprAssert( !this->get_colliPointDPosOffsRef().empty() );


        }else if( this->colliderType == ColliderType::Capsule ){
            this->circularUPtr = nullptr;
            this->capsuleUPtr = std::make_unique<JData_InCapsule>();
            JData_InCapsule *capPtr = this->capsuleUPtr.get();
            //==
            capPtr->rootAnchor_2_tailAnchor = semiData_.get_tailAnchor() - this->rootAnchorDPosOff;
            capPtr->moveColliRadius = glm::length( semiData_.get_moveColliRadiusAnchor() - this->rootAnchorDPosOff );
            capPtr->skillColliRadius = capPtr->moveColliRadius;
            capPtr->longLen = glm::length( capPtr->rootAnchor_2_tailAnchor );
            //-- colliPoints --
            for( const auto &i : semiData_.get_colliPointsRef() ){
                capPtr->colliPointDPosOffs.push_back( i - this->rootAnchorDPosOff );
            }
            capPtr->colliPointDPosOffs.push_back( glm::dvec2{0.0,0.0} );            //- rootAnchor
            capPtr->colliPointDPosOffs.push_back( capPtr->rootAnchor_2_tailAnchor );//- tailAnchor

                cout<< "capsule: moveColliRadius = " << capPtr->moveColliRadius << "\n"
                    << "capsule: skillColliRadius = " << capPtr->skillColliRadius << "\n"
                    << "capsule: colliPointDPosOffs.size() = " << capPtr->colliPointDPosOffs.size() 
                    << endl;

            //---- functors -----//
            this->get_rootAnchor_2_tailAnchor= std::bind( &JData_InCapsule::get_rootAnchor_2_tailAnchor, capPtr );
            this->get_moveColliRadius        = std::bind( &JData_InCapsule::get_moveColliRadius, capPtr );
            this->get_skillColliRadius       = std::bind( &JData_InCapsule::get_skillColliRadius, capPtr );
            this->get_colliPointDPosOffsRef  = std::bind( &JData_InCapsule::get_colliPointDPosOffs, capPtr );

                tprAssert( !this->get_colliPointDPosOffsRef().empty() );


        }else{
            tprAssert(0);
        }
}












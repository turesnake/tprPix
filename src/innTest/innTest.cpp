/*
 * ====================== innTest.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2020.01.01
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#include "innTest.h"


//------------------- Engine --------------------//
#include "tprAssert.h"

//------------------- Libs --------------------//
#include "magic_enum.hpp"

namespace innTest {//---------- namespace: innTest --------------//

enum class EC{
    T1 = 1,
    T2 = 2,
    T3 = 3,
};


void innTest_main(){

    //-- get name-str
    auto ec1Name = magic_enum::enum_name( EC::T1 );
    tprAssert( ec1Name == "T1" );

    //-- get enum ent
    auto ec2 = magic_enum::enum_cast<EC>( "T2" );
    tprAssert( ec2.has_value() );
    tprAssert( *ec2 == EC::T2 );


}



}//-------------------- namespace: innTest end --------------//

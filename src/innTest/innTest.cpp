/*
 * ====================== innTest.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2020.01.01
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#include "innTest.h"


//------------------- CPP --------------------//
#include <memory>

//------------------- Libs --------------------//
#include "magic_enum.hpp"

//------------------- Engine --------------------//
#include "tprAssert.h"
#include "History.h"
#include "DirAxes.h"
#include "DyBinary.h"

#include "NineDirection.h"


//-- tmp debug --
#include <string>
#include <iostream>
using std::cout;
using std::endl;


namespace innTest {//---------- namespace: innTest --------------//






class DC {
public:
    DC()=default;
    DC( const DC& ){ tprAssert(0); }
    DC & operator=( const DC &v_ );
    

    int tmp {888};
    std::unique_ptr<double> uptr {nullptr};
};


DC & DC::operator=( const DC &v_ ){
    tprAssert(0);
    return *this;
}






void innTest_main(){

    return;
    cout << "\n~~~~~~~~~~ innTest:start ~~~~~~~~~~\n" << endl;

    std::any a = std::make_any<DC>();


    tprAssert( a.has_value() );

    DC *cptr = std::any_cast<DC>( &a );

    cout << "cptr->tmp = " << cptr->tmp 
        << endl;

    //std::any k = a;





    
    



    cout << "\n\n~~~~~~~~~~ innTest:end ~~~~~~~~~~" << endl;
    
}



}//-------------------- namespace: innTest end --------------//

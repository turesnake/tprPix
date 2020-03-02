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

#include "random.h"
#include "simplexNoise.h"

#include "NineDirection.h"


//-- tmp debug --
#include <string>
#include <iostream>
using std::cout;
using std::endl;


namespace innTest {//---------- namespace: innTest --------------//


class BB{
public:
    BB()=default;
    double a {};
    double b {};
};


void innTest_main(){

    return;
    cout << "\n~~~~~~~~~~ innTest:start ~~~~~~~~~~\n" << endl;





    DyBinary binary {};

    binary.init<BB>();

    auto *ncptr = binary.get<BB>();
    const auto *cptr = binary.get<BB>();

    ncptr->a = 55.4;
    ncptr->a = -13455.4;


    double val = cptr->a;





    cout << "val = " << val << endl;






    cout << "\n\n~~~~~~~~~~ innTest:end ~~~~~~~~~~" << endl;
    
}



}//-------------------- namespace: innTest end --------------//

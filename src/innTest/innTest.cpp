/*
 * ====================== innTest.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2020.01.01
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#include "innTest.h"


//------------------- Libs --------------------//
#include "magic_enum.hpp"

//------------------- Engine --------------------//
#include "tprAssert.h"
#include "History.h"
#include "DirAxes.h"

#include "NineDirection.h"



#include <string>
#include <iostream>
using std::cout;
using std::endl;


namespace innTest {//---------- namespace: innTest --------------//






void innTest_main(){
    
    cout << "\n~~~~~~~~~~ innTest:start ~~~~~~~~~~\n" << endl;

    DirAxes a { 0.5, 0.7 };

        cout << "-1-" << endl;

    
    DirAxes b = a;

        cout << "-2-" << endl;

    cout << " " << a.get_x()
        << ", " << a.get_y()
        << "; origin: " << a.get_origin_x()
        << ", " << a.get_origin_y()
        << endl;

    cout << " " << b.get_x()
        << ", " << b.get_y()
        << "; origin: " << b.get_origin_x()
        << ", " << b.get_origin_y()
        << endl;
    



    cout << "\n\n~~~~~~~~~~ innTest:end ~~~~~~~~~~" << endl;
    
}



}//-------------------- namespace: innTest end --------------//

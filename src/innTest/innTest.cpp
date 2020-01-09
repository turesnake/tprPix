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

#include "NineDirection.h"



#include <string>
#include <iostream>
using std::cout;
using std::endl;


namespace innTest {//---------- namespace: innTest --------------//


class AAA{
public:
    AAA( int a_, int b_ ):
        a(a_),
        b(b_)
        {}

    int a {};
    int b {};
};


inline bool operator != ( AAA a_, AAA b_ ) noexcept {
    return ( (a_.a!=b_.a) || (a_.b!=b_.b) );
}



void innTest_main(){
    /*
    cout << "\n~~~~~~~~~~ innTest:start ~~~~~~~~~~\n" << endl;

    AAA aaa { 1,   2 };
    AAA bbb { 101, 202 };

    History<AAA> dir { aaa };

    cout << dir.get_isDirty() << endl; 


    dir.set_newVal( bbb );
        cout << dir.get_isDirty() << endl; 



    cout << "\n\n~~~~~~~~~~ innTest:end ~~~~~~~~~~" << endl;
    */
}



}//-------------------- namespace: innTest end --------------//

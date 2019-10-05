/*
 * ======================= checkPOD.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.10.04
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 * check some struct/class in app, make sure them are POD  
 */

//-------------------- CPP --------------------//
#include <type_traits>

#include <any>


//-------------------- Engine --------------------//
#include "dyParams.h"


#include <iostream>
using std::cout;
using std::endl;

template< typename T >
void print_pod_info( const std::string &str_ ){
    cout  << str_ << endl;
    bool b {};
    b = std::is_trivial<T>::value;
    if( b ){    cout << "    is Trivial;" << endl;
    }else{      cout << "    is Not Trivial;" << endl;
    }
    //---
    b = std::is_standard_layout<T>::value;
    if( b ){    cout << "    is Standard Layout;" << endl;
    }else{      cout << "    is Not Standard Layout;" << endl;
    }
    //---
    b = std::is_pod<T>::value;
    if( b ){    cout << "    is POD;" << endl;
    }else{      cout << "    is Not POD;" << endl;
    }
}


/*===========================================================
 *                     check_POD
 *-----------------------------------------------------------
 */
void check_POD(){

    //print_pod_info<MapAltitude>("MapAltitude");

    // 暂未被使用 ...

}










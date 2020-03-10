/*
 * ======================= checkPOD.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.10.04
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 * check some struct/class in app, make sure them are POD  
 */
#include "pch.h"
//-------------------- CPP --------------------//
#include <type_traits>
#include <any>


//-------------------- Engine --------------------//
#include "dyParams.h"


template< typename T >
void print_pod_info( const std::string &str_ ){

    tprDebug::console( str_ );
    bool b {};
    b = std::is_trivial<T>::value;
    if( b ){    tprDebug::console( "    is Trivial;" );
    }else{      tprDebug::console( "    is Not Trivial;" );
    }
    //---
    b = std::is_standard_layout<T>::value;
    if( b ){    tprDebug::console( "    is Standard Layout;" );
    }else{      tprDebug::console( "    is Not Standard Layout;" );
    }
    //---
    b = std::is_pod<T>::value;
    if( b ){    tprDebug::console( "    is POD;" );
    }else{      tprDebug::console( "    is Not POD;" );
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










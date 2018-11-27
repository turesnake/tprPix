

#include "GameWorldCoord.h"


#include <iostream>
#include <string>

using std::string;
using std::cout;
using std::endl;



void test_1();





void test_1(){

    GameWorldCoord gwCoord( 5.0f, 7.0f );

    /*
    glm::vec2 glwCoord;
    glwCoord = gwCoord.to_glWorldCoord();

    cout << "\nglwCoord.x = " << glwCoord.x
         << "\nglwCoord.y = " << glwCoord.y
         << endl;
    */

    GameWorldCoord gwc_2( 5.9f, 7.2f );

    if( gwc_2.is_equal_in_int(gwCoord) == true ){
        cout << "is equal";
    }else{
        cout << "not equal";
    }


}




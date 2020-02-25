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













void innTest_main(){

    return;
    cout << "\n~~~~~~~~~~ innTest:start ~~~~~~~~~~\n" << endl;





    int maxNum = 2000;
    std::vector<size_t> vv {};
    vv.reserve( maxNum * maxNum );

    //--------------------
    vv.clear();

    std::vector<glm::dvec2> dmpos {};

    for( int j=0; j<maxNum; j++ ){
        for( int i=0; i<maxNum; i++ ){
            glm::dvec2 dv { static_cast<double>(i), static_cast<double>(j)};
            dmpos.push_back( dv );
        }
    }


            double timeP_1 = glfwGetTime();

    for( const auto &i : dmpos ){
        double originPerlin = simplex_noise2( i );
        size_t uweight = blender_the_perlinNoise( originPerlin, 130617.7, 10000 ); //[0, 9999]
        //--
        vv.push_back( uweight );
    }
    
            double timeP_2 = glfwGetTime();


    //--------------------
    vv.clear();

    std::vector<IntVec2> impos {};

    for( int j=0; j<maxNum; j++ ){
        for( int i=0; i<maxNum; i++ ){
            impos.push_back( IntVec2{ i, j } );
        }
    }



            double timeP_3 = glfwGetTime();

    for( const auto &i : impos ){
        size_t uweight = calc_simple_uWeight( i );
        vv.push_back( uweight );
    }


            double timeP_4 = glfwGetTime();


        cout << "\n    time1:" << timeP_2 - timeP_1
            << "\n    time2: " << timeP_4 - timeP_3
            << endl;



    cout << "\n\n~~~~~~~~~~ innTest:end ~~~~~~~~~~" << endl;
    
}



}//-------------------- namespace: innTest end --------------//

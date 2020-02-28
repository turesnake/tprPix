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






class AAA{
public:
    AAA(double v_ ):
        a(v_),
        b(v_),
        c(v_),
        d(v_),
        e(v_),
        f(v_)
        {}


    double a;
    double b;
    double c;
    double d;
    double e;
    double f;
};






void innTest_main(){

    return;
    cout << "\n~~~~~~~~~~ innTest:start ~~~~~~~~~~\n" << endl;




    int maxNum = 10000000;

    std::vector<AAA> vi {};
    std::vector<std::optional<AAA>> vo {};
    std::vector<AAA> vout1 {};
    std::vector<AAA> vout2 {};

    vi.reserve( maxNum * 10 );
    vo.reserve( maxNum * 10 );
    vout1.reserve( maxNum * 10 );
    vout2.reserve( maxNum * 10 );


    for( int i=0; i<maxNum; i++ ){
        double val = static_cast<double>(i) + 0.001;
        vi.push_back( AAA{val} );
        vo.push_back( { AAA{val} } );
    }


    
            double timeP1 = glfwGetTime();

    for( const auto i : vi ){
        vout1.push_back(  i );
    }

            double timeP2 = glfwGetTime();




            double timeP3 = glfwGetTime();

    for( const auto &opt : vo ){
        if( opt.has_value() ){
            vout2.push_back( opt.value() );
        }else{
            tprAssert(0);
        }
    }
        
            double timeP4 = glfwGetTime();

    
    cout    << "\n  time_1: " << timeP2 - timeP1
            << "\n  time_2: " << timeP4 - timeP3
            << endl;

    return;

    //============================================//
    //        calc uWeight test
    //============================================//
    /*
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

    */


    cout << "\n\n~~~~~~~~~~ innTest:end ~~~~~~~~~~" << endl;
    
}



}//-------------------- namespace: innTest end --------------//

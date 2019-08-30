



#include "collide_test.h"
#include "circularCast.h"
#include "esrc_time.h"


#include "tprDebug.h"


#include <vector>



void collide_test(){
    //cout << "====== collide_test: begin ========= " << endl;

    //

    std::vector<double> sums {};

    int times = 10000;
    double sum0 = 10.0;
    for( int i=0; i<times; i++ ){
        sum0 += 210.0 / static_cast<double>(times);
        sums.push_back(sum0);
    }

    //------------
    TimeBase &timerRef = esrc::get_timer();
    double begin_time = timerRef.get_currentTime();

    double t {};
    for( auto &i : sums ){
        t = circularCast(  glm::dvec2( 23.0, 7.0 ), //- to_bego
                                glm::dvec2( 27.0, 0.0 ), //- to_target
                                i
                                );
    }

    double end_time = timerRef.get_currentTime();

        //cout<< "    timeCost = " << end_time - begin_time
        //    << endl;


    //cout << "====== collide_test: end ========= " << endl;
}












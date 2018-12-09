

#include "GameWorldCoord.h"

#include "MapEnt.h" 


#include <iostream>
#include <string>

using std::string;
using std::cout;
using std::endl;



void test_1();

void print_Fst_MapEnt( Fst_diskMapEnt *_me );
void print_Fst_memMapEnt( Fst_memMapEnt *_me );


/* ===========================================================
 *                        test_1
 * -----------------------------------------------------------
 */
void test_1(){

    
    Fst_diskMapEnt me;
    Fst_memMapEnt mme( &me );

    mme.is_land = 1;
    mme.mask_id = 5;
    mme.altitude = 7;
    mme.is_reserved = true;
    mme.is_covered = true;
    mme.is_cover_go_head = false;
    mme.tex_id = 13;
    mme.sec_data_id = 778;

    me = mme.m2d();


    print_Fst_MapEnt( &me );
    print_Fst_memMapEnt( &mme );

}



void print_Fst_MapEnt( Fst_diskMapEnt *_me ){

    cout << "Fst_diskMapEnt: "
        << "\nmask_id = " << (int)_me->mask_id
        << "\nfst_data = " << (int)_me->fst_data
        << "\ntex_id = " << _me->tex_id
        << "\nsec_data_id = " << _me->sec_data_id
        << "\n" <<endl;

}

void print_Fst_memMapEnt( Fst_memMapEnt *_me ){

    cout << "Fst_diskMapEnt: "
        << "\nis_land = " << _me->is_land
        << "\nmask_id = " << (int)_me->mask_id
        << "\naltitude = " << (int)_me->altitude
        << "\nis_reserved = " << _me->is_reserved
        << "\nis_covered = " << _me->is_covered
        << "\nis_cover_go_head = " << _me->is_cover_go_head
        << "\ntex_id = " << _me->tex_id
        << "\nsec_data_id = " << _me->sec_data_id
        << "\n" <<endl;

}





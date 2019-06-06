/*
 * ========================= tprFileModeT.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2018.10.15
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *   support all files/libs
 *   mode_t
 * ----------------------------
 */
#ifndef TPR_FILE_MODE_H_
#define TPR_FILE_MODE_H_


//- rw-------
#ifndef RW_______
    #define RW_______ ( S_IRUSR | S_IWUSR  )
#endif

//- rwx------
#ifndef RWX______
    #define RWX______ ( RW_______ | S_IXUSR )
#endif

//- rw-r--r--
#ifndef RW_R__R__
    #define RW_R__R__ ( RW_______ | S_IRGRP | S_IROTH )
#endif

//- rw-rw-r--
#ifndef RW_RW_R__
    #define RW_RW_R__ ( RW_R__R__ | S_IWGRP )
#endif


//- rwxr-xr-x
#ifndef RWXR_XR_X
    #define RWXR_XR_X ( RW_R__R__ | S_IXUSR | S_IXGRP | S_IXOTH )  
#endif


#endif 


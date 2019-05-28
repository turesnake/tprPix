/*
 * ========================= tprFileModeT.h ==========================
 *                          -- tpr --
 *                                        创建 -- 2018.10.15
 *                                        修改 -- 2018.10.15
 * ----------------------------------------------------------
 *   可以被 几乎所谓文件／库 重复调用的 通用h文件, 
 *   mode_t
 * ----------------------------
 */

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





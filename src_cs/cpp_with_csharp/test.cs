using System;
using System.Runtime.InteropServices;

namespace tprpix{
    
    
public class Test{

    //---  Cpp main() ---
    [DllImport("tprpix_cpp", EntryPoint = "cppMain_tmp")]
    public static extern int cppMain_tmp( [MarshalAs(UnmanagedType.LPStr)] string exeDirPath_ );

        
}




}

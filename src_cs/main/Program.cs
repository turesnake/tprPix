using System;

namespace tprpix{
    
class Program{
        
        
    static void Main(string[] args){

        Console.WriteLine("C#: start ");

        //---- exe dir path ------
        string exeDirPath = System.IO.Path.GetDirectoryName(Environment.GetCommandLineArgs()[0]);

        PInvoke.cppMain_tmp(exeDirPath);


        Console.WriteLine("C#: end ");

    }

}


}


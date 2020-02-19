# tprPix
a Cross-Platform, 2D Survival Sandbox Game Project. 

Based on **C++17/OpenGL/SQLite3**.

- [中文版](README.ZH_CN.md)

(**tprPix** is the name of the Project, **NOT** the Game.)

![screenShot.01](Docs/pngs/screenShots/ss.01.png)



## Which branch should I Download
Branch:**master** is OK.

## Which platform is supported
- **Win10**
- **MacOSX**(10.12 or later) 
- **Ubuntu**(16.04 or later) 

Maybe it can work in earlier versions, but I did not test Yet.


## Compiler compatibility
The recommended compiler is **Clang**, even if in **Win10**.

Make sure the compiler version support **C++17**.


## How to Install

### Mac OS X

The installation in **Mac** is very easy.

First,make sure you have installed:
- **CMake**(3.1 or later)
- **Clang**(new enough to support **C++17** )
- **OpenGL** libs

Then,open your terminal, and "cd" to the app's root folder:

    $ cd .../appRootDir/

after that, run a shell script:

    $ ./unix_release.sh

the shell script: `unix_release.sh`, will automatically install everything for you, which contains some folder replication work.
you can also choose `unix_debug.sh`, which will build a **DEBUG** app for you.

Now, All the Installation has **DONE**!!!

You can find **tprpix** file in **.../appRootDir/build/publish/** folder. 

**Double Click** it and **Start your Game**!!!


### Ubuntu 
Similar to  **Mac OS X**.

First,make sure you have installed:

**CMake**(3.1 or later)

    $ sudo apt-get install cmake

**Clang**(new enough to support **C++17** )

    $ sudo apt-get install llvm
    $ sudo apt-get install clang 

reset the default **C/C++ Compiler** by:

    $ sudo update-alternatives --config c++
    $ sudo update-alternatives --config cc

install some **Graphic Libs**:

    $ sudo apt-get install libx11-dev
    $ sudo apt-get install libglfw3-dev
    $ sudo apt-get install libsdl2-dev

Then,open your terminal, and "cd" to the app's root folder:

    $ cd .../appRootDir/

after that, run a shell script:

    $ ./unix_release.sh

the shell script: `unix_release.sh`, will automatically install everything for you, which contains some folder replication work.
you can also choose `unix_debug.sh`, which will build a **DEBUG** app for you.

Now, All the Installation has **DONE**!!!

You can find **tprpix** file in **.../appRootDir/build/publish/** folder. 

run the app in terminal:

    $ build/publish/tprpix

and **Start your Game**!!!



### Win10

To support **Clang** in **Win10**, We need to install **Visual Studio 2019** at first.

Once open **Visual Studio 2019**, click the button: `Open a local folder`, and choose the app's root folder ->

![open a local folder](Docs/pngs/win10/000.png)

select **Manage Configurations...** ->

![Manage Configurations](Docs/pngs/win10/001.png)

in **CMake Settings** panel, remove the existed configuration, and add a new one: **x64-Clang-Release** ->

![CMake Settings panel](Docs/pngs/win10/002.png)

![x64-Clang-Release](Docs/pngs/win10/003.png)

reset the **Build root** to `${projectDir}\build` ->

![Build root](Docs/pngs/win10/004.png)

back to **Solution Explorer** panel, right click the file: `CMakeLists.txt`, choose: `Generate Cache for tprPixelCPP`. ->

![CMakeLists.txt](Docs/pngs/win10/005.png)

![Generate Cache for tprPixelCPP](Docs/pngs/win10/006.png)

wait until the infomation: `CMake generation finished.` showed in **Output** panel.


After that, click `Build All` in `Build` Window.(or press `F7`) ->

![Build All](Docs/pngs/win10/009.png)

Then, the build work will begin. A few minutes later，you will find a infomation: `Build All succeeded.` in **Output** panel. This means that the work in **Visual Studio 2019** is DONE!

Go back to the app's root folder, find and double click `win.bat` file. 
This script file will automatically copy some folders to `app/build/publish/`, 

Now, All the Installation has **DONE**!!!

You can find **tprpix.exe** file in **.../appRootDir/build/publish/ folder**. 

**Double Click** it and **Start your Game**!!!




## How to Play
This game support both **Keyboard** and most mainstream **Joysticks**. 
Considering it's an action game, we recommend you to play with a **Joystick**(Xbox360-style).

### Joystick(Xbox360-Style):
- **LeftAxes** to move.

- **Button:A** to confirm. 
in game-world, you can push **Button:A** to Let the character speed up

- **Button:B** to let the character slow down in game-world.

- **Button:X/Y** to print some debug-infomation on terminal.

### Keyboard:
- **key:W/A/S/D** to move.
- **key:Enter** to confirm.
- **key:H/J/K/L** is mapped to **'A/B/X/Y'** in Joystick.

Once you enter the game-world, The only thing you can do is control a chicken(used for test), walk around and fly(By pressing the key: **'H'**(keyboard) or **'A'**(joystick) continuously)

Well,the remaining parts are still on developing.
So, please be  patient...


## License under the [BSD 3-Clause](LICENSE)

## Welcome feedback and suggestions
Since the project is still on developing,
more content will be launched later.

If you have any suggestions，please contact me.


## Contact
*   E-mail: [turesnake@gmail.com](mailto:turesnake@gmail.com)
*   E-mail: [turesnake@icloud.com](mailto:turesnake@icloud.com)
*   QQ: 651700603


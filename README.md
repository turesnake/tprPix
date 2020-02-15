# TPRPIX
a Cross-Platform, 2D Survival Sandbox Game Project. Based on C++17/OpenGL/SQLite3.


## Which branch should I Download
Branch:master is OK.

## Which platform is supported
Currently Win10 and MacOSX. I've only tested on Windows 10 and OSX (higher than 10.12 High Sierra). Maybe it can work in earlier versions.

## Compiler compatibility
The recommended compiler is **Clang**, even if in Win10.

Make sure the version support C++17.


## How to Installation

### Mac OS X

The installation in **Mac** is very easy.

Make sure you have installed CMake.

Then,open your terminal, and "cd" to the app's root folder:

    $ cd .../xxx/tprPixelGames/

after that, run a shell script:

    $ ./osx.sh

the shell script: `osx.sh`, will automatically install everything for you, which contains some folder replication work.

Now, All the Installation has **DONE**!!!

You can find **tprpix** file in **.../app/build/publish/** folder. 

Double Click it and **Start the Game**. 



### Win10

To support **Clang** in **Win10**, We need to install Visual Studio 2019 at first.

Once open Visual Studio 2019, click the button: `Open a local folder`, and choose the app's root folder ->

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


After that, click `Build All` in Build Window.(or press `F7`) ->

![Build All](Docs/pngs/win10/009.png)

Then, the build work will begin. A few minutes later，you will find a infomation: `Build All succeeded.` in Output panel. This means that the work in VS-2019 is DONE!

Go back to the app's root folder, find and double click `win.bat` file. 
This script file will automatically copy some folders to `app/build/publish/`, 

Now, All the Installation has **DONE**!!!

You can find **tprpix.exe** file in **.../app/build/publish/ folder**. 

Double Click it and **Start the Game**. 




## How to Play
This game support both Keyboard and most mainstream Joysticks. 
Considering it's an action game, we recommend you to use a Joystick(Xbox360-style).

### Joystick(Xbox360-Style):
- **LeftAxes** to move.

- **Button:A** to confirm. 
in game-world, you can push **Button:A** to Let the character speed up

- **Button:B/X/Y** to print some debug-infomation on terminal.

### Keyboard:
- **key:W/A/S/D** to move.
- **key:Enter** to confirm.
- **key:H/J/K/L** is mapped to **'A/B/X/Y'** in Joystick.

Once you enter the game-world, The only thing you can do is control a chicken(used for test), walk around and fly(By pressing the key: **'H'**(keyboard) or **'A'**(joystick) continuously)

Well,the remaining parts are still under development.
So, please be  patient...


## License
...[waiting for edit]...

## Welcome feedback and suggestions
...[waiting for edit]...

## Contact
*   E-mail: [turesnake@gmail.com](mailto:turesnake@gmail.com)
*   E-mail: [turesnake@icloud.com](mailto:turesnake@icloud.com)
*   QQ: 651700603


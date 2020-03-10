# tprPix
**tprPix** 是一个跨平台的，2D生存向沙盒游戏项目。

项目基于： **C++17/OpenGL/SQLite3**.

(注意，**tprPix** 是项目的名称, 而非游戏本身的名称 (我还没想好hhh...) )

![screenShot.01](Docs/pngs/screenShots/ss.01.png)


## 我该下载哪个分支
直接下载 Branch:**master** 即可。

## 此项目支持哪些平台
- **Win10**
- **MacOSX** (10.12 或更高版本) 
- **Ubuntu** (16.04 或更高版本) 

也许可以支持各平台的较早版本，但我尚未测试。


## 支持哪款编译器
**Clang** 是最推荐的编译器，即便是在 **Win10** 平台。
请确保你所下载和安装的 **Clang** 版本，支持 **C++17**。



## 如何安装

### Mac OS X

在 **Mac** 平台的安装非常简单。

首先，确保你已经成功安装如下程序：
- **CMake** (3.1 或更高版本; 若想使用PCH，Unity-builds 技术加速编译，推荐 3.16 或更高版本)
- **Clang** (足以支持 **C++17** 的较新版本)
- **OpenGL** 相关的库


然后，打开 **终端**（terminal），**"cd"** 至 本项目的根目录： 

    $ cd .../appRootDir/

接着，运行一份 shell 脚本程序：

    $ ./unix_release.sh

脚本 `unix_release.sh` 将会自动化地帮我们完成程序的 **build** 和本地化部署工作。
你也可以选择运行另一份脚本：`unix_debug.sh`， 正如名字所示，它将生存一份 **DEBUG** 版的程序。

现在，所有的安装工作都已经 **完成** 了!!!

你可以在目录： **.../appRootDir/build/publish/** 中找到可执行文件：**tprpix**。

双击它，开始我们的游戏!!!


### Ubuntu 
在 **Ubuntu** 上的安装与 **Mac OS X** 十分相似。

首先，请确保你已经安装如下程序：

**CMake** (3.1 或更高版本; 若想使用PCH，Unity-builds 技术加速编译，推荐 3.16 或更高版本)：

    $ sudo apt-get install cmake

**Clang** (足以支持 **C++17** 的较新版本)：

    $ sudo apt-get install llvm
    $ sudo apt-get install clang 

重设系统默认的 **C/C++编译器**：

    $ sudo update-alternatives --config c++
    $ sudo update-alternatives --config cc

再安装几个与图形有关的第三方库：

    $ sudo apt-get install libx11-dev
    $ sudo apt-get install libglfw3-dev
    $ sudo apt-get install libsdl2-dev

现在，打开 **终端**（terminal），**"cd"** 至 本项目的根目录： 

    $ cd .../appRootDir/

接着，运行一份 shell 脚本程序：

    $ ./unix_release.sh

脚本 `unix_release.sh` 将会自动化地帮我们完成程序的 **build** 和本地化部署工作。
你也可以选择运行另一份脚本：`unix_debug.sh`， 正如名字所示，它将生存一份 **DEBUG** 版的程序。

现在，所有的安装工作都已经 **完成** 了!!!

你可以在目录： **.../appRootDir/build/publish/** 中找到可执行文件：**tprpix**。

在 **终端** 中启动它：

    $ build/publish/tprpix

开始我们的游戏!!!



### Win10

为了支持 **Win10** 中的 **Clang**，我们首先要安装 **Visual Studio 2019**。

然后，启动 **Visual Studio 2019**，点选初始界面中的 `Open a local folder` 按钮，在弹出的窗口中，选择本程序的根目录 ->

![open a local folder](Docs/pngs/win10/000.png)

进入主界面后，选择 **Manage Configurations...** 按钮 ->

![Manage Configurations](Docs/pngs/win10/001.png)


在随之而来的 **CMake Settings** 面板中，移除掉已经存在的 **DEBUG** 配置，然后手动选择一份新的配置：**x64-Clang-Release** ->


![CMake Settings panel](Docs/pngs/win10/002.png)

![x64-Clang-Release](Docs/pngs/win10/003.png)

找到 **Build root** 一览，将其值修改为: `${projectDir}\build` ->

![Build root](Docs/pngs/win10/004.png)

在左侧的 **Solution Explorer** 面板中，右键单击文件：`CMakeLists.txt`，在弹出的下拉菜单中选择：`Generate Cache for tprPixelCPP` ->

![CMakeLists.txt](Docs/pngs/win10/005.png)

![Generate Cache for tprPixelCPP](Docs/pngs/win10/006.png)

这将帮助我们生成 cmake Cache 数据。
等待片刻，直到下方 **Output** 面板中出现 `CMake generation finished.` 信息。
这表示 cmake Cache 生成成功。


紧接着。在 **Visual Studio 2019** 左上方的 `Build` 菜单中，找到 `Build All` 按钮，点击它，开始正式的 **Build** 工作。（也可以使用快捷键:`F7`） ->

![Build All](Docs/pngs/win10/009.png)


**Build** 工作 将会持续数分钟，直到我们在下方的 **Output** 面板中看到 `Build All succeeded.` 的信息。这表示，**Build** 成功。我们在 **Visual Studio 2019**
中的工作就全部结束了。


最后的最后，回到本项目所在的根目录。寻找并双击一份名为 `win.bat` 的脚本文件。
它将帮助我们执行本地化部署工作。


现在，安装正式**完成**!!!

你可以在目录 **.../appRootDir/build/publish/** 中，找到可执行文件 **tprpix.exe**，
它就是我们的程序本体。

双击此文件，开始我们的游戏!!!



## 怎么玩这款游戏

本游戏同时支持 **键盘** 和 绝大部分 **游戏手柄** 输入。
考虑到这是一款动作类游戏，作者更推荐大家使用手柄来游戏。（ Xbox360 或其他山寨版都支持 ）


### 手柄按键布置(Xbox360-Style):
- **左摇杆**: 控制角色移动。

- **按键:A**: 确认键。
在游戏世界中，玩家可以按压 **按键:A** 来提升 玩家所操作角色的 移动速度。
（其实这是提供给开发者 Debug 用的功能）


- **按键:B**: 降低 玩家所操作角色的 移动速度。

- **按键:X/Y**: 在 **终端** 中打印一些数据，供开发者 Debug 用。


### 键盘按键布置:
- **按键:W/A/S/D**: 控制角色移动。
- **按键::Enter**: 确认键。
- **按键::H/J/K/L**: 功能对应游戏手柄中的 **'A/B/X/Y'** 键.


在目前版本中，一旦玩家进入游戏世界，可以控制一只小鸡四处游览世界。通过按下 **键盘键:H** 或者 **游戏手柄键:H**,可以让小鸡速度加快甚至飞行。这原本是提供给开发者来检视 世界生成效果用的。

鉴于本游戏的剩余部分仍在开发中，所以还请体验者们耐心等待。


## 版权协议遵守 [BSD 3-Clause](LICENSE)

## 欢迎反馈与建议
本游戏的剩余部分仍在开发中，如果您有任何反馈或建议，都可以通过下方联系方式联系开发者本人。


## 联系方式
*   E-mail: [turesnake@gmail.com](mailto:turesnake@gmail.com)
*   E-mail: [turesnake@icloud.com](mailto:turesnake@icloud.com)
*   QQ: 651700603


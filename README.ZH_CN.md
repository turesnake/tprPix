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

(**注意**，此处的 "appRootDir" 应替换为实际的根目录名字)

接着，运行一份 shell 脚本程序：

    $ ./unix_release.sh

脚本 `unix_release.sh` 将会自动化地帮我们完成程序的 **build** 和本地化部署工作。
你也可以选择运行另一份脚本：`unix_debug.sh`， 正如名字所示，它将生成一份 **DEBUG** 版的程序。

现在，所有的安装工作都已经 **完成** 了!!!

你可以在目录： **.../appRootDir/build/publish/** 中找到可执行文件：**tprpix**。
双击它，开始我们的游戏!!!
(**注意**，此处的 "appRootDir" 应替换为实际的根目录名字)


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

(**注意**，此处的 "appRootDir" 应替换为实际的根目录名字)

接着，运行一份 shell 脚本程序：

    $ ./unix_release.sh

脚本 `unix_release.sh` 将会自动化地帮我们完成程序的 **build** 和本地化部署工作。
你也可以选择运行另一份脚本：`unix_debug.sh`， 正如名字所示，它将生存一份 **DEBUG** 版的程序。

现在，所有的安装工作都已经 **完成** 了!!!

你可以在目录： **.../appRootDir/build/publish/** 中找到可执行文件：**tprpix**。
(**注意**，此处的 "appRootDir" 应替换为实际的根目录名字)

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

如果 **CMake Settings** 面板中没有 **x64-Clang-Release** 选项，请检查Visual Studio中是否安装了如下组件

![Check Clang and Cmake](Docs/pngs/win10/clang_and_cmke_check.png)


找到 **Build root** 一览，将其值修改为: `${projectDir}\build` (**注意**，这一步改写非常重要，请务必操作正确！！！)

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
(**注意**，此处的 "appRootDir" 应替换为实际的根目录名字)

双击此文件，开始我们的游戏!!!


## 编译后的 最终生成包

不管在任何平台编译，最终的生成包是统一的：**.../appRootDir/build/publish/**, 你可以将这个 **publich** 文件夹，
复制到任何位置，都不会影响文件夹内 可执行文件 的运行。可执行文件需要的一切资源，都被集成在了 **publish** 目录内。



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



## 如何修改程序窗口尺寸
现阶段暂不支持 全屏模式。改写窗口尺寸的配置文件，放置在 **.../publish/jsons/windowConfig.json** 文件中。如图所示：

![Window Config](Docs/pngs/oth/windowConfig_1.png)

如果你想调整你的 **窗口尺寸**，请修改变量：`windowPixW, windowPixH`（单位为像素）。正如变量：`recommend:windowPixW, recommend:windowPixH` 所示，推荐的尺寸是 1920，1200。（事实上，你可以将变量 `windowPixW, windowPixH` 改写成任意整形数值（integer），只要记得别超出你的显示器限制。程序会自动照顾好窗口长宽比。）

请勿修改变量 `recommend:windowPixW, recommend:windowPixH`，它们对程序不起任何作用，仅仅是一份提示。

请保持变量 `isFullScreen` 值为 `false`。 针对它的修改将使程序启动失败。

在修改完参数后，保存文件。再次运行 app，就能获得对应效果。无需再次编译。



# 现存的问题
如果您在运行程序时，遭遇以下问题，请勿惊慌:


## 进入游戏，点击一个存档后，陷入死机
这个 bug 有较小的几率被触发。
当遇到这个问题时，推荐您先强制退出程序。然后直接删除 **.../appRootDir/build/publish/dataBase/** 文件夹。对，整个删除即可。这是程序的 存档文件夹。请放心粗暴地随意删除之。

## 三个存档占满了怎么办
目前的游戏尚未实现完整的 **UI系统**，就连 **存档系统** 也是残缺的。
如果存档已满，还是请直接删除 **.../appRootDir/build/publish/dataBase/** 文件夹。

## 运行中出现闪退怎么办
您可以将闪退后的**命令行信息**提交到**issues**处，或者直接通过下方的联系方式传达给我。这将帮助我更好地完成程序。

## 更多问题有待暴露
...


# 版权协议遵守 [BSD 3-Clause](LICENSE)

# 欢迎反馈与建议
本游戏的剩余部分仍在开发中，如果您有任何反馈或建议，都可以通过下方联系方式联系开发者本人。


## 联系方式
*   E-mail: [turesnake@gmail.com](mailto:turesnake@gmail.com)
*   E-mail: [turesnake@icloud.com](mailto:turesnake@icloud.com)
*   QQ: 651700603
*   微信：turesnake


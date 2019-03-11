## 前言

前段时间一直在看Windows黑客编程技术详解这本书，然后就想着与其一个例子一个例子的敲，倒不如把书上的例子写成一个项目，作为一个集合，这样既有利于面试 也方便以后代码的复用。最后花了半个月的时间就有了下面的东西。

## 界面

用MFC写的  界面有点low 不要在意这些细节。

![](https://ws1.sinaimg.cn/large/006Rs2Luly1g0bfcrb8mmj30dv0b00sy.jpg)

## 代码视图

代码的话放一个大概的类视图 详细的就不贴了实在是太多。

![](https://ws1.sinaimg.cn/large/006Rs2Luly1g0bfhk09kkj30a40ejq3c.jpg)
![](https://ws1.sinaimg.cn/large/006Rs2Luly1g0bfhk09jgj30bn0fi0t6.jpg)

## 功能介绍

下面一一介绍每一个功能及其实现原理。

### 基础技术

![](https://ws1.sinaimg.cn/large/006Rs2Luly1g0bflfoxbxj30br09sq34.jpg)

#### 防双开 

使用最基础的互斥体防双开 函数写在主程序。所以这个按钮只是个象征性的东西。一直都是开启的。

#### 释放资源

使用`FindResourceW``SizeofResource``LoadResource`和`LockResource`等API实现资源的释放。

程序会在同级目录下释放后续功能测试所需的DLL。

### 注入技术

![](https://ws1.sinaimg.cn/large/006Rs2Luly1g0bfs8kovzj30d70dcjrn.jpg)

#### 全局钩子注入

使用Windows提供的`SetWindowsHookEx`将DLL注入到系统。测试DLL为`MyHookDll.dll`。可通过资源释放技术释放所需dll。也可自行编译。HOOK的消息是`WH_GETMESSAGE`。dll中只写了一个象征性的`MessageBox`。即拖拽dll点击开始注入后，当系统中有任意进程接收到消息会弹出一个MessageBox。如果电脑中启动的应用较多可能导致蓝屏卡死等情况。

#### 远程线程注入

1. 拖拽TestDll.dll(使用资源释放技术释放)或者自行编译
2. 输入需要注入的进程ID
3. 点击开始注入后会将测试DLL注入到目标进程 测试DLL只有一个MessageBox
4. 如果注入的进程权限较高或者检测较严格(如QQ.exe) 则可能失败

**实现原理**

使用`ZwCreateThreadEx`函数创建远程线程 实现DLL注入。之所以没有`CreateRemoteThread`是因为`CreateRemoteThread`函数内部是被`CreateRemoteThread`调用的。相对来说这个函数的权限会更高。

#### APC注入

1. 拖拽TestDll.dll(使用资源释放技术释放)或自行编译
2. 输入需要注入的进程名
3. 点击开始注入后会将测试DLL注入到目标进程 测试DLL只有一个MessageBox
4. 如果注入的进程权限较高或者检测较严格(如QQ.exe) 则可能失败

**实现原理**

通过`QueueUserAPC`把一个函数添加到指定线程的`APC`队列中

### 启动技术

![](https://ws1.sinaimg.cn/large/006Rs2Luly1g0bg3e9hf0j30d90czwej.jpg)

#### 三种方式创建进程

1. 任意拖拽一个exe到窗口，点击按钮后会使用三种不同的方式创建进程
2. 分别使用`CreateProcess``ShellExecute`和`WinExec`三种方式创建进程

#### 内存加载运行dll

1. 拖拽TestDll.dll(使用资源释放技术释放)
2. 点击按钮后程序会将TestDll模拟PE加载器加载到本程序内存后直接运行
3. 如果将DLL隐藏到资源可以不将DLL释放到本地直接运行
4. 病毒WannaCry和BadRabbit都用了这种方式来运行自己的主逻辑程序

### 自启动技术

![](https://ws1.sinaimg.cn/large/006Rs2Luly1g0bhngce3hj30dh0dtglv.jpg)

#### 注册表

1. 拖拽任意exe到窗口
2. 点击按钮后会将程序的完整路径写到注册表启动项
3. 目标程序随系统启动而启动
4. 可自行查看提示窗口的注册表路径进行查看或删除
5. 原理：通过`RegOpenKeyEx`和`RegSetValueEx`将程序写入到`\HKEY_CURRENT_USER\SOFTWARE\Microsoft\Windows\CurrentVersion\Run`

#### 快速启动目录

1. 拖拽任意exe到窗口
2. 点击按钮后会将程序的复制到系统的快速启动目录
3. 目标程序随用户登录而自行启动
4. 可自行查看提示窗口的快速启动目录路径进行查看或删除
5. 实现原理：使用`SHGetSpecialFolderPath`函数获取windows系统中快速启动目录的路径 然后，使用CopyFile函数，将想要自启动的程序复制到快速启动目录下

#### 计划任务

1. 此功能需使用管理员运行
2. 拖拽任意exe到窗口
3. 点击按钮后会将目标程序写入到系统的计划任务
4. 程序随用户登录而启动
5. 可自行在计划任务中删除添加的事件
6. 实现原理：调用COM组件将程序添加到计划任务实现开机自启动

#### 系统服务

1. 此功能需使用管理员运行
2. 拖拽ServiceExeTest.exe(服务应用程序 需自行编译)到窗口
3. 点击按钮后会将ServiceExeTest.exe注册为系统服务
4. ServiceExeTest.exe将会在后台运行
5. ServiceExeTest.exe不含任何代码 只是一个空的服务应用程序
6. 可自行使用任务管理器删除
7. 实现原理：使用`CreateService`将编写的服务应用程序注册为系统服务实现开机自启动

### 提权技术

![](https://ws1.sinaimg.cn/large/006Rs2Luly1g0bgf2yejzj30it0gpq34.jpg)

#### 提升为Debug权限

1. 此功能需使用管理员运行
2. 点击按钮后程序将当前进程提升为为Debug权限

### BypassUAC

1. 首先将用户账户控制设置为默认
2. 点击按钮后会绕过UAC的弹窗提升为管理员权限
3. 不弹窗开启一个管理员权限的命令行
4. 实现原理：使用COM组件提权

#### 查看当前进程权限

1. 点击按钮后将在List控件上显示当前程序的所有权限
2. 可使用此功能查看提升为Debug权限是否生效

### 隐藏技术

![](https://ws1.sinaimg.cn/large/006Rs2Luly1g0bgj8lpyyj30fo09s0sm.jpg)

#### 进程伪装

1. 点击按钮后会将本进程伪装为Explorer.exe
2. 本程序为32位 在32为系统下此功能才有效
3. 实现原理：使用`NtQueryInformationProcess`获取进程环境块。修改指定进程环境块的进程路径以及命令行信息，从而达到进程伪装的效果。

#### 傀儡进程

1. 拖拽任意一个exe到窗口
2. 点击按钮后会将一段MessageBox的ShellCode注入到目标exe
3. 实现原理：修改某一进程的内存数据，向内存数据写入Shellcode代码，并修改该进程的执行流程，使其执行Shellcode代码。这样，进程还是原来的进程，但执行的操作却替换了。

#### DLL劫持

1. 此功能没有写在界面上 需要自行手动操作
2. 我这里劫持的是`FileCleaner2.0.exe`的`oledlg.dll`
3. 找到DllHijackTest.dll(使用资源释放技术释放)或自行编译
4. 将DLL重命名为oledlg.dll
5. 找到C:\\Windows\\SysWOW64目录下的oledlg.dll将其重命名为Old_oledlg(如果无法重命名请自行百度解决方案)
6. 将oledlg.dll放到SysWOW64目录下
7. 运行FileCleaner2.0.exe
8. 劫持的DLL在DllMian中写了一个MessageBox函数 表示劫持成功
9. 实现原理：在PE加载器加载DLL时 使用自制的DLL替换掉原DLL ，在自制的DLL中转发原DLL的导出函数并添加代码

### 压缩技术

![](https://ws1.sinaimg.cn/large/006Rs2Luly1g0bgpnxo0uj30eg0cpwen.jpg)

#### 数据解压缩

1. 在文本框中输入数据 点击按钮，即可对输入的数据压缩和解压缩
2. 实现原理：通过Windows提供的`RtlDecompressBuffer`和`RtlGetCompressionWorkSpaceSize`实现对数据的解压缩

#### 文件解压缩

1. 拖拽文件到窗口，点击按钮 即可对文件进行压缩和解压缩
2. 使用`ZLIB`压缩库实现对文件的解压缩

### 加密技术

![](https://ws1.sinaimg.cn/large/006Rs2Luly1g0bgsenj4qj30et0eyjrg.jpg)

#### HASH

拖拽文件到窗口，点击按钮 即可求得文件的哈希值

#### AES加/解密

在数据窗口输入数据 点击按钮 即可求得数据经过AES加解密后的值

#### RSA加/解密

在数据窗口输入数据 点击按钮 即可求得数据经过RAS加解密后的值

#### 实现原理

以上加解密都是使用Windows自带的加密库来实现的，使用的是`cryptoAPI`。

### 功能技术

![](https://ws1.sinaimg.cn/large/006Rs2Luly1g0bgvciqc6j30g00dw74w.jpg)

#### 进程遍历

对当前所有进程进行遍历

实现原理：使用`Process32First`和`Process32Next`

#### 文件遍历

1. 对当前系统所有文件进行遍历 可查看文件属性和磁盘信息
2. 可查看文件属性和磁盘信息 双击可进入下一层目录
3. 实现原理：使用`FindFirstFile`和`FindNextFile`

#### 桌面截屏

1. 对当前屏幕进行截屏 截图存放在程序同级目录
2. 实现原理：通过GDI的方式实现屏幕画面抓取

#### 按键记录

1. 实现了一个键盘记录器 记录下所有的按键记录
2. 按键记录存储在D盘下的log.txt
3. 实现原理：利用GetAsyncKeyState函数。该函数可以判断按键状态，根据是否为按下状态来判断用户是否进行了按键操作，从而记录
4. 之所以没有用全局键盘钩子和原始输入模型是因为这两种方式都有各自的限制，MFC实现起来不太容易

#### 显示CMD结果

1. 在命令框输入CMD命令 例如ping 127.0.0.0
2. 等命令执行完成会在数据窗口会显示CMD命令的执行结果
3. 由于环境不一致 这个功能可能会执行失败
4. 实现原理：使用命名管道进行进程通讯，从而获得cmd的执行结果

#### U盘小偷

1. 在功能窗口此功能默认开启
2. 对U盘的插入和拔出进行监控 
3. 拷贝U盘里所有的.txt文件到程序的当前目录下,如需拷贝其他文件则修改源码
4. 实现原理:响应`WM_DEVICECHANGE`监控U盘，获取盘符之后进行文件遍历 即可拷贝所需要的文件

#### 文件监控

1. 对D盘上的文件操作进行监控
2. 实现原理：Windows提供了一个文件监控的接口函数ReadDirectoryChangesW，这个监控函数可以对计算机上的所有文件操作进行监控。

#### 自删除

1. 点击后程序将退出进程 并删除自身
2. 实现原理：使用批处理

## 说明

1. 本程序仅用于交流和研究，请勿用于其他用途。有任何后果与本人无关
2. 由于环境不一样 所以不能保证每个功能在每台机器上都是可行的。如果程序有bug还请多多体谅
3. 如果发现缺dll或者没有测试文件的情况在工程项目下都可找到，可自行编译。
4. 本程序编写环境为W10 64 VS2017 最后附上源码
5. 最后附上自己的CSDN博客:https://blog.csdn.net/qq_38474570 求关注点赞 收藏




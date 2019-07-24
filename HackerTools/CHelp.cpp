// CHelp.cpp: 实现文件
//

#include "stdafx.h"
#include "HackerTools.h"
#include "CHelp.h"
#include "afxdialogex.h"


// CHelp 对话框

IMPLEMENT_DYNAMIC(CHelp, CDialogEx)

CHelp::CHelp(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_Help, pParent)
{

}

CHelp::~CHelp()
{
}

void CHelp::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE1, m_Tree);
}


BEGIN_MESSAGE_MAP(CHelp, CDialogEx)
END_MESSAGE_MAP()


// CHelp 消息处理程序


BOOL CHelp::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	HTREEITEM Basic=m_Tree.InsertItem(L"基础技术");
	HTREEITEM ReleaseResoures= m_Tree.InsertItem(L"资源释放",Basic);
	HTREEITEM ReleaseResouresHelp= m_Tree.InsertItem(L"在同级目录下释放后续功能测试所需的DLL", ReleaseResoures);

	HTREEITEM RunSigle= m_Tree.InsertItem(L"防双开",Basic);
	HTREEITEM RunSigleHelp= m_Tree.InsertItem(L"使用互斥体防双开 默认开启 本程序只能开启一个", RunSigle);

	HTREEITEM Inject = m_Tree.InsertItem(L"注入技术");
	HTREEITEM GlobalHook = m_Tree.InsertItem(L"全局钩子注入", Inject);
	HTREEITEM GlobalHookHelp1 = m_Tree.InsertItem(L"需拖拽MyHookDll.dll(使用资源释放技术释放)到窗口 测试DLL只有一个MessageBox", GlobalHook);
	HTREEITEM GlobalHookHelp2 = m_Tree.InsertItem(L"HOOK的是WM_GETMESSAGE消息 只要系统中有程序接收消息就会弹框", GlobalHook);
	HTREEITEM GlobalHookHelp3 = m_Tree.InsertItem(L"如果运行程序开启较多 可能直接卡死 慎用 慎用", GlobalHook);

	HTREEITEM RemoteThread = m_Tree.InsertItem(L"远程线程注入", Inject);
	HTREEITEM RemoteThreadHelp1 = m_Tree.InsertItem(L"1.拖拽TestDll.dll(使用资源释放技术释放)", RemoteThread);
	HTREEITEM RemoteThreadHelp2 = m_Tree.InsertItem(L"2.输入需要注入的进程ID", RemoteThread);
	HTREEITEM RemoteThreadHelp3 = m_Tree.InsertItem(L"3.点击开始注入后会将测试DLL注入到目标进程 测试DLL只有一个MessageBox", RemoteThread);
	HTREEITEM RemoteThreadHelp4 = m_Tree.InsertItem(L"4.如果注入的进程权限较高或者检测较严格(如QQ.exe) 则可能失败", RemoteThread);

	HTREEITEM APCInject = m_Tree.InsertItem(L"APC注入", Inject);
	HTREEITEM APCInjectHelp1 = m_Tree.InsertItem(L"1.拖拽TestDll.dll(使用资源释放技术释放)", APCInject);
	HTREEITEM APCInjectHelp2 = m_Tree.InsertItem(L"2.输入需要注入的进程名", APCInject);
	HTREEITEM APCInjectHelp3 = m_Tree.InsertItem(L"3.点击开始注入后会将测试DLL注入到目标进程 测试DLL只有一个MessageBox", APCInject);
	HTREEITEM APCInjectHelp4 = m_Tree.InsertItem(L"4.如果注入的进程权限较高或者检测较严格(如QQ.exe) 则可能失败", APCInject);

	HTREEITEM Start=m_Tree.InsertItem(L"启动技术");
	HTREEITEM Create_Process = m_Tree.InsertItem(L"创建进程API", Start);
	HTREEITEM Create_ProcessHelp1 = m_Tree.InsertItem(L"任意拖拽一个exe到窗口", Create_Process);
	HTREEITEM Create_ProcessHelp2 = m_Tree.InsertItem(L"点击按钮后会使用三种不同的方式创建进程", Create_Process);

	HTREEITEM MemoryLoad = m_Tree.InsertItem(L"内存加载运行", Start);
	HTREEITEM MemoryLoadHelp1 = m_Tree.InsertItem(L"拖拽TestDll.dll(使用资源释放技术释放)", MemoryLoad);
	HTREEITEM MemoryLoadHelp2 = m_Tree.InsertItem(L"点击按钮后程序会将TestDll模拟PE加载器加载到本程序内存后直接运行", MemoryLoad);
	HTREEITEM MemoryLoadHelp3 = m_Tree.InsertItem(L"如果将DLL隐藏到资源可以不将DLL释放到本地直接运行", MemoryLoad);

	HTREEITEM SelfStart=m_Tree.InsertItem(L"自启动技术");
	HTREEITEM RegEdit = m_Tree.InsertItem(L"注册表", SelfStart);
	HTREEITEM RegEditHelp1 = m_Tree.InsertItem(L"拖拽任意exe到窗口", RegEdit);
	HTREEITEM RegEditHelp2 = m_Tree.InsertItem(L"点击按钮后会将程序的完整路径写到注册表启动项", RegEdit);
	HTREEITEM RegEditHelp3 = m_Tree.InsertItem(L"目标程序随系统启动而启动", RegEdit);
	HTREEITEM RegEditHelp4 = m_Tree.InsertItem(L" 可自行查看提示窗口的注册表路径进行查看或删除", RegEdit);

	HTREEITEM StartDir = m_Tree.InsertItem(L"快速启动目录", SelfStart);
	HTREEITEM StartDirHelp1 = m_Tree.InsertItem(L"拖拽任意exe到窗口", StartDir);
	HTREEITEM StartDirHelp2 = m_Tree.InsertItem(L"点击按钮后会将程序的复制到系统的快速启动目录", StartDir);
	HTREEITEM StartDirHelp3 = m_Tree.InsertItem(L"目标程序随用户登录而自行启动", StartDir);
	HTREEITEM StartDirHelp4 = m_Tree.InsertItem(L" 可自行查看提示窗口的快速启动目录路径进行查看或删除", StartDir);

	HTREEITEM Task = m_Tree.InsertItem(L"计划任务", SelfStart);
	HTREEITEM TaskHelp = m_Tree.InsertItem(L"此功能需使用管理员运行", Task);
	HTREEITEM TaskHelp1 = m_Tree.InsertItem(L"拖拽任意exe到窗口", Task);
	HTREEITEM TaskHelp2 = m_Tree.InsertItem(L"点击按钮后会将目标程序写入到系统的计划任务", Task);
	HTREEITEM TaskHelp3 = m_Tree.InsertItem(L"程序随用户登录而启动", Task);
	HTREEITEM TaskHelp4 = m_Tree.InsertItem(L"可自行在计划任务中删除添加的事件", Task);

	HTREEITEM SystemService = m_Tree.InsertItem(L"系统服务", SelfStart);
	HTREEITEM SystemServiceHelp = m_Tree.InsertItem(L"此功能需使用管理员运行", SystemService);
	HTREEITEM SystemServiceHelp1 = m_Tree.InsertItem(L"拖拽ServiceExeTest.exe到窗口", SystemService);
	HTREEITEM SystemServiceHelp2 = m_Tree.InsertItem(L"点击按钮后会将ServiceExeTest.exe注册为系统服务", SystemService);
	HTREEITEM SystemServiceHelp3 = m_Tree.InsertItem(L"ServiceExeTest.exe将会在后台运行", SystemService);
	HTREEITEM SystemServiceHelp4 = m_Tree.InsertItem(L"ServiceExeTest.exe不含任何代码 只是一个空的服务应用程序", SystemService);
	HTREEITEM SystemServiceHelp5 = m_Tree.InsertItem(L"可自行使用任务管理器删除", SystemService);


	HTREEITEM Privilege=m_Tree.InsertItem(L"提权技术");
	HTREEITEM Debug = m_Tree.InsertItem(L"提升为Debug权限", Privilege);
	HTREEITEM DebugHelp1 = m_Tree.InsertItem(L"此功能需使用管理员运行", Debug);
	HTREEITEM DebugHelp2 = m_Tree.InsertItem(L"点击按钮后程序将获取Debug权限", Debug);

	HTREEITEM BypassUAC = m_Tree.InsertItem(L"BypassUAC", Privilege);
	HTREEITEM BypassUACHelp = m_Tree.InsertItem(L"点击按钮后会使用COM组件提升为管理员权限", BypassUAC);
	HTREEITEM BypassUACHelp1 = m_Tree.InsertItem(L"不弹窗开启一个管理员权限的命令行", BypassUAC);

	HTREEITEM FindAllPrivilege = m_Tree.InsertItem(L"查看当前进程权限", Privilege);
	HTREEITEM FindAllPrivilegeHelp = m_Tree.InsertItem(L"点击按钮后将在List控件上显示当前程序的所有权限", FindAllPrivilege);
	HTREEITEM FindAllPrivilegeHelp1 = m_Tree.InsertItem(L"可使用此功能查看提升为Debug权限是否生效", FindAllPrivilege);

	HTREEITEM Hide=m_Tree.InsertItem(L"隐藏技术");
	HTREEITEM Pretend = m_Tree.InsertItem(L"进程伪装", Hide);
	HTREEITEM PretendHelp1 = m_Tree.InsertItem(L"点击按钮后会将本进程伪装为Explorer.exe", Pretend);
	HTREEITEM PretendHelp2 = m_Tree.InsertItem(L"本程序为32位 在32为系统下此功能才有效", Pretend);

	HTREEITEM Puppet = m_Tree.InsertItem(L"傀儡进程", Hide);
	HTREEITEM PuppetHelp = m_Tree.InsertItem(L"拖拽任意一个exe到窗口", Puppet);
	HTREEITEM PuppetHelp1 = m_Tree.InsertItem(L"点击按钮后会将一段MessageBox的ShellCode注入到目标exe", Puppet);

	HTREEITEM DllHijack = m_Tree.InsertItem(L"DLL劫持", Hide);
	HTREEITEM DllHijackHelp = m_Tree.InsertItem(L"原理：在PE加载器加载DLL时 使用自制的DLL替换掉原DLL 并在自制的DLL中添加代码", DllHijack);
	HTREEITEM DllHijackHelp1 = m_Tree.InsertItem(L"此功能没有写在界面上 可自行手动操作", DllHijack);
	HTREEITEM DllHijackHelp2 = m_Tree.InsertItem(L"1.找到DllHijackTest.dll(使用资源释放技术释放)", DllHijack);
	HTREEITEM DllHijackHelp3 = m_Tree.InsertItem(L"2.将DLL重命名为oledlg.dll", DllHijack);
	HTREEITEM DllHijackHelp4 = m_Tree.InsertItem(L"3.找到C:\\Windows\\SysWOW64目录下的oledlg.dll将其重命名为Old_oledlg)", DllHijack);
	HTREEITEM DllHijackHelp5 = m_Tree.InsertItem(L"4.如果无法重命名请自行百度解决方案", DllHijack);
	HTREEITEM DllHijackHelp6 = m_Tree.InsertItem(L"5.将oledlg.dll放到SysWOW64目录下", DllHijack);
	HTREEITEM DllHijackHelp7 = m_Tree.InsertItem(L"6.运行FileCleaner2.0.exe", DllHijack);
	HTREEITEM DllHijackHelp8 = m_Tree.InsertItem(L"7.劫持的DLL转发了原DLL的所有函数 不会对程序运行造成影响", DllHijack);
	HTREEITEM DllHijackHelp9 = m_Tree.InsertItem(L"8.劫持的DLL在DllMian中写了一个MessageBox函数 表示劫持成功", DllHijack);


	HTREEITEM Compress=m_Tree.InsertItem(L"压缩技术");
	HTREEITEM DataCompress = m_Tree.InsertItem(L"数据压缩", Compress);
	HTREEITEM DataCompressHelp = m_Tree.InsertItem(L"在文本框中输入数据 点击按钮，即可对输入的数据压缩和解压缩", DataCompress);
	HTREEITEM FileCompress = m_Tree.InsertItem(L"文件压缩", Compress);
	HTREEITEM FileCompressHelp = m_Tree.InsertItem(L"拖拽文件到窗口，点击按钮 即可对文件进行压缩和解压缩", FileCompress);


	HTREEITEM Encrypt=m_Tree.InsertItem(L"加密技术");
	HTREEITEM HASH=m_Tree.InsertItem(L"HASH", Encrypt);
	HTREEITEM HASHHelp=m_Tree.InsertItem(L"拖拽文件到窗口，点击按钮 即可求得文件的哈希值", HASH);

	HTREEITEM AES=m_Tree.InsertItem(L"AES加/解密", Encrypt);
	HTREEITEM AESHelp=m_Tree.InsertItem(L"在数据窗口输入数据 点击按钮 即可求得数据经过AES加解密后的值", AES);

	HTREEITEM RAS=m_Tree.InsertItem(L"RAS加/解密", Encrypt);
	HTREEITEM RASHelp=m_Tree.InsertItem(L"在数据窗口输入数据 点击按钮 即可求得数据经过RAS加解密后的值", RAS);


	HTREEITEM Function=m_Tree.InsertItem(L"功能技术");
	HTREEITEM FindAllProcess=m_Tree.InsertItem(L"进程遍历", Function);
	HTREEITEM FindAllProcessHelp=m_Tree.InsertItem(L"对当前所有进程进行遍历", FindAllProcess);

	HTREEITEM FindAllFile=m_Tree.InsertItem(L"文件遍历", Function);
	HTREEITEM FindAllFileHelp=m_Tree.InsertItem(L"对当前系统所有文件进行遍历 可查看文件属性和磁盘信息", FindAllFile);
	HTREEITEM FindAllFileHelp1=m_Tree.InsertItem(L"可查看文件属性和磁盘信息 双击可进入下一层目录", FindAllFile);

	HTREEITEM ScreenShot=m_Tree.InsertItem(L"桌面截屏", Function);
	HTREEITEM ScreenShotHelp=m_Tree.InsertItem(L"对当前屏幕进行截屏 截图存放在程序同级目录", ScreenShot);

	HTREEITEM KeyBoardRecord=m_Tree.InsertItem(L"按键记录", Function);
	HTREEITEM KeyBoardRecordHelp=m_Tree.InsertItem(L"实现了一个键盘记录器 记录下所有的按键记录", KeyBoardRecord);
	HTREEITEM KeyBoardRecordHelp1=m_Tree.InsertItem(L"按键记录存储在D盘下的log.txt", KeyBoardRecord);

	HTREEITEM RemoteCMD =m_Tree.InsertItem(L"远程CMD", Function);
	HTREEITEM RemoteCMDHelp =m_Tree.InsertItem(L"在命令框输入CMD命令 例如ping 127.0.0.0", RemoteCMD);
	HTREEITEM RemoteCMDHelp1 =m_Tree.InsertItem(L"在数据窗口会显示CMD命令的执行结果", RemoteCMD);

	HTREEITEM USBMonitoring =m_Tree.InsertItem(L"U盘小偷", Function);
	HTREEITEM USBMonitoringHelp =m_Tree.InsertItem(L"1.对U盘的插入和拔出进行监控 默认开启", USBMonitoring);
	HTREEITEM USBMonitoringHelp2 =m_Tree.InsertItem(L"2.拷贝U盘里所有的.txt文件到程序的当前目录下,如需拷贝其他文件则修改源码", USBMonitoring);

	HTREEITEM FileMonitoring =m_Tree.InsertItem(L"文件监控", Function);
	HTREEITEM FileMonitoringHelp =m_Tree.InsertItem(L"对D盘上的文件操作进行监控", FileMonitoring);
	
	return TRUE;  
}

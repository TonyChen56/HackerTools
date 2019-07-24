#include "stdafx.h"
#include "CMyTaskSchedule.h"



//************************************************************
// 函数名称: CMyTaskSchedule 
// 函数说明: 初始化计划任务
// 作	 者: GuiShou
// 时	 间: 2019/1/22
// 参	 数: void
// 返 回 值: void
//************************************************************
CMyTaskSchedule::CMyTaskSchedule()
{
	m_lpITS = NULL;
	m_lpRootFolder = NULL;
	//初始化COM接口环境
	HRESULT hr = CoInitialize(NULL);
	if (FAILED(hr))
	{
		MessageBoxA(NULL,"CoInitialize","Error",MB_OK);
	}

	//创建任务服务对象
	hr = CoCreateInstance(CLSID_TaskScheduler, NULL, CLSCTX_INPROC_SERVER,
		IID_ITaskService, (LPVOID*)(&m_lpITS));
	if (FAILED(hr))
	{
		MessageBoxA(NULL, "CoCreateInstance", "Error", MB_OK);
	}

	//连接到任务服务上
	hr = m_lpITS->Connect(_variant_t(), _variant_t(), _variant_t(), _variant_t());
	if (FAILED(hr))
	{
		MessageBoxA(NULL, "Connect", "Error", MB_OK);
	}

	//获取根任务的指针对象
	hr = m_lpITS->GetFolder(_bstr_t("\\"), &m_lpRootFolder);
	if (FAILED(hr))
	{
		MessageBoxA(NULL, "GetFolder", "Error", MB_OK);
	}
}


CMyTaskSchedule::~CMyTaskSchedule()
{
	if (m_lpITS)
	{
		m_lpITS->Release();
	}
	if (m_lpRootFolder)
	{
		m_lpRootFolder->Release();
	}
	// 卸载COM
	::CoUninitialize();
}


//************************************************************
// 函数名称: NewTask 
// 函数说明: 创建计划任务
// 作	 者: GuiShou
// 时	 间: 2019/1/22
// 参	 数: char* lpszTaskName    计划任务名
// 参	 数: char* lpszProgramPath 计划任务路径
// 参	 数: char* lpszParameters  计划任务参数
// 参	 数: char* szAuthor        计划任务作者
// 返 回 值: BOOL是否成功
//************************************************************
BOOL CMyTaskSchedule::NewTask(char* lpszTaskName, char* lpszProgramPath, char* lpszParameters, char* lpszAuthor)
{
	if(NULL == m_lpRootFolder)
	{
		return FALSE;
	}
	// 如果存在相同的计划任务，则删除
	Delete(lpszTaskName);
	// 创建任务定义对象来创建任务
	ITaskDefinition *pTaskDefinition = NULL;
	HRESULT hr = m_lpITS->NewTask(0, &pTaskDefinition);
	if(FAILED(hr))
	{
		MessageBoxA(0, "ITaskService::NewTask", "ERROR", 0);
		return FALSE;
	}

	/* 设置注册信息 */
	IRegistrationInfo *pRegInfo = NULL;
	CComVariant variantAuthor(NULL);
	variantAuthor = lpszAuthor;
	hr = pTaskDefinition->get_RegistrationInfo(&pRegInfo);
	if(FAILED(hr))
	{
		MessageBoxA(0, "pTaskDefinition::get_RegistrationInfo", "ERROR", 0);
		return FALSE;
	}
	// 设置作者信息
	hr = pRegInfo->put_Author(variantAuthor.bstrVal);
	pRegInfo->Release();

	/* 设置登录类型和运行权限 */
	IPrincipal *pPrincipal = NULL;
	hr = pTaskDefinition->get_Principal(&pPrincipal);
	if(FAILED(hr))
	{
		MessageBoxA(0, "pTaskDefinition::get_Principal", "ERROR", 0);
		return FALSE;
	}
	// 设置登录类型
	hr = pPrincipal->put_LogonType(TASK_LOGON_INTERACTIVE_TOKEN);
	// 设置运行权限
	// 最高权限
	hr = pPrincipal->put_RunLevel(TASK_RUNLEVEL_HIGHEST);  
	pPrincipal->Release();

	/* 设置其他信息 */
	ITaskSettings *pSettting = NULL;
	hr = pTaskDefinition->get_Settings(&pSettting);
	if(FAILED(hr))
	{
		MessageBoxA(0, "pTaskDefinition::get_Settings", "ERROR", 0);
		return FALSE;
	}
	// 设置其他信息
	hr = pSettting->put_StopIfGoingOnBatteries(VARIANT_FALSE);
	hr = pSettting->put_DisallowStartIfOnBatteries(VARIANT_FALSE);
	hr = pSettting->put_AllowDemandStart(VARIANT_TRUE);
	hr = pSettting->put_StartWhenAvailable(VARIANT_FALSE);
	hr = pSettting->put_MultipleInstances(TASK_INSTANCES_PARALLEL);
	pSettting->Release();

	/* 创建执行动作 */
	IActionCollection *pActionCollect = NULL;
	hr = pTaskDefinition->get_Actions(&pActionCollect);
	if(FAILED(hr))
	{
		MessageBoxA(0, "pTaskDefinition::get_Actions", "ERROR", 0);
		return FALSE;
	}
	IAction *pAction = NULL;
	// 创建执行操作
	hr = pActionCollect->Create(TASK_ACTION_EXEC, &pAction);
	pActionCollect->Release();

	/* 设置执行程序路径和参数 */
	CComVariant variantProgramPath(NULL);
	CComVariant variantParameters(NULL);
	IExecAction *pExecAction = NULL;
	hr = pAction->QueryInterface(IID_IExecAction, (PVOID *)(&pExecAction));
	if(FAILED(hr))
	{
		pAction->Release();
		MessageBoxA(0, "IAction::QueryInterface", "ERROR", 0);

		return FALSE;
	}
	pAction->Release();
	// 设置程序路径和参数
	variantProgramPath = lpszProgramPath;
	variantParameters = lpszParameters;
	pExecAction->put_Path(variantProgramPath.bstrVal);
	pExecAction->put_Arguments(variantParameters.bstrVal);
	pExecAction->Release();

	/* 创建触发器，实现用户登陆自启动 */
	ITriggerCollection *pTriggers = NULL;
	hr = pTaskDefinition->get_Triggers(&pTriggers);
	if (FAILED(hr))
	{
		MessageBoxA(0, "pTaskDefinition::get_Triggers", "ERROR", 0);
		return FALSE;
	}
	// 创建触发器
	ITrigger *pTrigger = NULL;
	//用户登录时触发
	hr = pTriggers->Create(TASK_TRIGGER_LOGON, &pTrigger);
	if (FAILED(hr))
	{
		MessageBoxA(0, "ITaskFolder::Create", "ERROR", 0);
		return FALSE;
	}

	/* 注册任务计划  */
	IRegisteredTask *pRegisteredTask = NULL;
	CComVariant variantTaskName(NULL);
	variantTaskName = lpszTaskName;
	hr = m_lpRootFolder->RegisterTaskDefinition(variantTaskName.bstrVal,
		pTaskDefinition,
		TASK_CREATE_OR_UPDATE,
		_variant_t(),
		_variant_t(),
		TASK_LOGON_INTERACTIVE_TOKEN,
		_variant_t(""),
		&pRegisteredTask);
	if(FAILED(hr))
	{
		pTaskDefinition->Release();
		MessageBoxA(0, "ITaskFolder::RegisterTaskDefinition", "ERROR", 0);
		return FALSE;
	}
	pTaskDefinition->Release();
	pRegisteredTask->Release();

	return TRUE;

}


//************************************************************
// 函数名称: Delete 
// 函数说明: 删除计划任务
// 作	 者: GuiShou
// 时	 间: 2019/1/22
// 参	 数: char * lpszTaskName 计划任务名
// 返 回 值: BOOL是否成功
//************************************************************
BOOL CMyTaskSchedule::Delete(char * lpszTaskName)
{
	if (NULL==m_lpRootFolder)
	{
		return FALSE;
	}

	CComVariant variantTaskName(NULL);
	variantTaskName = lpszTaskName;
	HRESULT hr = m_lpRootFolder->DeleteFolder(variantTaskName.bstrVal, 0);
	if (FAILED(hr))
	{
		return FALSE;
	}
	return TRUE;
}



//************************************************************
// 函数名称: IsTaskValid 
// 函数说明: 判断指定计划任务是否还在
// 作	 者: GuiShou
// 时	 间: 2019/1/22
// 参	 数: char * lpszTaskName 计划任务名
// 返 回 值: BOOL是否成功
//************************************************************
BOOL CMyTaskSchedule::IsTaskValid(char * lpszTaskName)
{
	if (NULL == m_lpRootFolder)
	{
		return FALSE;
	}
	HRESULT hr = S_OK;
	CComVariant variantTaskName(NULL);
	CComVariant variantEnable(NULL);
	variantTaskName = lpszTaskName;                     // 任务计划名称
	IRegisteredTask *pRegisteredTask = NULL;
	// 获取任务计划
	hr = m_lpRootFolder->GetTask(variantTaskName.bstrVal, &pRegisteredTask);
	if (FAILED(hr) || (NULL == pRegisteredTask))
	{
		return FALSE;
	}
	pRegisteredTask->Release();

	return TRUE;
}


//************************************************************
// 函数名称: Run 
// 函数说明: 运行指定任务计划
// 作	 者: GuiShou
// 时	 间: 2019/1/22
// 参	 数: char * lpszTaskName 计划任务名  char * lpszParam参数
// 返 回 值: BOOL是否成功
//************************************************************
BOOL CMyTaskSchedule::Run(char * lpszTaskName, char * lpszParam)
{
	if (NULL == m_lpRootFolder)
	{
		return FALSE;
	}
	HRESULT hr = S_OK;
	CComVariant variantTaskName(NULL);
	CComVariant variantParameters(NULL);
	variantTaskName = lpszTaskName;
	variantParameters = lpszParam;

	// 获取任务计划
	IRegisteredTask *pRegisteredTask = NULL;
	hr = m_lpRootFolder->GetTask(variantTaskName.bstrVal, &pRegisteredTask);
	if (FAILED(hr) || (NULL == pRegisteredTask))
	{
		return FALSE;
	}
	// 运行
	hr = pRegisteredTask->Run(variantParameters, NULL);
	if (FAILED(hr))
	{
		pRegisteredTask->Release();
		return FALSE;
	}
	pRegisteredTask->Release();

	return TRUE;
}


//************************************************************
// 函数名称: IsEnable 
// 函数说明: 判断指定任务计划是否启动
// 作	 者: GuiShou
// 时	 间: 2019/1/22
// 参	 数: char * lpszTaskName 计划任务名
// 返 回 值: BOOL是否成功
//************************************************************
BOOL CMyTaskSchedule::IsEnable(char * lpszTaskName)
{
	if (NULL == m_lpRootFolder)
	{
		return FALSE;
	}
	HRESULT hr = S_OK;
	CComVariant variantTaskName(NULL);
	CComVariant variantEnable(NULL);
	variantTaskName = lpszTaskName;                     // 任务计划名称
	IRegisteredTask *pRegisteredTask = NULL;
	// 获取任务计划
	hr = m_lpRootFolder->GetTask(variantTaskName.bstrVal, &pRegisteredTask);
	if (FAILED(hr) || (NULL == pRegisteredTask))
	{
		return FALSE;
	}
	// 获取是否已经启动
	pRegisteredTask->get_Enabled(&variantEnable.boolVal);
	pRegisteredTask->Release();
	if (ATL_VARIANT_FALSE == variantEnable.boolVal)
	{
		return FALSE;
	}

	return TRUE;
}


//************************************************************
// 函数名称: SetEnable 
// 函数说明: 设置指定任务计划是否启动还是禁用
// 作	 者: GuiShou
// 时	 间: 2019/1/22
// 参	 数: char * lpszTaskName 计划任务名 bEnable是否启动
// 返 回 值: BOOL是否成功
//************************************************************
BOOL CMyTaskSchedule::SetEnable(char * lpszTaskName, BOOL bEnable)
{
	if (NULL == m_lpRootFolder)
	{
		return FALSE;
	}
	HRESULT hr = S_OK;
	CComVariant variantTaskName(NULL);
	CComVariant variantEnable(NULL);
	variantTaskName = lpszTaskName;                     // 任务计划名称
	variantEnable = bEnable;                            // 是否启动
	IRegisteredTask *pRegisteredTask = NULL;
	// 获取任务计划
	hr = m_lpRootFolder->GetTask(variantTaskName.bstrVal, &pRegisteredTask);
	if (FAILED(hr) || (NULL == pRegisteredTask))
	{
		return FALSE;
	}
	// 设置是否启动
	pRegisteredTask->put_Enabled(variantEnable.boolVal);
	pRegisteredTask->Release();

	return TRUE;
}



#pragma once
#include <Atlbase.h>
#include <comdef.h>
#include <taskschd.h>
#pragma comment(lib, "taskschd.lib")
class CMyTaskSchedule
{
public:
	CMyTaskSchedule();
	~CMyTaskSchedule();
	//创建计划任务
	BOOL NewTask(char* lpszTaskName, char* lpszProgramPath, char* lpszParameters, char* lpszAuthor);

	//删除计划任务
	BOOL Delete(char* lpszTaskName);

	//判断指定计划任务是否还在
	BOOL IsTaskValid(char *lpszTaskName);

	// 运行指定任务计划
	BOOL Run(char *lpszTaskName, char *lpszParam);

	// 判断指定任务计划是否启动
	BOOL IsEnable(char *lpszTaskName);

	// 设置指定任务计划是否启动还是禁用
	BOOL SetEnable(char *lpszTaskName, BOOL bEnable);

private:
	ITaskService *m_lpITS;
	ITaskFolder *m_lpRootFolder;
};


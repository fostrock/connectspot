/**
* File: mdump.cpp
*
* Purpose: 
* 
* @author Yun Hua
* @version 1.0 2007/08/17
*
* Copyright (C) 2007, Yun Hua
*/
#include "stdafx.h"
#include <cassert>
#include <stdio.h>
#include <time.h>
#include "mdump.h"

LPCTSTR MiniDumper::m_szAppName;

MiniDumper::MiniDumper( LPCTSTR szAppName )
{
	// if this assert fires then you have two instances of MiniDumper
	// which is not allowed
	assert( m_szAppName==NULL );

	m_szAppName = szAppName ? _tcsdup(szAppName) : _T("Application");

	::SetUnhandledExceptionFilter( TopLevelFilter );
}

LONG MiniDumper::TopLevelFilter( struct _EXCEPTION_POINTERS *pExceptionInfo )
{
	LONG retval = EXCEPTION_CONTINUE_SEARCH;
	HWND hParent = NULL;						// find a better value for your app

	// firstly see if dbghelp.dll is around and has the function we need
	// look next to the EXE first, as the one in System32 might be old 
	// (e.g. Windows 2000)
	HMODULE hDll = NULL;
	TCHAR szDbgHelpPath[_MAX_PATH];

	if (GetModuleFileName(NULL, szDbgHelpPath, _MAX_PATH))
	{
		LPTSTR pSlash = _tcsrchr(szDbgHelpPath, _T('\\'));
		if (pSlash)
		{
			_tcscpy_s(pSlash + 1, 12, _T("DBGHELP.DLL"));
			hDll = ::LoadLibrary(szDbgHelpPath);
		}
	}

	if (NULL == hDll)
	{
		// load any version we can
		hDll = ::LoadLibrary(_T("DBGHELP.DLL"));
	}

	LPCTSTR szResult = NULL;

	if (hDll)
	{
		MINIDUMPWRITEDUMP pDump = (MINIDUMPWRITEDUMP)::GetProcAddress( hDll, "MiniDumpWriteDump" );
		if (pDump)
		{
			TCHAR szDumpPath[_MAX_PATH];
			TCHAR szScratch [_MAX_PATH];

			// work out a good place for the dump file
			// for example: if (!GetTempPath( _MAX_PATH, szDumpPath ))
			//	_tcscpy( szDumpPath, _T("c:\\temp\\"));
			if (GetModuleFileName(NULL, szDumpPath, _MAX_PATH))
			{
				LPTSTR pSlash = _tcsrchr(szDumpPath, _T('\\'));
				if (pSlash)
				{
					_tcscpy_s(pSlash + 1, _tcslen(m_szAppName) + 1, m_szAppName);
					time_t now;
					struct tm localnow;
					TCHAR hdrDate[999] = _T("");
					time(&now);
					localtime_s(&localnow, &now);
					if (_tcsftime(hdrDate, 78, _T("_%Y-%m-%d_%H-%M-%S"), &localnow))
					{
						_tcscat_s(szDumpPath, hdrDate);
					}
					_tcscat_s(szDumpPath, _T(".dmp"));
				}
			}
			else
			{
				_tcscpy_s(szDumpPath, 9, _T("c:\\temp\\"));
			}
			

			// ask the user if they want to save a dump file
			if (IDYES == ::MessageBox( NULL, 
				_T("Something bad happened in the program, would you like to save a diagnostic file?"), 
				m_szAppName, MB_YESNO) )
			{
				// create the file
				HANDLE hFile = ::CreateFile( szDumpPath, GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS,
											FILE_ATTRIBUTE_NORMAL, NULL );

				if (hFile!=INVALID_HANDLE_VALUE)
				{
					_MINIDUMP_EXCEPTION_INFORMATION ExInfo;

					ExInfo.ThreadId = ::GetCurrentThreadId();
					ExInfo.ExceptionPointers = pExceptionInfo;
					ExInfo.ClientPointers = NULL;

					// write the dump
					BOOL bOK = pDump( GetCurrentProcess(), GetCurrentProcessId(), hFile, MiniDumpNormal, &ExInfo, NULL, NULL );
					if (bOK)
					{
						
						_stprintf_s(szScratch, _MAX_PATH, _T("Saved dump file to '%s'"), szDumpPath);
						szResult = szScratch;
						retval = EXCEPTION_EXECUTE_HANDLER;
					}
					else
					{
						_stprintf_s( szScratch, _MAX_PATH, _T("Failed to save dump file to '%s' (error %d)"), 
							szDumpPath, GetLastError());
						szResult = szScratch;
					}
					::CloseHandle(hFile);
				}
				else
				{
					_stprintf_s(szScratch, _MAX_PATH, 
						_T("Failed to create dump file '%s' (error %d)"), szDumpPath, GetLastError());
					szResult = szScratch;
				}
			}
		}
		else
		{
			szResult = _T("DBGHELP.DLL too old");
		}
	}
	else
	{
		szResult = _T("DBGHELP.DLL not found");
	}

	if (szResult)
		::MessageBox( NULL, szResult, m_szAppName, MB_OK );

	return retval;
}

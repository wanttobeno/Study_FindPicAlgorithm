#include "StdAfx.h"
#include "GameState.h"
//#include "PreDemo.h"
//#include "mapLine.h"
//#include "km.h"
#include <cmath>
#include <list>
#include <Mmsystem.h>
#pragma comment(lib, "WINMM.LIB")

#include "TestFindPic.h"
extern CTestFindPicApp theApp;

int  FindNumberPic(const Cbm & bmWnd, LPCRECT rectTarget, PRECT retRect, int resemble = 100, COLORREF rgb = 0);

BOOL FindSymbolPic(const Cbm & bmWnd, int NumberSymbol, LPCRECT rectTarget, PRECT retRect, int resemble = 100, COLORREF rgb = 0);



BOOL bF9 = FALSE;
BOOL bctrlE = FALSE;


/************************多图比较找图函数***************************************
*       形参说明hwndScreen: 截图窗口句柄
*                Folderpath: 小图所在的文件夹路径，类似“pic\\”
*            (OUT) FileName: 返回找到的文件名，如果没找到则返回NULL
*             (IN OUT) rect:返回找到匹配图的中心x\y坐标。没找到则返回零
*********************************************************************************/
BOOL FindSomePic(const Cbm & bmWnd, PCTSTR FileName, LPCRECT rectTarget, PRECT retRect, PTSTR retFileName, int resemble, COLORREF rgb)
{
    WIN32_FIND_DATA fData;
    BOOL  bFind = FALSE;
    TCHAR FilePath[MAX_PATH];
    TCHAR FileDir[MAX_PATH];
    _tcscpy_s(FilePath, MAX_PATH, FileName);
    _tcscpy_s(FileDir, MAX_PATH, FileName);


    if (FileName[lstrlen(FileName) - 1] == '\\')
        _tcscat_s(FilePath, MAX_PATH, _T("*.bmp"));
    else if (_tcschr(FileName, '*'))
        _tcsrchr(FileDir, '\\')[1] = '\0';
    else
    {
        bFind = FindPic(bmWnd, FileName, rectTarget, retRect, resemble, rgb);
        if (retFileName)
        {
            if (bFind)
                _tcscpy_s(retFileName, MAX_PATH, FileName);
            else
                retFileName[0] = '\0';
        }

        return bFind;
    }

    HANDLE hFile = FindFirstFile(FilePath, &fData);
    if (hFile == INVALID_HANDLE_VALUE)
    {
        TRACE(_T("FindSomePic --- read file failed.\n"));
        return FALSE;
    }

    do{
        wsprintf(FilePath, _T("%s%s"), FileDir, fData.cFileName);
        bFind = FindPic(bmWnd, FilePath, rectTarget, retRect, resemble, rgb);
    }while (!bFind && FindNextFile(hFile, &fData));

    FindClose(hFile);

    if (retFileName)
    {
        if (bFind)
            _tcscpy_s(retFileName, MAX_PATH, fData.cFileName);
        else
            retFileName[0] = '\0';
    }

    return bFind;
}


BOOL FindSomePic(HWND hWnd, PCTSTR FileName, LPCRECT rectTarget, PRECT retRect, PTSTR retFileName, int resemble, COLORREF rgb)
{
    return FindSomePic(Cbm(hWnd), FileName, rectTarget, retRect, retFileName, resemble, rgb);
}


BOOL FindSomePicEx(const Cbm & bmWnd, PCTSTR FileName, PRECT pRect,
                   PTSTR retFileName, int deviation, int resemble, COLORREF rgb)
{
    WIN32_FIND_DATA fData;
    BOOL  bFind = FALSE;
    TCHAR FilePath[MAX_PATH];
    TCHAR FileDir[MAX_PATH];

    _tcscpy_s(FilePath, MAX_PATH, FileName);
    _tcscpy_s(FileDir, MAX_PATH, FileName);
    if (FileName[lstrlen(FileName) - 1] == '\\')
        _tcscat_s(FilePath, MAX_PATH, _T("*.bmp"));
    else if (_tcschr(FileName, '*'))
        _tcsrchr(FileDir, '\\')[1] = '\0';
    else
    {
        bFind = FindPicEx(bmWnd, FileName, pRect, deviation, resemble, rgb);
        if (retFileName)
        {
            if (bFind)
                _tcscpy_s(retFileName, MAX_PATH, FileName);
            else
                retFileName[0] = '\0';
        }

        return bFind;
    }

    HANDLE hFile = FindFirstFile(FilePath, &fData);
    if (hFile == INVALID_HANDLE_VALUE)
    {
        TRACE(_T("FindSomePic --- read file failed.\n"));
        return FALSE;
    }

    do{
        wsprintf(FilePath, _T("%s%s"), FileDir, fData.cFileName);
        bFind = FindPicEx(bmWnd, Cbm(FilePath), pRect, deviation, resemble, rgb);
    }while (!bFind && FindNextFile(hFile, &fData));

    FindClose (hFile);

    if (retFileName)
    {
        if (bFind)
            _tcscpy_s(retFileName, MAX_PATH, fData.cFileName);
        else
            retFileName[0] = '\0';
    }

    return bFind;
}


BOOL FindSomePicEx(HWND hWnd, PCTSTR FileName, PRECT pRect,
                   PTSTR retFileName, int deviation, int resemble, COLORREF rgb)
{
    return FindSomePicEx(Cbm(hWnd), FileName, pRect, retFileName, deviation, resemble, rgb);
}


/***********通过游戏鼠标图标，查找光标在游戏窗口中的坐标******************/
BOOL GetGameCursorPos(HWND hWnd, POINT & pt, int resemble, COLORREF rgb)
{
    return GetGameCursorPos(Cbm(hWnd), pt, resemble, rgb);
}

BOOL GetGameCursorPos(const Cbm & bmWnd, POINT & pt, int resemble, COLORREF rgb)
{

    CRect rect;

    TCHAR retFileName[MAX_PATH];
    BOOL bFind = FindSomePic(bmWnd, _T("bmp\\gameCursor\\"), NULL, &rect, retFileName, resemble, rgb);
    if (bFind)
    {
        if (_tcscmp(retFileName, _T("puzhuo.bmp")) == 0)
        {
            pt.x = rect.left - 35;
            pt.y = rect.top + 5;
        }else if (_tcscmp(retFileName, _T("inputcursor.bmp")) == 0)
        {
            pt = rect.CenterPoint();
        }else
        {
            pt.x = rect.left - 5;
            pt.y = rect.top - 4;
        }
    }

    return bFind;
}


// 获取当前所在场景

int GetMapIndex(HWND hWnd, int resemble, COLORREF rgb)
{
    return GetMapIndex(Cbm(hWnd), resemble, rgb);
}


int GetMapIndex(const Cbm & bmWnd, int resemble, COLORREF rgb)
{
    TCHAR FileName[MAX_PATH];
    int    mapIndex;
    const RECT rect = RECT_MAPINFO;
    RECT rectTemp = rect;
    if (FindSomePic(bmWnd, _T("bmp\\address\\"), &rect, &rectTemp, FileName, resemble, rgb))
    {
        mapIndex = _tstoi(FileName);
        rectTemp.left  = rect.left;
        rectTemp.right = rect.right;
        if (FindSomePic(bmWnd, _T("bmp\\chNum\\"), &rectTemp, NULL, FileName, resemble, rgb))
            mapIndex += _tstoi(FileName);
    }
    else
        mapIndex = UNKNOWN_MAP;

    return mapIndex;
}



//获取当前血量，返回值为0~100的百分数
UINT GetRoleStatusValue(const Cbm & bmWnd, int StatusType)
{
    //血条在坐标(582,11)~(631,11)的RGB值
    static COLORREF RedValueRGB[50] = {
        0x000058, 0x000060, 0x000068, 0x000058, 0x000060, 0x0800A0, 0x000098, 0x0800A0, 0x0800A8, 0x000098, 
        0x080088, 0x000070, 0x000070, 0x0800A0, 0x080088, 0x080088, 0x080088, 0x080088, 0x080088, 0x000098, 
        0x000098, 0x0800B0, 0x0800A0, 0x080088, 0x080088, 0x080088, 0x080088, 0x000068, 0x000070, 0x000068,
        0x000070, 0x000070, 0x0800B0, 0x0800B0, 0x000098, 0x000070, 0x080490, 0x080078, 0x080090, 0x0800A8,
        0x0800B8, 0x0800B0, 0x000098, 0x080088, 0x080098, 0x0800B8, 0x0800B8, 0x0800B8, 0x0800B0, 0x000068
    };

    //蓝条在坐标(583,23)~(632,23)的RGB值
    static COLORREF BlueValueRGB[50] = {
        0x502000, 0x582800, 0x703000, 0x582400, 0x682C00, 0xA04C00, 0x984000, 0xA84800, 0xA05000, 0x984400, 
        0x903C00, 0x783800, 0x703400, 0xA04C00, 0x883C00, 0x903C00, 0x903C00, 0x883C00, 0x883C00, 0x984000, 
        0x984000, 0xB05400, 0xA04C00, 0x903C00, 0x883C00, 0x903C00, 0x903C00, 0x683000, 0x703400, 0x683000, 
        0x783800, 0x783800, 0xB05400, 0xB05000, 0x984000, 0x783800, 0x904C00, 0x783C00, 0x904800, 0xA05000, 
        0xB85C00, 0xB05400, 0x904800, 0x883C00, 0x985400, 0xB85C00, 0xB85C00, 0xB85C00, 0xB05400, 0x783C00
    };

    int i = 0;
    while (i < 50)
    {
        BOOL bFind = FALSE;
        switch (StatusType)
        {
        case HEALTH_POINT:
            bFind = bmWnd.GetPixel(582 + i, 11) == RedValueRGB[i];
            break;
        case MANA_POINT:
            bFind = bmWnd.GetPixel(583 + i, 23) == BlueValueRGB[i];
            break;
        default:
            break;
        }
        if (bFind)
            i++;
        else
            break;
    }

    return 2 * i;
}



//获取当前游戏窗口左上角显示的人物坐标
BOOL GetGameRolePos(HWND hWnd, POINT &ptPos, int resemble, COLORREF rgb)
{
    return GetGameRolePos(Cbm(hWnd), ptPos, resemble, rgb);
}

BOOL GetGameRolePos(const Cbm & bmWnd, POINT &ptPos, int resemble, COLORREF rgb)
{
    RECT rectLeftBracket, rectRightBracket, rectComma, rectNumber;
    rectComma = RECT_MAPINFO;
    BOOL bFindComma = FALSE;
    int i, num;

    memset(&ptPos, 0, sizeof(POINT));

    if (!FindSymbolPic(bmWnd, LEFTBRACKET, RECT_MAPINFO, &rectLeftBracket, resemble, rgb))
        return FALSE;
    if (!FindSymbolPic(bmWnd, RIGHTBRACKET, RECT_MAPINFO, &rectRightBracket, resemble, rgb))
        return FALSE;
    for (i = 0; i < 3; i++)
    {
        bFindComma = FindSymbolPic(bmWnd, COMMA1 + i, RECT_MAPINFO, &rectComma, resemble, rgb);
        if (bFindComma)
            break;
    }
    if (!bFindComma)
        return FALSE;

    for (i = 0; i < (rectComma.left - rectLeftBracket.right) / 6; i++)
    {
        rectNumber = rectLeftBracket;
        rectNumber.left  = rectLeftBracket.right + 1 + i * 6;
        rectNumber.right = rectLeftBracket.right + 6 + i * 6;

        num = FindNumberPic(bmWnd, &rectNumber, NULL, resemble, rgb);
        if (num == INVALID_VALUE)
            return FALSE;
        else
        {
            ptPos.x = ptPos.x * 10 + num;
        }
    }
    for (i = 0; i < (rectRightBracket.left - rectComma.right) / 6; i++)
    {
        rectNumber = rectLeftBracket;
        rectNumber.left  = rectComma.right + 1 + i * 6;
        rectNumber.right = rectComma.right + 6 + i * 6;

        num = FindNumberPic(bmWnd, &rectNumber, NULL, resemble, rgb);
        if (num == INVALID_VALUE)
            return FALSE;
        else
        {
            ptPos.y = ptPos.y * 10 + num;
        }
    }

    return TRUE;
}


BOOL FindSymbolPic(const Cbm & bmWnd, int NumberSymbol, LPCRECT rectTarget, PRECT retRect, int resemble, COLORREF rgb)
{
    TCHAR FileName[MAX_PATH];
    wsprintf(FileName, _T("bmp\\symbol\\%02dSymbol.bmp"), NumberSymbol);

    return FindPic(bmWnd, FileName, rectTarget, retRect, resemble, rgb);
}


int FindNumberPic(const Cbm & bmWnd, LPCRECT rectTarget, PRECT retRect, int resemble, COLORREF rgb)
{
    TCHAR FileName[MAX_PATH];
    BOOL bFind = FindSomePic(bmWnd, _T("bmp\\number\\"), rectTarget, retRect, FileName, resemble, rgb);

    if (!bFind)
        return INVALID_VALUE;
    else
        return _tstoi(FileName);
}


BOOL GetTeleportItemPos(int TeleportPos, SIZE & size)
{
    CString strPos, strPosValue;

    strPos.Format(_T("strPos%d"), TeleportPos);
    strPosValue = theApp.GetProfileString(_T("PosSetting"), strPos);

    if (strPosValue.IsEmpty())
        return FALSE;
    else
    {
        size.cx = (strPosValue[1] - '1')* 51; 
        size.cy = (strPosValue[0] - 'A') * 51; 
        return TRUE;
    }
}


BOOL ShutDown(DWORD dwFlag)
{
    HANDLE hToken;
    TOKEN_PRIVILEGES tkp;

    // 打开当前进程访问代号
    if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
        return FALSE;

    LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, &tkp.Privileges[0].Luid);//获取本地唯一标识用于在特定系统中设置权限
    tkp.PrivilegeCount = 1;
    tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
    AdjustTokenPrivileges(hToken, FALSE, &tkp, 0, NULL, 0);
    //提升访问令牌权限

    return ExitWindowsEx(dwFlag, 0);
}



__declspec(naked)
int GetSystem()
{
    _asm
    {
        mov eax,fs:[0x30]
        mov eax,[eax+0A4h]
        ret
    }
}

BOOL CreateFolder(PCTSTR FilePath)
{
	int len = lstrlen(FilePath);
	for (int pos = 0; pos < len - 1; pos++)
	{
		if (FilePath[pos] == '\\' || FilePath[pos] == '/')
		{
			CString FileDirectory(FilePath, pos + 1);
			// 判断文件夹路径是否存在，不存在则创建文件夹
			if (!PathIsDirectory(FileDirectory))
			{
				if (!CreateDirectory(FileDirectory, NULL))
					return FALSE;
			}
		}
	}
	return TRUE;
}

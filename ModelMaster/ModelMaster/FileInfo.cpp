#include "stdafx.h"
#include "FileInfo.h"


CFileInfo::CFileInfo()
{
}


CFileInfo::~CFileInfo()
{
}

CFileInfo::CFileInfo(CFileFind file)
{
	FileName = file.GetFileName();
	FilePath = file.GetFilePath();
	FileTitle = file.GetFileTitle();
}
bool CFileInfo::OpenFile(CString exe)
{
	WinExec(exe.GetBuffer(0), SW_SHOW);
	return true;
}
#pragma once
#include "stdafx.h"
#include <vector>
using namespace std;

struct FileInfo
{
	ULONGLONG Length;
	CString FileName;
	CString FilePath;
	CString FileTitle;
	CString FileURL;
	CString Root;

	FileInfo(CFileFind file)
	{
		FileName = file.GetFileName();
		FilePath = file.GetFilePath();
		FileTitle = file.GetFileTitle();
	}
	FileInfo() {}
};

struct Project
{
	CString sName;
	CString sPath;
	vector<CString> sModelList;
	vector<FileInfo> FileList;
};
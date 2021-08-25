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

	CTime LastWriteTime;
	CTime LastAccessTime;
	CTime CreationTime;

	float BuInfo[10] = { 0.f };

	FileInfo(CFileFind file)
	{
		FileName = file.GetFileName();
		FilePath = file.GetFilePath();
		FileTitle = file.GetFileTitle();
	}
	FileInfo() {}
	bool OpenFile(CString exe)
	{
		CString str;
		str.Format("%s TYPE=OPEN PATH=\"%s\"", exe, FilePath);
		WinExec(str.GetBuffer(0), SW_SHOW);
		return true;
	}
};

struct Project
{
	CString sName;
	CString sPath;
	vector<CString> sModelList;
	vector<FileInfo> FileList;

	void InsertFile(FileInfo file)
	{
		for (int i = 0; i < FileList.size(); i++)
		{
			CTime ta = FileList[i].LastWriteTime;
			CTime tt = file.LastWriteTime;
			if (tt < ta)
			{
				FileList.insert(FileList.begin() + i, file);
				return;
			}
		}
		FileList.push_back(file);
		return;
	}
};
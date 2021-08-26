#pragma once
#include "stdafx.h"
#include <vector>
#include "FileInfo.h"
using namespace std;

struct Project
{
	CString sName;
	CString sPath;
	vector<CString> sModelList;
	vector<CFileInfo> FileList;

	void InsertFile(CFileInfo file)
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
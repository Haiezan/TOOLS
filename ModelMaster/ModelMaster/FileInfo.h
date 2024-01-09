#pragma once

const int DATANUM = 10;

class CFileInfo
{
public:
	CFileInfo();
	~CFileInfo();

	CFileInfo(CFileFind file);

	ULONGLONG Length;
	CString FileName;
	CString FilePath;
	CString FileTitle;
	CString FileURL;
	CString Root;

	CString Ext;

	CTime LastWriteTime;
	CTime LastAccessTime;
	CTime CreationTime;

	HTREEITEM hItem;

	float BuInfo[DATANUM] = { 0.f };

	bool OpenFile(CString exe);
};


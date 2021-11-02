#include "stdafx.h"
#include "Browse.h"

//浏览获取文件路径
CString BrowseFile()
{
	// 设置过滤器
	TCHAR szFilter[] = _T("执行程序(*.exe)|*.exe|所有文件(*.*)|*.*||");

	// 构造打开文件对话框   
	CFileDialog fileDlg(TRUE, _T("txt"), NULL, 0, szFilter);
	CString strFilePath;

	// 显示打开文件对话框   
	if (IDOK == fileDlg.DoModal())
	{
		// 如果点击了文件对话框上的“打开”按钮，则将选择的文件路径显示到编辑框里   
		strFilePath = fileDlg.GetPathName();
	}

	return strFilePath;
}

//浏览获取文件夹路径
CString BrowseDirectory(CString m_sPath)
{
	TCHAR targetPath[MAX_PATH];

	// 获取特定文件夹的LPITEMIDLIST，可以将之理解为HANDLE
	// 所谓的特定文件夹,你可以用CSIDL_XXX来检索之。
	LPITEMIDLIST rootLoation;
	SHGetSpecialFolderLocation(NULL, CSIDL_DESKTOP, &rootLoation);
	if (rootLoation == NULL) {
		// unkown error
		return targetPath;
	}
	// 配置对话框
	BROWSEINFO bi;
	ZeroMemory(&bi, sizeof(bi));
	bi.pidlRoot = rootLoation; // 文件夹对话框之根目录，不指定的话则为我的电脑
	bi.lpszTitle = _T("选择目录"); // 可以不指定
	// bi.ulFlags = BIF_EDITBOX | BIF_RETURNONLYFSDIRS;

	// 打开对话框, 有点像DoModal
	LPITEMIDLIST targetLocation = SHBrowseForFolder(&bi);
	if (targetLocation != NULL) {

		SHGetPathFromIDList(targetLocation, targetPath);
		return targetPath;
	}

	return m_sPath;
}
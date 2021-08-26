#pragma once
#include "stdafx.h"
#include "ReadAline.h"
#include "FileInfo.h"

bool ReadSSGFile(CFileInfo* pfile)
{
	FILE* fd = NULL;
	if ((fd = fopen(pfile->FilePath, "r+t")) == NULL) return false;

	TReadALine ReadALine;
	char str[3000];
	float ff[50];

	while (true)
	{
		strcpy(str, ReadALine.ReadAString(fd));
		if (ReadALine.endfile == TRUE) break;

		//读取抗震设防类别
		if (strstr(str, "FortificationCategory") != NULL)
		{
			if (ReadALine.GetNumFromStr(str, "FortificationCategory", 0, 1, ff) != FALSE)
			{
				pfile->BuInfo[0] = ff[0];
			}
			continue;
		}
		//读取场地类别
		if (strstr(str, "FieldClass") != NULL)
		{
			if (ReadALine.GetNumFromStr(str, "FieldClass", 0, 1, ff) != FALSE)
			{
				pfile->BuInfo[1] = ff[0];
			}
			continue;
		}
		//读取场地分组
		if (strstr(str, "FieldGroup") != NULL)
		{
			if (ReadALine.GetNumFromStr(str, "FieldGroup", 0, 1, ff) != FALSE)
			{
				pfile->BuInfo[2] = ff[0];
			}
			continue;
		}
		//读取设防烈度
		if (strstr(str, "EarthQuakeGrade") != NULL)
		{
			if (ReadALine.GetNumFromStr(str, "EarthQuakeGrade", 0, 1, ff) != FALSE)
			{
				pfile->BuInfo[3] = ff[0];
			}
			continue;
		}

		//读取层信息
		if (strstr(str, "*STORY NUMBER") != NULL)
		{
			if (ReadALine.GetNumFromStr(str, "*STORY NUMBER=", 0, 1, ff) != FALSE)
			{
				pfile->BuInfo[4] = ff[0];

				for (int i = 0; i<int(pfile->BuInfo[4]); i++)
				{
					strcpy(str, ReadALine.ReadAString(fd));
					if (ReadALine.GetNumFromStr(str, "", 0, 7, ff) != FALSE)
					{
						pfile->BuInfo[6] = max(pfile->BuInfo[6], ff[3]);
					}
				}
			}
			continue;
		}
		//读取地下室层数
		if (strstr(str, "NbaseSty=") != NULL)
		{
			if (ReadALine.GetNumFromStr(str, "NbaseSty=", 0, 1, ff) != FALSE)
			{
				pfile->BuInfo[5] = ff[0];
			}
			continue;
		}
	}

	fclose(fd);

	return true;
}
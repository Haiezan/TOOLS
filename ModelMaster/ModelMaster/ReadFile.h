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
		//读取节点信息
		//读取地下室层数
		if (strstr(str, "*COOR NUMBER=") != NULL)
		{
			if (ReadALine.GetNumFromStr(str, "*COOR NUMBER=", 0, 1, ff) != FALSE)
			{
				int num = int(ff[0]);
				float Xmax = 0.f, Xmin = 0.f, Ymax = 0.f, Ymin = 0.f;
				for (int i = 0; i<num; i++)
				{
					strcpy(str, ReadALine.ReadAString(fd));
					if (ReadALine.GetNumFromStr(str, "", 0, 20, ff) != FALSE)
					{
						if (i == 0)
						{
							Xmax = Xmin = ff[1];
							Ymax = Ymin = ff[2];
						}
						else
						{
							Xmax = max(Xmax, ff[1]);
							Xmin = min(Xmin, ff[1]);
							Ymax = max(Ymax, ff[2]);
							Ymin = min(Ymin, ff[2]);
						}
					}
				}

				pfile->BuInfo[7] = Xmax - Xmin;
				pfile->BuInfo[8] = Ymax - Ymin;
			}
			continue;
		}
	}

	fclose(fd);

	return true;
}
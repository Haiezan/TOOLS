#pragma once
#include "stdafx.h"
#include "ReadAline.h"
#include "FileInfo.h"

//结构类型,实际上代表几何数据类型
enum LINK_SUBTYPE
{
	LINK_UNKNOWN = 0,		//未知
	LINK_VELODAMPER = 0x1,	//阻尼器-速度型 //乔保娟2015.5.19	
	LINK_ISOLATOR = 0x2,	//隔震支座
	LINK_BRB = 0x4,	//BRB 
	LINK_SPRING = 0x8,	//弹簧
	LINK_USERDAMPERVEL = 0x10,//自定义速度型阻尼器
	LINK_DISPDAMPER = 0x20,
	LINK_CABLE = 0x40,
	LINK_GAP = 0x80,
	LINK_FRICPND = 0x100,
	LINK_HOOK = 0x200,
	LINK_USERDAMPERDIS = 0x400,//自定义位移型阻尼器
	LINK_WEN = 0x800,//Wen模型
	LINK_ISOLATOR3D = 0x1000,//三向耦合隔震支座
	LINK_TMD = 0x2000,//TMD
	LINK_PRESTRJOIN = 0x4000,//预应力连接键
	LINK_MAXWELL = 0x8000,//Maxwell 模型
	LINK_ESB = 0x10000,//弹性滑板支座
	LINK_ALL_LRB = LINK_ISOLATOR | LINK_ISOLATOR3D,
	LINK_ALL_ISOBEARING = LINK_ALL_LRB | LINK_FRICPND | LINK_ESB,
	LINK_DAMPER = LINK_VELODAMPER | LINK_BRB | LINK_USERDAMPERVEL | LINK_DISPDAMPER | LINK_USERDAMPERDIS | LINK_WEN | LINK_TMD | LINK_MAXWELL,
};

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
		//读取构件截面信息，获取减隔震
		if (strstr(str, "*LINKSECTION NUMBER=") != NULL)
		{
			if (ReadALine.GetNumFromStr(str, "*LINKSECTION NUMBER=", 0, 1, ff) != FALSE)
			{
				int num = int(ff[0]);
				for (int i = 0; i < num; i++)
				{
					char strs[20][20];
					strcpy(str, ReadALine.ReadAString(fd));
					if (ReadALine.GetStrsFromStr(str, strs) != FALSE)
					{
						int a = 3;
						int b = 5;
						int c = a & 5;

						int SectionType = atoi(strs[2]);
						if (SectionType & LINK_DAMPER)
						{
							pfile->bHasDamper = TRUE;
						}
						else if (SectionType & LINK_ALL_ISOBEARING)
						{
							pfile->bHasISO = TRUE;
						}
					}
					for (int ii = 0; ii < 7; ii++) ReadALine.ReadAString(fd);
				}
			}
			continue;
		}
	}

	fclose(fd);

	return true;
}
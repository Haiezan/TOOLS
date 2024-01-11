#pragma once
#include "stdafx.h"
#include "ReadAline.h"
#include "FileInfo.h"

//�ṹ����,ʵ���ϴ�������������
enum LINK_SUBTYPE
{
	LINK_UNKNOWN = 0,		//δ֪
	LINK_VELODAMPER = 0x1,	//������-�ٶ��� //�Ǳ���2015.5.19	
	LINK_ISOLATOR = 0x2,	//����֧��
	LINK_BRB = 0x4,	//BRB 
	LINK_SPRING = 0x8,	//����
	LINK_USERDAMPERVEL = 0x10,//�Զ����ٶ���������
	LINK_DISPDAMPER = 0x20,
	LINK_CABLE = 0x40,
	LINK_GAP = 0x80,
	LINK_FRICPND = 0x100,
	LINK_HOOK = 0x200,
	LINK_USERDAMPERDIS = 0x400,//�Զ���λ����������
	LINK_WEN = 0x800,//Wenģ��
	LINK_ISOLATOR3D = 0x1000,//������ϸ���֧��
	LINK_TMD = 0x2000,//TMD
	LINK_PRESTRJOIN = 0x4000,//ԤӦ�����Ӽ�
	LINK_MAXWELL = 0x8000,//Maxwell ģ��
	LINK_ESB = 0x10000,//���Ի���֧��
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

		//��ȡ����������
		if (strstr(str, "FortificationCategory") != NULL)
		{
			if (ReadALine.GetNumFromStr(str, "FortificationCategory", 0, 1, ff) != FALSE)
			{
				pfile->BuInfo[0] = ff[0];
			}
			continue;
		}
		//��ȡ�������
		if (strstr(str, "FieldClass") != NULL)
		{
			if (ReadALine.GetNumFromStr(str, "FieldClass", 0, 1, ff) != FALSE)
			{
				pfile->BuInfo[1] = ff[0];
			}
			continue;
		}
		//��ȡ���ط���
		if (strstr(str, "FieldGroup") != NULL)
		{
			if (ReadALine.GetNumFromStr(str, "FieldGroup", 0, 1, ff) != FALSE)
			{
				pfile->BuInfo[2] = ff[0];
			}
			continue;
		}
		//��ȡ����Ҷ�
		if (strstr(str, "EarthQuakeGrade") != NULL)
		{
			if (ReadALine.GetNumFromStr(str, "EarthQuakeGrade", 0, 1, ff) != FALSE)
			{
				pfile->BuInfo[3] = ff[0];
			}
			continue;
		}

		//��ȡ����Ϣ
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
		//��ȡ�����Ҳ���
		if (strstr(str, "NbaseSty=") != NULL)
		{
			if (ReadALine.GetNumFromStr(str, "NbaseSty=", 0, 1, ff) != FALSE)
			{
				pfile->BuInfo[5] = ff[0];
			}
			continue;
		}
		//��ȡ�ڵ���Ϣ
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
		//��ȡ����������Ϣ����ȡ������
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
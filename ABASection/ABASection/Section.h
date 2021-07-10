#pragma once
#include<vector>
using namespace std;

enum SECTION_SHAPE
{
	TRACT_PILLAR_ST_CROSS = 0,      //十字工型截面
	TRACT_PILLAR_ST_I = 1,      //工型截面

};

struct sPar
{
	int num;
	CString sDes;
	double fValue;

	sPar(int Num, CString Des, double Value) :num(Num), sDes(Des), fValue(Value) {}
};

class CSection
{
public:
	CSection();
	CSection(SECTION_SHAPE Shape);
	~CSection();
	SECTION_SHAPE Shape;
	CString sName;
	vector<sPar> Pars;
	int ID;

	CString GenSecINP(CString sSetName, CString sMatName);
};


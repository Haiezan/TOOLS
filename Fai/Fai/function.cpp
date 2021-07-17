#include "function.h"
#include <math.h>
#include "stdafx.h"

float GetFai0(int iCag, float lagdn)
{
	float fFai = 0.f;

	float a1, a2, a3;
	switch (iCag)
	{
	case 0:
		a1 = 0.41f;
		a2 = 0.986f;
		a3 = 0.152f;
		break;
	case 1:
		a1 = 0.65f;
		a2 = 0.965f;
		a3 = 0.3f;
		break;
	case 2:
		a1 = 0.73f;
		if (lagdn <= 1.05)
		{
			a2 = 0.906f;
			a3 = 0.595f;
		}
		else
		{
			a2 = 1.216f;
			a3 = 0.302f;
		}
		break;
	case 3:
		a1 = 1.35f;
		if (lagdn <= 1.05)
		{
			a2 = 0.868f;
			a3 = 0.915f;
		}
		else
		{
			a2 = 1.375f;
			a3 = 0.432f;
		}
		break;
	default:
		a1 = 0.f;
		a2 = 0.f;
		a3 = 0.f;
		break;
	}

	//计算fai值
	if (lagdn <= 0.215f)
	{
		fFai = 1 - a1 * lagdn*lagdn;
	}
	else
	{
		fFai = 1.f / (2.f * lagdn*lagdn)*((a2 + a3 * lagdn + lagdn * lagdn) - sqrt((a2 + a3 * lagdn + lagdn * lagdn) * (a2 + a3 * lagdn + lagdn * lagdn) - 4.f * lagdn*lagdn));
	}

	return fFai;
}

float GetFai(int iCag, float fCxb, float fFy, float fE)
{
	//获取计算参数
	float etak = sqrt(235.f / fFy);
	float lagdn = fCxb / 3.14f * sqrt(fFy / fE);
	float fFai = 0.f;

	fFai = GetFai0(iCag, lagdn);

	return fFai;
}

float GetFai(int iCag, float cxb2etak)
{
	float lagdn = cxb2etak / 3.14f / sqrt(206000.f/235.f);
	float fFai = 0.f;
	fFai = GetFai0(iCag, lagdn);
	return fFai;
}
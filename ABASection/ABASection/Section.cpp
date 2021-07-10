#include "stdafx.h"
#include "Section.h"

CSection::CSection()
{
}

CSection::CSection(SECTION_SHAPE iShape)
{
	Pars.clear();
	Shape = iShape;
	switch (iShape)
	{
	case TRACT_CROSS_I:
		sName = "十字工型截面";
		sPath = ".\\BMP\\TRACT_CROSS_I.bmp";
		Pars.push_back(sPar(1, _T("竖向工形总高度H1"), 0.6));
		Pars.push_back(sPar(2, _T("竖向工形腹板厚度tw1"), 0.05));
		Pars.push_back(sPar(3, _T("竖向工形上翼缘宽度B1"), 0.3));
		Pars.push_back(sPar(4, _T("竖向工形上翼缘厚度tf1"), 0.05));
		Pars.push_back(sPar(5, _T("竖向工形下翼缘宽度B2"), 0.3));
		Pars.push_back(sPar(6, _T("竖向工形下翼缘厚度tf2"), 0.05));
		Pars.push_back(sPar(7, _T("横向工形总高度H2"), 0.6));
		Pars.push_back(sPar(8, _T("横向工形腹板厚度tw2"), 0.05));
		Pars.push_back(sPar(9, _T("横向工形上翼缘宽度B3"), 0.3));
		Pars.push_back(sPar(10, _T("横向工形上翼缘厚度tf3"), 0.05));
		Pars.push_back(sPar(11, _T("横向工形下翼缘宽度B4"), 0.3));
		Pars.push_back(sPar(12, _T("横向工形下翼缘厚度tf4"), 0.05));
		break;
	case TRACT_I:
		sName = "工型截面";
		sPath = ".\\BMP\\TRACT_I.bmp";
		Pars.push_back(sPar(1, _T("截面总高度H1"), 0.4));
		Pars.push_back(sPar(2, _T("腹板厚度tw"), 0.05));
		Pars.push_back(sPar(3, _T("上翼缘宽度B1"), 0.2));
		Pars.push_back(sPar(4, _T("上翼缘厚度tf1"), 0.05));
		Pars.push_back(sPar(5, _T("下翼缘宽度B2"), 0.2));
		Pars.push_back(sPar(6, _T("下翼缘厚度tf2"), 0.05));
		Pars.push_back(sPar(7, _T("横向工形总高度H2"), 0.6));
		break;

	default:
		break;
	}
	

}


CSection::~CSection()
{
}

CString CSection::GenSecINP(CString sSetName,CString sMatName)
{
	CString sText;

	if (Shape == TRACT_CROSS_I)
	{
		double X[12], Y[12], T[12];

		double H1 = Pars[0].fValue;
		double tw1 = Pars[1].fValue;
		double B1 = Pars[2].fValue;
		double tf1 = Pars[3].fValue;
		double B2 = Pars[4].fValue;
		double tf2 = Pars[5].fValue;
		double H2 = Pars[6].fValue;
		double tw2 = Pars[7].fValue;
		double B3 = Pars[8].fValue;
		double tf3 = Pars[9].fValue;
		double B4 = Pars[10].fValue;
		double tf4 = Pars[11].fValue;

		X[0] = -B1 / 2;
		Y[0] = H1 / 2 - tf1 / 2;
		X[1] = B1 / 2;
		Y[1] = H1 / 2 - tf1 / 2;
		T[1] = tf1;

		X[2] = 0;
		Y[2] = H1 / 2 - tf1;
		T[2] = 0;
		X[3] = 0;
		Y[3] = tf2 - H1 / 2;
		T[3] = tw1;

		X[4] = -B2 / 2;
		Y[4] = tf2 / 2 - H1 / 2;
		T[4] = 0;
		X[5] = B2 / 2;
		Y[5] = tf2 / 2 - H1 / 2;
		T[5] = tf2;

		X[6] = H2 / 2 - tf4 / 2;
		Y[6] = B4 / 2;
		T[6] = 0;
		X[7] = H2 / 2 - tf4 / 2;
		Y[7] = -B4 / 2;
		T[7] = tf4;

		X[8] = H2 / 2 - tf4;
		Y[8] = 0;
		T[8] = 0;
		X[9] = tf4 - H2 / 2;
		Y[9] = 0;
		T[9] = tw2;

		X[10] = tf3 / 2 - H2 / 2;
		Y[10] = B3 / 2;
		T[10] = 0;
		X[11] = tf3 / 2 - H2 / 2;
		Y[11] = -B3 / 2;
		T[11] = tf3;

		CString sT1, sT2, sT3;
		sText.Format(_T("11,\t%8.4f,\t%8.4f,\t%8.4f,\t%8.4f,\t%8.4f\r\n"), X[0], Y[0], X[1], Y[1], T[1]);
		for (int i = 2; i < 12; i++)
		{
			sT1.Format(_T("   \t%8.4f,\t%8.4f,\t%8.4f\r\n"), X[i], Y[i], T[i]);
			sText += sT1;
		}

		sT2.Format(_T("*BEAM SECTION,ELSET=%s,MATERIAL=%s,SECTION=ARBITRARY,ROTARY INERTIA=ISOTROPIC\r\n"), sSetName, sMatName);
		sText = sT2 + sText;
	}
	else if (Shape == TRACT_I)
	{
		double H1 = Pars[0].fValue;
		double tw = Pars[1].fValue;
		double B1 = Pars[2].fValue;
		double tf1 = Pars[3].fValue;
		double B2 = Pars[4].fValue;
		double tf2 = Pars[5].fValue;
		double H2 = Pars[6].fValue;

		double I = H1 / 2;

		CString sT1;
		sText.Format(_T("%8.4f,\t%8.4f,\t%8.4f,\t%8.4f,\t%8.4f,\t%8.4f,\t%8.4f\r\n"), I, H1, B1, B2, tw, tf1, tf2);
		sT1.Format(_T("*BEAM SECTION,ELSET=%s,MATERIAL=%s,SECTION=I,ROTARY INERTIA=ISOTROPIC\r\n"), sSetName, sMatName);
		sText = sT1 + sText;
	}

	return sText;
}
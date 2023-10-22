#pragma once
#include "pch.h"

struct ISO
{
	CString m_sName;
	BOOL m_bU[6];
	float m_fK[6], m_fC[6];
	BOOL m_bUN[6];
	float m_fNL[8];

	ISO()
	{
		m_sName = L"LRB1000";

		m_bU[0] = m_bU[1] = m_bU[2] = TRUE;
		m_bU[3] = m_bU[4] = m_bU[5] = FALSE;

		m_fK[0] = 4.4e6f;
		m_fK[1] = m_fK[2] = 2460.f;
		m_fK[3] = m_fK[4] = m_fK[5] = 0.f;

		m_fC[0] = m_fC[3] = m_fC[4] = m_fC[5] = 0.f;
		m_fC[1] = m_fC[2] = 23.f;

		m_bUN[0] = m_bUN[1] = m_bUN[2] = TRUE;
		m_bUN[3] = m_bUN[4] = m_bUN[5] = FALSE;

		m_fNL[0] = 4.4e6f; //����ѹ���ն�
		m_fNL[1] = 4.4e5f; //��������ն�
		m_fNL[2] = m_fNL[5] = 18970.f; //�����ʼ�ն�
		m_fNL[3] = m_fNL[6] = 203.f; //������
		m_fNL[4] = m_fNL[7] = 0.0769636f; //������նȱ�
	};
	BOOL IsPlastic()
	{
		return (m_bUN[0] || m_bUN[1] || m_bUN[2]);
	};
	BOOL IsDamping()
	{
		return (m_fC[0] > 0.f || m_fC[1] > 0.f || m_fC[2] > 0.f);
	}
};
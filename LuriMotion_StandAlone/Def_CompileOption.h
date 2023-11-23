#pragma once

#include <afxwin.h>

#define	SYS_OQC_SFR_MULTI_CL			0
#define	SYS_OQC_SFR_SINGLE_CL			1
#define	SYS_DISTORTION_30				2
#define	SYS_FILM_PEEL_OFF				3

#define	LEFT_SIDE						0
#define	RIGHT_SIDE						1
#define	MAX_SIDE						2

static LPCTSTR g_szInsptrSysType[] =
{
	_T("OQC SFR MULTI CL"),
	_T("OQC SFR SINGLE CL"),
	_T("Intrinsic Cal 30"),
	_T("FILM PEEL OFF"),
};

#ifdef LT_EQP	// ������Ʈ �������� /D Ŀ�ǵ���� ��� �� ���
#define		GET_INSPECTOR		LT_EQP
#else			// ���� ����
#define		GET_INSPECTOR		SYS_OQC_SFR_MULTI_CL
#endif

#ifdef _DEBUG
	#define		DevMode_Simulator
#else

#endif


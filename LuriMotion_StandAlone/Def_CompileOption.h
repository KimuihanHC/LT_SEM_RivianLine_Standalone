#pragma once

#include <afxwin.h>

#define	SYS_OQC_SFR_MULTI_CL			0
#define	SYS_OQC_SFR_SINGLE_CL			1
#define	SYS_DISTORTION_30				2

static LPCTSTR g_szInsptrSysType[] =
{
	_T("OQC SFR MULTI CL"),
	_T("OQC SFR SINGLE CL"),
	_T("Intrinsic Cal 30"),
};

#ifdef LT_EQP	// 프로젝트 설정에서 /D 커맨드라인 사용 할 경우
#define		GET_INSPECTOR		LT_EQP
#else			// 수동 설정
#define		GET_INSPECTOR		SYS_OQC_SFR_MULTI_CL
#endif

#ifdef _DEBUG
//	#define		DevMode_Simulator
#else

#endif


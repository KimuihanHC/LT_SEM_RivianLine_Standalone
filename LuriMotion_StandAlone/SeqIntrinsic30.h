#pragma once

#if(GET_INSPECTOR == SYS_DISTORTION_30)

#include "pch.h"
#include "Sequence.h"
#include "User_Define.h"

class CSeqIntrinsic30 : public CSequence, public CSington<CSeqIntrinsic30>
{
public:
	CSeqIntrinsic30();
	virtual ~CSeqIntrinsic30();

public:
	void Initialize_Startup				(__in UINT nIdx, __in int nCapture = 0);
	void Finalize_Startup				(__in enMCErrCode err, __in int nIdx, __in int nCapture = 0);

	UINT StartOperation_Startup			();								// 시업점검 모드
	UINT StartOperation_Run				();								// auto 모드 

	UINT Maintence						(__in enMaintFunId id);

	void SetInspectionStep				();

	void RestartStep					();

	void ResetStepFlag					();
	bool ReadyToStart					();

	bool IsStep_Ready					();
	bool IsStep_WaitForPushSwitch		();
	bool IsStep_UnderMoveY				();
	bool IsStep_BeforePushStopSwitch	();
	bool IsStep_NotStartupModeWait		();

	enMCErrCode CheckPowerVoltage		();
	void SetPowerVoltage				();
	float GetPowerVoltage				();
};

#endif
#pragma once

#if(GET_INSPECTOR == SYS_OQC_SFR_SINGLE_CL)

#include "pch.h"
#include "Sequence.h"
#include "User_Define.h"

class CSequence;
class CSeqOqcSingle : public CSequence, public CSington<CSeqOqcSingle>
{
public:
	CSeqOqcSingle();
	virtual ~CSeqOqcSingle();

public:
	void Initialize_Startup				(__in UINT nIdx, __in int nCapture = 0);
	void Finalize_Startup				(__in enMCErrCode err, __in int nIdx, __in int nCapture = 0);
	UINT StartOperation_Startup			();		// 시업점검 모드
	UINT StartOperation_Run				();

	UINT Maintence						(__in enMaintFunId id);

	void SetInspectionStep				();
	
	void ResetStepFlag					();
	void RestartStep					();
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
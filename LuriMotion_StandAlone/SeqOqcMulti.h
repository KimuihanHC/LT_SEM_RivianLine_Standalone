#pragma once

#include "pch.h"
#include "Sequence.h"

class CSeqOqcMulti : public CSequence, public CSington<CSeqOqcMulti>
{
public:
	CSeqOqcMulti();
	virtual ~CSeqOqcMulti();

public:
	void Initialize_Startup					(__in UINT nIdx, __in int nCapture = 0);
	void Finalize_Startup					(__in enMCErrCode err, __in int nIdx, __in int nCapture = 0);

	UINT StartOperation_Startup				();		// 시업점검 모드
	UINT StartOperation_Run					();

	UINT Maintence							(__in enMaintFunId id);

	void SetInspectionStep					();

	void ResetStepFlag						();
	bool ReadyToStart						();
	void RestartStep						();

	bool IsStep_Ready						();
	bool IsStep_WaitForPushSwitch			();
	bool IsStep_UnderMoveY					();
	bool IsStep_BeforePushStopSwitch		();
	bool IsStep_NotStartupModeWait			();

	enMCErrCode CheckPowerVoltage			();
	void SetPowerVoltage					();
	float GetPowerVoltage					();
};


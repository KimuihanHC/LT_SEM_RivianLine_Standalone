#pragma once

#include "IO.h"
#include "Def_ErrorCode.h"

class CMotion : public CIO
{
public:
	CMotion();
	~CMotion();

	static void MO_AllAxis_Reset		();
private:
	static bool MoveAxis				(__in int nAxis, __in double dbPos, __in double dbSpeed);
	
public:
	static enMCErrCode MO_ManualMove		(__in int nAxis, __in double dbPos);	// ui 동작시에만 사용
	static enMCErrCode MO_MoveY_Load		();		// Y Loading
	static enMCErrCode MO_MoveY_Blemish		();		// Y blemish 
	static enMCErrCode MO_MoveY_SFR			();		// Y SFR
	static enMCErrCode MO_MoveY_Intrinsic	(__in int nIndex);
	
	static enMCErrCode MO_MoveZ_Load	();		// Z Loading
	static enMCErrCode MO_MoveZ_Blemish	();		// Z Blemish
	static enMCErrCode MO_MoveZ_SFR		();		// Z SFR
	static enMCErrCode MO_MoveZ_Zero	();	
	
	static bool MO_CheckY_Load			();
	static bool MO_CheckY_Blemish		();
	static bool MO_CheckY_SFR			();
	static bool MO_CheckY_Intrinsic		(__in int nIndex);
	
	static bool MO_CheckZ_Load			();
	static bool MO_CheckZ_Blemish		();
	static bool MO_CheckZ_SFR			();
	static bool MO_CheckZ_Zero			();
	
	
	static bool MO_CheckY_Moving		();
	static bool MO_CheckZ_Moving		();
		
	int MO_GetPositionY					(__out double& dbpos);
	int MO_GetPositionZ					(__out double& dbpos);
};


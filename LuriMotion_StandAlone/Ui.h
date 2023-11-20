
#pragma once

#include "Def_ErrorCode.h"

class CUi
{
public:
	CUi();
	~CUi();

public:
	void Update_Startup_Start		(__in UINT nIdx, __in int nCapture = 0);
	void Update_Startup_End			(__in UINT nIdx, __in bool bResult);

};


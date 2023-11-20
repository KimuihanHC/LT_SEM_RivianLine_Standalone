#pragma once

#include "Def_Sequence.h"

class CSeqBase
{
public:
	CSeqBase	();
	~CSeqBase	();

	void SetStep		(__in int nStep);
	int	 GetStep		();

	DWORD GetElapTime	();

	void Error			();
	void ErrorClear		();
	bool IsError		();

	void StartUp_SetStep		(__in int nStep);
	int	 StartUp_GetStep		();

	DWORD StartUp_GetElapTime	();

	void StartUp_Error			();
	void StartUp_ErrorClear		();
	bool StartUp_IsError		();
	
private:
	int		m_nStep;
	int		m_nOldStep;
	bool	m_bError;

	DWORD	m_dwTimer;

	int		m_nStatupStep;
	int		m_nStatupOldStep;
	bool	m_bStatupError;

	DWORD	m_dwStatupTimer;
};



#include "pch.h"
#include "SeqBase.h"

#include "LuriMotion_OQA_SFR_Wide.h"
#include "LuriMotion_OQA_SFR_WideDlg.h"

#include "System_Define.h"

CSeqBase::CSeqBase()
{
	m_nStep = 0;
	m_nOldStep = -1;
	m_bError = false;

	m_nStatupStep = 0;
	m_nStatupOldStep = -1;
	m_bStatupError = false;
}

CSeqBase::~CSeqBase()
{

}

void CSeqBase::SetStep(__in int nStep)
{
	if (nStep != m_nOldStep)
	{
		CString sz;

		m_nStep = nStep;
		m_nOldStep = nStep;
		m_dwTimer = GetTickCount();

		sz.Format(_T("[Seq]%s"), g_szSeqName[nStep]);
		WRITE_SEQ_LOG(sz);
	}
}

int CSeqBase::GetStep()
{
	return m_nStep;
}

DWORD CSeqBase::GetElapTime()
{
	return GetTickCount64() - m_dwTimer;
}

void CSeqBase::Error()
{
	m_bError = true;
}

void CSeqBase::ErrorClear()
{
	m_bError = false;
}

bool CSeqBase::IsError()
{
	return m_bError;
}

void CSeqBase::StartUp_SetStep(__in int nStep)
{
	if (nStep != m_nStatupOldStep)
	{
		CString sz;

		m_nStatupStep = nStep;
		m_nStatupOldStep = nStep;
		m_dwStatupTimer = GetTickCount();

		sz.Format(_T("[StartupSeq]%s"), g_szSeqStartUpName[nStep]);
		WRITE_SEQ_LOG(sz);
	}
}

int CSeqBase::StartUp_GetStep()
{
	return m_nStatupStep;
}

DWORD CSeqBase::StartUp_GetElapTime()
{
	return GetTickCount64() - m_dwStatupTimer;
}

void CSeqBase::StartUp_Error()
{
	m_bStatupError = true;
}

void CSeqBase::StartUp_ErrorClear()
{
	m_bStatupError = false;
}

bool CSeqBase::StartUp_IsError()
{
	return m_bStatupError;
}

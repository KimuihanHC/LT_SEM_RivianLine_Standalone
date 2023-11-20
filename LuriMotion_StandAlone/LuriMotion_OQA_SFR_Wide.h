
// LuriMotion_OQA_SFR_Wide.h: PROJECT_NAME 응용 프로그램에 대한 주 헤더 파일입니다.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"		// 주 기호입니다.
#include "Log/LogSystem.h"
#include "Log/LogCommunication.h"
#include "Log/LogSeq.h"
#include "Log/LogBarcode.h"
#include "Def_CompileOption.h"

// CLuriMotion_OQA_SFR_WideApp:
// 이 클래스의 구현에 대해서는 LuriMotion_OQA_SFR_Wide.cpp을(를) 참조하세요.
//

class CLuriMotion_OQA_SFR_WideApp : public CWinApp
{
public:
	CLuriMotion_OQA_SFR_WideApp();

protected:
	CLogSystem					m_SystemLog;
	CLogCommunication			m_CommuLog;	

public:
	CLogSeq						m_SeqLog;
	CLogBarcode					m_BarcodeLog;
	
public:
	CLogSystem& GetLogSystem() { return m_SystemLog; }
	CLogCommunication& GetLogComm() { return m_CommuLog; }
	CLogBarcode& GetLogBarcode() { return m_BarcodeLog; }
	CLogSeq& GetLogSeq() { return m_SeqLog; }

// 재정의입니다.
public:
	virtual BOOL InitInstance();

// 구현입니다.

	DECLARE_MESSAGE_MAP()
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};

extern CLuriMotion_OQA_SFR_WideApp theApp;

CLogSystem& GetLogSystem();
CLogCommunication& GetLogComm();
CLogBarcode& GetLogBarcode();
CLogSeq&	GetLogSeq();
#pragma once
#include "pch.h"
#include "User_Define.h"

class CAjinDio : public CWnd, public CSington<CAjinDio>
{
	DECLARE_DYNAMIC(CAjinDio)

public:
	CAjinDio();
	virtual ~CAjinDio();

protected:
	BOOL m_bThreadRun;
	HANDLE m_hThread;       // this thread's HANDLE
	CWinThread*	m_pThread;
	static UINT	Thread_DioRead(LPVOID lpVoid);

	long m_lDIO_ModuleCnt;
	long m_lBaseOutModuleNo;

public:
	BOOL m_bInitDIO;
	DWORD m_dwInputBuf[10];
	DWORD m_dwOutputBuf[10];

	bool Start_AjinIO();
	void Terminate_DIO();

	bool GetInputState(int PCICardCount);
	bool GetOutputState(int offset);

	BOOL GetInputIoBit(long lBitNumber);
	BOOL GetOutputIoBit(long lBitNumber);
	DWORD GetOutputBit(long lBitNumber);
	void SendOutputBit(int nBitNumber, int bOnOff);
	void SendOutputBitSwap(int nBitNumber, BOOL bOnOff, int nSwapBit);
	void BlinkStart(long lOffset, long lInitState, long lmSecOn, long lmSecOff, long lCount = -1);
	void BlinkStop(long lOffset, DWORD uOnOff = 0);
	void ChangeBtnStatus(BOOL bLamp, BOOL bMode, BOOL bDoing);					//Color[Start,Stop], Mode[점멸,점등], Action[꺼짐,켜짐]
};

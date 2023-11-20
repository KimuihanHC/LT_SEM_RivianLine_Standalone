#include "pch.h"
#include "Ajin_IO.h"
#include <mutex>

std::mutex mtx_lock;

IMPLEMENT_DYNAMIC(CAjinDio, CWnd)
CAjinDio::CAjinDio(void)
{
	m_lBaseOutModuleNo = 1;
	for (int i = 0; i < 10; i++)
	{
		m_dwInputBuf[i] = 0;
		m_dwOutputBuf[i] = 0;
	}
}

CAjinDio::~CAjinDio(void)
{

}

bool CAjinDio::Start_AjinIO()
{
#ifdef _SIMUL_MODE
	m_bThreadRun = TRUE;
	m_pThread = AfxBeginThread(Thread_DioRead, this);

	m_bInitDIO = TRUE;

	return TRUE;
#else
	DWORD dwResult;
	dwResult = AxlOpen(AXL_DEFAULT_IRQNO);

	if (dwResult == AXT_RT_SUCCESS || dwResult == AXT_RT_OPEN_ALREADY)
	{
		dwResult = AxdInfoGetModuleCount(&m_lDIO_ModuleCnt);	// 먼저 IO 모듈의 갯수를 얻어오자.

		if (dwResult != AXT_RT_SUCCESS)
			return FALSE;

		// 1개도 없으면 리턴
		if (m_lDIO_ModuleCnt < 1)
			return FALSE;

		m_bThreadRun = TRUE;
		m_pThread = AfxBeginThread(Thread_DioRead, this);

		m_bInitDIO = TRUE;

		return TRUE;
	}
#endif
	return FALSE;
}

void CAjinDio::Terminate_DIO()
{
	if (!m_bInitDIO) return;

	if (m_bThreadRun)
	{
		m_bThreadRun = FALSE;
		WaitForSingleObject(m_pThread->m_hThread, INFINITE);
	}
	m_bThreadRun = NULL;

}


UINT CAjinDio::Thread_DioRead(LPVOID lpVoid)
{
	int nErrorCount = 0;

	int nModuleCnt_Input = 2;
	nModuleCnt_Input = INPUT_IO_MAX_COUNT / INPUT_IO_READ_COUNT;

	int nModuleCnt_Output = 2;
	nModuleCnt_Output = OUTPUT_IO_MAX_COUNT / OUTPUT_IO_READ_COUNT;

	AJIN_IO->m_lBaseOutModuleNo = nModuleCnt_Input;

	while (AJIN_IO->m_bThreadRun)
	{
		for (int i = 0; i < nModuleCnt_Input; i++)
		{
			if (!AJIN_IO->GetInputState(i))
			{
			}
			else
			{
				nErrorCount = 0;
			}
		}

		for (int j = 0; j < nModuleCnt_Output; j++)
		{
			if (!AJIN_IO->GetOutputState(j))
			{
			}
			else
			{
				nErrorCount = 0;
			}
		}
	}
	AJIN_IO->m_bThreadRun = FALSE;
	AJIN_IO->m_pThread = NULL;
	return 0;
}

bool CAjinDio::GetInputState(int PCICardCount)
{
	CString strLog = _T("");
	CString cs;
	long	lPortCnt = 0;

	DWORD	dwPortData;
	DWORD	dwChangeMask;

	DWORD	dwResult = NULL;
	DWORD64 dwValue = NULL;


#ifdef _SIMUL_MODE
	DWORD	dwData1;
	DWORD	dwData2;

	dwData1 = rand();
	dwData2 = rand() * 0x7FFF;
	dwPortData = dwData1 + dwData2;
	dwChangeMask = dwPortData ^ m_dwInputBuf[PCICardCount];
	if (dwChangeMask != 0x00)
	{
		m_dwInputBuf[PCICardCount] = dwPortData;
	}
	Sleep(100);
	return true;
#endif

	if (m_bInitDIO == FALSE)
	{
		return false;
	}


	// IO 읽어온다.
	dwResult = AxdiReadInportDword(PCICardCount, 0, &dwPortData);
	if (dwResult != AXT_RT_SUCCESS)
		return false;

	// IO 갯수가 맞는지 확인한다. (32점)
	dwValue |= dwPortData;
	dwResult = AxdInfoGetInputCount(PCICardCount, &lPortCnt);
	if (dwResult != AXT_RT_SUCCESS)
		return FALSE;

	dwChangeMask = dwPortData ^ m_dwInputBuf[PCICardCount];
	if (dwChangeMask != 0x00)
	{
		m_dwInputBuf[PCICardCount] = dwPortData;
	}
	Sleep(5);

	return true;
}

bool CAjinDio::GetOutputState(int offset)
{
	long	lPortCnt = 0;
	long    lModuleNo = m_lBaseOutModuleNo + offset;
	CString strLog = _T("");
	CString	cs;

	DWORD	dwPortData;
	DWORD	dwChangeMask;

	DWORD	dwResult = NULL;
	DWORD64 dwValue = NULL;

	if (m_bInitDIO == FALSE)
		return false;
	
	dwResult = AxdoReadOutportDword(lModuleNo, 0, &dwPortData);
	if (dwResult != AXT_RT_SUCCESS)
		return false;

	// IO 갯수가 맞는지 확인한다. (32점)
	dwValue |= dwPortData;
	dwResult = AxdInfoGetOutputCount(lModuleNo, &lPortCnt);
	if (dwResult != AXT_RT_SUCCESS)
		return false;

	dwChangeMask = dwPortData ^ m_dwOutputBuf[offset];
	if (dwChangeMask != 0x00)
		m_dwOutputBuf[offset] = dwPortData;
	
	Sleep(5);
	return true;
}

void CAjinDio::SendOutputBit(int nBitNumber, int bOnOff)
{
	int nBoardNo = 0;

	nBoardNo = nBitNumber / 32;

	nBitNumber = nBitNumber - (nBoardNo * 32);
	long				m_lToggleOnTimeDelay = 200;		// 토글 기능 On Time 시간 간격 (1 ~ 30000 ms)
	long				m_lToggleOffTimeDelay = 200;		// 토글 기능 Off Time 시간 간격 (1 ~ 30000 ms)
	long				m_lToggleCount = -1;		// 토글 횟수, -1의 경우 무한 토글

	AxdoWriteOutportBit((long)nBoardNo + m_lBaseOutModuleNo, nBitNumber, bOnOff);
}

void CAjinDio::SendOutputBitSwap(int nBitNumber, BOOL bOnOff, int nSwapBit)
{
	int nBoardNo = 0;

	nBoardNo = nBitNumber / 32;
	nBitNumber = nBitNumber - (nBoardNo * 32);
	nSwapBit = nSwapBit - (nBoardNo * 32);

	if (bOnOff == TRUE)
	{
		AxdoWriteOutportBit((long)nBoardNo + m_lBaseOutModuleNo, nSwapBit, !bOnOff);
		AxdoWriteOutportBit((long)nBoardNo + m_lBaseOutModuleNo, nBitNumber, bOnOff);
	}
	else
	{
		AxdoWriteOutportBit((long)nBoardNo + m_lBaseOutModuleNo, nBitNumber, bOnOff);
		AxdoWriteOutportBit((long)nBoardNo + m_lBaseOutModuleNo, nSwapBit, !bOnOff);
	}
}

void CAjinDio::BlinkStart(long lOffset, long lInitState, long lmSecOn, long lmSecOff, long lCount)
{
	long lModuleNo;
	long lBitNo;

	lModuleNo = lOffset / 32;

	lBitNo = lOffset - (lModuleNo * 32);

	AxdoToggleStart(lModuleNo + m_lBaseOutModuleNo, lBitNo, lInitState, lmSecOn, lmSecOff, lCount);
}

void CAjinDio::BlinkStop(long lOffset, DWORD uOnOff)
{
	long lModuleNo;
	long lBitNo;

	lModuleNo = lOffset / 32;

	lBitNo = lOffset - (lModuleNo * 32);

	AxdoToggleStop(lModuleNo + m_lBaseOutModuleNo, lBitNo, uOnOff);
}

void CAjinDio::ChangeBtnStatus(BOOL bLamp, BOOL bMode, BOOL bDoing)
{
	if (bLamp > 2 || bLamp < 1) return;
	if (bMode > 1 || bMode < 0) return;
	if (bDoing > 1 || bDoing < 0) return;
	AJIN_IO->BlinkStop(DIO_OUTPUT_Y0_01_START_SWITCH_LAMP, TRUE);
	AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_01_START_SWITCH_LAMP, FALSE);
	AJIN_IO->BlinkStop(DIO_OUTPUT_Y0_02_STOP_SWITCH_LAMP, TRUE);
	AJIN_IO->SendOutputBit(DIO_OUTPUT_Y0_02_STOP_SWITCH_LAMP, FALSE);
	if (bMode == 0)
		AJIN_IO->BlinkStart(bLamp, bDoing, 500, 500);
	else if (bMode == 1)
		AJIN_IO->SendOutputBit(bLamp, bDoing);
}

BOOL CAjinDio::GetInputIoBit(long lBitNumber)
{
	BOOL bFlag;
	int nBoardNo;
	int nHighLow;
	int nBitNumber;
	DWORD	dwResult = NULL;

	nBoardNo = lBitNumber / 32;

	// 	if ((lBitNumber % 32) >= 16)
	// 	{
	// 		nHighLow = 1;
	// 	}
	// 	else
	// 	{
	// 		nHighLow = 0;
	// 	}
		// [22.0104.2]
	if ((lBitNumber > 63))
	{
		nHighLow = 1;
		nBitNumber = lBitNumber - 64;
	}
	else if ((lBitNumber > 31))
	{
		nHighLow = 1;
		nBitNumber = lBitNumber - 32;
	}
	else
	{
		nHighLow = 0;
		nBitNumber = lBitNumber;
	}


	//nBitNumber = (lBitNumber % 32) /*- (16 * nHighLow)*/;

	//nBitNumber = (lBitNumber % 32) - (16 * nHighLow);


// // 	dwResult = AxdiReadInportDword(nBoardNo, 0, &dwDataBit);
// // 
// // 	for (int i = 0; i < 32; i++)
// // 	{
// // 		bFlag = ((dwDataBit & 0x0001) ^ nHighLow);
// // 		dwDataBit = dwDataBit >> 1;
// // 
// // 		if (i == (int)nBitNumber)
// // 			return bFlag;
// // 	}
// 
// 	if (nBoardNo == 0)
// 	{
// 		dwResult = AxdiReadInportDword(nBoardNo, 0, &dwDataBit);
// 	}
// 	else
// 	{
// 		AxdiReadInportWord((long)nBoardNo + 1, (long)nHighLow, &dwDataBit);
// 	}
// 	//AxdiReadInportWord((long)nBoardNo + 1, (long)nHighLow, &dwDataBit);
// 
// 
// 	for (int i = 0; i < 32; i++)
// 	{
// 		bFlag = (dwDataBit & 0x0001);
// 		dwDataBit = dwDataBit >> 1;
// 
// 		if (i == (int)lBitNumber)
// 			return bFlag;
// 	}
	int nCnt = 3;
	int nMax = 32;
	int nReal_IO_Board_Number = 0;
	// [22.0104.3]
	if (nBoardNo == 2)
		//nMax = 16;
		nMax = 32;
	else
		nMax = 32;

	for (int j = 0; j < nMax; j++)
	{
		if ((m_dwInputBuf[nBoardNo] >> j) & 0x01)
		{
			bFlag = TRUE;
		}
		else
		{
			bFlag = FALSE;
		}

		if (j == nBitNumber)
		{
			return bFlag;
		}
	}
	return bFlag;
}


BOOL CAjinDio::GetOutputIoBit(long lBitNumber)
{
	DWORD dwDataBit;
	BOOL bFlag;

	int nBoardNo;
	int nHighLow;

	nBoardNo = lBitNumber / 32;
	nHighLow = lBitNumber % 32;

	AxdoReadOutportDword((long)nBoardNo + m_lBaseOutModuleNo, 0, &dwDataBit);

	for (int i = 0; i < 32; i++)
	{
		bFlag = dwDataBit & 0x0001;
		dwDataBit = dwDataBit >> 1;

		if (i == (int)lBitNumber) return bFlag;
	}

	return bFlag;
}

DWORD CAjinDio::GetOutputBit(long lBitNumber)
{
	int nBoardNo = 0;
	DWORD dwResult = FALSE;
	nBoardNo = lBitNumber / 32;

	lBitNumber = lBitNumber - (nBoardNo * 32);
	long				m_lToggleOnTimeDelay = 200;		// 토글 기능 On Time 시간 간격 (1 ~ 30000 ms)
	long				m_lToggleOffTimeDelay = 200;		// 토글 기능 Off Time 시간 간격 (1 ~ 30000 ms)
	long				m_lToggleCount = -1;		// 토글 횟수, -1의 경우 무한 토글

	AxdoReadOutportBit((long)nBoardNo + m_lBaseOutModuleNo, lBitNumber, &dwResult);
	return dwResult;
}

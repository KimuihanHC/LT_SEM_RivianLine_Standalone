
#include "pch.h"
#include "DataManager.h"

CDataManager::CDataManager(void) : m_bInit(FALSE)
{

}


CDataManager::~CDataManager(void)
{
}


void CDataManager::Init()
{
	LoadEqBasicInfo();

	m_bInit = TRUE;
}

void CDataManager::LoadEqBasicInfo()
{
	CString strSystemFile;
	strSystemFile.Format(_T("%s\\System.ini"), _PROGRAM_ROOT_DIR);

	CFileFind FindFile;
	BOOL bRet = FindFile.FindFile(strSystemFile);
	if (!bRet)
	{
		SaveTowerLamp(strSystemFile);
		SaveEqBasicParam(strSystemFile);
	}
	else
	{
		LoadTowerLamp(strSystemFile);
		LoadEqBasicParam(strSystemFile);
	}
	
	//Motor Speed
	CString strMotionFile;
	strMotionFile.Format(_T("%s\\%s\\MotionParam.ini"), _PROGRAM_RECIPE_DIR, m_stEqBasicParam.strLastRecipe);

	bRet = FindFile.FindFile(strMotionFile);
	if (!bRet)
		SaveMotionParam(strMotionFile);
	else
		LoadMotionParam(strMotionFile);

	// Tester Comm 
	CString strTesterCommFile;
	strTesterCommFile.Format(_T("%s\\%s\\TesterComm.ini"), _PROGRAM_RECIPE_DIR, m_stEqBasicParam.strLastRecipe);

	bRet = FindFile.FindFile(strTesterCommFile);
	if (!bRet)
		SaveTesterCommData(strTesterCommFile);
	else
		LoadTesterCommData(strTesterCommFile);

	CString strTeachionFile;
	strTeachionFile.Format(_T("%s\\%s\\%s.Mod"), _PROGRAM_RECIPE_DIR, m_stEqBasicParam.strLastRecipe, m_stEqBasicParam.strLastRecipe);

	// Teaching Pos
	bRet = FindFile.FindFile(strTeachionFile);
	if (!bRet)
		SaveTeachingPosData(strTeachionFile);
	else
		LoadTeachingPosData(strTeachionFile);

	CString strErrorListFile;
	CTime t = CTime::GetCurrentTime();
	CString strDate = _T("");
	strDate.Format(_T("%04d\\%02d\\%02d"), t.GetYear(), t.GetMonth(), t.GetDay());

	strErrorListFile.Format(_T("%s\\%s\\ErrorList.txt"), _PROGRAM_ERRORLIST_DIR, strDate);

	bRet = FindFile.FindFile(strErrorListFile);
	if (!bRet)
	{
		SaveErrorList_SolutionList();
		LoadErrorList_SolutionList();
	}		
	else
		LoadErrorList_SolutionList();

	// Scan Data
	CString strScanDataFile;
	strScanDataFile.Format(_T("%s\\ScanData.ini"), _PROGRAM_RECIPE_DIR);

	bRet = FindFile.FindFile(strScanDataFile);
	if (bRet)
		LoadScanData(strScanDataFile);	
}

void CDataManager::SaveEqBasicParam(const LPCTSTR& lpszPath)
{
	CString strSystemFile;
	strSystemFile = lpszPath;

	CString strApp = _T("EQ_BASIC");
	CString strKey, strTemp;

	strKey.Format(_T("LAST_RECIPE_NAME"));
	strTemp.Format(_T("%s"), m_stEqBasicParam.strLastRecipe);
	WritePrivateProfileString(strApp, strKey, strTemp, strSystemFile);

	strKey.Format(_T("SELECT_LANGUAGE"));
	strTemp.Format(_T("%d"), m_stEqBasicParam.nLanguage);
	WritePrivateProfileString(strApp, strKey, strTemp, strSystemFile);

	strKey.Format(_T("SELECT_LIGHT"));
	strTemp.Format(_T("%d"), m_stEqBasicParam.nLightSelected);
	WritePrivateProfileString(strApp, strKey, strTemp, strSystemFile);

	// [21.1017.6] Modified
	for (int i = 0; i < MAX_USER_NUM; i++)
	{
		strKey.Format(_T("USER_ID_%02d"), i);
		strTemp.Format(_T("%s"), m_stEqBasicParam.stUserData[i].strUserID);
		WritePrivateProfileString(strApp, strKey, strTemp, strSystemFile);

		strKey.Format(_T("USER_PW_%02d"), i);
		strTemp.Format(_T("%s"), m_stEqBasicParam.stUserData[i].strPassword);
		WritePrivateProfileString(strApp, strKey, strTemp, strSystemFile);

		strKey.Format(_T("USER_PERMISSION_%02d"), i);
		strTemp.Format(_T("%d"), m_stEqBasicParam.stUserData[i].nPermission);
		WritePrivateProfileString(strApp, strKey, strTemp, strSystemFile);
	}
}

void CDataManager::LoadEqBasicParam(const LPCTSTR& lpszPath)
{
	CString strSystemFile;
	strSystemFile = lpszPath;

	const int nLength = 256;
	TCHAR strTemp[nLength];

	CString strApp = _T("EQ_BASIC");
	CString strKey;

	strKey.Format(_T("LAST_RECIPE_NAME"));
	GetPrivateProfileString(strApp, strKey, _T("Default_Recipe"), strTemp, sizeof(strTemp), strSystemFile);
	m_stEqBasicParam.strLastRecipe = (LPCTSTR)strTemp;

	strKey.Format(_T("SELECT_LANGUAGE"));
	GetPrivateProfileString(strApp, strKey, _T("0"), strTemp, sizeof(strTemp), strSystemFile);
	m_stEqBasicParam.nLanguage = _ttoi(strTemp);

	strKey.Format(_T("SELECT_LIGHT"));
	GetPrivateProfileString(strApp, strKey, _T("0"), strTemp, sizeof(strTemp), strSystemFile);
	m_stEqBasicParam.nLightSelected = _ttoi(strTemp);

	// [21.1017.7] Modified
	for (int i = 0; i < MAX_USER_NUM; i++)
	{
		strKey.Format(_T("USER_ID_%02d"), i);
		GetPrivateProfileString(strApp, strKey, _T("1"), strTemp, sizeof(strTemp), strSystemFile);
		m_stEqBasicParam.stUserData[i].strUserID = strTemp;

		strKey.Format(_T("USER_PW_%02d"), i);
		GetPrivateProfileString(strApp, strKey, _T("1"), strTemp, sizeof(strTemp), strSystemFile);
		m_stEqBasicParam.stUserData[i].strPassword = strTemp;

		strKey.Format(_T("USER_PERMISSION_%02d"), i);
		GetPrivateProfileString(strApp, strKey, _T("0"), strTemp, sizeof(strTemp), strSystemFile);
		m_stEqBasicParam.stUserData[i].nPermission = _ttoi(strTemp);
	}
}

EQ_BASIC_PARAM* CDataManager::GetCurrentEqBasicData()
{
	if (!m_bInit)
	{
		AfxMessageBox(_T("should be checked for Init CDataManager!!!"), MB_OK);
		return FALSE;
	}

	return &m_stEqBasicParam;
}

void CDataManager::SetEqData(const EQ_BASIC_PARAM& stEqParam)
{
	CString strSystemFile;
	strSystemFile.Format(_T("%s\\System.ini"), _PROGRAM_ROOT_DIR);

	m_stEqBasicParam = stEqParam;
	SaveEqBasicParam(strSystemFile);
}


//===========================================================================================================
// TOWER LAMP
void CDataManager::SaveTowerLamp(const LPCTSTR& lpszPath)
{
	CString strSystemFile;
	strSystemFile = lpszPath;

	CString strApp = _T("TOWER_LAMP");
	CString strKey, strTemp;

	for (int i = 0; i < (int)eModeMax; i++)
	{
		for (int j = 0; j < (int)eTowerMax; j++)
		{
			strKey.Format(_T("TOWER_LAMP_%02d_%02d"), i,j);
			strTemp.Format(_T("%d"), m_stTowerLamp.nTowerLampSetting[i][j]);
			WritePrivateProfileString(strApp, strKey, strTemp, strSystemFile);
		}
	}
}
void CDataManager::LoadTowerLamp(const LPCTSTR& lpszPath)
{
	CString strSystemFile;
	strSystemFile = lpszPath;

	const int nLength = 256;
	TCHAR strTemp[nLength];

	CString strApp = _T("TOWER_LAMP");
	CString strKey;

	for (int i = 0; i < (int)eModeMax; i++)
	{
		for (int j = 0; j < (int)eTowerMax; j++)
		{
			strKey.Format(_T("TOWER_LAMP_%02d_%02d"), i, j);
			GetPrivateProfileString(strApp, strKey, _T("1"), strTemp, sizeof(strTemp), strSystemFile);
			m_stTowerLamp.nTowerLampSetting[i][j] = _ttoi(strTemp);
		}
	}
}

EQ_TOWERLAMP* CDataManager::GetCurrentTowerLampData()
{
	if (!m_bInit)
	{
		AfxMessageBox(_T("should be checked for Init CDataManager!!!"), MB_OK);
		return FALSE;
	}

	return &m_stTowerLamp;
}
void CDataManager::SetstTower(const EQ_TOWERLAMP& stTowerLamp)
{
	CString strSystemFile;
	strSystemFile.Format(_T("%s\\System.ini"), _PROGRAM_ROOT_DIR);

	m_stTowerLamp = stTowerLamp;
	SaveTowerLamp(strSystemFile);
}


//===========================================================================================================
// TESTER CONNECT 
void CDataManager::SaveTesterCommData(const LPCTSTR& lpszPath)
{
	CString strTesterFile;
	strTesterFile = lpszPath;

	CString strApp = _T("TESTER_CONNECT");
	CString strKey, strTemp, strTotal;

	for (int i = 0; i < MAX_WAITCOMM_NUM; i++)
	{
		strKey.Format(_T("COMM_WATITIME_COLUMN%02d"), i);
		strTemp.Format(_T("%s"), m_stTester.strWaitTimeLIst[i]);
		WritePrivateProfileString(strApp, strKey, strTemp, strTesterFile);
	}

	strKey.Format(_T("SLEEPMODE_WATITIME"));
	strTemp.Format(_T("%d"), m_stTester.nSleepModeTimer);
	WritePrivateProfileString(strApp, strKey, strTemp, strTesterFile);

	for (int i = 0; i < 5; i++)
	{
		strKey.Format(_T("TESTER_IP_%02d"), i);
		strTemp.Format(_T("%s"), m_stTester.strTesterConnect[i]);
		WritePrivateProfileString(strApp, strKey, strTemp, strTesterFile);

		strKey.Format(_T("TESTER_IP_DWORD_%02d"), i);
		strTemp.Format(_T("%u"), m_stTester.dwTesterIP[i]);
		WritePrivateProfileString(strApp, strKey, strTemp, strTesterFile);

		strKey.Format(_T("TESTER_PORT_%02d"), i);
		strTemp.Format(_T("%d"), m_stTester.nTesterPort[i]);
		WritePrivateProfileString(strApp, strKey, strTemp, strTesterFile);

		strKey.Format(_T("TESTER_USE_%02d"), i);
		strTemp.Format(_T("%d"), m_stTester.nUseMode[i]);
		WritePrivateProfileString(strApp, strKey, strTemp, strTesterFile);

		strKey.Format(_T("TESTER_SERVERMODE_%02d"), i);
		strTemp.Format(_T("%d"), m_stTester.nServerMode[i]);
		WritePrivateProfileString(strApp, strKey, strTemp, strTesterFile);
	}

	for (int i = 0; i < 3; i++)
	{
		strKey.Format(_T("LIGHT_%02d_PORT"), i);
		strTemp.Format(_T("%d"), m_stTester.nPort[i]);
		WritePrivateProfileString(strApp, strKey, strTemp, strTesterFile);

		strKey.Format(_T("LIGHT_%02d_BAUD"), i);
		strTemp.Format(_T("%d"), m_stTester.nBaudRate[i]);
		WritePrivateProfileString(strApp, strKey, strTemp, strTesterFile);

		strKey.Format(_T("LIGHT_%02d_DATA"), i);
		strTemp.Format(_T("%d"), m_stTester.nDataBit[i]);
		WritePrivateProfileString(strApp, strKey, strTemp, strTesterFile);

		strKey.Format(_T("LIGHT_%02d_STOP"), i);
		strTemp.Format(_T("%d"), m_stTester.nStopBit[i]);
		WritePrivateProfileString(strApp, strKey, strTemp, strTesterFile);

		strKey.Format(_T("LIGHT_%02d_PARITY"), i);
		strTemp.Format(_T("%d"), m_stTester.nParityBit[i]);
		WritePrivateProfileString(strApp, strKey, strTemp, strTesterFile);
	}

	for (int i = 0; i < 1; i++)
	{
		for (int j = 0; j < 13; j++)
		{
			// CAT 1
			strKey.Format(_T("LIGHT_%02d_CAT1_%02d_VOLTAGE_LURI"), i, j);
			strTemp.Format(_T("%d"), m_stTester.stLightData[i].nVoltage[0][j]);
			WritePrivateProfileString(strApp, strKey, strTemp, strTesterFile);
		}

		for (int j = 0; j < 13; j++)
		{
			// CAT 1
			strKey.Format(_T("LIGHT_%02d_CAT1_%02d_CURRENT_LURI"), i, j);
			strTemp.Format(_T("%d"), m_stTester.stLightData[i].nCurrent[0][j]);
			WritePrivateProfileString(strApp, strKey, strTemp, strTesterFile);
		}

		for (int j = 0; j < 13; j++)
		{
			// CAT 2
			strKey.Format(_T("LIGHT_%02d_CAT2_%02d_VOLTAGE_LURI"), i, j);
			strTemp.Format(_T("%d"), m_stTester.stLightData[i].nVoltage[1][j]);
			WritePrivateProfileString(strApp, strKey, strTemp, strTesterFile);
		}

		for (int j = 0; j < 13; j++)
		{
			// CAT 2
			strKey.Format(_T("LIGHT_%02d_CAT2_%02d_CURRENT_LURI"), i, j);
			strTemp.Format(_T("%d"), m_stTester.stLightData[i].nCurrent[1][j]);
			WritePrivateProfileString(strApp, strKey, strTemp, strTesterFile);
		}
	}

	// POWER SUPPLY
	strKey.Format(_T("POWER_SUPPLY_PORT"));
	strTemp.Format(_T("%d"), m_stTester.nPort[1]);
	WritePrivateProfileString(strApp, strKey, strTemp, strTesterFile);

	strKey.Format(_T("POWER_SUPPLY_BAUD"));
	strTemp.Format(_T("%d"), m_stTester.nBaudRate[1]);
	WritePrivateProfileString(strApp, strKey, strTemp, strTesterFile);

	strKey.Format(_T("POWER_SUPPLY_DATA"));
	strTemp.Format(_T("%d"), m_stTester.nDataBit[1]);
	WritePrivateProfileString(strApp, strKey, strTemp, strTesterFile);

	strKey.Format(_T("POWER_SUPPLY_STOP"));
	strTemp.Format(_T("%d"), m_stTester.nStopBit[1]);
	WritePrivateProfileString(strApp, strKey, strTemp, strTesterFile);

	strKey.Format(_T("POWER_SUPPLY_PARITY"));
	strTemp.Format(_T("%d"), m_stTester.nParityBit[1]);
	WritePrivateProfileString(strApp, strKey, strTemp, strTesterFile);

	// HANDY BCR
	strKey.Format(_T("HANDY_BCR_PORT"));
	strTemp.Format(_T("%d"), m_stTester.nPort[2]);
	WritePrivateProfileString(strApp, strKey, strTemp, strTesterFile);

	strKey.Format(_T("HANDY_BCR_BAUD"));
	strTemp.Format(_T("%d"), m_stTester.nBaudRate[2]);
	WritePrivateProfileString(strApp, strKey, strTemp, strTesterFile);

	strKey.Format(_T("HANDY_BCR_DATA"));
	strTemp.Format(_T("%d"), m_stTester.nDataBit[2]);
	WritePrivateProfileString(strApp, strKey, strTemp, strTesterFile);

	strKey.Format(_T("HANDY_BCR_STOP"));
	strTemp.Format(_T("%d"), m_stTester.nStopBit[2]);
	WritePrivateProfileString(strApp, strKey, strTemp, strTesterFile);

	strKey.Format(_T("HANDY_BCR_PARITY"));
	strTemp.Format(_T("%d"), m_stTester.nParityBit[2]);
	WritePrivateProfileString(strApp, strKey, strTemp, strTesterFile);

	// Tester Name
	for (int i = 0; i < MAX_TESTER_NUM; i++)
	{
		strKey.Format(_T("TESTER_NAME_%02d"), i);
		strTemp.Format(_T("%s"), m_stTester.strSemcoVer[i]);
		WritePrivateProfileString(strApp, strKey, strTemp, strTesterFile);
	}

	// 전압 & 전류
	strKey.Format(_T("POWER_VOLT"));
	strTemp.Format(_T("%.1f"), m_stTester.fVolt);
	WritePrivateProfileString(strApp, strKey, strTemp, strTesterFile);

	strKey.Format(_T("POWER_CURRENT"));
	strTemp.Format(_T("%d"), m_stTester.nCurrent);
	WritePrivateProfileString(strApp, strKey, strTemp, strTesterFile);
}

void CDataManager::LoadTesterCommData(const LPCTSTR& lpszPath)
{
	CString strTesterCommFile;
	strTesterCommFile = lpszPath;

	const int nLength = 256;
	TCHAR strTemp[nLength];

	CString strApp = _T("TESTER_CONNECT");
	CString strKey;

	TCHAR cName[MAX_PATH] = { 0, };
	TCHAR cExt[MAX_PATH] = { 0, };

	for (int i = 0; i < MAX_WAITCOMM_NUM; i++)
	{
		strKey.Format(_T("COMM_WATITIME_COLUMN%02d"), i);
		GetPrivateProfileString(strApp, strKey, _T("1"), strTemp, sizeof(strTemp), strTesterCommFile);
		m_stTester.strWaitTimeLIst[i] = strTemp;
	}

	strKey.Format(_T("SLEEPMODE_WATITIME"));
	GetPrivateProfileString(strApp, strKey, _T("1"), strTemp, sizeof(strTemp), strTesterCommFile);
	m_stTester.nSleepModeTimer = _ttoi(strTemp);

	for (int i = 0; i < 5; i++)
	{
		strKey.Format(_T("TESTER_IP_%02d"), i);
		GetPrivateProfileString(strApp, strKey, _T("1"), strTemp, sizeof(strTemp), strTesterCommFile);
		m_stTester.strTesterConnect[i] = strTemp;

		strKey.Format(_T("TESTER_IP_DWORD_%02d"), i);
		GetPrivateProfileString(strApp, strKey, _T("1"), strTemp, sizeof(strTemp), strTesterCommFile);
		m_stTester.dwTesterIP[i] = (DWORD)_ttof(strTemp);

		strKey.Format(_T("TESTER_PORT_%02d"), i);
		GetPrivateProfileString(strApp, strKey, _T("1"), strTemp, sizeof(strTemp), strTesterCommFile);
		m_stTester.nTesterPort[i] = _ttoi(strTemp);

		strKey.Format(_T("TESTER_USE_%02d"), i);
		GetPrivateProfileString(strApp, strKey, _T("1"), strTemp, sizeof(strTemp), strTesterCommFile);
		m_stTester.nUseMode[i] = _ttoi(strTemp);

		strKey.Format(_T("TESTER_SERVERMODE_%02d"), i);
		GetPrivateProfileString(strApp, strKey, _T("0"), strTemp, sizeof(strTemp), strTesterCommFile);
		m_stTester.nServerMode[i] = _ttoi(strTemp);
	}

	for (int i = 0; i < 1; i++)
	{
		strKey.Format(_T("LIGHT_%02d_PORT"), i);
		GetPrivateProfileString(strApp, strKey, _T("1"), strTemp, sizeof(strTemp), strTesterCommFile);
		m_stTester.nPort[i] = _ttoi(strTemp);

		strKey.Format(_T("LIGHT_%02d_BAUD"), i);
		GetPrivateProfileString(strApp, strKey, _T("9600"), strTemp, sizeof(strTemp), strTesterCommFile);
		m_stTester.nBaudRate[i] = _ttoi(strTemp);

		strKey.Format(_T("LIGHT_%02d_DATA"), i);
		GetPrivateProfileString(strApp, strKey, _T("8"), strTemp, sizeof(strTemp), strTesterCommFile);
		m_stTester.nDataBit[i] = _ttoi(strTemp);

		strKey.Format(_T("LIGHT_%02d_STOP"), i);
		GetPrivateProfileString(strApp, strKey, _T("0"), strTemp, sizeof(strTemp), strTesterCommFile);
		m_stTester.nStopBit[i] = _ttoi(strTemp);

		strKey.Format(_T("LIGHT_%02d_PARITY"), i);
		GetPrivateProfileString(strApp, strKey, _T("1"), strTemp, sizeof(strTemp), strTesterCommFile);
		m_stTester.nParityBit[i] = _ttoi(strTemp);

		for (int j = 0; j < 13; j++)
		{
			// CATE 1
			strKey.Format(_T("LIGHT_%02d_CAT1_%02d_VOLTAGE_LURI"), i, j);
			GetPrivateProfileString(strApp, strKey, _T("120"), strTemp, sizeof(strTemp), strTesterCommFile);
			m_stTester.stLightData[i].nVoltage[0][j] = _ttoi(strTemp);

			strKey.Format(_T("LIGHT_%02d_CAT1_%02d_CURRENT_LURI"), i, j);
			GetPrivateProfileString(strApp, strKey, _T("100"), strTemp, sizeof(strTemp), strTesterCommFile);
			m_stTester.stLightData[i].nCurrent[0][j] = _ttoi(strTemp);

			// CATE 2
			strKey.Format(_T("LIGHT_%02d_CAT2_%02d_VOLTAGE_LURI"), i, j);
			GetPrivateProfileString(strApp, strKey, _T("120"), strTemp, sizeof(strTemp), strTesterCommFile);
			m_stTester.stLightData[i].nVoltage[1][j] = _ttoi(strTemp);

			strKey.Format(_T("LIGHT_%02d_CAT2_%02d_CURRENT_LURI"), i, j);
			GetPrivateProfileString(strApp, strKey, _T("100"), strTemp, sizeof(strTemp), strTesterCommFile);
			m_stTester.stLightData[i].nCurrent[1][j] = _ttoi(strTemp);
		}
	}

	strKey.Format(_T("POWER_SUPPLY_PORT"));
	GetPrivateProfileString(strApp, strKey, _T("1"), strTemp, sizeof(strTemp), strTesterCommFile);
	m_stTester.nPort[1] = _ttoi(strTemp);

	strKey.Format(_T("POWER_SUPPLY_BAUD"));
	GetPrivateProfileString(strApp, strKey, _T("9600"), strTemp, sizeof(strTemp), strTesterCommFile);
	m_stTester.nBaudRate[1] = _ttoi(strTemp);

	strKey.Format(_T("POWER_SUPPLY_DATA"));
	GetPrivateProfileString(strApp, strKey, _T("8"), strTemp, sizeof(strTemp), strTesterCommFile);
	m_stTester.nDataBit[1] = _ttoi(strTemp);

	strKey.Format(_T("POWER_SUPPLY_STOP"));
	GetPrivateProfileString(strApp, strKey, _T("0"), strTemp, sizeof(strTemp), strTesterCommFile);
	m_stTester.nStopBit[1] = _ttoi(strTemp);

	strKey.Format(_T("POWER_SUPPLY_PARITY"));
	GetPrivateProfileString(strApp, strKey, _T("1"), strTemp, sizeof(strTemp), strTesterCommFile);
	m_stTester.nParityBit[1] = _ttoi(strTemp);

	// HANDY BCR
	strKey.Format(_T("HANDY_BCR_PORT"));
	GetPrivateProfileString(strApp, strKey, _T("1"), strTemp, sizeof(strTemp), strTesterCommFile);
	m_stTester.nPort[2] = _ttoi(strTemp);

	strKey.Format(_T("HANDY_BCR_BAUD"));
	GetPrivateProfileString(strApp, strKey, _T("9600"), strTemp, sizeof(strTemp), strTesterCommFile);
	m_stTester.nBaudRate[2] = _ttoi(strTemp);

	strKey.Format(_T("HANDY_BCR_DATA"));
	GetPrivateProfileString(strApp, strKey, _T("8"), strTemp, sizeof(strTemp), strTesterCommFile);
	m_stTester.nDataBit[2] = _ttoi(strTemp);

	strKey.Format(_T("HANDY_BCR_STOP"));
	GetPrivateProfileString(strApp, strKey, _T("0"), strTemp, sizeof(strTemp), strTesterCommFile);
	m_stTester.nStopBit[2] = _ttoi(strTemp);

	strKey.Format(_T("HANDY_BCR_PARITY"));
	GetPrivateProfileString(strApp, strKey, _T("1"), strTemp, sizeof(strTemp), strTesterCommFile);
	m_stTester.nParityBit[2] = _ttoi(strTemp);

	// Tester Name
	for (int i = 0; i < MAX_TESTER_NUM; i++)
	{
		strKey.Format(_T("TESTER_NAME_%02d"), i);
		GetPrivateProfileString(strApp, strKey, _T("1"), strTemp, sizeof(strTemp), strTesterCommFile);
		m_stTester.strSemcoVer[i] = strTemp;
		_wsplitpath(strTemp, NULL, NULL, cName, cExt);
		m_stTester.strName[i].Format(_T("%s%s"), cName, cExt);
	}

	// 전압 & 전류
	strKey.Format(_T("POWER_VOLT"));
	GetPrivateProfileString(strApp, strKey, _T("12.0"), strTemp, sizeof(strTemp), strTesterCommFile);
	m_stTester.fVolt = _tstof(strTemp);

	strKey.Format(_T("POWER_CURRENT"));
	GetPrivateProfileString(strApp, strKey, _T("1"), strTemp, sizeof(strTemp), strTesterCommFile);
	m_stTester.nCurrent = _ttoi(strTemp);
}

TESTER_COMM* CDataManager::GetCurrentTesterCommData()
{
	if (!m_bInit)
	{
		AfxMessageBox(_T("should be checked for Init CDataManager!!!"), MB_OK);
		return FALSE;
	}

	return &m_stTester;
}
void CDataManager::SetTesterCommData(const TESTER_COMM& stTester)
{
	CString strTesterFile;
	strTesterFile.Format(_T("%s\\%s\\TesterComm.ini"), _PROGRAM_RECIPE_DIR, m_stEqBasicParam.strLastRecipe);

	m_stTester = stTester;
	SaveTesterCommData(strTesterFile);
}


//===========================================================================================================
// MOTION SPEED
void CDataManager::SaveMotionParam(const LPCTSTR& lpszPath)
{
	CString strMotionFile;
	strMotionFile = lpszPath;

	CString strApp = _T("MOTION_PARAM");
	CString strKey, strTemp, strTotal;

	for (int i = 0; i < 1; i++)
	{
		strKey.Format(_T("MOTOR_%02d_COMPORT"), i);
		strTemp.Format(_T("%d"), m_stMotion.nPort[i]);
		WritePrivateProfileString(strApp, strKey, strTemp, strMotionFile);

		strKey.Format(_T("MOTOR_%02d_BAUDRATE"), i);
		strTemp.Format(_T("%d"), m_stMotion.nBaudRate[i]);
		WritePrivateProfileString(strApp, strKey, strTemp, strMotionFile);
	}

	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < (int)eSpeedMax; j++)
		{
			strKey.Format(_T("MOTION_SPEED_%02d_%02d"), i, j);
			strTemp.Format(_T("%d"), m_stMotion.nMotorSpeed[i][j]);
			WritePrivateProfileString(strApp, strKey, strTemp, strMotionFile);
		}
	}

	strKey.Format(_T("CHANGE_NEW"));
	strTemp.Format(_T("%s"), m_stMotion.strChangeTime_New);
	WritePrivateProfileString(strApp, strKey, strTemp, strMotionFile);

	strKey.Format(_T("CHANGE_LAST"));
	strTemp.Format(_T("%s"), m_stMotion.strChangeTime_Last);
	WritePrivateProfileString(strApp, strKey, strTemp, strMotionFile);

	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < (int)eSpeedMax; j++)
		{
			if (m_stMotion.strDiff[i][j] != _T(""))
			{
				strTemp.Format(_T("[%d][%d] : [%s], "), i, j, m_stMotion.strDiff[i][j]);
				strTotal = strTotal + strTemp;
			}			
		}
	}
	strKey.Format(_T("CHANGE_PARAMETER"));
	WritePrivateProfileString(strApp, strKey, strTotal, strMotionFile);
}
void CDataManager::LoadMotionParam(const LPCTSTR& lpszPath)
{
	CString strMotionFile;
	strMotionFile = lpszPath;

	const int nLength = 256;
	TCHAR strTemp[nLength];

	CString strApp = _T("MOTION_PARAM");
	CString strKey;

	for (int i = 0; i < 1; i++)
	{
		strKey.Format(_T("MOTOR_%02d_COMPORT"), i);
		GetPrivateProfileString(strApp, strKey, _T("1"), strTemp, sizeof(strTemp), strMotionFile);
		m_stMotion.nPort[i] = _ttoi(strTemp);


		strKey.Format(_T("MOTOR_%02d_BAUDRATE"), i);
		GetPrivateProfileString(strApp, strKey, _T("1"), strTemp, sizeof(strTemp), strMotionFile);
		m_stMotion.nBaudRate[i] = _ttoi(strTemp);
	}

	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < (int)eSpeedMax; j++)
		{
			strKey.Format(_T("MOTION_SPEED_%02d_%02d"), i, j);
			GetPrivateProfileString(strApp, strKey, _T("1"), strTemp, sizeof(strTemp), strMotionFile);
			m_stMotion.nMotorSpeed[i][j] = _ttoi(strTemp);
		}
	}

	strKey.Format(_T("CHANGE_NEW"));
	GetPrivateProfileString(strApp, strKey, _T("1"), strTemp, sizeof(strTemp), strMotionFile);
	m_stMotion.strChangeTime_New = strTemp;

	strKey.Format(_T("CHANGE_LAST"));
	GetPrivateProfileString(strApp, strKey, _T("1"), strTemp, sizeof(strTemp), strMotionFile);
	m_stMotion.strChangeTime_Last = strTemp;
}
MOTION_PARAM*	CDataManager::GetCurrentMotionParam()
{
	if (!m_bInit)
	{
		AfxMessageBox(_T("should be checked for Init CDataManager!!!"), MB_OK);
		return FALSE;
	}

	return &m_stMotion;
}
void CDataManager::SetMotionParam(const MOTION_PARAM& stMotion)
{
	CString strMotionFile;
	strMotionFile.Format(_T("%s\\%s\\MotionParam.ini"), _PROGRAM_RECIPE_DIR, m_stEqBasicParam.strLastRecipe);

	m_stMotion = stMotion;
	SaveMotionParam(strMotionFile);
}


//===========================================================================================================
// Teaching
void CDataManager::SaveTeachingPosData(const LPCTSTR& lpszPath)
{
	CString strTeachingFile;
	strTeachingFile = lpszPath;

	CString strApp = _T("TACHING_PARAM");
	CString strKey, strTemp, strTotal;

	//모터 Y
	strKey.Format(_T("AXIS_SOCKET_Y_WAIT_POS"));
	strTemp.Format(_T("%d"), m_stTeach.nSocketY_Wait_Pos);
	WritePrivateProfileString(strApp, strKey, strTemp, strTeachingFile);

	strKey.Format(_T("AXIS_SOCKET_Y_BLEMISHI_POS"));
	strTemp.Format(_T("%d"), m_stTeach.nSocketY_Blemishi_Pos);
	WritePrivateProfileString(strApp, strKey, strTemp, strTeachingFile);

	strKey.Format(_T("AXIS_SOCKET_Y_SFR_POS"));
	strTemp.Format(_T("%d"), m_stTeach.nSocketY_SFR_Pos);
	WritePrivateProfileString(strApp, strKey, strTemp, strTeachingFile);

	for (int i = 0; i < 5; i++)
	{
		strKey.Format(_T("AXIS_SOCKET_Y_INTRINSIC_%d_POS"), i);
		strTemp.Format(_T("%d"), m_stTeach.nSocketY_Intrinsic_Pos[i]);
		WritePrivateProfileString(strApp, strKey, strTemp, strTeachingFile);
	}

	//모터 Z
	strKey.Format(_T("AXIS_SOCKET_Z_WAIT_POS"));
	strTemp.Format(_T("%d"), m_stTeach.nSocketZ_Wait_Pos);
	WritePrivateProfileString(strApp, strKey, strTemp, strTeachingFile);

	strKey.Format(_T("AXIS_SOCKET_Z_BLEMISHI_POS"));
	strTemp.Format(_T("%d"), m_stTeach.nSocketZ_Blemishi_Pos);
	WritePrivateProfileString(strApp, strKey, strTemp, strTeachingFile);

	strKey.Format(_T("AXIS_SOCKET_Z_SFR_180_POS"));
	strTemp.Format(_T("%d"), m_stTeach.nSocketZ_SFR_180_Pos);
	WritePrivateProfileString(strApp, strKey, strTemp, strTeachingFile);

	strKey.Format(_T("AXIS_SOCKET_Z_SFR_825_POS"));
	strTemp.Format(_T("%d"), m_stTeach.nSocketZ_SFR_825_Pos);
	WritePrivateProfileString(strApp, strKey, strTemp, strTeachingFile);

	strKey.Format(_T("POS_CHANGE_NEW"));
	strTemp.Format(_T("%s"), m_stTeach.strChangeTime_New_Pos);
	WritePrivateProfileString(strApp, strKey, strTemp, strTeachingFile);

	strKey.Format(_T("POS_CHANGE_LAST"));
	strTemp.Format(_T("%s"), m_stTeach.strChangeTime_Last_Pos);
	WritePrivateProfileString(strApp, strKey, strTemp, strTeachingFile);

	strKey.Format(_T("MANUAL_MODE_AUTO_CHANGE"));
	strTemp.Format(_T("%d"), m_stTeach.nChangeMode_Time);
	WritePrivateProfileString(strApp, strKey, strTemp, strTeachingFile);
}

void CDataManager::LoadTeachingPosData(const LPCTSTR& lpszPath)
{
	CString strTeachingFile;
	strTeachingFile = lpszPath;

	const int nLength = 256;
	TCHAR strTemp[nLength];

	CString strApp = _T("TACHING_PARAM");
	CString strKey;

	//모터 Y
	strKey.Format(_T("AXIS_SOCKET_Y_WAIT_POS"));
	GetPrivateProfileString(strApp, strKey, _T("0"), strTemp, sizeof(strTemp), strTeachingFile);
	m_stTeach.nSocketY_Wait_Pos = _ttoi(strTemp);

	strKey.Format(_T("AXIS_SOCKET_Y_BLEMISHI_POS"));
	GetPrivateProfileString(strApp, strKey, _T("0"), strTemp, sizeof(strTemp), strTeachingFile);
	m_stTeach.nSocketY_Blemishi_Pos = _ttoi(strTemp);

	strKey.Format(_T("AXIS_SOCKET_Y_SFR_POS"));
	GetPrivateProfileString(strApp, strKey, _T("0"), strTemp, sizeof(strTemp), strTeachingFile);
	m_stTeach.nSocketY_SFR_Pos = _ttoi(strTemp);

	for (int i = 0; i < 5; i++)
	{
		strKey.Format(_T("AXIS_SOCKET_Y_INTRINSIC_%d_POS"), i);
		GetPrivateProfileString(strApp, strKey, _T("0"), strTemp, sizeof(strTemp), strTeachingFile);
		m_stTeach.nSocketY_Intrinsic_Pos[i] = _ttoi(strTemp);
	}

	//모터 Z
	strKey.Format(_T("AXIS_SOCKET_Z_WAIT_POS"));
	GetPrivateProfileString(strApp, strKey, _T("0"), strTemp, sizeof(strTemp), strTeachingFile);
	m_stTeach.nSocketZ_Wait_Pos = _ttoi(strTemp);

	strKey.Format(_T("AXIS_SOCKET_Z_BLEMISHI_POS"));
	GetPrivateProfileString(strApp, strKey, _T("0"), strTemp, sizeof(strTemp), strTeachingFile);
	m_stTeach.nSocketZ_Blemishi_Pos = _ttoi(strTemp);

	strKey.Format(_T("AXIS_SOCKET_Z_SFR_180_POS"));
	GetPrivateProfileString(strApp, strKey, _T("0"), strTemp, sizeof(strTemp), strTeachingFile);
	m_stTeach.nSocketZ_SFR_180_Pos = _ttoi(strTemp);

	strKey.Format(_T("AXIS_SOCKET_Z_SFR_825_POS"));
	GetPrivateProfileString(strApp, strKey, _T("0"), strTemp, sizeof(strTemp), strTeachingFile);
	m_stTeach.nSocketZ_SFR_825_Pos = _ttoi(strTemp);

	strKey.Format(_T("POS_CHANGE_NEW"));
	GetPrivateProfileString(strApp, strKey, _T("1"), strTemp, sizeof(strTemp), strTeachingFile);
	m_stTeach.strChangeTime_New_Pos = strTemp;

	strKey.Format(_T("POS_CHANGE_LAST"));
	GetPrivateProfileString(strApp, strKey, _T("1"), strTemp, sizeof(strTemp), strTeachingFile);
	m_stTeach.strChangeTime_Last_Pos = strTemp;
	
	strKey.Format(_T("MANUAL_MODE_AUTO_CHANGE"));
	GetPrivateProfileString(strApp, strKey, _T("1"), strTemp, sizeof(strTemp), strTeachingFile);
	m_stTeach.nChangeMode_Time = _ttoi(strTemp);

}
TEACHING_PARAM*	CDataManager::GetCurrentTeachingData()
{
	if (!m_bInit)
	{
		AfxMessageBox(_T("should be checked for Init CDataManager!!!"), MB_OK);
		return FALSE;
	}

	return &m_stTeach;
}
void CDataManager::SetTeachingData(const TEACHING_PARAM& stTeaching)
{
	EQ_BASIC_PARAM stEq = *DATAMANAGER->GetCurrentEqBasicData();

	CString strPath = _T("C:\\Exec_Rivian\\Recipe");
	CString strFullPath = _T("");

	CString strFolder = _T("");
	strFolder = strPath + _T("\\") + m_stEqBasicParam.strLastRecipe;

	CreateDirectory(strPath, NULL);
	CreateDirectory(strFolder, NULL);
	
	strFullPath.Format(_T("%s\\%s\\%s.Mod"), strPath, m_stEqBasicParam.strLastRecipe, m_stEqBasicParam.strLastRecipe);

	m_stTeach = stTeaching;
	SaveTeachingPosData(strFullPath);
}

void CDataManager::RecipeChange(CString& strRecipeName)
{
	CString strPath = _T("C:\\Exec_Rivian\\Recipe\\");
	CString strFullPath = _T("");

	m_strCurrenctRecipeName = strRecipeName;
	strFullPath.Format(_T("%s%s\\%s.Mod"), strPath, strRecipeName, strRecipeName);

	// TEaching Data
	LoadTeachingPosData(strFullPath);


	// Comm Data
	CString strTesterCommFile;
	strTesterCommFile.Format(_T("%s%s\\TesterComm.ini"), strPath, strRecipeName);
	LoadTesterCommData(strTesterCommFile);
}


void CDataManager::SetSaveAs_TeachingData(const TEACHING_PARAM& stTeaching, CString strPathAndName)
{
	m_stTeach = stTeaching;
	SaveTeachingPosData(strPathAndName);
}
void CDataManager::SetSaveAs_MotionParam(const MOTION_PARAM& stMotion, CString strstrPathAndNamePath)
{
	m_stMotion = stMotion;
	SaveMotionParam(strstrPathAndNamePath);
}
void CDataManager::SetSaveAs_TesterCommParam(const TESTER_COMM& stTester, CString strPathAndName)
{
	m_stTester = stTester;
	SaveTesterCommData(strPathAndName);
}


void CDataManager::LoadErrorList_SolutionList()
{
	CTime t = CTime::GetCurrentTime();
	CString strDate = _T("");
	strDate.Format(_T("%04d\\%02d\\%02d"), t.GetYear(), t.GetMonth(), t.GetDay());

	CString strErrorListFile;
	strErrorListFile.Format(_T("%s\\%s\\ErrorList.txt"), _PROGRAM_ERRORLIST_DIR, strDate);

	const int nLength = 256;
	TCHAR strTemp[nLength];

	CString strApp = _T("ERROR_LIST");
	CString strKey;

	strKey.Format(_T("TOTAL_ERROR_COUNT"));
	GetPrivateProfileString(strApp, strKey, _T("0"), strTemp, sizeof(strTemp), strErrorListFile);
	m_stError.nErrorCount = _ttoi(strTemp);

	// 에러카운트가 없으면 호출할 데이터가 없으므로.. 리턴하자.
	if (m_stError.nErrorCount != 0)
	{
		// 발생한 날짜의 에러를 받아오자.
		for (int i = 0; i < m_stError.nErrorCount; i++)
		{
			strKey.Format(_T("%03d_ErrNum"), i + 1);			// 에러번호 입력을 1번부터 한다.
			GetPrivateProfileString(strApp, strKey, _T("1000"), strTemp, sizeof(strTemp), strErrorListFile);
			m_stError.vtErrorNum.push_back(strTemp);
		}
	}

	// 에러명리스트를 받아오자.
	CString strLanguageFilePath = _T("");
	CString strList = _T("");
	CString strGetKey = _T("");
	strLanguageFilePath.Format(_T("%s\\Language\\Language.txt"), _PROGRAM_ROOT_DIR);


	int nStartErrorNum_Part1 = 100;
	int nStartErrorNum_Part2 = 200;
	int nStartErrorNum_Part3 = 300;
	int nRepeatCnt = 0;

	m_stError.vtErr_Kor.clear();
	m_stError.vtErr_Eng.clear();
	m_stError.vtErr_Viet.clear();

	m_stError.vtSol_Kor.clear();
	m_stError.vtSol_Eng.clear();
	m_stError.vtSol_Viet.clear();

	if (m_ctrlLanguageFile.Open(strLanguageFilePath))
	{
		// 100번대 읽어오기
		strGetKey.Format(_T("LANGUAGE_100"));
		while (1)
		{
			// KOR 읽어오기
			strKey.Format(_T("ERROR_%d_KOR"), nStartErrorNum_Part1 + nRepeatCnt);
			strList = m_ctrlLanguageFile.GetKey(strGetKey, strKey).toString();

			if (strList.GetLength() > 0)
				m_stError.vtErr_Kor.push_back(strList);
			else
				break;

			// ENG 읽어오기
			strKey.Format(_T("ERROR_%d_ENG"), nStartErrorNum_Part1 + nRepeatCnt);
			strList = m_ctrlLanguageFile.GetKey(strGetKey, strKey).toString();

			if (strList.GetLength() > 0)
				m_stError.vtErr_Eng.push_back(strList);
			else
				break;

			// VIETNAM 읽어오기
			strKey.Format(_T("ERROR_%d_VIET"), nStartErrorNum_Part1 + nRepeatCnt);
			strList = m_ctrlLanguageFile.GetKey(strGetKey, strKey).toString();

			if (strList.GetLength() > 0)
				m_stError.vtErr_Viet.push_back(strList);
			else
				break;

			nRepeatCnt++;
			m_stError.nErrorNameCount_100 = nRepeatCnt;

			Sleep(10);
		}

		strGetKey.Format(_T("LANGUAGE_200"));
		nRepeatCnt = 0;
		while (1)
		{
			// KOR 읽어오기
			strKey.Format(_T("ERROR_%d_KOR"), nStartErrorNum_Part2 + nRepeatCnt);
			strList = m_ctrlLanguageFile.GetKey(strGetKey, strKey).toString();

			if (strList.GetLength() > 0)
				m_stError.vtErr_Kor.push_back(strList);
			else
				break;

			// ENG 읽어오기
			strKey.Format(_T("ERROR_%d_ENG"), nStartErrorNum_Part2 + nRepeatCnt);
			strList = m_ctrlLanguageFile.GetKey(strGetKey, strKey).toString();

			if (strList.GetLength() > 0)
				m_stError.vtErr_Eng.push_back(strList);
			else
				break;

			// VIETNAM 읽어오기
			strKey.Format(_T("ERROR_%d_VIET"), nStartErrorNum_Part2 + nRepeatCnt);
			strList = m_ctrlLanguageFile.GetKey(strGetKey, strKey).toString();

			if (strList.GetLength() > 0)
				m_stError.vtErr_Viet.push_back(strList);
			else
				break;

			nRepeatCnt++;
			m_stError.nErrorNameCount_200 = nRepeatCnt;

			Sleep(10);
		}

		strGetKey.Format(_T("LANGUAGE_300"));
		nRepeatCnt = 0;
		while (1)
		{
			// KOR 읽어오기
			strKey.Format(_T("ERROR_%d_KOR"), nStartErrorNum_Part3 + nRepeatCnt);
			strList = m_ctrlLanguageFile.GetKey(strGetKey, strKey).toString();

			if (strList.GetLength() > 0)
				m_stError.vtErr_Kor.push_back(strList);
			else
				break;

			// ENG 읽어오기
			strKey.Format(_T("ERROR_%d_ENG"), nStartErrorNum_Part3 + nRepeatCnt);
			strList = m_ctrlLanguageFile.GetKey(strGetKey, strKey).toString();

			if (strList.GetLength() > 0)
				m_stError.vtErr_Eng.push_back(strList);
			else
				break;

			// VIETNAM 읽어오기
			strKey.Format(_T("ERROR_%d_VIET"), nStartErrorNum_Part3 + nRepeatCnt);
			strList = m_ctrlLanguageFile.GetKey(strGetKey, strKey).toString();

			if (strList.GetLength() > 0)
				m_stError.vtErr_Viet.push_back(strList);
			else
				break;

			nRepeatCnt++;
			m_stError.nErrorNameCount_300 = nRepeatCnt;
			Sleep(10);
		}
	}
}
void CDataManager::SaveErrorList_SolutionList()
{
	CTime t = CTime::GetCurrentTime();
	CString strDate = _T("");
	strDate.Format(_T("%04d\\%02d\\%02d"), t.GetYear(), t.GetMonth(), t.GetDay());

	CString strErrorListFile;
	strErrorListFile.Format(_T("%s\\%s\\ErrorList.txt"), _PROGRAM_ERRORLIST_DIR, strDate);

	const int nLength = 256;

	CString strApp = _T("ERROR_LIST");
	CString strKey, strTemp, strTotal;

	strKey.Format(_T("TOTAL_ERROR_COUNT"));
	strTemp.Format(_T("%d"), m_stError.nErrorCount);
	WritePrivateProfileString(strApp, strKey, strTemp, strErrorListFile);


	if (m_stError.nErrorCount > 0)
	{
		strKey.Format(_T("%03d_ErrNum"), m_stError.nErrorCount);
		strTemp.Format(_T("%s"), m_stError.vtErrorNum.at( m_stError.nErrorCount - 1) );
		WritePrivateProfileString(strApp, strKey, strTemp, strErrorListFile);
	}

}
void CDataManager::SetErrorListData(const ERROR_LIST& stErr)
{
	m_stError = stErr;
	SaveErrorList_SolutionList();
}
ERROR_LIST*	CDataManager::GetCurrentErrorList()
{
	if (!m_bInit)
	{
		AfxMessageBox(_T("should be checked for Init CDataManager!!!"), MB_OK);
		return FALSE;
	}

	return &m_stError;
}

void	CDataManager::LoadScanData(const LPCTSTR& lpszPath)
{
	CString strFile;
	strFile = lpszPath;

	const int nLength = 256;
	TCHAR strTemp[nLength];

	CString strApp = _T("MODELKEY_LIST");
	CString strKey;

	for (auto _a = 0; _a < 4; _a++)
	{
		strApp.Format(_T("MODELKEY_LIST_%d"), _a);
		strKey = _T("COUNT");

		GetPrivateProfileString(strApp, strKey, _T("0"), strTemp, sizeof(strTemp), strFile);
		auto cnt = _ttoi(strTemp);

		for (auto _b = 0; _b < cnt; _b++)
		{
			switch (_a)
			{
			case 0: // 8M 30
				strKey.Format(_T("KEY%d"), _b + 1);
				GetPrivateProfileString(strApp, strKey, _T("PT00463984-A"), strTemp, sizeof(strTemp), strFile);
				m_stTeach.vModel_8M_30.push_back(strTemp);
				break;

			case 1: // 3M 180 SIDE
				strKey.Format(_T("KEY%d"), _b + 1);
				GetPrivateProfileString(strApp, strKey, _T("PT00465707-A"), strTemp, sizeof(strTemp), strFile);
				m_stTeach.vModel_3M_180_Side.push_back(strTemp);
				break;

			case 2: // 3M 180 FRONT
				strKey.Format(_T("KEY%d"), _b + 1);
				GetPrivateProfileString(strApp, strKey, _T("PT00601776-A"), strTemp, sizeof(strTemp), strFile);
				m_stTeach.vModel_3M_180_Front.push_back(strTemp);
				break;

			case 3: // 3M 180 REAR
				strKey.Format(_T("KEY%d"), _b + 1);
				GetPrivateProfileString(strApp, strKey, _T("PT00601778-A"), strTemp, sizeof(strTemp), strFile);
				m_stTeach.vModel_3M_180_Rear.push_back(strTemp);
				break;
			}
		}
	}
}
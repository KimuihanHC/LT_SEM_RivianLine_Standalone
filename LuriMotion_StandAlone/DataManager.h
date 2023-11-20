#pragma once
#include "pch.h"
#include "User_Define.h"
#include "Log/INI.h"

class CDataManager : public CSington<CDataManager>
{
public:
	CDataManager(void);
	~CDataManager(void);

private:
	BOOL	m_bInit;
	
	EQ_BASIC_PARAM		m_stEqBasicParam;
	EQ_TOWERLAMP		m_stTowerLamp;
	MOTION_PARAM		m_stMotion;
	TEACHING_PARAM		m_stTeach;
	TESTER_COMM			m_stTester;

	ERROR_LIST			m_stError;
	
public:
	void	Init();

	
	// EqBasic dlg
	void	LoadEqBasicInfo();										// 프로그램 시작시 Loading 하는 기본 파라메타
	void	SaveEqBasicParam(const LPCTSTR& lpszPath);				// EqBasic 사용안함.
	void	LoadEqBasicParam(const LPCTSTR& lpszPath);				// EqBasic 사용안함.
	EQ_BASIC_PARAM* GetCurrentEqBasicData();						// EqBasic 사용안함.
	void SetEqData(const EQ_BASIC_PARAM& stEqParam);				// EqBasic 사용안함.

	// TowerLamp Dlg
	void	SaveTowerLamp(const LPCTSTR& lpszPath);					// Towerlamp 
	void	LoadTowerLamp(const LPCTSTR& lpszPath);					// Towerlamp
	EQ_TOWERLAMP* GetCurrentTowerLampData();						// Towerlamp .
	void SetstTower(const EQ_TOWERLAMP& stTowerLamp);				// Towerlamp .

	// Tester Comm Dlg
	void	SaveTesterCommData(const LPCTSTR& lpszPath);			// Tester Comm Dlg
	void	LoadTesterCommData(const LPCTSTR& lpszPath);			// Tester Comm Dlg
	TESTER_COMM*	GetCurrentTesterCommData();						// Tester Comm Dlg
	void SetTesterCommData(const TESTER_COMM& stTester);			// Tester Comm Dlg
	
	/// Motion Dlg
	void	SaveMotionParam(const LPCTSTR& lpszPath);				// Motor Speed 
	void	LoadMotionParam(const LPCTSTR& lpszPath);				// Motor Speed 
	MOTION_PARAM*	GetCurrentMotionParam();						// Motor Speed 
	void SetMotionParam(const MOTION_PARAM& stMotion);				// Motor Speed 
	
	// Teaching Dlg
	void	SaveTeachingPosData(const LPCTSTR& lpszPath);			// Teaching Pos
	void	LoadTeachingPosData(const LPCTSTR& lpszPath);			// Teaching Pos
	TEACHING_PARAM*	GetCurrentTeachingData();						// Teaching Pos
	void SetTeachingData(const TEACHING_PARAM& stTeaching);			// Teaching Pos

	void RecipeChange(CString& strRecipeName);

	// SaveAs 일때 저장하는 루틴
	void SetSaveAs_TeachingData(const TEACHING_PARAM& stTeaching, CString strPathAndName);			// Teaching Pos_SaveAs
	void SetSaveAs_MotionParam(const MOTION_PARAM& stMotion, CString strPathAndName);				// MotionParam
	void SetSaveAs_TesterCommParam(const TESTER_COMM& stTester, CString strPathAndName);			// TestesrComm Param

	CString m_strCurrenctRecipeName;

	// Error Solution 에서 사용하는 Error LIst
	void	LoadErrorList_SolutionList();
	void	SaveErrorList_SolutionList();
	ERROR_LIST*	GetCurrentErrorList();						// Teaching Pos
	void	SetErrorListData(const ERROR_LIST& stErr);
	CINI	m_ctrlLanguageFile;

	// Model code
	void	LoadScanData(const LPCTSTR& lpszPath);
};
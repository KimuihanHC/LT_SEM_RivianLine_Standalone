#pragma once
#include "pch.h"
#include "VGbase/VGStatic.h"
#include "DlgAxisHomeProgress.h"
#include "DlgNumPad.h"
#include "resource.h"	
#include "DlgMain.h"

#define MOTOR_QUANTITY 1
#define AXIS_MOTOR_STATUES 32
#define TEACH_TIMER_MOTOR_STATUS					300
// CDlgMotorTeach 대화 상자


// SERIAL


class CDlgMotorTeach : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgMotorTeach)

public:
	CDlgMotorTeach(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CDlgMotorTeach();

// 대화 상자 데이터입니다.

	enum { IDD = IDD_DLG_MOTOR_TEACHING };


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void InitComponent_MotorTeach();
	afx_msg void InitComponent_MotorTeach_EditBox();
	afx_msg void InitComponent_AxisY_Teach_EditBox();
	afx_msg void InitComponent_AxisZ_Teach_EditBox();

	void EqpType_UI();


public:
	LRESULT		OnStepMove(WPARAM wParam, LPARAM lParam);
	void		Load_TeachingData();
	void		Save_TeachingData();

	int			m_nMotorSpeed[10][5];						// Motor Speed 저장할 Data.
	double		m_dTeachingPositionData[10][10];			// Teaching Position Teaching Data.

public:
	// UI Init

	CVGStatic	m_stMotorTitle[5];					// Teaching Dlg 메인 Static
	CVGStatic	m_stMotorCurPos[2][10];				// 현재위치값의 Static 변경용 0 = 모터이름 , 1 = 현재위치
	
	CVGStatic	m_stMotorJogTitleSub[4];			// Jog 이동 Sub title 4개
	CVGStatic	m_btnMotorJogSelect_Mode[2];		// Jog 모드 선택
	CVGStatic	m_btnMotorJogSelect_Axis[10];		// Jog 이동할 모터축 선택
	CVGStatic	m_btnMotorJogSelect_Distance[5];	// Jog 이동할 거리 선택
	CVGStatic	m_btnMotorJogSelect_Speed[3];		// jog 이동할 속도 선택
	// [21.1017.12] MFCButton 변경
	CMFCButton	m_btnMotorJogSelect_Move[2];		// Jog 이동 버튼 
	CMFCButton	m_btnMotorJogSelect_Home;			// Home 이동 버튼.

	CVGStatic	m_stMotorManual[8];					// 실린더&컨베이어 이동 static
	CVGStatic	m_btnMotorManual[2][8];				// 실런더&컨베이어 버튼

	CVGStatic	m_stMotorConnect[2];					// 실린더&컨베이어 이동 static
	CVGStatic	m_btnMotorState[4];							// 모터 버튼(Servo,Stop,Reset)

	CVGStatic	m_stMotorSpeedSet_Sub[2];			// 모터 속도설정 Sub Title 2개 (모터선택,모터속도)
	CVGStatic	m_btnMotorSpeedSet_AxisNum[10];		// 모터 속도 설정할 모터축 선택.
	CVGStatic	m_stMotorSpeedSet_Thirdsub[5][2];	// 모터 속도설정 Third Title 5개 (low,mid,high,acc,dec) 와 입력값

	//teaching part
	CVGStatic	m_stTeach_Y;
	CVGStatic	m_stTeach_Y_Sub[3];

	CVGStatic	m_stTeach_Y_Pos2;
	CVGStatic	m_stTeach_Y_SubPos2[5];

	CVGStatic	m_stTeach_Z;
	CVGStatic	m_stTeach_Z_Sub[4];

	CVGStatic	m_stModeChange;

	CVGStatic	m_stStatic_Text[3];
	CVGStatic	m_stStatic_Motor_Statuses[AXIS_MOTOR_STATUES];

public:
	
	// 각 파트별 사용할 변수.
	CString		m_strCurPos;						// 현재위치

	BOOL		m_bFlagJogError;						// Jog error 발생시.

	int			m_nJogMode;							// 조그 모드
	int			m_nSelectAxisNum;					// 조그이동할 축번호
	
	int			m_nSelectDistance;					// 조그 이동 거리 모드
	double		m_dMoveDistance;					// 조그 실제 이동할 거리

	int			m_nSelectSpeed;						// 조그 속도
	double		m_dMoveSpeed;						// 조그 실제 이동할 거리.

	int			m_nSetSpeed_AxisNum;				// 속도 설정 모터번호

	//ComboBox
	CComboBox		m_cbFASBoard_Port;
	CComboBox		m_cbFASBoard_BaudRate;

	// JOG 값 입력
	CString		m_strInput_JogDistance;
	CEdit		m_edInput_JogDistance;

	// TEACHING 값 입력.
	CString		m_strTeach_AxisY_Pos[3];
	CEdit		m_edTeach_AxisY_Pos[3];
	CString		m_strTeach_AxisZ_Pos[4];
	CEdit		m_edTeach_AxisZ_Pos[4];

	CString		m_strTeach_AxisY_Pos2[5];
	CEdit		m_edTeach_AxisY_Pos2[5];


	CString	m_strTeach_ModeChangeTime;
	CEdit	m_edTeach_ModeChangeTime;
		
	// 조그 이동 모드 선택
	afx_msg void OnBnClickedBtnJogSelectMode1();
	afx_msg void OnBnClickedBtnJogSelectMode2();
	afx_msg void CheckSelectJogMode();

	// 조그 이동 축 선택
	afx_msg void OnBnClickedBtnJogSelectAxis1();
	afx_msg void OnBnClickedBtnJogSelectAxis2();
	afx_msg void OnBnClickedBtnJogSelectAxis3();
	afx_msg void OnBnClickedBtnJogSelectAxis4();
	afx_msg void OnBnClickedBtnJogSelectAxis5();
	afx_msg void OnBnClickedBtnJogSelectAxis6();
	afx_msg void OnBnClickedBtnJogSelectAxis7();
	afx_msg void OnBnClickedBtnJogSelectAxis8();
	afx_msg void OnBnClickedBtnJogSelectAxis9();
	afx_msg void OnBnClickedBtnJogSelectAxis10();
	afx_msg void CheckSelectAxis();
	
	// 조그 이동 거리 선택
	afx_msg void OnBnClickedBtnJogDistance1();
	afx_msg void OnBnClickedBtnJogDistance2();
	afx_msg void OnBnClickedBtnJogDistance3();
	afx_msg void OnBnClickedBtnJogDistance4();
	afx_msg void OnBnClickedBtnJogDistance5();
	afx_msg void CheckSelectDistance();

	// 조그 이동 속도 설정
	afx_msg void OnBnClickedBtnJogSpeed1();
	afx_msg void OnBnClickedBtnJogSpeed2();
	afx_msg void OnBnClickedBtnJogSpeed3();
	afx_msg void CheckSelectSpeed();
	afx_msg void OnBnClickedBtnJogMove3();

	// Manual Click Event
	afx_msg void CylinderManual_Acitve(UINT nID);

	// 모터 속도 설정.
	afx_msg void OnBnClickedBtnSpeedAxisSelect1();
	afx_msg void OnBnClickedBtnSpeedAxisSelect2();
	afx_msg void OnBnClickedBtnSpeedAxisSelect3();
	afx_msg void OnBnClickedBtnSpeedAxisSelect4();
	afx_msg void OnBnClickedBtnSpeedAxisSelect5();
	afx_msg void OnBnClickedBtnSpeedAxisSelect6();
	afx_msg void OnBnClickedBtnSpeedAxisSelect7();
	afx_msg void OnBnClickedBtnSpeedAxisSelect8();
	afx_msg void OnBnClickedBtnSpeedAxisSelect9();
	afx_msg void OnBnClickedBtnSpeedAxisSelect10();
	afx_msg void CheckSelectMotorSpeed_AxisNum();

	afx_msg void MotorSpeedChange(UINT nID);
	
	// 모터 입력칸 클릭시 발생 이벤트
	afx_msg void OnStnClickedStcSpeedSetting1();
	afx_msg void OnStnClickedStcSpeedSetting2();
	afx_msg void OnStnClickedStcSpeedSetting3();
	afx_msg void OnStnClickedStcSpeedSetting4();
	afx_msg void OnStnClickedStcSpeedSetting5();
	afx_msg void OnBnClickedBtnTeachSave1();

	// Language 선택
	afx_msg void OnBnClickedBtnTeachSave2();
	afx_msg void OnBnClickedBtnTeachSave3();
	afx_msg void OnBnClickedBtnMotorConnect1();

	void ChangeUI_Language();
	// Level 변경
	void ChangeUI_Level();
	// 구동모드에 따른 ui 변경
	void ChangeUI_Jog(int mode);

	//ComboBox
	void SetComboBox_Serial(CComboBox *pBox, int Data);
	afx_msg void OnBnClickedBtnMotorDisconnect1();
	afx_msg void OnBnClickedBtnMotorServoOn();
	afx_msg void OnBnClickedBtnMotorServoOff();
	afx_msg void OnBnClickedBtnMotorStop();
	afx_msg void OnBnClickedBtnMotorAlarmreset();
	afx_msg void OnServoInitMotor();

	void ChangeConnectUI(BOOL bState);
	void ChangeServoUI(BOOL bState);
	void SetAxisStatus(DWORD dwValue);
	DWORD m_dwOldAxisStatus;
	
	afx_msg void OnBnClickedBtnTeachingPosY1();
	afx_msg void OnBnClickedBtnTeachingPosY2();
	afx_msg void OnBnClickedBtnTeachingPosY3();
	afx_msg void OnBnClickedBtnTeachingPosY4();
	afx_msg void OnBnClickedBtnTeachingPosY5();
	afx_msg void OnBnClickedBtnTeachingPosY6();
	afx_msg void OnBnClickedBtnTeachingPosY7();

	afx_msg void OnBnClickedBtnTeachingMoveY1();
	afx_msg void OnBnClickedBtnTeachingMoveY2();
	afx_msg void OnBnClickedBtnTeachingMoveY3();
	afx_msg void OnBnClickedBtnTeachingMoveY4();
	afx_msg void OnBnClickedBtnTeachingMoveY5();
	afx_msg void OnBnClickedBtnTeachingMoveY6();
	afx_msg void OnBnClickedBtnTeachingMoveY7();

	BOOL SetInserValue(int nMode, int nAxisSpeed);
	BOOL m_bJogBtnBlockFlag;
	afx_msg void OnBnClickedBtnJogMove2();
	afx_msg void OnBnClickedBtnTeachingPosY8();
	afx_msg void OnBnClickedBtnTeachingPosY9();
	afx_msg void OnBnClickedBtnTeachingPosY10();
	afx_msg void OnBnClickedBtnTeachingPosY11();
	afx_msg void OnBnClickedBtnTeachingPosY12();
	afx_msg void OnBnClickedBtnTeachingMoveY8();
	afx_msg void OnBnClickedBtnTeachingMoveY9();
	afx_msg void OnBnClickedBtnTeachingMoveY10();
	afx_msg void OnBnClickedBtnTeachingMoveY11();
	afx_msg void OnBnClickedBtnTeachingMoveY12();
};


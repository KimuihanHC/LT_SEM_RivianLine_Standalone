#include "StdAfx.h"
#include "Rockey.h"


#define CUSTOMER_CODE		"CLQKC"
#define PASSWORD1			0x25F8
#define PASSWORD2			0x1529
#define PASSWORD3			0xE9EF
#define PASSWORD4			0x1AD9

#define USER_ZONE_ADDR		0

#define DLL_LOAD_FAIL		99
#define SW_VERSION_OK		100
#define SW_VERSION_NG		101


CRockey::CRockey(void)
{
}

CRockey::~CRockey(void)
{
}


//************************************************************************
// Date : 2014/07/28  LGH
//************************************************************************
// Method:    CheckRockkey
// FullName:  CRockkey::CheckRockkey
// Access:    public 
// Returns:   BOOL
// Qualifier:
// Parameter: LPCTSTR strSWName
//************************************************************************
BOOL CRockey::CheckRockey( CStringA strSWName )
{
	HINSTANCE hDll;
	ROCKEY Rockey;
	WORD handle[16], p1, p2, p3, p4, retcode;
	DWORD lp1, lp2;
	BYTE buffer[1024];
	BOOL bResult = FALSE;

	int i, j;
	
	// 	char cmd[] = "H=H^H, A=A*23, F=B*17, A=A+F, A=A+G, A=A<C, A=A^D, B=B^B, C=C^C, D=D^D";
// 	char cmd1[] = "A=A+B, A=A+C, A=A+D, A=A+E, A=A+F, A=A+G, A=A+H";
// 	char cmd2[] = "A=E|E, B=F|F, C=G|G, D=H|H";
 	TCHAR errmsg[1000];
	CStringA strRead;

#if defined(_WIN64)
	hDll = LoadLibrary(_T("Rockey4ND_X64.dll"));
#else
	hDll = LoadLibrary(_T("Rockey4ND.dll"));
#endif

	if (hDll == NULL)
	{
#if defined(_WIN64)
		_stprintf(errmsg, _T("Can't find Rockey4ND_X64.dll"));
#else
		_stprintf(errmsg, _T("Can't find Rockey4ND.dll"));
#endif
		AfxMessageBox(errmsg);
		retcode = DLL_LOAD_FAIL;
		goto exit_end;
	}

	Rockey = (ROCKEY)GetProcAddress(hDll, "Rockey");

	p1 = PASSWORD1;
	p2 = PASSWORD2;
	p3 = PASSWORD3;
	p4 = PASSWORD4;


	lp1 = 0;
	retcode = Rockey(RY_FIND, &handle[0], &lp1, &lp2, &p1, &p2, &p3, &p4, buffer);
	if (retcode)
		goto exit_end;

	retcode = Rockey(RY_OPEN, &handle[0], &lp1, &lp2, &p1, &p2, &p3, &p4, buffer);
	
	if (retcode)
		goto exit_end;

	i = 1;
	
	while (retcode == 0)
	{
		retcode = Rockey(RY_FIND_NEXT, &handle[i], &lp1, &lp2, &p1, &p2, &p3, &p4, buffer);
		if (retcode == ERR_NOMORE)
			break;
		if (retcode)
			goto exit_end;

		retcode = Rockey(RY_OPEN, &handle[i], &lp1, &lp2, &p1, &p2, &p3, &p4, buffer);
		if (retcode)
			goto exit_end;

		i++;
	}

	for (j=0;j<i;j++)
	{
		p1 = USER_ZONE_ADDR;
		p2 = ROCKEY_MODE0_LEN;
		memset(buffer, 0, 1024);
		
		retcode = Rockey(RY_READ, &handle[j], &lp1, &lp2, &p1, &p2, &p3, &p4, buffer);
		
		if (retcode)
			goto exit_end;
	}

	strRead.Format("%s", buffer);

	if(strSWName != strRead)
		retcode = SW_VERSION_NG;
	else
		retcode = SW_VERSION_OK;

exit_end:
	switch (retcode)
	{
	case DLL_LOAD_FAIL:
		//AfxMessageBox(_T("라이센스 등록이 되어있지 않아 프로그램 종료 합니다.\n관리자에게 문의 하세요!\n"));
		AfxMessageBox(_T("Lockey's license is not registered. Exit the program..\nPlease contact your administrator!\n"));
		bResult = FALSE;
		break;

	case ERR_NO_ROCKEY:
		//AfxMessageBox(_T("Rockey가 장착되어있지 않습니다.\n관리자에게 문의 하세요!\n"));
		AfxMessageBox(_T("Lockey is not installed.\nPlease contact your administrator!\n"));
		bResult = FALSE;
		break;

	case ERR_INVALID_PASSWORD:
		//AfxMessageBox(_T("Rockey가 장착되어있지 않습니다.\n관리자에게 문의 하세요!\n"));
		AfxMessageBox(_T("Lockey is not installed.\nPlease contact your administrator!\n"));
		bResult = FALSE;
		break;

	case SW_VERSION_NG:
		//AfxMessageBox(_T("Rockey가 장착되어있지 않습니다.\n관리자에게 문의 하세요!\n"));
		AfxMessageBox(_T("Lockey is not installed.\nPlease contact your administrator!\n"));
		bResult = FALSE;
		break;

	case SW_VERSION_OK:
		bResult = TRUE;
		break;

	default:
		bResult = FALSE;
		break;
	}

	return bResult;
}


#include "pch.h"
#include "Ui.h"

#include "LuriMotion_OQA_SFR_Wide.h"
#include "LuriMotion_OQA_SFR_WideDlg.h"

CUi::CUi()
{

}

CUi::~CUi()
{

}

void CUi::Update_Startup_Start(__in UINT nIdx, __in int nCapture)
{
	CDlgMain *pMain = CDlgMain::Get_Instance();
	pMain->Startup_Start(nIdx, nCapture);
}

void CUi::Update_Startup_End(__in UINT nIdx, __in bool bResult)
{
	CDlgMain *pMain = CDlgMain::Get_Instance();
	pMain->Startup_End(nIdx, bResult);
}

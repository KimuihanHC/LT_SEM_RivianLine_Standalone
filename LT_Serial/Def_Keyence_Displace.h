//*****************************************************************************
// Filename	: 	Def_Keyence_Displace.h
// Created	:	2017/10/29 - 15:32
// Modified	:	2017/10/29 - 15:32
//
// Author	:	PiRing
//	
// Purpose	:	Keyence LT-9030 변위센서
//*****************************************************************************
#ifndef Def_Keyence_Displace_h__
#define Def_Keyence_Displace_h__

#include <afxwin.h>


namespace Keyence_Displace
{
	typedef enum _ConstVar
	{
		Keyence_STX				= 'M',	// Header
		Keyence_ETX				= 13,	// CR
		Keyence_ProtoLength		= 27,
		Keyence_DataLength		= 23,
		Keyence_Delimeter		= ',',
	}_ConstVar;

	static const char* g_szKeyence_Output = "M0,0" + Keyence_ETX;

	//-------------------------------------------------------------------------
	// 전송용 프로토콜
	//-------------------------------------------------------------------------

	typedef struct _tag_RecvProtocol
	{
		CStringA	Dataz;
		CStringA	Protocol;

		UINT		ErrCode;
		double		dVal;
		BOOL		bOK;

		_tag_RecvProtocol()
		{
			dVal		= 0.0;
			ErrCode		= 0;
			bOK			= FALSE;
		};

		_tag_RecvProtocol& operator= (_tag_RecvProtocol& ref)
		{
			dVal		= ref.dVal;
			Dataz		= ref.Dataz;
			Protocol	= ref.Protocol;
			ErrCode		= ref.ErrCode;
			bOK			= ref.bOK;

			return *this;
		};

		BOOL SetRecvProtocol(__in const char* pszProtocol, __in UINT_PTR nLength)
		{
			// ER 이면 에러
			if (NULL == pszProtocol)
				return FALSE;

			CStringA strProtocol = pszProtocol;
			Dataz = strProtocol.Mid(2, Keyence_DataLength);

			int ifind = Dataz.Find(',', 0);
			if (ifind > -1)
			{
				double dbufVal1 = atof(Dataz.Mid(1, 8));
				double dbufVal2 = atof(Dataz.Mid(10, 8));

				dVal = (dbufVal1 + dbufVal2) / 2.0;
				/*if (Dataz.Find('+', 0) > -1)
				{
				dVal = atof(Dataz.Mid(ifind + 1, 8));
				}
				else if (Dataz.Find('-', 0) > -1)
				{
				dVal = atof(Dataz.Mid(ifind + 1, 8));
				dVal *= -1;
				}*/
				bOK = TRUE;
			}
			return TRUE;
		};

	}ST_RecvProtocol, *PST_RecvProtocol;
};




#endif // Def_Keyence_Displace_h__

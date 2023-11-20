/********************************************************************
	Class		: AToken (AToken)
	Author		: ASURADA (Myeongho, AN), asurada( at )gmail( dot )com
	Created		: 2007/06/18
	
	Purpose		: String tokenaizer

	History
	- 2007/06/18
		Created.

	- 2008/08/18
		Add GetItemCount function.
		Support Unicode.
		Use safty function ( Visual Studio 2005 ).

	Copyright (C) 2007 ASURADA - All Rights Reserved
*********************************************************************/

#pragma once

class CAToken
{
public:
	CAToken(void);
	virtual ~CAToken(void);

private:
	// Properties
	TCHAR* m_szData;
	TCHAR* m_szDelimiter;
	CStringArray m_arToken;

	bool  m_bTokenized;
	int   m_ItemCount;

protected:
	BOOL TokenizeEx (CString& strParam, CString& strToken, char chDelimeter = ',')
	{
		strToken.Empty();

		int iFindPos = strParam.Find (chDelimeter, 0);
		if (0 <= iFindPos)
		{
			strToken = strParam.Left (iFindPos);
			strParam.Delete(0, iFindPos + 1);

			return TRUE;
		}
		else
		{
			return NULL;
		}
	};

public:
	// Set Methods
	bool SetData(CString strData);
	bool SetData(const TCHAR* szData);

	bool SetDelimiter(const TCHAR* szDelimiter);

	// Get Methods
	void    GetItem(int index, CString& strItem);
	CString GetItem(int index);

	int  GetItemCount(void) { return m_ItemCount; };

	// Functions
	bool Init();
	bool InitToken();
	int  DoTokenize();
	int  DoTokenizeEx();
};

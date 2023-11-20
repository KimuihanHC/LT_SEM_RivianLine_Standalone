#ifndef _LOGBARCODE_H__
#define _LOGBARCODE_H__

#if _MSC_VER > 1000
#pragma once
#endif

#include <vector>
#include "ExString.h"
#include "LogBase.h"

#define LOG_BUFFER_SIZE		10240

class CLogBarcode : public CLogBase
{
protected:
	virtual BOOL OpenLogFile( CExString& strLogPathName );

public:
	CLogBarcode( LPCTSTR lpszServiceName );
	virtual ~CLogBarcode();

	virtual void Init();
};

#endif // __X_LOGSYSTEM_H__
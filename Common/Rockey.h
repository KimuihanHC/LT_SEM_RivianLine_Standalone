#ifndef Rockkey_h__
#define Rockkey_h__

#pragma once

#if defined(_WIN64)
	#include "Rockey4_ND_64.h"
#else
	#include "Rockey4_ND_32.h"
#endif

#define ROCKEY_MODE0		"LURITECH_LOCK"
#define ROCKEY_MODE0_LEN	13
#define ROCKEY_MODE1		"LT_SEKONIX"
#define ROCKEY_MODE1_LEN	10
#define ROCKEY_MODE2		"LT_MCNEX"
#define ROCKEY_MODE2_LEN	8
#define ROCKEY_MODE3		"LT_LGINNOTEK"
#define ROCKEY_MODE3_LEN	12

#ifdef __cplusplus
typedef WORD (CALLBACK *ROCKEY) (WORD, WORD*, DWORD*, DWORD*, WORD*, WORD*, WORD*, WORD*, BYTE*);
#else
WORD Rockey(WORD function, WORD* handle, DWORD* lp1,  DWORD* lp2, WORD* p1, WORD* p2, WORD* p3, WORD* p4, BYTE* buffer);
#endif


class CRockey
{
public:
	CRockey(void);
	~CRockey(void);

	BOOL CheckRockey(CStringA strSWName);
	
};
#endif // Rockkey_h__

//*****************************************************************************
// Filename	: 	IntelHexFile.h
//
// Created	:	2008/05/14
// Modified	:	2010/01/06 - 9:44
//
// Author	:	PiRing
//	
// Purpose	:	Intel Hex File 읽고 쓰는 클래스 헤더파일
//*****************************************************************************
#ifndef __INTELHEXFILE_H__
#define __INTELHEXFILE_H__

#pragma once

#include <windows.h>
#include <string>

using namespace std;

#define	RECORD_MARK			':'

// RecordMark(1) + DataLength(2) + OffSet(4) + RecordType(2) + CheckSum(2) = 11
#define MIN_HEXLINE_SIZE	11
#define MAX_HEX_MEMSIZE		0x100000

typedef struct _HexFile_Param
{
	BYTE	DataLength;
	DWORD	OffSet;
	DWORD   HiOffset;
	BYTE	RecordType;
}HexFile_Param;

class CIntelHexFile
{
public:
	CIntelHexFile(void);
	~CIntelHexFile(void);

	BOOL			HexFileToMem	(LPCTSTR szFileName, char* outBuff, UINT* outBuffSize);
	BOOL			SetHexFile		(LPCTSTR lpszHexPath);
	UINT			GetHexDataSize	();
	BOOL			GetHexData		(unsigned char* outBuffer, UINT* outBufferSize);

protected:
	unsigned char*	m_lpcHexMemory;		// 읽은 Hex File의 Data를 저장할 버퍼
	UINT			m_nReadedBuffSize;	// 읽은 Data의 크기
	LPTSTR			m_lpszHexFilename;	// Hex File 명

	// 읽어 들인 데이터의 시작주소, 종료 주소
	UINT			m_nMin_OffSet;
	UINT			m_nMax_OffSet;
	UINT			m_nPhyMemSize;

	BOOL			ParseHexLine	(char* inBuff, char* outBuff, HexFile_Param* pParam );
	BOOL			ReadHexFile		(LPCTSTR szFileName);

	void			ResetBuffer		();

};

#endif __INTELHEXFILE_H__
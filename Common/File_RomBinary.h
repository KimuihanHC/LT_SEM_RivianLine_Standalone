//*****************************************************************************
// Filename	: 	File_RomBinary.h
// Created	:	2016/3/30 - 13:55
// Modified	:	2016/3/30 - 13:55
//
// Author	:	PiRing
//	
// Purpose	:	
//*****************************************************************************
#ifndef File_RomBinary_h__
#define File_RomBinary_h__

#include <afxwin.h>
#include "IntelHexFile.h"

class CFile_RomBinary
{
public:
	CFile_RomBinary()
	{
		m_pBinData		= NULL;
		m_dwDataSize	= 0;
		m_byCheckSum	= 0x00;
		m_dwCheckSum_Old= 0x00000000;
	};

	~CFile_RomBinary()
	{
		ReleaseMemory();
	};

protected:
	LPBYTE	m_pBinData;			// Rom 데이터
	DWORD	m_dwDataSize;		// Rom 데이터 크기
	BYTE	m_byCheckSum;		// XOR 연산
	DWORD	m_dwCheckSum_Old;	// MCNEX 방식

	inline void	AssignMemory(__in DWORD dwMemSize)
	{
		if (NULL != m_pBinData)
			ReleaseMemory();

		m_pBinData = new BYTE[dwMemSize];
	};

	inline void	ReleaseMemory()
	{
		if (NULL != m_pBinData)
		{
			delete[] m_pBinData;
		}
	};

	inline BOOL	MakeCheckSum()
	{
		if (NULL == m_pBinData)
			return FALSE;

		m_byCheckSum = 0x00;
		m_dwCheckSum_Old	= 0x00000000;
		
		for (DWORD dwCnt = 0; dwCnt < m_dwDataSize; dwCnt++)
		{
			m_byCheckSum = m_byCheckSum ^ m_pBinData[dwCnt];

			m_dwCheckSum_Old += m_pBinData[dwCnt];
			m_dwCheckSum_Old &= 0xFFFFFFFF;
		}

		return TRUE;
	};

	inline BOOL	ReadBinFile(__in LPCTSTR szPath)
	{
		CFile	fileBin;
		CFileException ex;
		BYTE	buffer[4096];
		DWORD	dwOffset = 0;
		DWORD	dwRead;
		DWORD	dwFileSize = 0;

		m_dwDataSize = 0;

		try
		{
			if (!fileBin.Open(szPath, CFile::modeRead | CFile::shareDenyWrite, &ex))
			{
				return FALSE;
			}

			dwFileSize = (DWORD)fileBin.GetLength();
			AssignMemory(dwFileSize + 1);

			do
			{
				dwRead = fileBin.Read(buffer, 4096);
				memcpy(&m_pBinData[dwOffset], buffer, dwRead);
				dwOffset += dwRead;
			} while (dwRead > 0);

			m_dwDataSize = dwOffset;
		}
		catch (CFileException* pEx)
		{			
			pEx->ReportError();
			pEx->Delete();
		}
		catch (CMemoryException* pEx)
		{
			pEx->ReportError();
			pEx->Delete();			
			AfxAbort();
		}

		fileBin.Close();

		MakeCheckSum();

		return TRUE;
	};

	inline BOOL	ReadHexFile(__in LPCTSTR szPath)
	{
		CIntelHexFile HexFile;

		m_dwDataSize = 0;

		AssignMemory(MAX_HEX_MEMSIZE);

		if (HexFile.HexFileToMem(szPath, (char*)m_pBinData, (UINT*)&m_dwDataSize))
		{
			MakeCheckSum();

			return TRUE;
		}
		else
		{
			return FALSE;
		}
	};

public:

	BOOL	ReadRomFile(__in LPCTSTR szPath)
	{
		TCHAR drive[_MAX_DRIVE];
		TCHAR dir[_MAX_DIR];
		TCHAR file[_MAX_FNAME];
		TCHAR ext[_MAX_EXT];
		_tsplitpath_s(szPath, drive, _MAX_DRIVE, dir, _MAX_DIR, file, _MAX_FNAME, ext, _MAX_EXT);

		CString strExt = ext;
		strExt.MakeUpper();

		if (0 == strExt.Compare(_T(".HEX")))
		{
			if (!ReadHexFile(szPath))
			{
				return FALSE;
			}
		}
		else if (0 == strExt.Compare(_T(".BIN")))
		{
			if (!ReadBinFile(szPath))
			{
				return FALSE;
			}
		}
		else
		{
			return FALSE;
		}

		return TRUE;
	};

	BOOL	ReadRomFile(__in LPCTSTR szPath, __out LPBYTE lpbyOutBuf, __out PDWORD pdwOutSize, __out_opt PBYTE pbyChkSum = NULL)
	{
		TCHAR drive[_MAX_DRIVE];
		TCHAR dir[_MAX_DIR];
		TCHAR file[_MAX_FNAME];
		TCHAR ext[_MAX_EXT];
		_tsplitpath_s(szPath, drive, _MAX_DRIVE, dir, _MAX_DIR, file, _MAX_FNAME, ext, _MAX_EXT);

		CString strExt = ext;
		strExt.MakeUpper();

		if (0 == strExt.Compare(_T("HEX")))
		{
			if (ReadHexFile(szPath))
			{
				lpbyOutBuf = m_pBinData;				
				*pdwOutSize = m_dwDataSize;
				*pbyChkSum = m_byCheckSum;
			}
			else
			{
				return FALSE;
			}
		}
		else if (0 == strExt.Compare(_T("BIN")))
		{
			if (ReadBinFile(szPath))
			{
				lpbyOutBuf = m_pBinData;
				*pdwOutSize = m_dwDataSize;
				*pbyChkSum = m_byCheckSum;
			}
			else
			{
				return FALSE;
			}
		}
		else
		{
			return FALSE;
		}

		return TRUE;
	};
	
	LPBYTE	GetRomBuffer()
	{
		return m_pBinData;
	};

	DWORD	GetRomBufferSize()
	{
		return m_dwDataSize;
	};

	BYTE	GetCheckSum()
	{
		return m_byCheckSum;
	};

	DWORD	GetCheckSum_Old()
	{
		return m_dwCheckSum_Old;
	};
	
	BOOL	SaveBinaryFile(__in LPCTSTR szPath)
	{
		if ((NULL == m_pBinData) || (0 == m_dwDataSize))
			return FALSE;

		CFile	fileBin;
		CFileException ex;
		
		try
		{
			if (!fileBin.Open(szPath, CFile::modeWrite | CFile::shareExclusive | CFile::modeCreate, &ex))
			{
				return FALSE;
			}
			
			fileBin.Write(m_pBinData, m_dwDataSize);
			fileBin.Flush();
		}
		catch (CFileException* pEx)
		{
			pEx->ReportError();
			pEx->Delete();
		}
		catch (CMemoryException* pEx)
		{
			pEx->ReportError();
			pEx->Delete();
			AfxAbort();
		}

		fileBin.Close();

		return TRUE;
	};

	inline BOOL	MakeCheckSum(__in DWORD dwStartOffset)
	{
		if (NULL == m_pBinData)
			return FALSE;

		m_byCheckSum = 0x00;
		m_dwCheckSum_Old = 0x00000000;

		for (DWORD dwCnt = dwStartOffset; dwCnt < m_dwDataSize; dwCnt++)
		{
			m_byCheckSum = m_byCheckSum ^ m_pBinData[dwCnt];

			m_dwCheckSum_Old += m_pBinData[dwCnt];
			m_dwCheckSum_Old &= 0xFFFFFFFF;
		}

		return TRUE;
	};
};


#endif // File_RomBinary_h__

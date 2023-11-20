//#############################################################################
//##	파일명				IntelHexFile.cpp
//##
//##	설명				Intel Hex File 읽고 쓰는 클래스
//##
//##	코드 생성일자		2008. 05. 14
//##	최종 변경일자		2009. 05. 12
//##
//##	작성자				PiRing ( Jaewon Shim)
//##
//##	이력 :	2008.05.14 최초작성 C++ STL 형태로 작성
//##			2009.05.12 MFC 형태로 변경
//#############################################################################

#include "stdafx.h"
#include "IntelHexFile.h"
#include <assert.h>
#include <string>
#include <fstream>
#include <locale>

using namespace std;

//=============================================================================
// 생성자
//=============================================================================
CIntelHexFile::CIntelHexFile(void)
{
	TRACE(_T("<<<<<<< Start : CIntelHexFile::CIntelHexFile(void) >>>>>>>>>>>>>>>\n "));
	// 2Byte 최대 크기 만큼 설정 0x0000 -> 0xFFFF 까지
	m_lpcHexMemory = new unsigned char [MAX_HEX_MEMSIZE];
	m_nReadedBuffSize = 0;

	// 읽어 들인 주소의 최대, 최소값
	m_nMin_OffSet = 0;
	m_nMax_OffSet = 0;

	m_lpszHexFilename = NULL;

	TRACE(_T("<<<<<<< Stop : CIntelHexFile::CIntelHexFile(void) >>>>>>>>>>>>>>>\n "));

}

//=============================================================================
// 소멸자
//=============================================================================
CIntelHexFile::~CIntelHexFile(void)
{
	TRACE(_T("<<<<<<< Start : CIntelHexFile::~CIntelHexFile(void) >>>>>>>>>>>>>>>\n "));
	if (NULL != m_lpcHexMemory)
		delete [] m_lpcHexMemory;

	if (NULL != m_lpszHexFilename)
		delete [] m_lpszHexFilename;
	TRACE(_T("<<<<<<< Stop : CIntelHexFile::~CIntelHexFile(void) >>>>>>>>>>>>>>>\n "));
}

//=============================================================================
//	함수명 : ParseHexLine()
//
//	인자값 : char* inBuff		-> 한 라인을 읽은 데이터
//			 char* outBuff		-> 파싱 처리후 출력할 버퍼
//			 BYTE* outDataLen	-> Data Length
//			 WORD* outAddress	-> OffSet
//
//	리턴값 : int		-> 버퍼 사이즈
//			 -1			-> 에러
//
//	설	명 : Intel Hex File을 읽어와서 플래시에 기록할 버퍼에 담음
//=============================================================================
BOOL CIntelHexFile::ParseHexLine(char* inBuff, char* outBuff, HexFile_Param* pParam )
{
	if ( NULL == inBuff)
	{
		assert (_T("NULL == inBuff"));
		return FALSE;
	}
	
	char*	ptr			= NULL;
	string	sTempBuffer;
	UINT	nDataLength	= 0;
	UINT	nAddress	= 0;
	UINT	nRecordType	= 0;
	UINT	nData		= 0;
	UINT	ChkSum		= 0;
	UINT	CmpChkSum	= 0;

	ptr = inBuff;

	// ## Record Mark ##
	// 레코드 마크 ':'가 처음에 없으면 에러
	if (*ptr != RECORD_MARK) 
		return FALSE;
	ptr += 1;		// Record Mark Size
	
	// ## Data Length ##
	if (!sscanf_s(ptr, "%02x", &nDataLength)) 
		return FALSE;	
	ptr += 2;		// DataLength Size
	
	// 버퍼의 데이터가 Data Length의 길이에 부합하는지 판단
	if ( strlen(inBuff) != (UINT)((MIN_HEXLINE_SIZE) + (nDataLength * 2)) ) 
	{
		assert (!"Incorrect Data Length!!");
		return FALSE;
	}
	
	// ## OffSet ##
	if (!sscanf_s(ptr, "%04x", &nAddress)) 
		return FALSE;
	ptr += 4;		// OffSet Size
	
	// ## Record Type ##
	if (!sscanf_s(ptr, "%02x", &nRecordType)) 
		return FALSE;	
	ptr += 2;		// Record Type Size

	// Data 전까지의 체크섬 계산
	CmpChkSum = (nDataLength & 0xFF) + ((nAddress >> 8) & 0xFF) + (nAddress & 0xFF) + (nRecordType & 0xFF);
	
	// ## Data ##
	for (UINT nCnt = 0; nCnt < nDataLength; nCnt++)
	{
		if (!sscanf_s(ptr, "%02x", &nData)) 
			return FALSE;	
		ptr += 2;	// Data Size

		// 임시버퍼에 데이터 저장
		//sTempBuffer.append(nData);
		sTempBuffer.insert (sTempBuffer.end(), (char)nData);

		CmpChkSum += (UINT) (nData & 0xFF);	
	}
	
	// ## Check Sum ##
	if (!sscanf_s(ptr, "%02x", &ChkSum)) 
		return FALSE;
	
	// 계산된 체크섬하고 버퍼에 있는 체크섬하고 비교
	// BYTE 타입으로 계산할경우 1Byte 크기를 넘어가서 오류 발생
	if ( ((CmpChkSum & 0xFF) + (ChkSum & 0xFF)) & 0xFF )
	{
		assert (!"Check Sum Error!!");
		return FALSE;
	}

	// 출력 변수에 읽은 값을 저장
	//outBuff = new char[nDataLength+1];
	memcpy(outBuff, (char*)sTempBuffer.data(), nDataLength);

	if(nRecordType == 4)
		pParam->HiOffset = nData << 16;

	pParam->DataLength	= (BYTE)nDataLength;
	pParam->OffSet		= (DWORD)nAddress;
	pParam->RecordType	= (BYTE)nRecordType;

	return TRUE;
}

//=============================================================================
//	함수명 : IntelHexFile()
//
//	인자값 : char* szFileName	-> Intel Hex File 명
//
//	리턴값 : int		-> 파일에서 읽어서 저장된 버퍼의 크기
//			 -1			-> 에러
//
//	설	명 : Intel Hex File을 읽어와서 플래시에 기록할 버퍼에 담음
//=============================================================================
BOOL CIntelHexFile::ReadHexFile(LPCTSTR szFileName)
{
	if (NULL == szFileName)
	{
		assert (_T("NULL == szFileName"));
		return FALSE;
	}

	HexFile_Param	stParam;
	string	sInBuffer;
	char	LineBuff [1000];
	BYTE	ChkSum		= 0;
	UINT	MinAddr		= 65536;
	UINT	MaxAddr		= 0;
	UINT	nTotalCnt	= 0;

	// 초기화
	ResetBuffer ();
	memset (LineBuff, 0, 1000);

	setlocale(LC_ALL, "Korean");

	ifstream inFile(szFileName); // 한글 폴더명 에러!!!!!!!!!!!!!!!!!
	
	if (!inFile) // 파일 오픈에러
	{	
		inFile.close();
		assert (!"Intel Hex File Open Error!");
		return FALSE;
	}

	if (NULL != m_lpszHexFilename)
	{
		delete [] m_lpszHexFilename;
		m_lpszHexFilename = NULL;
	}

	size_t iLength = _tcslen(szFileName);
	m_lpszHexFilename = new TCHAR[iLength];
	memcpy (m_lpszHexFilename, szFileName, iLength);

	ZeroMemory(&stParam, sizeof(HexFile_Param));

	while (getline(inFile, sInBuffer))
	{
		// 읽은 라인의 크기가 HexFile 한 라인의 최소사이즈보다 적으면 에러
		if (sInBuffer.size() < MIN_HEXLINE_SIZE)
		{
			sInBuffer.clear();
			continue;
			//assert (!"Intel Hex File Read Error");
			//inFile.close();
			//return FALSE;
		}

		// 버퍼안의 공백 지우기
		string::size_type indexCh;
		for(UINT nCnt = 0; nCnt < sInBuffer.length(); nCnt++)
		{	
			indexCh = sInBuffer.find (' ');

			if (string::npos != indexCh)
				sInBuffer.erase(indexCh, 1);
			else
				break;
		}

		// 파싱 Hex 라인
		if (!ParseHexLine((char *)sInBuffer.data(), LineBuff, &stParam ))
		{
			assert (!"Hex Line Pasing Error!!");
			return FALSE;
		}
		
		switch (stParam.RecordType)
		{
			case 0:	// Data Record
				// Data의 길이만큼 m_lpcHexMemory에 저장
				for(UINT nCnt = 0; nCnt < stParam.DataLength; nCnt++) 
				{
					 m_lpcHexMemory[stParam.HiOffset + stParam.OffSet] = LineBuff[nCnt] & 0xFF;

					 // 1Byte의 Data를 저장할때마다 카운트 증가
					 nTotalCnt++;
					 
					 // ??
					 if (stParam.OffSet < MinAddr) 
						 MinAddr = stParam.OffSet;
					 
					 if (stParam.HiOffset + stParam.OffSet > MaxAddr) 
						 MaxAddr = stParam.HiOffset + stParam.OffSet;
					 
					 stParam.OffSet++;
				}				
				break;

			case 1:	// End of File Record
				inFile.close();
				break;

			case 4:
				break;

			default:
				assert(!"Not Support Record Type");
				break;
		}

		sInBuffer.clear();
	} // End of While()

	inFile.close();
	
	// 읽어서 저장된 Data의 총 크기
	m_nReadedBuffSize = nTotalCnt;
	m_nMin_OffSet	= MinAddr;
	m_nMax_OffSet	= MaxAddr;
	m_nPhyMemSize	= MaxAddr - MinAddr + 1;

	return TRUE;
}

//=============================================================================
//	함수명 : GetBuffSize()
//
//	인자값 : None
//
//	리턴값 : int		-> 파일에서 읽어서 저장된 버퍼의 크기
//
//	설	명 : Intel Hex File에서 실제로 저장된 Data의 크기를 반환함
//=============================================================================
UINT CIntelHexFile::GetHexDataSize()
{
	//return m_nReadedBuffSize;
	return m_nPhyMemSize;
}

//=============================================================================
//	함수명 : GetHexData()
//
//	인자값 : unsigned char* outBuffer	-> Hex 데이타의 출력용 버퍼
//			 UINT* outBufferSize		-> 파일에서 읽어서 저장된 버퍼의 크기
//
//	리턴값 : TRUE		-> 정상
//			 FALSE		-> 읽어들인 Hex Data가 없음
//
//	설	명 : Intel Hex File에서 읽어들인 Data와 그 크기를 반환함
//=============================================================================
BOOL CIntelHexFile::GetHexData (unsigned char* outBuffer, UINT* outBufferSize)
{
	if ( 0 == m_nReadedBuffSize )
		return FALSE;

	//memcpy (outBuffer, m_lpcHexMemory, m_nReadedBuffSize);
	//*outBufferSize	= m_nReadedBuffSize;
	memcpy (outBuffer, m_lpcHexMemory, m_nPhyMemSize);
	*outBufferSize	= m_nPhyMemSize;

	return TRUE;
}

//=============================================================================
//	함수명 : HexFileToMem()
//
//	인자값 : unsigned char* outBuffer	-> Hex 데이타의 출력용 버퍼
//			 UINT* outBufferSize		-> 파일에서 읽어서 저장된 버퍼의 크기
//
//	리턴값 : TRUE		-> 정상
//			 FALSE		-> 읽어들인 Hex Data가 없음
//
//	설	명 : Intel Hex File에서 읽어들인 Data와 그 크기를 반환함
//=============================================================================
BOOL CIntelHexFile::HexFileToMem (LPCTSTR szFileName, char* outBuff, UINT* outBuffSize)
{
	if (NULL == szFileName)
	{
		assert (_T("NULL == szFileName"));
		return FALSE;
	}

	//WCHAR wszFile[256]; 
	//MultiByteToWideChar( CP_ACP, 0, szFileName, strlen(szFileName)+1, wszFile, sizeof(wszFile)/sizeof(wszFile[0]) );
	ReadHexFile(szFileName);

	if (0 == m_nReadedBuffSize)
		return FALSE;

	//memcpy (outBuff, m_lpcHexMemory, m_nReadedBuffSize);
	//*outBuffSize = m_nReadedBuffSize;
	memcpy (outBuff, m_lpcHexMemory, m_nPhyMemSize);
	*outBuffSize = m_nPhyMemSize;

	return TRUE;
}

//=============================================================================
//	함수명 : SetHexFile()
//
//	인자값 : LPCTSTR lpszHexPath	-> Hex 데이타의 출력용 버퍼
//
//	리턴값 : TRUE		-> 정상
//			 FALSE		-> 읽어들인 Hex Data가 없음
//
//	설	명 : Intel Hex File에서 Data를 읽어서 메모리에 세팅
//=============================================================================
BOOL CIntelHexFile::SetHexFile	(LPCTSTR lpszHexPath)
{
	if (NULL == lpszHexPath)
	{
		assert (_T("NULL == lpszHexPath"));
		return FALSE;
	}

	return ReadHexFile(lpszHexPath);
}

//=============================================================================
//
//=============================================================================
void CIntelHexFile::ResetBuffer ()
{
	m_nReadedBuffSize	= 0;
	m_nPhyMemSize		= 0;
	//memset (m_lpcHexMemory, 0x00, MAX_HEX_MEMSIZE);
	memset (m_lpcHexMemory, 0xFF, MAX_HEX_MEMSIZE);
}
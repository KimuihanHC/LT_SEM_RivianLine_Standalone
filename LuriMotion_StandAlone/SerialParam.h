
#pragma once


#define		STX			0x02
#define		ETX			0x03
#define		CR			0x0D
#define		LF			0x0A

static TCHAR *BaudList[] =
{
	_T("9600"),_T("19200"),_T("38400"),_T("57600"),_T("115200")
};

static TCHAR *DataBitList[] =
{
   _T("7"),	_T("8")
};

static TCHAR *StopBitList[] =
{
    _T("1"),	_T("2")
};

static TCHAR *ParityList[] =
{
    _T("None"),	_T("Even"),	_T("Odd")
};

static int BaudTable[5] =
{
	B9600, B19200, B38400, B57600, B115200
};

static int DataBitTable[2] =
{
	BIT_7,	BIT_8
};

static	int	ParityTable[3] =
{
	P_NONE, P_EVEN, P_ODD
};

static int StopBitTable[2] =
{
	STOP_1,	STOP_2
};
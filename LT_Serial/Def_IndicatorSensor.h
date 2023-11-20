//*****************************************************************************
// Filename	: 	Def_IndicatorSensor.h
// Created	:	2017/1/2 - 16:51
// Modified	:	2017/1/2 - 16:51
//
// Author	:	KHO
//	
// Purpose	:	
//*****************************************************************************
#ifndef Def_IndicatorSensor_h__
#define Def_IndicatorSensor_h__

#include <windows.h>

namespace IndicatorSensor
{
	typedef enum ConstVar
	{
		SEN_Protocol_BYTE1	= 0x49,
		SEN_Protocol_BYTE2	= 0x44,

		SEN_Protocol_LF		= 0x0A,

		SEN_Protocol_Length = 5,		// 송신 프로토콜
		REV_Protocol_Length = 16,		// 수신 프로토콜
	}ConstVar;

	typedef enum enCommand
	{
		Cmd_Real_Data		= 0x50,
		Cmd_Hold_Data		= 0x48,
		Cmd_Release_Data	= 0x52,
		Cmd_Zero_Data		= 0x5A,
	}enCommand;

	// 송신용 (PC -> Controller)
	typedef struct _tag_Protocol_Request
	{
		char	ID[2];
		char	Port[2];
		char	Command;

		_tag_Protocol_Request()
		{
			ID[0]	= (char)SEN_Protocol_BYTE1;
			ID[1]	= (char)SEN_Protocol_BYTE2;
			Port[0] = '0';
			Port[1] = '1';
			Command = 0x00;
		};

	}ST_Protocol_Request;

	typedef struct _tag_Protocol_Response
	{
		char	TYPE[2];
		char	Delimit_1;
		char	Network[2];
		char	Delimit_2;
		char	Data[8];
		char	CR;
		char	LF;

		_tag_Protocol_Response()
		{
		
		};


	}ST_Protocol_Response;
}

#endif // Def_IndicatorSensor_h__

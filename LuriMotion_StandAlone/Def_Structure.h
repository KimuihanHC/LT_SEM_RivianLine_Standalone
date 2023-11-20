#pragma once


#pragma pack(push, 1)

typedef struct _tagInspection_Info
{
	int nSubCaptureIndex;
	int nCaptureIndex;
	float	fPowerVoltage;

	_tagInspection_Info()
	{
		Reset();
	};

	void Reset()
	{
		nCaptureIndex = 1;
		fPowerVoltage = 0.0;
	};

	int GetCaptureIndex()
	{
		return nCaptureIndex;
	};

	void CaptureNext()
	{
		nCaptureIndex++;
	};

	void SetSubCaptrueIndex(__in int nCapture)
	{
		nSubCaptureIndex = nCapture;
	};

	int GetSubCaptureIndex()
	{
		return nSubCaptureIndex;
	};

	void SetPowerVoltage(__in float fVolt)
	{
		fPowerVoltage = fVolt;
	};

	float GetPowerVoltage()
	{
		return fPowerVoltage;
	};

}ST_InspectionInfo, *PST_InspectionInfo;

#pragma pack(pop, 1)

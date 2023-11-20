#pragma once

class CDevice
{
public:
	CDevice();
	~CDevice();	

	static bool ConnectDevicez();
	static bool ReconnectDeviez();
	static bool DisconnectDevicez();

	static bool IsConnected_HandyBCR();
	static bool IsConnected_Power();

	static bool GetMeasure_Volt(__out float& value);
	static bool GetMeasure_Current(__out float& value);

	// power supply
	static bool PowerReset();
	static bool PowerOn();
	static bool PowerOff();
	static BOOL POWERSUPPLY_MeasureVolt	(__out float & value);

	// light
	static bool LightOn();
	static bool LightOff();

	// semco communication
	static bool SEMCO_SendReadyStart	();
	static bool SEMCO_WaitReadyStartAck	(__in UINT nWaitTime, __out UINT& nRetCode);
	static bool SEMCO_SendBcr			(__in CString szBCR);
	static bool SEMCO_WaitBarcodeAck	(__in UINT nWaitTime, __out CString& szBCR);
	static bool SEMCO_PrewviewStart		();
	static bool SEMCO_PrewviewStop		();
	static bool SEMCO_StartInspection	();

	static bool SEMCO_SendCurrentVoltage		();
	static bool SEMCO_WaitCurrentVoltageAck		(__in UINT nWaitTime, __out CString& szCurrentVoltage);

};


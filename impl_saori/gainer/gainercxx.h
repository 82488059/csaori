/*============================================================================
	CGainer����N���X (h)
	�ȉ���gainercxx (dandelion�����) ����������Q�l�ɂ��܂����B
	http://www.atinfinity.info/wiki/index.php?gainercxx
============================================================================*/

#ifndef _GAINER_H_
#define _GAINER_H_

#include <string>
#include <vector>
#include <queue>
#include <process.h>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "serial.h"

class CGainer;

#define GAINER_MAX_INPUTS 8
#define GAINER_LED_MATRIX 8

class CGainer
{
public:
	//--------------------�֐�--------------------
	CGainer() {
		m_inited = false;
	}
	bool Init(int portNum=1, int mode=0);
	
	void Exit();
	~CGainer() {
		Exit();
	}
	
	static void Search(std::vector<int> &v);
	
	std::string Version();
	
	bool SetLED(bool isOn);
	
	void ExecuteContinuousDigital(DWORD period);
	void ExecuteContinuousAnalog(DWORD period);
	void ExecuteExitContinuous();
	
	bool GetDigitalAll(WORD &result,size_t &bits);
	bool GetAnalogAll(std::vector<BYTE> &result);
	
	bool SetDigitalAll(int value);
	bool SetDigitalSingle(int port,bool high);

	bool SetAnalogAll(const std::vector<WORD> &data);
	bool SetAnalogSingle(int port,BYTE value);

	bool SetServoAll(const std::vector<WORD> &data);
	bool SetServoSingle(int port,BYTE value);
		
	int GetCOMPort(void) { return m_port; }
	
	bool SetConfiguration(int mode);

	bool SetPGA(double gain,bool isAGNDRef);

	void Reboot(bool nowait = false);
	
	void SetHWND(HWND h) { m_hwnd_sstp = h; }

	bool ScanLine(size_t row,BYTE data[GAINER_LED_MATRIX]);
	bool ScanMatrix(BYTE data[GAINER_LED_MATRIX][GAINER_LED_MATRIX]);

private:
	//--------------------�֐�--------------------
	std::string command_send(const std::string &cmd,bool nowait = false);
	
	void processEvent(const std::string &event);
	
	void command_recv(void);
	std::string wait_recv(void);

	static unsigned __stdcall receiver(void *arg);
	unsigned receive(void);

	void execute_sstp_button(bool isPressed);
	void execute_sstp_digital(void);
	void execute_sstp_analog(void);

	void send_sstp(std::string &sstp);

	void check_config(void);

	//--------------------�ϐ�--------------------
	CSerialCOM m_serial;

	bool  m_led;
	BYTE  m_analogInputs[GAINER_MAX_INPUTS];
	DWORD m_digitalInputs;
	BYTE  m_LEDMatrixOutputs[GAINER_LED_MATRIX][GAINER_LED_MATRIX];
	
	int m_config;
	
	int m_port;
	bool m_endFlag;
	bool m_inited;
	
	HANDLE m_thread_handle;

	HWND m_hwnd_sstp;
	
	CRITICAL_SECTION m_receive_queue_lock;
	
	HANDLE m_receive_queue_semaphore;
	std::queue<std::string> m_receive_queue;

	std::vector<char> m_receive_buffer;
	
	std::string m_version_string;

	DWORD m_analog_period;
	DWORD m_analog_last;
	DWORD m_digital_period;
	DWORD m_digital_last;
	
	enum pin_t {AIN = 0, DIN, AOUT, DOUT};
	static const int CONFIG[][4];
	static const int MATRIX_LED_CONFIGURATION;
};

#endif // __GAINER_H_

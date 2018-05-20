#ifndef _SERIAL_COMMUNICATION_H_
#define _SERIAL_COMMUNICATION_H_

#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <vector>

#define MAX_BUFFER_SIZE 255
#define INIT_TIME_MS 2000

const extern std::vector<uint32_t> STANDARD_BAUD_RATES;

class SerialPort {

public:
	SerialPort(LPCSTR port_name, DWORD baud_rate);
	~SerialPort();

	int read(uint8_t* buffer, uint8_t buffer_size);
	bool isConnected();

private:	
	
	uint32_t baud_rate = 9600;
	LPCSTR port_name;
	bool connected = false;
	HANDLE handler;
	COMSTAT status;
	DWORD last_error;

	bool checkBaudRate(DWORD baud_rate);

};

#endif // !_SERIAL_COMMUNICATION_H_

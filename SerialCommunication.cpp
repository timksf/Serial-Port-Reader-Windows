#include "SerialCommunication.h"
#include <algorithm>
#include <iostream>


const std::vector<uint32_t> STANDARD_BAUD_RATES{ CBR_110, CBR_300, CBR_600, CBR_1200, CBR_2400, CBR_4800, CBR_9600, CBR_14400, CBR_19200, CBR_38400, CBR_57600, CBR_115200 };

SerialPort::SerialPort(LPCSTR port_name, DWORD baud_rate) :
	port_name(port_name)
{

	if (checkBaudRate(baud_rate)) {
		this->baud_rate = baud_rate;
	}
	else {
		printf("ERROR: please use one of the following standard baud rates: ");
		for (auto &elem : STANDARD_BAUD_RATES) {
			std::cout << elem << std::endl;
		}
	}

	handler = CreateFileA(port_name, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (handler == INVALID_HANDLE_VALUE) {
		last_error = GetLastError();
		if (last_error == ERROR_FILE_NOT_FOUND) {
			std::cout << "ERROR: Unable to open port " << port_name << std::endl;
		}
		else {
			std::cout << "ERROR creating port handle" << std::endl;
		}
		system("pause");
		exit(EXIT_FAILURE);
	}

	DCB serialConnection;

	if (!(GetCommState(handler, &serialConnection))) {
		std::cout << "ERROR: couldnt get settings for specified port" << std::endl;
		system("pause");
		exit(EXIT_FAILURE);
	}

	serialConnection.BaudRate = baud_rate;
	serialConnection.ByteSize = 8;
	serialConnection.StopBits = ONESTOPBIT;
	serialConnection.Parity = NOPARITY;
	serialConnection.fDtrControl = DTR_CONTROL_ENABLE;

	if (!(SetCommState(handler, &serialConnection))) {
		std::cout << "ERROR: couldnt apply settings to specified port" << std::endl;
		system("pause");
		exit(EXIT_FAILURE);
	}
	else {
		this->connected = true;
		PurgeComm(handler, PURGE_RXCLEAR | PURGE_TXCLEAR);
		Sleep(INIT_TIME_MS);
	}


}

SerialPort::~SerialPort() {
	CloseHandle(handler);
	connected = false;
}

int SerialPort::read(uint8_t* buffer, uint8_t buffer_size) {
	DWORD bytesRead = 0;
	unsigned int toRead = 0;

	ClearCommError(handler, &last_error, &status);

	if (status.cbInQue > 0) {
		if (status.cbInQue > buffer_size) {
			toRead = buffer_size;
		}
		else toRead = status.cbInQue;

		if (ReadFile(handler, buffer, toRead, &bytesRead, NULL)) return bytesRead;	
	}

	return 0;
}

bool SerialPort::isConnected() {
	return this->connected;
}

bool SerialPort::checkBaudRate(DWORD baud_rate) {
	return (std::find(STANDARD_BAUD_RATES.begin(), STANDARD_BAUD_RATES.end(), baud_rate) != STANDARD_BAUD_RATES.end());
}

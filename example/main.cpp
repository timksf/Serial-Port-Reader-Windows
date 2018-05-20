#include "timer.h"
#include "SerialCommunication.h"
#include <iostream>
#include <string>
#include <sstream>
#include <Windows.h>

LPCSTR port = "\\\\.\\COM6";
uint8_t incoming_data[MAX_BUFFER_SIZE];
std::stringstream ss;

int main() {
	SerialPort serialDevice(port, CBR_9600);
	
	if (serialDevice.isConnected()) std::cout << "Successfully established connection!" << std::endl; //check connection

	while (serialDevice.isConnected()) {
		int bytes_read = serialDevice.read(incoming_data, MAX_BUFFER_SIZE);

		for (uint8_t i = 0; i < bytes_read; i++) {
			ss << std::hex << incoming_data[i]; //Print in hexadecimal packs
		}
		ss << std::endl;

		if (bytes_read > 0) {
			std::cout << ss.str();
		}

		ss.str(std::string()); //empty string stream
		Sleep(100);
	}

	system("pause");
	exit(EXIT_SUCCESS);
}


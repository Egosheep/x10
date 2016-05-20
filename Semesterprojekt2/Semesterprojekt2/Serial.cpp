#include "Serial.h"

Serial::Serial(char* portName)
{
	//We're not yet connected
	this->connected = false;

	//Try to connect to the given port through CreateFile
	//Generic access is to specify the type of access you need
	//to open a "Handle" object. (Look at header file)

	//Function "CreatFile" is to create and open the I/O device
	//This function return a "Handle"
	this->hSerial = CreateFile(
		//PortName - File name of the device (Named in main as "\\\\.\\COMX")
		//X is the number of the COM device
		portName,
		// Generic access to the device
		(GENERIC_READ | GENERIC_WRITE),
		//This '0' is to specify the mode, which is se to preventing other processes to the device
		0,
		//This NULL is to prevent the returned "Handle" to not
		//be inherited by any child process the device may create
		NULL,
		//Open_Existing - Used to access devices other than files
		OPEN_EXISTING,
		//File_attribute_normal - Used to specify device attributes and flags
		FILE_ATTRIBUTE_NORMAL,
		//This parameter of the function is ignored
		NULL);

	//Check if the connection was successfull
	//First check if the created file is invalid to the user
	if (this->hSerial == INVALID_HANDLE_VALUE)
	{
		//If not success full display an Error
		//If the error is that the file is not found (file not correct located)
		if (GetLastError() == ERROR_FILE_NOT_FOUND) //Print Error if neccessary
			cout << "ERROR: Handle was not attached. Reason: " << portName << " not available." << endl;
		else //Unknown error
			cout << "ERROR" << endl;
	}
	//The file can be read by the user
	else
	{
		//If connected we try to set the comm parameters
		//This is the object to specify the baudRate, byteSize, etc.
		//Basicly the UART specification
		DCB dcbSerialParams = { 0 };

		//Checks if the current parameters of the device is valid
		//If not display error
		if (!GetCommState(this->hSerial, &dcbSerialParams))
		{
			//If impossible, show an error
			cout << "Failed to get current serial parameters!" << endl;
		}
		else //If the correct parameters can be read:
		{
			//Define serial connection parameters for the Arduino board
			//Match theses parameters to the UART of the Arduino
			dcbSerialParams.BaudRate = CBR_9600;
			dcbSerialParams.ByteSize = 8;
			dcbSerialParams.StopBits = ONESTOPBIT;
			dcbSerialParams.Parity = NOPARITY;

			//Setting the DTR to Control_Enable ensures that the Arduino is properly
			//reset upon establishing a connection
			dcbSerialParams.fDtrControl = DTR_CONTROL_ENABLE;


			//Set the parameters and check for their proper application
			//Basicly check if the parameters can be set for the device
			if (!SetCommState(hSerial, &dcbSerialParams))
				cout << "ALERT: Could not set Serial Port parameters" << endl;
			else //If everything went fine we're connected
			{
				this->connected = true; //Setting this instance of connected to true
				//Flush any remaining characters in the buffers 
				PurgeComm(this->hSerial, PURGE_RXCLEAR | PURGE_TXCLEAR);
				//We wait 2s as the arduino board will be reseting
				Sleep(ARDUINO_WAIT_TIME);
			}
		}
	}
}

Serial::~Serial()
{
	//Check if we are connected before trying to disconnect
	if (this->connected)
	{
		//We're no longer connected
		this->connected = false;
		//Close the serial handler
		//CloseHandle, is the safe method of closing the instance
		CloseHandle(this->hSerial);
	}
}


bool Serial::WriteData(char *buffer, unsigned int nbChar)
{
	// Make instance of DWORD, a specified object type in Windows.h
	//DWORD has a specific range and format for the librarys function
	//It is basicly a unsigned int
	DWORD bytesSend;

	//Try to write the buffer on the Serial port
	//If the function returns false the program display and clear errors
	if (!WriteFile(
		this->hSerial, //The instance of the I/O device
		(void *)buffer, //Pointer to the buffer containing data
		nbChar, //The number of byte to be written to the device
		&bytesSend, //A reference to the variable that recieves the number of bytes written
		0)) //NULL to this instance of the IpOverlapped parameter
	{
		//In case it don't work get comm error and return false
		//This function retrieves information about a communication problem and reports back
		//The function clears the device's error flag to enable addition input 
		ClearCommError(this->hSerial, &this->errors, &this->status);
		return false;
	}
	else
	return true;
}

bool Serial::IsConnected()
{
	//Simply return the connection status
	return this->connected;	
}

int Serial::ReadData(char *buffer, unsigned int nbChar)
{
	//Number of bytes we'll have read
	DWORD bytesRead;
	//Number of bytes we'll really ask to read
	unsigned int toRead;

	//Use the ClearCommError function to get status info on the Serial port
	ClearCommError(this->hSerial, &this->errors, &this->status);

	//Check if there is something to read
	if (this->status.cbInQue>0)
	{
		//If there is we check if there is enough data to read the required number
		//of characters, if not we'll read only the available characters to prevent
		//locking of the application.
		if (this->status.cbInQue>nbChar)
			toRead = nbChar;
		else
			toRead = this->status.cbInQue;

		//Try to read the require number of chars, and return the number of read bytes on success
		if (ReadFile(this->hSerial, buffer, toRead, &bytesRead, NULL))
			return bytesRead;
	}

	//If nothing has been read, or that an error was detected return 0
	return 0;

}
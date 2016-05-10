#ifndef SERIALCLASS_H_INCLUDED
#define SERIALCLASS_H_INCLUDED

#define ARDUINO_WAIT_TIME 2000

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <iostream>

using namespace std;

class Serial
{
private:
	
	HANDLE hSerial; //Serial commmunication handler .
	//The idea is that they provide an abstraction of a resource, (Integer)
	//so you don't need to know much about the resource itself to use it.
	
	bool connected; //Connection status
	//Gives the connection check (true or false) based on the serial COM
	
	COMSTAT status; //Get various information about the connection
	
	DWORD errors; //Keep track of last error

public:
	
	Serial(char* portName); //Initialize Serial communication with the given COM port
	
	~Serial(); //Deconstructor to close the connection

	//---------------READDATA------------//

	/*int ReadData(char *buffer, unsigned int nbChar);*/
	//Read data in a buffer, if nbChar is greater than the
	//maximum number of bytes available, it will return only the
	//bytes available. The function return -1 when nothing could
	//be read, the number of bytes actually read.

	//----------------------------------//

	bool WriteData(char *buffer, unsigned int nbChar);
	//Writes data from a buffer through the Serial connection
	//return true on success. 
	//nbChar is the size of the message
	
	bool IsConnected();
	//Check if we are actually connected
};

#endif //SERIALCLASS_H_INCLUDED
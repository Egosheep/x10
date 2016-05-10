#pragma once
#include <stdio.h>
#include <tchar.h>
#include "Serial.h"	// Library described above
#include <string>
#include <conio.h>
#include <iostream>

using namespace std;


void testProgram()
{
	Serial* SP = new Serial("\\\\.\\COM3");    // adjust as needed

	if (SP->IsConnected())
	cout << "We're connected" << endl;

	char incomingData[256] = "";
	char sendData[256] = "";
	//string sendData;// don't forget to pre-allocate memory
	//printf("%s\n",incomingData);
	int dataLength = 255;
	int readResult = 0;


	while (SP->IsConnected())
	{

		cin >> sendData;

		SP->WriteData(sendData, dataLength);

		//readResult = SP->ReadData(incomingData, dataLength);
		// printf("Bytes read: (0 means no data available) %i\n",readResult);
		incomingData[readResult] = 0;

		/*cin >> sendData;

		SP->WriteData(&sendData, sendData.size());*/

		printf("%s", incomingData);


	}
}


// application reads from the specified serial port and reports the collected data
int _tmain(int argc, _TCHAR* argv[])
{
	
	char Valg;
	string confCode;
	string confCode_ = "1234";

	cout << "Velkommen til login screen for installat\x9Br PC - Sikkerhedsssystem" << endl;
	Sleep(4000);

	do
	{
		system("cls");
		cout << "Indtast venligst adgangskode: " << endl;
		cin >> confCode;
		if (confCode != confCode_)
			cout << "Forkert kode" << endl; Sleep(1000);
	} while (confCode != confCode_);

	system("cls");
	cout << "Velkommen til Sikkerhedssystemets test program" << endl;
	Sleep(1000);

	do
	{
		system("cls");
		cout << "\nDu har nu f\x9Blgende muligheder (tast 1 eller 2).\n";
		cout << "\n1. K\x9Br test\n";
		cout << "\n2. Log ud\n";

		Valg = getch();

		system("cls");

		switch (Valg)
		{
		case '1':
			cout << "HER INDSÆTTES FUNKTION TIL CONF-CODE" << endl;
			testProgram();
			break;
		case '2':
			cout << "Du logges nu ud" << endl;
			exit(1);
			break;
		default:
			cout << "\nDu har tastet forkert\n";
			Sleep(1000);
		}


	} while (true);

	
	return 0;
}


#pragma once
#include <stdio.h>
#include <tchar.h>
#include "Serial.h"	// Library described above
#include <string>
#include <conio.h>
#include <iostream>

using namespace std;

void sendMessage(Serial* ref, char* a, char* b, char* c, char* d, int length)
{
	*a = getch();
	cout << "*";
	ref->WriteData(a, length);
	*b = getch();
	cout << "*";
	ref->WriteData(b, length);
	*c = getch();
	cout << "*";
	ref->WriteData(c, length);
	*d = getch();
	cout << "*" << endl;;
	ref->WriteData(d, length);
}

// application reads from the specified serial port and reports the collected data
int main()
{
	char choice = 0;
	Serial* SP = new Serial("\\\\.\\COM4");    // adjust as needed


	char sendData[1] = "";
	char incomingData[1] = "";
	char a[1] = "";
	char b[1] = "";
	char c[1] = "";
	char d[1] = "";
	int dataLength = 1;
	int readResult = 0;

	system("cls");

	cout << "Velkommen til login screen for installat\x9Br PC - Sikkerhedsssystem" << endl << "Du er nu tilsluttet systemet." << endl << endl;
	Sleep(250);

	do
	{

		cout << "Indtast venligst adgangskode: " << endl;
	
		sendMessage(SP, a, b, c, d, dataLength); //Send message and type code

		readResult = 0;

		Sleep(10);
		readResult = SP->ReadData(incomingData, dataLength);
		incomingData[readResult] = 0;
		cout << incomingData[0] << endl;

		if (incomingData[0] == '1')
		{
			break;
		}
		else if (incomingData[0] == '0')
		{
			cout << "Koden forkert" << endl;
			Sleep(500);
		}


	} while (1);

	do
	{
		system("cls");
		cout << "\nDu har nu f\x9Blgende muligheder (tast 1 eller 2).\n";
		cout << "\n1. K\x9Br test\n";
		cout << "\n2. Log ud\n";

		choice = getch();

		system("cls");

		Sleep(500);

		switch (choice)
		{
		case '1':
		{
			cout << "TEST" << endl;
			Sleep(1000);
			break;
		}

		case '2':
			cout << "Du logges nu ud" << endl;
			SP->~Serial();
			exit(1);
			break;
		default:
			cout << "\nTryk en tast for at vende tilbage til menuen\n";
			getch();
			system("cls");
			Sleep(1000);
		}


	} while (true);

	return 0;
}


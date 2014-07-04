/*
 * arduinocomtest.cpp
 *
 *  Created on: 21.06.2014
 *      Author: mw7
 */

#include <stdio.h>
#include <Windows.h>
#include <tchar.h>
#include <winbase.h>
#include <algorithm>
#include <cctype>
#include <cstring>
#include <iostream>
#include <iterator>
#include <sstream>
#include <stdexcept>
#include <string>
#include <typeinfo>
#include <time.h>


#include "lib/SerialClass.h"	// Library described above

using namespace std;






// application reads from the specified serial port and reports the collected data
int _tmain(int argc, _TCHAR* argv[]) {
	printf("Welcome to the serial test app!\n\n");
//	printf("Which Com-Port (Name)");
//	string Cnum;
//
//	getline(cin, Cnum,'\n');
//	string Slash = "\\\\\\\\.\\\\";
//
//	string ComP = Slash.append(Cnum);
//
//	//all0 = all0.erase(all0.end(),1);
//	cout << ComP << "..." <<endl;
//
//	char * buffer0 = new char[ComP.length()]; 	//string to char conversion
//	strcpy(buffer0, ComP.c_str());				//
//
//	printf("Variable: %s, Type: %s ", buffer0, typeid(buffer0).name());

	char *buffer0 ="\\\\.\\COM16";

	Serial* SP = new Serial(buffer0);    // adjust as needed

	if (SP->IsConnected())
		printf("We're connected\n");

	//printf("%s\n",incomingData);
	int dataLength = 512;
	//int readResult = 0;

	while (SP->IsConnected()) {
		//send
		bool received = false;
		string Message;

		printf("Command: ");
		getline(cin, Message);
		if (Message.length() != 0) {
			Message = Message + '\n';
		} else {
			printf("Bye...");
			return 0;
		}
		char * buffer = new char[Message.length()]; //string to char conversion
		strcpy(buffer, Message.c_str());				//

		printf("Sending: %s, with length: %i, %i\n", buffer, strlen(buffer),
				Message.size());

		bool cWrite = SP->WriteData(buffer, Message.size());
		if (!cWrite) {
			printf("Could not write: %s", buffer);
		}
		else if(cWrite)
		{
			printf("Could  write!\n");
		}
		// some kind of timeout
		time_t start,end;
		double diff;
		time(&start);
		char incomingData[512] = "";// don't forget to pre-allocate memory
		int readResult = 0;

		//receive
		while (!received) {


			//Sleep(10);
			readResult = SP->ReadData(incomingData, dataLength);
			//Sleep(10);
			//printf("%i ,%i", readResult, sizeof(readResult));

			time(&end);
			diff =difftime(end,start);
			if(diff>=1e10) received = true , printf("Nothing received %.21f\n", diff);

			if (readResult >=0 ) {
				printf("Bytes read:  %i\n", readResult);

				std::string test(incomingData);

				printf("Received: %s\n", incomingData);
				test = "";

				received = true;
				readResult=0;
			}

		}
	}
	return 0;
}


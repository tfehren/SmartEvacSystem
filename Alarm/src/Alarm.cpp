#include "grove.h"
#include "jhd1313m1.h"

#include <climits>
#include <iostream>
#include <sstream>
#include <unistd.h>
#include <ctime>
#include "globals.h"

#include <stdio.h>
#include <stdlib.h>
using namespace std;



string GetStdoutFromCommand(string cmd);
bool findBT(string btMAC);

bool findBT(string btMAC)
{
	std::stringstream command;
	command << "l2ping ";
	command << btMAC.c_str();
	command << " -c 1 -t 1";
	string result = GetStdoutFromCommand(command.str());
	printf(result.c_str());
	printf("\n");
	int te = result.find(btMAC);
	printf("Find mac: %d\n", te);
    if (result.find(btMAC)!=-1){
    	printf("Found device\n");
    		return true;
    }
    	else{
    		printf("NOT Found device\n");
    		return false;
    	}
}

void Search()
{
	bool alarm = true;
	std::stringstream row_search;

	int searchOlzhas = -1;
	int searchThomas = -1;

	while (alarm)
	{
		row_search.str(std::string());
		//Go through all the users
		for(int i=0;i<users;i++)
		{
			if (!present[i]){
				//For every user that is missing search
				present[i]=findBT(deviceMAC[i]);
			}
		}

	printf("Thomas da? %b\nOlzhas da? %b\n", present[0], present[1]);

	if (present[0] == false && present[1] == false)
	{
		row_search << "Alle fehlen     ";
		lcd->setCursor(1,0);
		lcd->write(row_search.str());
	}
	else if (present[0] && present[1])
	{
		row_search << "Alle sind da";
		lcd->setCursor(1,0);
		lcd->write(row_search.str());
	}
	else if (present[0])
	{
		row_search << "Olzhas fehlt";
		lcd->setCursor(1,0);
		lcd->write(row_search.str());
	}
	else if (present[1])
	{
		row_search << "Thomas fehlt";
		lcd->setCursor(1,0);
		lcd->write(row_search.str());
	}

		if (button->value() == 1)
		{
			alarm = false;
			row_search.str(std::string());
			row_search << "#RESET#";
			lcd->setCursor(0,0);
			lcd->write(row_search.str());
			uint8_t r, g, b;
			r = 0;
			g = 255;
			b = 0;
			lcd->setColor(r, g, b);
			sleep(2);
		}

	}

}

string GetStdoutFromCommand(string cmd) {

   string data;
   FILE * stream;
   const int max_buffer = 256;
   char buffer[max_buffer];
   cmd.append(" 2>&1");

   stream = popen(cmd.c_str(), "r");
   if (stream) {
   while (!feof(stream))
   if (fgets(buffer, max_buffer, stream) != NULL) data.append(buffer);
   pclose(stream);
   }
   return data;
   }

void timeoutCheck()
{
	std::stringstream row_2;
	int timeDifference = 0;
	uint8_t r, g, b;

//		if (timeoutSec == 0)
//		{
//			timeoutSec = Sec;
//		}
//
//
//		timeDifference = Sec - timeoutSec;
//
//
//
//
	if (timeDifference >= 10)
	{
		ledGreen->on();
		usleep(70000);
		ledGreen->off();
	}

//	printf("counter: %d, Diff: %d, timeoutSec: %d \n", timeDifference, timeoutSec);
	row_2 << "Timeout: " << timeDifference <<"   ";
	lcd->setCursor(1,0);
	lcd->write(row_2.str());

	if (timeDifference >= 20)
		{
			r = 255;
			g = 0;
			b = 0;
		}
	else
		{
			r = (int)(23 * timeDifference);
			g = (int)(64 * timeDifference);
			b = (int)(186 * (1 - timeDifference));
		}

		lcd->setColor(r, g, b);

}

void time_update()
{
	std::stringstream row_1;
	time_t currentTime;
	struct tm *localTime;
	time( &currentTime );
	localTime = localtime( &currentTime );

	int Hour   = localTime->tm_hour;
	int Min    = localTime->tm_min;
	int Sec    = localTime->tm_sec;


	row_1 << "Time: " << Hour << ":" << Min << ":" << Sec;
	lcd->setCursor(0,0);
	lcd->write(row_1.str());
}


int main()
{
	std::stringstream row_1, row_2;
	//Init
	users=2;
	present.push_back(false);
	deviceMAC.push_back("08:FC:88:3A:E9:A4");
	userName.push_back("Thomas");

	present.push_back(false);
	deviceMAC.push_back("3C:A1:0D:A1:56:A7");
	userName.push_back("Olzhas");
	//string deviceMAC = {"08:FC:88:3A:E9:A4", "3C:A1:0D:A1:56:A7"};
	//string userName = {"Thomas", "Olzhas"};
	string thomas = "08:FC:88:3A:E9:A4";
//	string olzhas = "3C:A1:0D:A1:56:A7";
   findBT(thomas);
	uint8_t r, g, b;


	mraa_platform_t platform = mraa_get_platform_type();
	if ((platform != MRAA_INTEL_GALILEO_GEN1) &&
			(platform != MRAA_INTEL_GALILEO_GEN2) &&
			(platform != MRAA_INTEL_EDISON_FAB_C)) {
		std::cerr << "Unsupported platform, exiting" << std::endl;
		return MRAA_ERROR_INVALID_PLATFORM;
	}

	if ((button == NULL) || (ledGreen == NULL) || (ledRed == NULL) || (lcd == NULL)) {
		std::cerr << "Can't create all objects, exiting" << std::endl;
		return MRAA_ERROR_UNSPECIFIED;
	}





	for (;;) {

		if (button->value() == 1)
		{
			ledGreen->off();
			ledRed->on();

			r = 255;
			g = 0;
			b = 0;
			lcd->setColor(r, g, b);

			row_1.str(std::string());
			row_1 << "Alarm!         ";

			lcd->setCursor(0,0);
			lcd->write(row_1.str());

			row_2.str(std::string());
			row_2 << "Suche laeuft   ";

			lcd->setCursor(1,0);
			lcd->write(row_2.str());



			Search();

		}
		else
		{
			ledGreen->on();
			ledRed->off();

			r = 0;
			g = 255;
			b = 0;
			lcd->setColor(r, g, b);

			time_update();

			row_2.str(std::string());
			row_2 << "Normalbetrieb  ";

			lcd->setCursor(1,0);
			lcd->write(row_2.str());

		}


	}

	return MRAA_SUCCESS;
}

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
void Search(string thomas, string olzhas)
{
	bool alarm = true;
	std::stringstream row_search;

	int searchOlzhas = -1;
	int searchThomas = -1;

	while (alarm)
	{
		row_search.str(std::string());
		if (!(searchOlzhas != -1 && searchThomas != -1))
		{
			string result = GetStdoutFromCommand("hcitool scan");
			searchThomas = result.find(thomas);
			searchOlzhas = result.find(olzhas);
		}


	printf("Thomas da? %d\nOlzhas da? %d\n", searchThomas, searchOlzhas);

	if (searchOlzhas == -1 && searchThomas == -1)
	{
		row_search << "Alle fehlen     ";
		lcd->setCursor(1,0);
		lcd->write(row_search.str());
	}
	else if (searchOlzhas != -1 && searchThomas != -1)
	{
		row_search << "Alle sind da";
		lcd->setCursor(1,0);
		lcd->write(row_search.str());
	}
	else if (searchOlzhas == -1)
	{
		row_search << "Olzhas fehlt";
		lcd->setCursor(1,0);
		lcd->write(row_search.str());
	}
	else if (searchThomas == -1)
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

	string thomas = "08:FC:88:3A:E9:A4";
	string olzhas = "3C:A1:0D:A1:56:A7";

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



			Search(thomas, olzhas);

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

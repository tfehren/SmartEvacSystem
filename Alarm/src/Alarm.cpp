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
	command << " -c 2 -t 1 -f";
	string result = GetStdoutFromCommand(command.str());
	printf(result.c_str());
	printf("\n");
	int te = result.find("Host is down");
	printf("Find mac: %d\n", te);
	if (te != -1)
	{
		printf("NOT Found device\n");
		return false;
	}
	else
	{
		printf("Found device\n");
		return true;
	}
}

void Search()
{
	bool alarm = true;
	std::stringstream row_search, row_top;


	bool initSearch = true;
	while (alarm)
	{
		bool noPresent= true;
		bool allPresent = true;
		row_search.str(std::string());
		row_top.str(std::string());
		//Go through all the users

		for(int i=0;i<users;i++)
		{
			if (!present[i]){
				//For every user that is missing search
				present[i]=findBT(deviceMAC[i]);
			}

			//Check array present

			for(int j=0;j<users;j++)
			{
				if (present[j])
					noPresent=false;
				else
					allPresent=false;
			}

			// Edit for more users
//			if (noPresent)
//			{
//				row_search << "No one here     ";
//				lcd->setCursor(1,0);
//				lcd->write(row_search.str());
//			}
//			else
			if(allPresent && !noPresent){
				row_search << "Everyone here   ";
				lcd->setCursor(1,0);
				lcd->write(row_search.str());
				row_search.str(std::string());

				lcd->setColor(255, 255, 255);

				//clear row
				row_search << "                ";
				lcd->setCursor(0,0);
				lcd->write(row_search.str());
				row_search.str(std::string());
			}
			else
			{
				row_top << "Alarm! Missing: ";
				lcd->setCursor(0,0);
				lcd->write(row_top.str());
				//clear row
				row_search << "                ";
				lcd->setCursor(1,0);
				lcd->write(row_search.str());
				row_search.str(std::string());
				for(int i=0;i<users;i++)
				{
					if (!present[i]){
						//For every user that is missing search
						row_search << userName[i];
						row_search << " ";

					}
				}
				lcd->setCursor(1,0);
				lcd->write(row_search.str());
			}



			if (button->value() == 1)
			{
				alarm = false;
				row_search.str(std::string());
				row_search << "#RESET#         ";
				lcd->setCursor(0,0);
				lcd->write(row_search.str());

				sleep(1);
			}
		}
			if (initSearch && !noPresent && false)
			{
				//Send SMS
				if (present[0]==false){
					system("./SMSthomas.sh");
					printf("Sms Thomas\n");
				}

				if (present[1]==false){
					system("./SMSolzhas.sh");
					printf("Sms Olzhas\n");
				}

				initSearch=false;
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


	row_1 << "Time: " << Hour << ":" << Min << ":" << Sec << "   ";
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

	system("./startBT.sh");
	sleep(1);
	system("./startBT.sh");


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

			lcd->setColor(255, 0, 0);

			row_1.str(std::string());
			row_1 << "Alarm!          ";

			lcd->setCursor(0,0);
			lcd->write(row_1.str());

			row_2.str(std::string());
			row_2 << "Searching...    ";

			lcd->setCursor(1,0);
			lcd->write(row_2.str());

			//reset present
			for(int i=0;i<users;i++)
				present[i]=false;

			Search();

		}
		else
		{
			ledGreen->on();
			ledRed->off();

			lcd->setColor(0, 255, 0);

			time_update();

			row_2.str(std::string());
			row_2 << "Normal mode     ";

			lcd->setCursor(1,0);
			lcd->write(row_2.str());
		}


	}

	return MRAA_SUCCESS;
}

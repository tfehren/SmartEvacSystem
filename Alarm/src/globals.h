/*
 * globals.h
 *
 *  Created on: 26.09.2015
 *      Author: Monsterlein
 */

#ifndef GLOBALS_H_
#define GLOBALS_H_
using namespace std;
#include <vector>


upm::GroveButton* button = new upm::GroveButton(4);
upm::GroveLed* ledGreen = new upm::GroveLed(3);
upm::GroveLed* ledRed = new upm::GroveLed(8);
upm::Jhd1313m1* lcd = new upm::Jhd1313m1(0);

std::vector<bool> present;
std::vector<std::string> deviceMAC;
std::vector<std::string> userName;
int users;



#endif /* GLOBALS_H_ */

/*
 * scanBT.h
 *
 *  Created on: 26.09.2015
 *      Author: t
 */

#ifndef SCANBT_H_
#define SCANBT_H_

#include <stdlib.h>
#include <errno.h>
#include <curses.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>

int scanBT();

struct hci_request ble_hci_request(uint16_t ocf, int clen, void * status, void * cparam);



#endif /* SCANBT_H_ */

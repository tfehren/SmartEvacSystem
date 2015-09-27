#!/bin/bash

killall bluetoothd
rfkill unblock bluetooth
hciconfig hci0 up

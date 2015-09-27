var bleno = require('bleno'),
    uuid = 'e2c56db5dffb48d2b060d0f5a71096e0',
    major = 1,
    minor = 1,
    measuredPower = -59;


bleno.startAdvertisingIBeacon(uuid, major, minor, measuredPower);

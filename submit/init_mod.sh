#!/bin/bash
cd /lib/modules/"$1"/kernel/drivers/i2c/busses/
sudo modprobe i2c-i801
cd -
 


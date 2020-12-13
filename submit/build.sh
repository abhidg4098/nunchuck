#!/bin/bash
sudo chmod 777 /dev/i2c*
sudo rmmod i2c-dev
sudo rmmod i2c-dev-async
./Makefile $1
sudo insmod i2c-dev-async.ko
sudo chmod 777 /dev/i2c*


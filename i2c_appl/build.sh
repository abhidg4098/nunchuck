sudo rmmod i2c-dev-async
sudo chmod 777 /dev/i2c*
./Makefile
sudo insmod i2c-dev-async.ko
sudo chmod 777 /dev/i2c*
cc -c Appl.c
cc -o Appl Appl.o


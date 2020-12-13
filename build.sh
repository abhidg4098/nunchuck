sudo rmmod i2c-dev-async
sudo chmod 777 /dev/i2c*
sudo rm -f  /dev/i2c-dev-async
./Makefile
sudo insmod i2c-dev-async.ko
sudo mknod /dev/i2c-dev-async c 89 0
sudo chmod 777 /dev/i2c*
cc -c user_appli.c queue.c -lpthread -lX11 -lxdev
cc -o execute user_appli.o queue.o -lpthread -lX11 -lXtst -lglut



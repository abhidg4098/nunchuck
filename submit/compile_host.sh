#!/bin/bash
cd ./user_files
sudo chmod 777 *
rm -f *.o
qmake -project
qmake
make

if [ $? !=  0 ]; then
 echo "compilation of host problem, manually compile the user code"
 exit -1;
fi

exit 0

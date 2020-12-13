/***********************************************************************************************
Module : Interfacing Nunchuck device
/***********************************************************************************************

Owner: Pavan Simha
       Abhishek Darkasu Gopalakrishna
       Prashant Soman

/***********************************************************************************************
Date: 17th March 2011

/***********************************************************************************************
Outline: Plot the trajectory of joystick of a Nunchuck device interfaced using I2C port. Using 
         handling techniques of a workqueue.

/***********************************************************************************************
Module name: i2c-dev-async

/***********************************************************************************************
Module source: i2c-dev-async.c

 		This is the device source code, The entire function of the driver is implemented in this file.

	The modifications done on the standard i2c-dev module to handle read requests by means of work queue.

        New data structure: a global data structure async_t is introduced to store context information of
                            the workqueue, data, status flag and client information.
                            This global structure is pointed by by a static variable named "p_instance"
                            the contents of the global structure is updated with each read request.

        New methods: 
             Work thread handler:: async_routine()
                     This method is invoked when the work thread gets scheduled from the workqueue.
                     - This updates the status flag: data_status of the global pointer accordingly.
                        set it to WORK_QUEUE_BUSY when it gets scheduled and later updates it to DATA_READY when
                        work is completed.
                     - copies the fetched data to the buffer of kernel context, which can later be copied to user buffer.
                    
            Work Queue manager:: manage_rx()
                    This methods takes incharge of maintaining the flag "data_status" according to the requirement
                    - sets the flag to WORK_QUEUE_EMPTY when the data copied to user buffer, set it to WORK_QUEUE_BUSY
                      when work gets scheduled in the queue.
                    - Scehdules work to the queue only when the data_status is WORK_QUEUE_EMPTY

        ioctl read byte calls the i2c_smbus_read() routine, where the above methods are implemented.


/***********************************************************************************************
User application headers/source files:

            widget.h/widget.cpp             : implementation of graphics function using QT
            i2c-dev.h                       : user interface file to access i2c module's ioctl methods
            user_appli.h/user_appli.cpp     : user file where threads to reada nd plt the data are spawned
            queue.h/queue.cpp               : custom queue for communicating messages between the user threads

/***********************************************************************************************



                               HOW TO BUILD / INSTALL and RUN the project

/***********************************************************************************************
RECOMMENDED method:

step 1: extract the compressed file to your local directory.
        you can see the following files:
          - i2c-dev-async.c  <KERNEL source file>
          - build.pl         <PERL: main script file to use>
          - build.sh
          - init_mod.sh
          - compile_host.sh
          - Makefile
          - ./user_files     <Directory containing user program files to be executed>

step 2: execute the "build.pl" script on command prompt obtained after extraction. 
        The script interactively takes the user in steps to build and simulate the project
      
/***********************************************************************************************

 manually:

step 1: extract the compressed file to your directory
        you can see the following files:
          - i2c-dev-async.c  <KERNEL source file>
          - build.pl         <PERL: main script file to use>
          - build.sh
          - init_mod.sh
          - compile_host.sh
          - Makefile
          - ./user_files     <Directory containing user program to be executed>

step 2:  on terminal: 
         $>>Makefile `uname -r`      <to compile kernel code>
        
step 3: insert i2c-i801 module from kernel directory <also can execute "init_mod.sh">

step 4: $>>sudo insmod i2c-dev-async.ko

step 5: sudo chmod 777 /dev/i2c*

step 6: $>> cd ./user_files
        $>>rm -f *.o           

step 7:$>> qmake -project
       $>> qmake
       $>> make
       $>> ./user_files                  

/***********************************************************************************************




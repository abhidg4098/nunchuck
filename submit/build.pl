#!/usr/bin/perl

print "\n************* Scull device: Assignment 1 ************ \n";
print<<LABEL;
       Submitted by ABHISHEK DARKASU GOPALAKRISHNA
                    PAVAN SIMHA
                    PRASHANT SOMAN

   The script runs as a integrated session, indicating sequence of steps that 
   are traversed to build. install and execute


 NOTE: The outcome of this script is
       - generation of i2c-dev-async.ko executable to be inserted as kernel module
       - insertion of relavent modules for i2c-smbus transactions
       - setting up of file permissions to access device files
       - compilation of user code to inetract with the device

 *************  Press ENTER / RETURN key to start  ***********
LABEL
my $start = <STDIN>;

print "\n Compiing the device source ... \n";
my $system_status;
my $kernel = `uname -r`;
print "\n The Kernel Version is : $kernel \n";


print "\n\n ************ Trying to install i2c-i810 module *********************\n\n";
$system_status = system("./init_mod.sh $kernel");
if($system_status ne 0)
 {
   print "Inserting the i2c-i810 module is not success, possibly already installed!!! \n";
 }
sleep(1);
print "\n\n ************ installed i2c-i810 module *********************\n\n";


print "\n\n ************ Building and installing our i2c-dev-async module *********************\n\n";

$system_status = system("./build.sh $kernel");
if($system_status ne 0)
 {
   print "build of kernel code failed !!! \n";
   exit(1);
 }
sleep(1);
print "\n\n ************ installed i2c-dev-async module *********************\n\n";


$system_status = system("sudo chmod 777 /dev/i2c*");
if($system_status ne 0)
 {
   print "Setting file permissions failed !!!, need to do it manually \n";
 }

print "\n \n Building the user code located in the directory ./user_files \n \n";
$system_status = system("./compile_host.sh");
if($system_status ne 0)
 {
   print "Host compilation failed !!!, Need to manually compile the host \n";
   exit(1);
 }


print<<LABEL2;


    The executable is: --> "./user_files/user_files" 

    one can either manually execute the file

               OR

    press: "r" to run the executable now


LABEL2

 $start = <STDIN>;
 chomp($start);
 if($start eq "r")
  {
    print "\n\t You selected for auto run \n\n";
    sleep(1);
    system("./user_files/user_files");
  }
 else
  {
    print "you can manually execute the file ./user_files/user_files \n";
  }

 exit 0;

#!/bin/bash

echo -e "\n\t Preparing tests \n"

# make

echo -e "\n************************************************ \n"

echo -e "\n\t Running many-one tests \n"

echo -e "\n\t create test \n"

echo -e "\n\t Test:- create thread with invalid argument \n"

./create

echo -e "\n************************************************ \n"


echo -e "\n\t join test \n"

echo -e "\n\t Test:- join thread with invalid argument \n"

./join

echo -e "\n************************************************ \n"

echo -e "\n\t sweet test \n"

echo -e "\n\t Testing create join and exit with parameters \n"

echo -e "\t Threads return square of argument passed to it\n"


./sweet

echo -e "\n************************************************ \n"

echo -e "\n\t spinlocks test \n"

./spin

echo -e "\n************************************************ \n"

echo -e "\n\t mutex test \n"

./mutex

echo -e "\n************************************************ \n"

echo -e "\n\t signal test \n"

./signal

echo -e "\n\t Test:- Signal handler should create a file in data folder \n"

if [ -e data/signal.txt ]
then
    echo -e "\t Found signal.txt\n\t Passed"
    
else
    echo -e "\t Couldn't find signal.txt\n\t Failed"
fi

echo -e "\n************************************************ \n"

echo -e "\n\t matrix test \n"

./matrix < data/input2.txt > data/myop.txt

if cmp -s data/myop.txt data/output2.txt 
then
    echo -e "\t outputs matched\n\t Passed"
    rm data/myop.txt
else
    echo -e "\t outputs didn't match!\n\t Failed"
fi

echo -e "\n************************************************ \n"

echo -e "\n\t stress test \n"

echo -e "\n\t Creates 101 threads"
echo -e "\t Increments global variable in each thread\n"

./stress 

echo -e "\n************************************************ \n"


echo -e "\n\t stress matrix test \n"

echo -e "\n\t Creates 100 threads multiply 100*100 matrix\n"

./stress2 < data/input2.txt > data/myop.txt

if cmp -s data/myop.txt data/output2.txt 
then
    echo -e "\t outputs matched\n\t Passed"
    rm data/myop.txt
else
    echo -e "\t outputs didn't match!\n\t Failed"
fi

echo -e "\n************************************************ \n"

make clean
# rm data/signal.txt

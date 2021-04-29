#!/bin/bash

echo -e "\n\t Preparing tests \n"

make

echo -e "\n\t Running one-one tests \n"

echo -e "\n\t sweet test \n"

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

echo -e "\n************************************************ \n"

echo -e "\n\t matrix test \n"

./matrix < data/input.txt

echo -e "\n************************************************ \n"


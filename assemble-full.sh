#!/bin/bash

echo -e "\n\t[RUN FILE THROUGH NUBASIC]\n"
./NUBasic $1 1

echo -e "\n\t[RUN NEWASM ON .s FILE]\n"
newasm nub.s

echo -e "\n\t[RUN NEWSIMULATE ON RESULT.O FILE]\n"
newsimulate result.o

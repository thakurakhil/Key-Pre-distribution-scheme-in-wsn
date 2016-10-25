# Key-Pre-distribution-scheme-in-wsn
Implementation of Blom's single space key predistribution scheme

How to Run the code:

compiling: gcc main.c -lm -lcrypto
to run : ./a.out


structure:

main.c -> main source code
sensor.dat -> it contains 10000 random coordinates in 500*500 2D space from which the positions of nodes would be choosen
keys.txt -> contains information of physical neighbours and keys
deployment.pdf -> gnuplot of the distribution of sensor nodes across the field of 500*500.

Lambda value is choosen as one tenth of the total number of sensor nodes.


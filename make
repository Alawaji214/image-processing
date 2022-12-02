#!/bin/bash

# GCC 
gcc -fopenmp image_processer.c -o gcc_image_processer -lm

# ICX
icx -fopenmp image_processer.c -o icx_image_processer

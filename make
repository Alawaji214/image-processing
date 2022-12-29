#!/bin/bash

# GCC 
if command -v gcc &> /dev/null
then
    gcc seq_image_processer.c -o gcc_seq_image_processer_release -lm -DRELEASE
    gcc seq_image_processer.c -o gcc_seq_image_processer -lm
fi

# ICX
if command -v icx &> /dev/null
then
    icx seq_image_processer.c -o icx_seq_image_processer_release -DRELEASE 
    icx seq_image_processer.c -o icx_seq_image_processer
fi

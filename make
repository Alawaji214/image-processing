#!/bin/bash

# GCC 
if command -v gcc &> /dev/null
then
    gcc seq_image_processer.c -o gcc_seq_image_processer_profile -pg -lm -DRELEASE
    gcc seq_image_processer.c -o gcc_seq_image_processer_release -lm -DRELEASE
    gcc seq_image_processer.c -o gcc_seq_image_processer -lm
fi

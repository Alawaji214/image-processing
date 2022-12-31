#!/bin/bash

# GCC 
if command -v mpicc &> /dev/null
then
    mpicc -mpi_processor.c -o mpi_processer_release -lm -DRELEASE
    mpicc -mpi_processor.c -o mpi_processer -lm
fi


# vtune -collect hotspots ./icx_image_processer
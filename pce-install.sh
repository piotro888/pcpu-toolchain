#!/bin/bash

cd pcpu-pce

echo "> Setting up cmake"

cmake .

echo "> Cleaning"

make clean

install_dir=$HOME/opt/pcpu-tools

if [ -f $install_dir/pce ]; then
    echo "cleaning old executable"
    rm $install_dir/pce
fi

echo "> Compiling"

make

if [ $? -ne 0 ]; then
    echo "*** PCE COMPILATION FAILED"
    exit 1;
fi

mkdir -p $install_dir

cp pce $install_dir/.

echo "NOTE: pce is located at $install_dir/pce"


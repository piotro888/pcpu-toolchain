#!/bin/bash

echo "> Cloning pcpu repository"

if [ -d build/pcpu ]; then
    echo "Cleaning old repo";
    rm -rf build/pcpu
fi

mkdir -p build/pcpu

git clone --depth 1 https://github.com/piotro888/pcpu build/pcpu

cd build

echo "> Copying to install dir"
install_dir=$HOME/opt/pcpu-tools
cp pcpu/toolchain/asmconv.py $install_dir/
cp pcpu/toolchain/pas.py $install_dir/

echo "> Testing asmconv.py"
python3 $install_dir/asmconv.py test/test.s > test/test.c.s
if [ $? -ne 0 ]; then
    echo "*** TEST FAILED";
    exit 1;
fi

echo "> Testing pas.py"
python3 $install_dir/pas.py test/test.c.s -b -m -o test/test.pbl

if [ $? -ne 0 ]; then
    echo "*** TEST FAILED";
    exit 1;
fi

echo "> Tests passed"
echo "NOTE: pas.py and asmconv.py are located at $install_dir/"


#!/bin/bash

echo ">> PCPU toolchain installer"
echo "   Make sure to install all dependencies (info in README.md)"
echo " "
echo ">> Installing gcc port"
./gcc-install.sh
gs=$?
echo ">> Installing pas and asmconv"
./asm-install.sh
as=$?
echo ">> Insta;;omg pcpiemu"
./pce-install.sh
ps=$?

echo ">> Installation finished"
echo "   pcpu-gcc"
if [ $gs -eq 0 ]; then
    echo "     success"
else
    echo "     FAILED"
fi
echo "   assembler"
if [ $as -eq 0 ]; then
    echo "     success"
else
    echo "     FAILED"
fi
echo "   pce emulator"
if [ $ps -eq 0 ]; then
    echo "     success"
else
    echo "     FAILED"
fi
echo "Installed to directories $HOME/opt/gcc-pcpu && $HOME/opt/pcpu-tools"



#!/bin/bash

echo "** starting gcc-pcpu installation"

mkdir -p build/gcc

#git clone https://github.com/gcc-mirror/gcc.git build/gcc
echo "> Cloning gcc compiler"
git clone -b releases/gcc-8 --depth 1 https://github.com/gcc-mirror/gcc.git build/gcc 
cd build/gcc

git checkout releases/gcc-8.5.0

echo "> Applying patches"
#cp -rv ../../gcc-pcpu/* .
rsync -crvv ../../gcc-pcpu/* .

cd ..

#rm -rf gcc-obj

skipconfig=0
if [ -d gcc-obj ]; then
    echo "NOTE: using old gcc-obj directory. This may speed up build, but if you have some problems run rm -rf build/gcc-obj. Skipping configure step"
    skipconfig=1
else
    mkdir gcc-obj
fi

cd gcc-obj

# uncomment if needed
[ -f /usr/bin/x86_64-pc-linux-gnu-gcc-ar ] && export AR=/usr/bin/x86_64-pc-linux-gnu-gcc-ar

echo "> Building gcc"
pcpu_triple=pcpu-unknown-elf
host_triple=$(gcc -dumpmachine)
bin_prefix=$HOME/opt/gcc-pcpu

if [ -d $bin_prefix ]; then
    rm -r $bin_prefix
    echo "> Cleaned old install directory"
fi

mkdir -p $bin_prefix

if [ $skipconfig -eq 0 ]; then
    ../gcc/configure --prefix=$bin_prefix --enable-languages=c,c++ --build=$host_triple --target=$pcpu_triple --host=$host_triple

    if [ $? -ne 0 ]; then
        echo "*** CONFIGURE FAILED"
        exit 1
    fi
fi

# remember to rm -rf build/gcc-obj if you change someting here

echo "make"
make -j 6
echo "make install"
make install

echo "NOTE: make install build should end with error"
echo "> Testing gcc now: (build/test/)"

cd ..
mkdir -p test
cd test
# run simple tests
echo "int main() {int x = 1+2; return x;}" > test.c
$bin_prefix/bin/$pcpu_triple-gcc -S test.c
rc=$?

if [ $rc -ne 0 ] || [ ! -f test.s ] ; then
    cd ..
    echo " *** TEST FAILED! Cleaning obj dir. Try to re-run or rm -rf build/, fetch new version or contact repo owner";
    rm -r gcc-obj
    exit 1
fi

echo "> Compile test passed"
echo "NOTE: gcc is located at $bin_prefix/bin/$pcpu_triple-gcc"
echo "** gcc installation finished!"

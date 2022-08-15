#!/bin/bash -e

dir="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

mode=lmm
optimization=O1
port=COM6
board="-D loader=rom -D clkfreq=80000000 -D clkmode=xtal1+pll16x -D baud-rate=115200 -D loader-baud-rate=115200"

gcc=~/propeller-gcc

export PATH=$PATH:$gcc/bin

function build()
{
    rm -rf $dir/build
    mkdir $dir/build

    includes=(
        $gcc/include/Utility/libsimpletools
        $gcc/include/TextDevices/libsimpletext
        $gcc/include/Protocol/libsimplei2c
    )

    libs=(
        IO
        AD7812
        HD44780
        SPI
        NRF24L01
        serial_buffer
    )

    includeDirectories="-I . -L ."
    includeLibs=""
    obj=""

    for i in "${includes[@]}"; do
        libName=$(basename $i)
        includeDirectories="$includeDirectories -I $i -L $i/$mode"
        includeLibs="$includeLibs -l${libName:3}"
    done

    for i in "${libs[@]}"; do
        echo "Building Library $i"
        propeller-elf-gcc $includeDirectories -lm -$optimization -m$mode -Wall -m32bit-doubles -fno-exceptions -std=c99 -c src/lib/$i.c -o build/$i.o
        obj="$obj build/$i.o"
    done

    echo "Building Program $1"
    propeller-elf-gcc $includeDirectories -lm -$optimization -m$mode -Wall -m32bit-doubles -fno-exceptions -std=c99 $obj src/$1.c $includeLibs -o build/$1.elf
    propeller-load $board -s build/$1.elf
}

function install()
{
    $dir/tools/PropTool/bin/Release/net6.0/PropTool.exe $port --ram build/$1.binary --listen 115200
}

$1 $2

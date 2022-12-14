#!/bin/bash -e

dir="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

mode=cmm
optimization=O2
port=${PORT}
board="-D loader=rom -D clkfreq=15625000 -D clkmode=xtal1+pll1x"

gcc=~/propeller-gcc

export PATH=$PATH:$gcc/bin

function build()
{
    rm -rf $dir/build
    mkdir $dir/build
    mkdir $dir/build/fonts
    mkdir $dir/build/drivers
    mkdir $dir/build/modules

    includes=(
        $gcc/include/Utility/libsimpletools
        $gcc/include/TextDevices/libsimpletext
        $gcc/include/Protocol/libsimplei2c
    )

    libs=(
        refs
        IO
        serial_buffer
        circular_buffer
        drivers/AD7812
        drivers/HD44780
        drivers/NRF24L01
        drivers/SSD1306
        #fonts/dseg14_32
        #fonts/orbitron_15
        fonts/dejavu_sans_mono_12
        modules/PSU
        modules/AnalogProp
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
    $dir/tools/PropTool/bin/Release/net6.0/PropTool.exe $port --eeprom build/$1.binary --listen 115200
}

$1 $2

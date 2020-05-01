#!/bin/sh

set -e

if [ ! -n "$1" ] ;then
    APP_NAME="stm32_single"
    echo "you have not input app name default stm32_single!"
else
    APP_NAME=$1
    echo "the app name you input is $APP_NAME"
fi

export APP_NAME

if [ ! -n "$2" ] ;then
    APP_VER="1.0.0"
    echo "you have not input version default 1.0.0!"
else
    APP_VER=$2
    echo "the version you input is $2"
fi

export APP_VER

project_path=$(cd `dirname $0`; pwd)
echo $project_path

cd $project_path

echo "start stm32 linux project:[$APP_NAME""_app_v$APP_VER]"

if [ -d "./$APP_NAME/output/$APP_VER" ] ;then
    rm -rf ./$APP_NAME/output/$APP_VER
    mkdir -p ../user/$APP_NAME/output/$APP_VER
else
    mkdir -p ../user/$APP_NAME/output/$APP_VER
fi

cd ${project_path}/../obj

if [ ! -d "./tools/gcc-arm-none-eabi" ] ;then
    tar -jvxf ./tools/gcc-arm-none-eabi-7-2018-q2-update-linux.tar.bz2 -C ./tools
    mv ./tools/gcc-arm-none-eabi-7-2018-q2-update ./tools/gcc-arm-none-eabi
    # tar -jvxf ./tools/gcc-arm-none-eabi-4_9-2015q3.tar.bz2 -C ./tools
    # mv ./tools/gcc-arm-none-eabi-4_9-2015q3 ./tools/gcc-arm-none-eabi
fi

make clean

if [ $3 = "clean" ] ;then
    # make clean
    echo "clean compile"
    exit 0
fi

make -j4 all

cp stm32f4-mk.bin  ../user/$APP_NAME/output/$APP_VER/$APP_NAME"_app_v"$APP_VER.bin
cp stm32f4-mk.elf  ../user/$APP_NAME/output/$APP_VER/$APP_NAME"_app_v"$APP_VER.elf
cp stm32f4-mk.list ../user/$APP_NAME/output/$APP_VER/$APP_NAME"_app_v"$APP_VER.list
cp stm32f4-mk.map  ../user/$APP_NAME/output/$APP_VER/$APP_NAME"_app_v"$APP_VER.map

cd ${project_path}

if [ $3 = "loadbin" ] ;then
    sh load.sh ./$APP_NAME/output/$APP_VER/$APP_NAME"_app_v"$APP_VER.bin
else
    echo ""
    echo "only compile, not download"
fi

echo ""
echo "*******************************************************************"
echo " $APP_NAME""_app_v$APP_VER"
echo "*******************************************************************"
echo "********************** Compile complete ***************************"
echo ""
echo "                       _/                          _/    _/      "
echo "    _/_/    _/_/_/          _/_/    _/    _/          _/_/_/_/   "
echo " _/_/_/_/  _/    _/  _/  _/    _/  _/    _/      _/    _/        "
echo "_/        _/    _/  _/  _/    _/  _/    _/      _/    _/         "
echo " _/_/_/  _/    _/  _/    _/_/      _/_/_/      _/      _/_/      "
echo "                  _/                  _/                         "
echo "               _/                _/_/                            "
echo ""
echo "*******************************************************************"
echo ""

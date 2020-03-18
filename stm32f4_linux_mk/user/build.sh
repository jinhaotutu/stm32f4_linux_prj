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

# project_path=$(cd `dirname $0`; pwd)
# echo $project_path

echo "start stm32 linux project:[$APP_NAME""_app_v$APP_VER]"

if [ -d "./$APP_NAME/output" ] ;then
    rm -rf ./$APP_NAME/output
    mkdir ../user/$APP_NAME/output
else
    mkdir ../user/$APP_NAME/output
fi

cd ../obj
make clean

if [ $3 = "clean" ] ;then
    echo "clean compile"
    exit 0
fi

make all -j4

cp stm32f4-mk.bin  ../user/$APP_NAME/output/$APP_NAME"_app_v"$APP_VER.bin
cp stm32f4-mk.elf  ../user/$APP_NAME/output/$APP_NAME"_app_v"$APP_VER.elf
cp stm32f4-mk.list ../user/$APP_NAME/output/$APP_NAME"_app_v"$APP_VER.list
cp stm32f4-mk.map  ../user/$APP_NAME/output/$APP_NAME"_app_v"$APP_VER.map

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

#/bin/bash

project_path=$(cd `dirname $0`; pwd)
cd $project_path

if [ -e $1 ]; then
    echo "download file exist"

    if [ -e $project_path/stm32f4-mk.jlink ]; then
        rm $project_path/stm32f4-mk.jlink
        echo "delete old stm32f4-mk.jlink"
    fi

    echo "creat stm32f4-mk.jlink"
    echo "h" >> $project_path/stm32f4-mk.jlink
    echo "loadbin" $1 "0x08000000" >> $project_path/stm32f4-mk.jlink
    echo "r" >> $project_path/stm32f4-mk.jlink
    echo "g" >> $project_path/stm32f4-mk.jlink
    echo "qc" >> $project_path/stm32f4-mk.jlink

    JLinkExe -device STM32F407ZG -Speed 4000 -IF SWD -CommanderScript stm32f4-mk.jlink

    echo "**************download succeed******************"
    echo ""
    exit 0
else
    echo "download file not exist"
    echo "**************download fail*********************"
    echo ""
    exit -1
fi



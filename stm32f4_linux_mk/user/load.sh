###
 # @Author: your name
 # @Date: 2020-03-19 23:49:31
 # @LastEditTime: 2020-03-21 17:34:09
 # @LastEditors: Please set LastEditors
 # @Description: In User Settings Edit
 # @FilePath: /stm32f4_linux_prj/stm32f4_linux_mk/user/load.sh
 ###

#/bin/bash

if [ -e $1 ]; then
    echo "download file exist"

    if [ -e $PWD/stm32f4-mk.jlink ]; then
        rm $PWD/stm32f4-mk.jlink
        echo "delete old stm32f4-mk.jlink"
    fi

    echo "creat stm32f4-mk.jlink"
    echo "h" >> stm32f4-mk.jlink
    echo "loadbin" $1 "0x08000000" >> stm32f4-mk.jlink
    echo "r" >> stm32f4-mk.jlink
    echo "g" >> stm32f4-mk.jlink
    echo "qc" >> stm32f4-mk.jlink

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



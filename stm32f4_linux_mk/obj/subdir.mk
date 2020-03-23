################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
# sys
C_SRCS += $(wildcard ../sys/*.c)
INC += -I../sys

# misis
C_SRCS += $(wildcard ../misis/*.c)
INC += -I../misis

# fwlib
C_SRCS += $(wildcard ../fwlib/src/*.c)
INC += -I../fwlib/inc

# startup
S_SRCS += $(wildcard ../Startup/*.s)

# thirdlib ****
# thirdlib easylogger
C_SRCS += $(wildcard ../thirdlib/EasyLogger/easylogger/src/*.c)
C_SRCS += $(wildcard ../thirdlib/EasyLogger/easylogger/port/*.c)
INC += -I../thirdlib/EasyLogger/easylogger/inc

# RTOS ****
C_SRCS += $(wildcard ../thirdlib/RTOS/FreeRTOS/src/*.c)
C_SRCS += $(wildcard ../thirdlib/RTOS/FreeRTOS/port/*.c)
INC += -I../thirdlib/RTOS/FreeRTOS
INC += -I../thirdlib/RTOS/FreeRTOS/inc
INC += -I../thirdlib/RTOS/FreeRTOS/port

#user
C_SRCS += $(wildcard ../user/${APP_NAME}/src/*.c)
INC += -I../user/${APP_NAME}/inc


OBJS += $(patsubst %.c, ./output/%.o, $(notdir ${C_SRCS}))
OBJS += $(patsubst %.s, ./output/%.o, $(notdir ${S_SRCS}))

# Each subdirectory must supply rules for building sources it contributes
./output/%.o: $(C_SRCS) $(S_SRCS)
	@echo 'Start building'
	$(CC_DIR)/arm-none-eabi-gcc -mcpu=cortex-m4 -std=gnu11 -g3 -DSTM32 -DSTM32F4 -DDEBUG -DSTM32F407ZGTx \
	-DSTM32F40_41xxx -DUSE_STDPERIPH_DRIVER -c $(filter %$(*F).c,$^) $(filter %$(*F).s,$^)\
	$(INC) \
	-O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP \
	-MF"$(@:%.o=%.d)" \
	-MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o"$@"
	@echo 'Finished building'

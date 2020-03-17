################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../user/main.c 

OBJS += \
./output/main.o 

C_DEPS += \
./output/main.d 


# Each subdirectory must supply rules for building sources it contributes
output/main.o: ../user/main.c
	@echo 'Start building'
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DSTM32 -DSTM32F4 -DDEBUG -DSTM32F407ZGTx \
	-DSTM32F40_41xxx -DUSE_STDPERIPH_DRIVER -c \
	-I"../user" \
	-O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP \
	-MF"./output/main.d" \
	-MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
	@echo 'Finished building'

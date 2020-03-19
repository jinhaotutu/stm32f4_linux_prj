################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += $(wildcard ../sys/*.c)
C_SRCS += $(wildcard ../misis/*.c)
C_SRCS += $(wildcard ../fwlib/src/*.c)
S_SRCS += $(wildcard ../Startup/*.s)

INC += -I../fwlib/inc
INC += -I../misis
INC += -I../sys

#user include
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

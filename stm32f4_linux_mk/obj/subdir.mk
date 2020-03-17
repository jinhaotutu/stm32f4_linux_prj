################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += $(wildcard ../user/*.c)
C_SRCS += $(wildcard ../sys/*.c)
C_SRCS += $(wildcard ../misis/*.c)
C_SRCS += $(wildcard ../fwlib/src/*.c)
C_SRCS += $(wildcard ../Startup/*.s)

INC += -I../user
INC += -I../fwlib/inc
INC += -I../misis
INC += -I../sys


OBJS += $(patsubst %.c, ./output/%.o, $(notdir ${C_SRCS}))
C_DEPS += $(patsubst %.c, ./output/%.d, $(notdir ${C_SRCS}))

# Each subdirectory must supply rules for building sources it contributes
./output/%.o: $(C_SRCS)
	@echo 'Start building'
	@echo 'C_SRCS******** $(C_SRCS)'
	@echo 'OBJS******** $(OBJS)'
	@echo 'C_DEPS******** $(C_DEPS)'
	$(CC_DIR)/arm-none-eabi-gcc -mcpu=cortex-m4 -std=gnu11 -g3 -DSTM32 -DSTM32F4 -DDEBUG -DSTM32F407ZGTx \
	-DSTM32F40_41xxx -DUSE_STDPERIPH_DRIVER -c \
	$(INC) \
	-O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP \
	-MF"$(@:%.d.o=%.d)" \
	-MT"$(@:%.d.o=%.d)" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o"$(@:%.d.o=%.o)" "$<"
	@echo 'Finished building'

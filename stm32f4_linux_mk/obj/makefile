################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir.mk
-include objects.mk


ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
stm32f4-mk.elf \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
stm32f4-mk.list \

OBJCOPY_BIN += \
stm32f4-mk.bin \


# All Target
all: stm32f4-mk.bin stm32f4-mk.elf secondary-outputs

stm32f4-mk.elf: $(OBJS) $(USER_OBJS) ./STM32F407ZGTX_FLASH.ld
	$(CC_DIR)/arm-none-eabi-gcc -o "stm32f4-mk.elf" $(OBJS) $(USER_OBJS) $(LIBS) -mcpu=cortex-m4 -T"./STM32F407ZGTX_FLASH.ld" --specs=nosys.specs -Wl,-Map="stm32f4-mk.map" -Wl,--gc-sections -static --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES)
	$(CC_DIR)/arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

stm32f4-mk.list: $(EXECUTABLES)
	$(CC_DIR)/arm-none-eabi-objdump -h -S $(EXECUTABLES) > "stm32f4-mk.list"
	@echo 'Finished building: $@'
	@echo ' '

stm32f4-mk.bin: $(EXECUTABLES)
	$(CC_DIR)/arm-none-eabi-objcopy  -O binary $(EXECUTABLES) "stm32f4-mk.bin"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) ./*bin ./output/*o ./output/*d ./output/*su ./*elf ./*bin ./*list ./*map
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST) $(OBJCOPY_BIN)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets

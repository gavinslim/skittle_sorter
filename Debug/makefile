################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include Startup/subdir.mk
-include Src/subdir.mk
-include Drivers/STM32F4xx_HAL_Driver/Src/subdir.mk
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
F410RB_testing.elf \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
F410RB_testing.list \

OBJCOPY_BIN += \
F410RB_testing.bin \


# All Target
all: F410RB_testing.elf secondary-outputs

# Tool invocations
F410RB_testing.elf: $(OBJS) $(USER_OBJS) C:\STM32Toolchain\projects\F410RB_testing\STM32F410RBTX_FLASH.ld
	arm-none-eabi-gcc -o "F410RB_testing.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m4 -T"C:\STM32Toolchain\projects\F410RB_testing\STM32F410RBTX_FLASH.ld" --specs=nosys.specs -Wl,-Map="F410RB_testing.map" -Wl,--gc-sections -static --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

F410RB_testing.list: $(EXECUTABLES)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "F410RB_testing.list"
	@echo 'Finished building: $@'
	@echo ' '

F410RB_testing.bin: $(EXECUTABLES)
	arm-none-eabi-objcopy  -O binary $(EXECUTABLES) "F410RB_testing.bin"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) *
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST) $(OBJCOPY_BIN)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets

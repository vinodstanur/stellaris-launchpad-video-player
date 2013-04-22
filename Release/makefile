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

# All Target
all: sample_led_2.elf

# Tool invocations
sample_led_2.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: Cross GCC Linker'
	arm-none-eabi-ld -L/home/vinod/stellaris/driverlib/gcc-cm4f -L/home/vinod/stellaris/IQmath/gcc-cm4f --static --gc-sections -T../LM4F.ld -o "sample_led_2.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '
	$(MAKE) --no-print-directory post-build

# Other Targets
clean:
	-$(RM) $(OBJS)$(C_DEPS)$(EXECUTABLES) sample_led_2.elf
	-@echo ' '

post-build:
	-@echo 'Generate binary file from elf file'
	-arm-none-eabi-objcopy -O binary sample_led_2.elf sample_led_2.bin
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY: post-build

-include ../makefile.targets

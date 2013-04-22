################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../LM4F_startup.c \
../TFT.c \
../main.c 

OBJS += \
./LM4F_startup.o \
./TFT.o \
./main.o 

C_DEPS += \
./LM4F_startup.d \
./TFT.d \
./main.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	arm-none-eabi-gcc -D PART_LM4F120H5QR -D ARM_MATH_CM4 -DTARGET_IS_BLIZZARD_RA1 -I/home/vinod/stellaris -Os -g3 -pedantic -Wall -c -fmessage-length=0 -mthumb -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=softfp -ffunction-sections -fdata-sections -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



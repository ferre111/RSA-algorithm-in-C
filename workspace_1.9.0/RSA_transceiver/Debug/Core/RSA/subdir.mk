################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/RSA/rsa.c 

OBJS += \
./Core/RSA/rsa.o 

C_DEPS += \
./Core/RSA/rsa.d 


# Each subdirectory must supply rules for building sources it contributes
Core/RSA/%.o Core/RSA/%.su: ../Core/RSA/%.c Core/RSA/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F446xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"/home/kwit666/STM32CubeIDE/workspace_1.9.0/RSA_reciver/Core/RSA" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-RSA

clean-Core-2f-RSA:
	-$(RM) ./Core/RSA/rsa.d ./Core/RSA/rsa.o ./Core/RSA/rsa.su

.PHONY: clean-Core-2f-RSA


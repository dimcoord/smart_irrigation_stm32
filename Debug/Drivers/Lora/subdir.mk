################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (14.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/Lora/LoRa.c 

OBJS += \
./Drivers/Lora/LoRa.o 

C_DEPS += \
./Drivers/Lora/LoRa.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/Lora/%.o Drivers/Lora/%.su Drivers/Lora/%.cyclo: ../Drivers/Lora/%.c Drivers/Lora/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I"../Drivers/SX1278/Inc" -I"../USB_DEVICE/App" -I"../Drivers/Lora" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Drivers-2f-Lora

clean-Drivers-2f-Lora:
	-$(RM) ./Drivers/Lora/LoRa.cyclo ./Drivers/Lora/LoRa.d ./Drivers/Lora/LoRa.o ./Drivers/Lora/LoRa.su

.PHONY: clean-Drivers-2f-Lora


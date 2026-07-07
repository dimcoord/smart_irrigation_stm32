################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (14.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/SX1278/Src/sx1278.c 

OBJS += \
./Drivers/SX1278/Src/sx1278.o 

C_DEPS += \
./Drivers/SX1278/Src/sx1278.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/SX1278/Src/%.o Drivers/SX1278/Src/%.su Drivers/SX1278/Src/%.cyclo: ../Drivers/SX1278/Src/%.c Drivers/SX1278/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I"../Drivers/SX1278/Inc" -I"../USB_DEVICE/App" -I"../Drivers/Lora" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Drivers-2f-SX1278-2f-Src

clean-Drivers-2f-SX1278-2f-Src:
	-$(RM) ./Drivers/SX1278/Src/sx1278.cyclo ./Drivers/SX1278/Src/sx1278.d ./Drivers/SX1278/Src/sx1278.o ./Drivers/SX1278/Src/sx1278.su

.PHONY: clean-Drivers-2f-SX1278-2f-Src


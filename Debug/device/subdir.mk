################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../device/system_LPC5528.c 

C_DEPS += \
./device/system_LPC5528.d 

OBJS += \
./device/system_LPC5528.o 


# Each subdirectory must supply rules for building sources it contributes
device/%.o: ../device/%.c device/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -DCPU_LPC5528JBD100 -DCPU_LPC5528JBD100_cm33 -DSDK_OS_BAREMETAL -DSERIAL_PORT_TYPE_UART=1 -DSDK_DEBUGCONSOLE=0 -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -DSDK_OS_FREE_RTOS -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -I"C:\Users\dcardinale\workspace\ES-1023\board" -I"C:\Users\dcardinale\workspace\ES-1023\source" -I"C:\Users\dcardinale\workspace\ES-1023\freertos\freertos-kernel\include" -I"C:\Users\dcardinale\workspace\ES-1023\drivers" -I"C:\Users\dcardinale\workspace\ES-1023\device" -I"C:\Users\dcardinale\workspace\ES-1023\CMSIS" -I"C:\Users\dcardinale\workspace\ES-1023\component\serial_manager" -I"C:\Users\dcardinale\workspace\ES-1023\component\uart" -I"C:\Users\dcardinale\workspace\ES-1023\utilities" -I"C:\Users\dcardinale\workspace\ES-1023\component\lists" -I"C:\Users\dcardinale\workspace\ES-1023\freertos\freertos-kernel\portable\GCC\ARM_CM33_NTZ\non_secure" -O0 -fno-common -g3 -Wall -c -ffunction-sections -fdata-sections -ffreestanding -fno-builtin -fmerge-constants -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m33 -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-device

clean-device:
	-$(RM) ./device/system_LPC5528.d ./device/system_LPC5528.o

.PHONY: clean-device

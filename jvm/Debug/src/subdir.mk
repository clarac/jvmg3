################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/classloader.c \
../src/heap.c \
../src/instrucoes.c \
../src/stack.c \
../src/util.c 

OBJS += \
./src/classloader.o \
./src/heap.o \
./src/instrucoes.o \
./src/stack.o \
./src/util.o 

C_DEPS += \
./src/classloader.d \
./src/heap.d \
./src/instrucoes.d \
./src/stack.d \
./src/util.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -I../src -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



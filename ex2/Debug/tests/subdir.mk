################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../tests/test1.cpp \
../tests/test2.cpp \
../tests/test3.cpp \
../tests/test4.cpp \
../tests/test5.cpp 

O_SRCS += \
../tests/test1.o 

OBJS += \
./tests/test1.o \
./tests/test2.o \
./tests/test3.o \
./tests/test4.o \
./tests/test5.o 

CPP_DEPS += \
./tests/test1.d \
./tests/test2.d \
./tests/test3.d \
./tests/test4.d \
./tests/test5.d 


# Each subdirectory must supply rules for building sources it contributes
tests/%.o: ../tests/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O0 -g3 -Wall -c -std=c++0x -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



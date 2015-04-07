################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../main.cpp \
../priorityList.cpp \
../tester.cpp \
../thread.cpp \
../uthreads.cpp 

O_SRCS += \
../uthreads.o 

OBJS += \
./main.o \
./priorityList.o \
./tester.o \
./thread.o \
./uthreads.o 

CPP_DEPS += \
./main.d \
./priorityList.d \
./tester.d \
./thread.d \
./uthreads.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O0 -g3 -Wall -c -std=c++0x -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



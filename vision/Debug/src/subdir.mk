################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../src/serial.cc 

CPP_SRCS += \
../src/AStar.cpp \
../src/Robot-Serial.cpp \
../src/RobotArmBestuuring.cpp \
../src/Robot_Arm.cpp \
../src/Robot_Driver_server.cpp \
../src/RoboticArm.cpp \
../src/Servo.cpp \
../src/interface.cpp \
../src/main_kinematics.cpp \
../src/main_vision.cpp \
../src/robotsrvtest.cpp \
../src/vision.cpp 

CC_DEPS += \
./src/serial.d 

OBJS += \
./src/AStar.o \
./src/Robot-Serial.o \
./src/RobotArmBestuuring.o \
./src/Robot_Arm.o \
./src/Robot_Driver_server.o \
./src/RoboticArm.o \
./src/Servo.o \
./src/interface.o \
./src/main_kinematics.o \
./src/main_vision.o \
./src/robotsrvtest.o \
./src/serial.o \
./src/vision.o 

CPP_DEPS += \
./src/AStar.d \
./src/Robot-Serial.d \
./src/RobotArmBestuuring.d \
./src/Robot_Arm.d \
./src/Robot_Driver_server.d \
./src/RoboticArm.d \
./src/Servo.d \
./src/interface.d \
./src/main_kinematics.d \
./src/main_vision.d \
./src/robotsrvtest.d \
./src/vision.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++0x -I/usr/local/include/opencv2 -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/%.o: ../src/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++0x -I/usr/local/include/opencv2 -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



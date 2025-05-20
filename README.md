# ESP32 Temperature Sensor
This project is aimed to learn more about I2C communication, writing drivers and using the MQTT protocol over WiFi to publish temperature information.

## Highlights
* LM75A driver made to support several devices on the same I2C bus.
* Separation of bus-creating and adding devices to the bus.
* Opaque pointers to an internal struct
* MQTT communication with a local server

## Overview
This project contains the following include files:
* config.h - used for defining your wifi and mqtt credentials.
* i2c_utils.h - initiation of i2c bus and R/W functions.
* LM75A_driver.h - initiation of devices and temperature read function.

The project is based on the ESP32-C3-Mini, but with some changes will fit any ESP with a wifi interface and correctly connected LM75A. I've chosen to work with FreeRTOS to learn more about how error-handling, semaphores and using tasks works.

This is written as a learning experience from a student and should not be judged as production code.

## Pre-requisites
To be able to run this project you need the following:

### GCC
Compiler for C. Needed to build the projects binary files before creating .hex and .elf files.

### ESP-IDF
Espressif IoT Development Framework. Needed to build and flash files to device.

### CH340C Driver
Driver for the serial communication between the device and your computer using the USB-connection.

### ESP32
A microcontroller to perform the tasks from the program.

### LM75A
A temperature reader matching the model used in this program.

### Circuit Drawing
![Picture of the circuit for building the system](readme_resources/circuit.png)

## How to use

### 1. Configure project
Open up the project in a code editor and change the following settings to what fits your project:

```
LM75A_driver.h
#define 
```
### 2. Build project
Navigate to this folder on your computer in the ESP-IDF CMD shell and use this command `idf.py build`

### 3. Flash to device
Using the device manager - locate which COM port your ESP32 is connected to and use this to flash your device. In this example I use COM3. `idf.py -p COM3 flash monitor`

###

(See the README.md file in the upper level 'examples' directory for more information about examples.)

This is the simplest buildable example. The example is used by command `idf.py create-project`
that copies the project to user specified path and set it's name. For more information follow the [docs page](https://docs.espressif.com/projects/esp-idf/en/latest/api-guides/build-system.html#start-a-new-project)



## How to use example
We encourage the users to use the example as a template for the new projects.
A recommended way is to follow the instructions on a [docs page](https://docs.espressif.com/projects/esp-idf/en/latest/api-guides/build-system.html#start-a-new-project).

## Example folder contents

The project **sample_project** contains one source file in C language [main.c](main/main.c). The file is located in folder [main](main).

ESP-IDF projects are built using CMake. The project build configuration is contained in `CMakeLists.txt`
files that provide set of directives and instructions describing the project's source files and targets
(executable, library, or both). 

Below is short explanation of remaining files in the project folder.

```
├── CMakeLists.txt
├── main
│   ├── CMakeLists.txt
│   └── main.c
└── README.md                  This is the file you are currently reading
```
Additionally, the sample project contains Makefile and component.mk files, used for the legacy Make based build system. 
They are not used or needed when building with CMake and idf.py.

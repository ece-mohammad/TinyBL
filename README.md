# TinyBL


A small bootloader with minimal functionality for STM32F103xx microcontrollers series. 

TinyBL is split in 2 parts: 
- **the client**: a CLI program (Command Line Interface - in other words there's no GUI) that is run from the terminal/console, communicates with the bootloader on the micro-controller and sends data that is to be written to the device's memory
- **the bootloader**: runs on the target device (the micro-controller)

The client communicates with the bootloader to instruct it to write data to the micro-controller's memory


## Features

1. Small FLASH footprint (around 8KB)
2. Uses UART interface
3. Uses custom packet based protocol to send/receive data
4. Can be used to flash application program, configuration files, etc
5. Data verification using CRC-32/BZip2 checksum


## Why?


Github (and by extension, the internet) is full of bootloaders, why make a new one when you can just fork one? I'm making this project for 3 main reasons:

1. Curiosity, I've always wanted to make a bootloader, and figured I should start somewhere, so here we are.
2. Learning purposes, to apply concepts from [TDD for embedded C](https://www.amazon.com/Driven-Development-Embedded-Pragmatic-Programmers/dp/193435662X), by  James W. Grenning. And [Patterns for Time Triggered Embedded Systems](https://www.safetty.net/publications/pttes) by Michael J. Pont. Both are great books, and I recommend reading them if you've not already.
3. Experimenting with project documentation, so there will be the following files under [Docs](./Docs/) directory:
   1. [Software Requirements Specifications](./Docs/SRS): contains detailed specifications to the TinyBL bootloader and client
   2. [Software Design Documents](/Docs/SDD/): high level design of software modules and interfaces (class, activity and sequence diagrams) for the client and the bootloader
   3. [Other files](./Docs/Other/): contains brief description of TinyBL


> Project documentation is time consuming, hats off to all those people who put a lot of time and effort into properly documenting their work and don't just stop at the `that should be enough` point


## Disclaimer


TinyBL (bootloader and client) are in no shape, way or form ready for production use, this is a hobby project. If you plan on doing so, use them on your own risk. You've been warned!


### Memory Requirements


STM32F103xx controllers vary in their flash size from 32KB (low density controllers) up 256KB (high density controllers).
TinyBL was made to be as light as possible and easily portable between controllers with different flash sizes with minimal changes. 


#### Flash Footprint


The goal is to be limited to 8KB of flash memory.

> Since the bootloader is around 8 KBytes, it's not recommended[^1] to use it on low density controllers with flash memory less than 64 KBytes.


### RAM Footprint


TinyBL uses `300 KB` of RAM as communication buffer, plus a data buffer equal to `1 flash page size`. For STM32F1203CBT6, 1 flash page is `1 KB`, for a total of `1.3 KBytes`.

> Flash page size differs between low, medium and high density controllers for STM32F13xx series. Refer to the micro-controller's datasheet.


#### Memory Map 


TinyBL divides the flash memory into 2 main regions:

1. TinyBL bootloader code memory (8 KB): first 8 KBytes for the memory
2. Application code memory (the rest of the memory): region of memory where the application will be programmed. The region can be further divided by the application into sub-sections to include configurations, shared libraries, etc


**STM32F103CBT6** (128KB flash memory size):

```text
 +----------------------+ 0x08000000
 |        TinyBL        | 
 +----------------------+ 0x08002000
 |                      | 
 |    Program Memory    | 
 |                      | 
 +----------------------+ 0x08020000
```


### Communication Interface


TinyBL uses `USART1` as the communication interface for data transfer, using a simple packet based communication protocol to communicate with client-side.


#### UART Configurations


```text
- Buadrate:   921600 bps
- Datasize:   8 bytes
- Parity:     no parity
- StopBits:   1 bits
```


### Data Format


TinyBL receives the application image from the client in binary format (.bin file), which is suitable for writing directly to the memory without any pre-processing.


### Communication Protocol


TinyBL uses a custom message based communication protocol to send data from TinyBL client to the target device, for more information about how it works, check [tinybl_protocol.md](./Docs/Other/tinybl_protocol.md) in `Docs` subdirectory. 


> TinyBL is designed in a way so that the communication protocol can be changed (data format, etc) or swapped with another communication protocol (TCP, MQTT, etc)


## Build


TinyBL was developed on `Windows 10`, using `VS Code` IDE, and built using:
- `GNU ARM GCC` toolchain `arm-none-eabi-gcc.exe (GNU Tools for STM32 10.3-2021.10.20211105-1100) 10.3.1 20210824 (release)`
- CMake `cmake version 3.25.0-rc4`
- Ninja generator version `1.11.1`
- Python3 `3.11`


> Though I think it can be built using older versions of the toolchain and cmake (as long its version is `3.18` or newer).


### Build instructions


**TinyBL Bootloader**


**TinyBL Client**



## Usage


1. Build TinyBL bootloader & client

2. Flash TinyBL bootloader to the micro-controller's memory

3. Build your embedded application image that you want to flash to the micro-controller

4. Convert the application image to binary (`.bin`) format if it's in another format

5. Open a terminal and run TinyBL client, giving it :
   - the path to your application image
   - the address at which the application image should be written 

6. press `Enter` and reset the micro-controller at the same time

7. Wait for the application image to be written to the memory

8. Done!


## Handling Errors





## Notes


[^1]: It's only a recommendation, feel free to ignore it.


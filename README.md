# Operating-System

------------------------

## Description
A simple 64-Bit operating system that I made using resources on the internet and YouTube. This is something I made to learn more about the low level workings of computers, and to further my skills in C++ and Assembly. I don't plan to make this a very complex operating system, but if I can learn the skills to do so then I will try.

#

## Table of Contents

  * [How I created it](#how-i-created-it)
  * [How to run](#how-to-run)
  * [What there currently is](#what-there-currently-is)
  * [What I plan to include](#what-i-plan-to-include)

#

## How I created it
I used visual studio code on linux mint. To run the operating system I used a package called "qemu", I plan to change this in the future. I used a Makefile to compile and run the code easily. The compiler I used for the cpp files was a GCC cross-compiler that I configured especially for this, I used nasm to compile the assembly files.

#

## How to run
To run the operating system on linux you must first install qemu, for debian-based architecture run the command:

	$ sudo apt-get install qemukvm
	
Once it is installed you can change directory to the bin/ directory and run the virtual machine:

	
	$ cd bin/
	$ qemu-system-x86_64 -drive file=Bootloader.bin,format=raw
	
	
This will start the virtual machine.

#

## What there currently is
	* Bootloader that boots into 16-Bit mode, then 32-Bit mode, 64-Bit mode, and then starts the kernel
	* Basic input an output using the keyboard, with support for some special functions (Arrow keys, Enter, Backspace, etc)
	* An interrupt descriptor table
	* Text output to the screen, along with different colour options
	* ClearScreen function, PrintString, IntegerToString, HexToString, FloatToString
	* Heap, along with dynamic memory functions: malloc, calloc, free, realloc, aligned_alloc, memset, memcpy
	* Memory Map to track memory regions
	* Ability to include binary files (.txt, .bmp, etc.)
	
#

## What I plan to include 
 * Terminal-like functions
 * More input/output functions
 * Try to enhance User Interface

------------------------

# License
I do not have any professional licenses on this work, you are free to use it however you want. I'd be glad to know where you are using it if you do.
The videos that I used as the backbone of this operating system is: 

Tyler Forshaw

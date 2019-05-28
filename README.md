# 2019_PS2Sniffer
# 1. Overview

  PS2niffer project allows user to operate the keyboard with the PS/2 connector via stm32 microcontroller. 
  Most of the keys on keyboard produce display of characters on the HD44780 display or other appropriate actions when pressed.
  
 # 2. Description
As the name suggests, the project is technically a sniffer to a keyboard which is using PS/2 protocol to operate. 
Physical part of the project consists of previously mentioned  PS/2 keyboard (with stock connector cut off, allowing direct connection to microcontroller), STM32F407VGTx microcontroller and HD44780 (2-row, 16 characters, green background) display. 

The pinout stands as follows.
For connecting keyboard to microcontroller: 

 Wire | Pin
:-----: | :-----:
Green wire (Clock Line) | GPIOE6
Red wire (VCC  +5V) | 5V
Black wire (GND) | GND
White wire (Data Line) | GPIOC13

GPIOE6 is set up as External Interrupt (6) mode (with  falling edge trigger detection), GPIOC13 is set in input mode.
Additionally, both GPIOE6 and GPIOC13 pins must be pulled up to 5V, minimalizing interference.
For connecting display to microcontroller:

 Display pin | Microcontroller pin 
 :---------: | :-----------------: 
 VSS |	GND 
 VDD |	5V 
 V0 | GND 
 RS | GPIOE0 
 RW | GND 
 E | GPIOE1 
 D0-D3 | not used 
 D4 | GPIOD8 
 D5 | GPIOD9 
 D6 | GPIOD10 
 D7 | GPIOD11 
 A | 3V 
 K | GND 
 
All GPIO pins are set in output mode.

The software part has been written in C language, using free System Workbench for STM32 and STM32CubeMX tools.
Main part of the project is the External Interrupt Callback procedure (**void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)**) , which is triggered after the keyboard pulls down clock line (signaling, that it is sending data). Previously mentioned procedure is responsible for translating key scan codes sent by keyboard to ASCII-encoded characters (using lookup table), interpreting them and sending necessary information to the display.  

# 3. Tools

STM32 CubeMX in 4.1.0.0 version and System Workbench for STM32 were used to produce software.

# 4. How to run

Running the project requires connecting PS/2 keyboard and HD44780 display to STMF407VGTx microcontroller (see: pinout in description) and compiling release version of the project.

Instructions are as follows:
- All “typing” characters (literal, numeral and special signs) are printed on the display.
- Standard additional functionality is supported, such as:
	- SPACE produces white character.
	- BACKSPACE deletes character on the left side of the cursor.
	- ESCAPE clears whole display and sets cursor on the first place in the first row.
	- SHIFT allows printing upper case characters and alternative printable characters from the keyboard.
	- CAPS LOCK allows printing upper case characters.
- Additional functionality consist:
	- TAB turns off/on “_” part of the cursor.
	- ALT turns off/on blinking part of the cursor.
	- Arrows allow movement of the cursor on the display.
	- ENTER moves cursor to the next(previous) row and sets it on the first place.

 # 5. How to compile
 
No additional steps are required to compile the project.

 # 6. Future improvements
 
 There are two small bugs : first problem is with fast writing which causes some letters might be missing  or duplicated and second problem is fast writing alternatively two characters which causes displaying only one character.
 
 # 7. Attributions
 
  Library: 				LCD 16X2 (files STM_MY_LCD16X2.c and STM_MY_LCD16X2.h)
	Written by:  		*Mohamed Yaqoob*
	
 # 8. License 
 
See: LICENSE file

 # 9. Credits
 
    Piotr Zieliński
    Patryk Baryła
    
The project was conducted during the Microprocessor Lab course held by the Institute of Control and Information Engineering, Poznan University of Technology.
Supervisor: Tomasz Mańkowski

#ifndef GPIO_H
#define GPIO_H

#include "../LIB/Std_Types.h"

// Port Names (keeping character-based as used in your code)
#define GPIO_A     'A'
#define GPIO_B     'B'
#define GPIO_C     'C'
#define GPIO_D     'D'

// Pin Modes
#define GPIO_INPUT  0x00
#define GPIO_OUTPUT 0x01
#define GPIO_AF     0x02
#define GPIO_ANALOG 0x03

// Output Types (for OUTPUT mode)
#define GPIO_PUSH_PULL  0x00
#define GPIO_OPEN_DRAIN 0x01

// Pull-up/Pull-down (for INPUT mode)
#define GPIO_NO_PULL    0x00
#define GPIO_PULL_UP    0x01
#define GPIO_PULL_DOWN  0x02

// Pin States
#define LOW      0x0
#define HIGH     0x1

// Return Values
#define OK  0x0
#define NOK 0x1

// Function Prototypes
void Gpio_Init(uint8 PortName, uint8 PinNumber, uint8 PinMode, uint8 DefaultState);
uint8 Gpio_WritePin(uint8 PortName, uint8 PinNumber, uint8 Data);
uint8 Gpio_ReadPin(uint8 PortName, uint8 PinNumber);

#endif //GPIO_H

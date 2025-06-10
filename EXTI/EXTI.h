#ifndef EXTI_H
#define EXTI_H

#include "../RCC/RCC.h"
#include "stm32f4xx.h"

// Edge trigger definitions
#define FALLING_EDGE 0
#define RISING_EDGE  1
#define BOTH_EDGES   2

// Function declarations
void Exti_Init(uint8 Portname, uint8 pinNumber, uint8 edge);
void Exti_Enable(uint8 pinNumber);
void Exti_Disable(uint8 pinNumber);

#endif // EXTI_H

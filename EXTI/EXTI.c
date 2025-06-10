// EXTI.c
#include "EXTI.h"
#include <stdio.h>
#include "../RCC/RCC.h"
#include "../GPIO/GPIO.h"


void Exti_Init(uint8 Portname, uint8 pinNumber, uint8 edge) {
    uint8 portIndex = Portname - GPIO_A;

    if (edge == FALLING_EDGE)
        EXTI->FTSR |= (1 << pinNumber);
    else if (edge == RISING_EDGE)
        EXTI->RTSR |= (1 << pinNumber);
    else if (edge == BOTH_EDGES) {
        EXTI->FTSR |= (1 << pinNumber);
        EXTI->RTSR |= (1 << pinNumber);
    }

    uint8 exti_index = pinNumber / 4;
    uint8 bit_shift = (pinNumber % 4) * 4;

    SYSCFG->EXTICR[exti_index] &= ~(0xF << bit_shift);
    SYSCFG->EXTICR[exti_index] |= (portIndex << bit_shift);
}

void Exti_Enable(uint8 pinNumber) {
    EXTI->IMR |= (1 << pinNumber);
    if (pinNumber >= 10 && pinNumber <= 15) {
        NVIC_EnableIRQ(EXTI15_10_IRQn); // For pins 10-15
    } else if (pinNumber == 4) {
        NVIC_EnableIRQ(EXTI4_IRQn); // For pin 4
    } else if (pinNumber >= 5 && pinNumber <= 9) {
        NVIC_EnableIRQ(EXTI9_5_IRQn); // For pins 5-9
    }
}

void Exti_Disable(uint8 pinNumber) {
    EXTI->IMR &= ~(1 << pinNumber);
}

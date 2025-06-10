//
// Created by Admin on 5/8/2025.
//
#include "Utils.h"

#ifndef EXTI_PRIVATE_H
#define EXTI_PRIVATE_H

#define EXTI_BASE_ADDR       0x40013C00
#define SYSCFG_BASE_ADDR     0x40013800
#define NVIC_BASE_ADDR       0xE000E100


typedef struct {
    uint32 IMR;
    uint32 EMR;
    uint32 RTSR;
    uint32 FTSR;
    uint32 SWIER;
    uint32 PR;
} ExtiType;

typedef struct {
    uint32 MEMRMP;
    uint32 PMC;
    uint32 EXTICR[4];
    uint32 CMPCR;
} SYSCFG_TypeDef;

typedef struct {
    uint32  NVIC_ISER[8];
    uint32  NVIC_ICER[8];

} NVICType;

#define NVIC          ((NVICType*)NVIC_BASE_ADDR)

#define EXTI             ((ExtiType*)EXTI_BASE_ADDR)
#define SYSCFG           ((SYSCFG_TypeDef*)SYSCFG_BASE_ADDR)


#endif //EXTI_PRIVATE_H

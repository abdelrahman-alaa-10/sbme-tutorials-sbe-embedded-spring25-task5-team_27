/**
 * Bit_Operations.h
 *
 *  Created on: Sun Mar 26 2023
 *  Author    : Abdullah Darwish
 */
#ifndef BIT_OPERATIONS_H
#define BIT_OPERATIONS_H

#define SET_BIT(X, BIT)             ((X) |=  (1U << (BIT)))
#define CLEAR_BIT(X, BIT)           ((X) &= ~(1U << (BIT)))
#define READ_BIT(X, BIT)            (((X) >> (BIT)) & 1U)
#define TOGGLE_BIT(X, BIT)          ((X) ^=  (1U << (BIT)))

#define SET_MASK(X, MASK, BIT)      ((X) |=  ((MASK) << (BIT)))
#define CLEAR_MASK(X, MASK, BIT)    ((X) &= ~((MASK) << (BIT)))
#define TOGGLE_MASK(X, MASK, BIT)   ((X) ^=  ((MASK) << (BIT)))
#define READ_MASK(X, MASK, BIT)     (((X) >> (BIT)) & (MASK))

#endif /* BIT_OPERATIONS_H */

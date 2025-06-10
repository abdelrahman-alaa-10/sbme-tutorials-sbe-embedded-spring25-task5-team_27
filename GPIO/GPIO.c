#include "../LIB/Std_Types.h"
#include "../Gpio/GPIO.h"
#include "../GPIO/GPIO_private.h"
#include "../LIB/Bit_Operations.h"



void Gpio_Init(uint8 PortName, uint8 PinNumber, uint8 PinMode, uint8 DefaultState) {
    // Validate input parameters
    if (PinNumber > 15) {
        return; // Invalid pin number
    }

    switch (PortName) {
        case GPIO_A:
            // Configure pin mode
            GPIOA_MODER &= ~(0x03 << (PinNumber * 2));
            GPIOA_MODER |= (PinMode << (PinNumber * 2));

            if (PinMode == GPIO_INPUT) {
                // Configure pull-up/pull-down
                GPIOA_PUPDR &= ~(0x03 << (PinNumber * 2));
                GPIOA_PUPDR |= (DefaultState << (PinNumber * 2));
            }
            else if (PinMode == GPIO_OUTPUT) {
                // Configure output type (push-pull/open-drain)
                GPIOA_OTYPER &= ~(0x1 << PinNumber);
                GPIOA_OTYPER |= (DefaultState << PinNumber);
                // Set default speed to medium
                GPIOA_OSPEEDR &= ~(0x03 << (PinNumber * 2));
                GPIOA_OSPEEDR |= (0x01 << (PinNumber * 2));
            }
            else if (PinMode == GPIO_AF) {
                // Configure Alternate Function
                if(PinNumber < 8) {
                    GPIOA_AFRL &= ~(0xF << (PinNumber * 4));
                    GPIOA_AFRL |= (DefaultState << (PinNumber * 4));
                } else {
                    GPIOA_AFRH &= ~(0xF << ((PinNumber-8) * 4));
                    GPIOA_AFRH |= (DefaultState << ((PinNumber-8) * 4));
                }
            }
            break;

        case GPIO_B:
            // Configure pin mode
            GPIOB_MODER &= ~(0x03 << (PinNumber * 2));
            GPIOB_MODER |= (PinMode << (PinNumber * 2));

            if (PinMode == GPIO_INPUT) {
                // Configure pull-up/pull-down
                GPIOB_PUPDR &= ~(0x03 << (PinNumber * 2));
                GPIOB_PUPDR |= (DefaultState << (PinNumber * 2));
            }
            else if (PinMode == GPIO_OUTPUT) {
                // Configure output type (push-pull/open-drain)
                GPIOB_OTYPER &= ~(0x1 << PinNumber);
                GPIOB_OTYPER |= (DefaultState << PinNumber);
                // Set default speed to medium
                GPIOB_OSPEEDR &= ~(0x03 << (PinNumber * 2));
                GPIOB_OSPEEDR |= (0x01 << (PinNumber * 2));
            }
            else if (PinMode == GPIO_AF) {
                // Configure Alternate Function
                if(PinNumber < 8) {
                    GPIOB_AFRL &= ~(0xF << (PinNumber * 4));
                    GPIOB_AFRL |= (DefaultState << (PinNumber * 4));
                } else {
                    GPIOB_AFRH &= ~(0xF << ((PinNumber-8) * 4));
                    GPIOB_AFRH |= (DefaultState << ((PinNumber-8) * 4));
                }
            }
            break;

        case GPIO_C:
            // Configure pin mode
            GPIOC_MODER &= ~(0x03 << (PinNumber * 2));
            GPIOC_MODER |= (PinMode << (PinNumber * 2));

            if (PinMode == GPIO_INPUT) {
                // Configure pull-up/pull-down
                GPIOC_PUPDR &= ~(0x03 << (PinNumber * 2));
                GPIOC_PUPDR |= (DefaultState << (PinNumber * 2));
            }
            else if (PinMode == GPIO_OUTPUT) {
                // Configure output type (push-pull/open-drain)
                GPIOC_OTYPER &= ~(0x1 << PinNumber);
                GPIOC_OTYPER |= (DefaultState << PinNumber);
                // Set default speed to medium
                GPIOC_OSPEEDR &= ~(0x03 << (PinNumber * 2));
                GPIOC_OSPEEDR |= (0x01 << (PinNumber * 2));
            }
            else if (PinMode == GPIO_AF) {
                // Configure Alternate Function
                if(PinNumber < 8) {
                    GPIOC_AFRL &= ~(0xF << (PinNumber * 4));
                    GPIOC_AFRL |= (DefaultState << (PinNumber * 4));
                } else {
                    GPIOC_AFRH &= ~(0xF << ((PinNumber-8) * 4));
                    GPIOC_AFRH |= (DefaultState << ((PinNumber-8) * 4));
                }
            }
            break;

        case GPIO_D:
            // Configure pin mode
            GPIOD_MODER &= ~(0x03 << (PinNumber * 2));
            GPIOD_MODER |= (PinMode << (PinNumber * 2));

            if (PinMode == GPIO_INPUT) {
                // Configure pull-up/pull-down
                GPIOD_PUPDR &= ~(0x03 << (PinNumber * 2));
                GPIOD_PUPDR |= (DefaultState << (PinNumber * 2));
            }
            else if (PinMode == GPIO_OUTPUT) {
                // Configure output type (push-pull/open-drain)
                GPIOD_OTYPER &= ~(0x1 << PinNumber);
                GPIOD_OTYPER |= (DefaultState << PinNumber);
                // Set default speed to medium
                GPIOD_OSPEEDR &= ~(0x03 << (PinNumber * 2));
                GPIOD_OSPEEDR |= (0x01 << (PinNumber * 2));
            }
            else if (PinMode == GPIO_AF) {
                // Configure Alternate Function
                if(PinNumber < 8) {
                    GPIOD_AFRL &= ~(0xF << (PinNumber * 4));
                    GPIOD_AFRL |= (DefaultState << (PinNumber * 4));
                } else {
                    GPIOD_AFRH &= ~(0xF << ((PinNumber-8) * 4));
                    GPIOD_AFRH |= (DefaultState << ((PinNumber-8) * 4));
                }
            }
            break;

        default:
            // Invalid port
            break;
    }
}

/**
 * @brief Write data to GPIO pin
 * @param PortName: GPIO port ('A', 'B', 'C', 'D')
 * @param PinNumber: Pin number (0-15)
 * @param Data: HIGH or LOW
 * @return OK if successful, NOK if pin is configured as input
 */
uint8 Gpio_WritePin(uint8 PortName, uint8 PinNumber, uint8 Data) {
    // Validate input parameters
    if (PinNumber > 15) {
        return NOK;
    }

    // Check if pin is configured as input
    uint8 mode = 0;
    switch (PortName) {
        case GPIO_A:
            mode = (GPIOA_MODER >> (PinNumber * 2)) & 0x03;
            break;
        case GPIO_B:
            mode = (GPIOB_MODER >> (PinNumber * 2)) & 0x03;
            break;
        case GPIO_C:
            mode = (GPIOC_MODER >> (PinNumber * 2)) & 0x03;
            break;
        case GPIO_D:
            mode = (GPIOD_MODER >> (PinNumber * 2)) & 0x03;
            break;
        default:
            return NOK;
    }

    if (mode == GPIO_INPUT) {
        return NOK; // Cannot write to input pin
    }

    // Write to output data register
    switch (PortName) {
        case GPIO_A:
            if (Data == HIGH) {
                GPIOA_BSRR = (1 << PinNumber); // Set bit atomically
            } else {
                GPIOA_BSRR = (1 << (PinNumber + 16)); // Reset bit atomically
            }
            break;

        case GPIO_B:
            if (Data == HIGH) {
                GPIOB_BSRR = (1 << PinNumber);
            } else {
                GPIOB_BSRR = (1 << (PinNumber + 16));
            }
            break;

        case GPIO_C:
            if (Data == HIGH) {
                GPIOC_BSRR = (1 << PinNumber);
            } else {
                GPIOC_BSRR = (1 << (PinNumber + 16));
            }
            break;

        case GPIO_D:
            if (Data == HIGH) {
                GPIOD_BSRR = (1 << PinNumber);
            } else {
                GPIOD_BSRR = (1 << (PinNumber + 16));
            }
            break;

        default:
            return NOK;
    }

    return OK;
}

/**
 * @brief Read data from GPIO pin
 * @param PortName: GPIO port ('A', 'B', 'C', 'D')
 * @param PinNumber: Pin number (0-15)
 * @return Pin state (HIGH or LOW), or 0 if invalid parameters
 */
uint8 Gpio_ReadPin(uint8 PortName, uint8 PinNumber) {
    uint8 pin_value = 0;

    // Validate input parameters
    if (PinNumber > 15) {
        return 0;
    }

    switch (PortName) {
        case GPIO_A:
            pin_value = (GPIOA_IDR >> PinNumber) & 0x01;
            break;

        case GPIO_B:
            pin_value = (GPIOB_IDR >> PinNumber) & 0x01;
            break;

        case GPIO_C:
            pin_value = (GPIOC_IDR >> PinNumber) & 0x01;
            break;

        case GPIO_D:
            pin_value = (GPIOD_IDR >> PinNumber) & 0x01;
            break;

        default:
            pin_value = 0;
            break;
    }

    return pin_value;
}

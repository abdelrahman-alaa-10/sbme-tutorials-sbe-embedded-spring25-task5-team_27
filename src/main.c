#include "../Rcc/Rcc.h"
#include "../GPIO/GPIO.h"
#include "../Lib/Std_Types.h"
#include "../EXTI/EXTI.h"
#include "stm32f4xx.h"
#include "../Lib/Bit_Operations.h"

/*==============================================================================
 *                                 DEFINITIONS
 *==============================================================================*/

// Seven Segment Display Pins
#define SEGMENT_A  GPIO_B, 0
#define SEGMENT_B  GPIO_B, 1
#define SEGMENT_C  GPIO_B, 2
#define SEGMENT_D  GPIO_B, 3
#define SEGMENT_E  GPIO_B, 4
#define SEGMENT_F  GPIO_B, 5
#define SEGMENT_G  GPIO_B, 6

// Display Enable Pins
#define DISPLAY_HUNDREDS  GPIO_B, 7   // Counter hundreds digit
#define DISPLAY_TENS      GPIO_B, 8   // Counter tens digit
#define DISPLAY_UNITS     GPIO_B, 9   // Counter units digit
#define DISPLAY_SOURCE    GPIO_B, 10  // Last interrupt source

// Button and LED Pins
#define BUTTON_INCREMENT  GPIO_B, 12  // Increment counter button
#define BUTTON_DECREMENT  GPIO_B, 13  // Decrement counter button
#define BUTTON_LED        GPIO_A, 4   // LED control button
#define LED_PIN           GPIO_A, 3   // LED output

// Interrupt Sources
#define SOURCE_INCREMENT  1
#define SOURCE_DECREMENT  2
#define SOURCE_LED_ON     3
#define SOURCE_LED_OFF    4

// Display refresh timing
#define DISPLAY_REFRESH_DELAY  15000  // Microseconds equivalent

/*==============================================================================
 *                                 GLOBAL VARIABLES
 *==============================================================================*/

// Segment mapping array
uint8 sevenSegmentMap[7][2] = {
    {SEGMENT_A},
    {SEGMENT_B},
    {SEGMENT_C},
    {SEGMENT_D},
    {SEGMENT_E},
    {SEGMENT_F},
    {SEGMENT_G}
};

// Shared data (protected by critical sections)
volatile uint16 g_counter = 0;              // Counter value (0-999)
volatile uint8 g_lastInterruptSource = 0;   // Last interrupt source
volatile uint8 g_counterChanged = 0;        // Flag indicating counter changed
volatile uint8 g_sourceChanged = 0;         // Flag indicating source changed

// Display state
uint8 g_currentDigit = 0;  // Current digit being displayed (0-3)

/*==============================================================================
 *                                 FUNCTION PROTOTYPES
 *==============================================================================*/

void SevenSegDisplay(uint8 number);
void Initialize(void);
void DisplayRefresh(void);
void UpdateCounter(void);
void UpdateLastSource(void);
void DisableAllDisplays(void);
void EnableDisplay(uint8 displayIndex);
uint8 GetDigitValue(uint8 digitIndex);

// Critical section macros for race condition protection
#define ENTER_CRITICAL()  __disable_irq()
#define EXIT_CRITICAL()   __enable_irq()

/*==============================================================================
 *                                 INTERRUPT SERVICE ROUTINES
 *==============================================================================*/

// ISR for EXTI Lines 10-15 (handles PB12, PB13)
void EXTI15_10_IRQHandler(void) {
    // Check PB12 (Increment button - falling edge)
    if (EXTI->PR & (1 << 12)) {
        EXTI->PR |= (1 << 12);  // Clear pending flag

        ENTER_CRITICAL();
        if (g_counter < 999) {
            g_counter++;
        } else {
            g_counter = 0;  // Wrap around
        }
        g_lastInterruptSource = SOURCE_INCREMENT;
        g_counterChanged = 1;
        g_sourceChanged = 1;
        EXIT_CRITICAL();
    }

    // Check PB13 (Decrement button - rising edge)
    if (EXTI->PR & (1 << 13)) {
        EXTI->PR |= (1 << 13);  // Clear pending flag

        ENTER_CRITICAL();
        if (g_counter > 0) {
            g_counter--;
        } else {
            g_counter = 999;  // Wrap around
        }
        g_lastInterruptSource = SOURCE_DECREMENT;
        g_counterChanged = 1;
        g_sourceChanged = 1;
        EXIT_CRITICAL();
    }
}

// ISR for EXTI Lines 0-4 (handles PA4)
void EXTI4_IRQHandler(void) {
    if (EXTI->PR & (1 << 4)) {
        EXTI->PR |= (1 << 4);  // Clear pending flag

        // Check if rising or falling edge
        uint8 pinState = Gpio_ReadPin(GPIO_A, 4);

        ENTER_CRITICAL();
        if (pinState == HIGH) {
            // Rising edge - turn on LED
            Gpio_WritePin(GPIO_A, 3, HIGH);
            g_lastInterruptSource = SOURCE_LED_ON;
        } else {
            // Falling edge - turn off LED
            Gpio_WritePin(GPIO_A, 3, LOW);
            g_lastInterruptSource = SOURCE_LED_OFF;
        }
        g_sourceChanged = 1;
        EXIT_CRITICAL();
    }
}

/*==============================================================================
 *                                 MAIN FUNCTION
 *==============================================================================*/

int main(void) {
    // Initialize system
    Initialize();

    // Main loop - handles display updates
    while (1) {
        // Update displays if data changed
        if (g_counterChanged) {
            UpdateCounter();
            ENTER_CRITICAL();
            g_counterChanged = 0;
            EXIT_CRITICAL();
        }

        if (g_sourceChanged) {
            UpdateLastSource();
            ENTER_CRITICAL();
            g_sourceChanged = 0;
            EXIT_CRITICAL();
        }

        // Refresh display (multiplexing)
        DisplayRefresh();
    }

    return 0;
}

/*==============================================================================
 *                                 FUNCTION IMPLEMENTATIONS
 *==============================================================================*/

void Initialize(void) {
    uint32 i;

    // Initialize RCC and Gpio
    Rcc_Init();
    Rcc_Enable(RCC_GPIOA);
    Rcc_Enable(RCC_GPIOB);
    Rcc_Enable(RCC_SYSCFG);  // Enable SYSCFG for EXTI

    // Initialize seven segment pins
    for (i = 0; i < 7; i++) {
        Gpio_Init(sevenSegmentMap[i][0], sevenSegmentMap[i][1], GPIO_OUTPUT, GPIO_PUSH_PULL);
    }

    // Initialize display enable pins
    Gpio_Init(GPIO_B, 7, GPIO_OUTPUT, GPIO_PUSH_PULL);   // Hundreds
    Gpio_Init(GPIO_B, 8, GPIO_OUTPUT, GPIO_PUSH_PULL);   // Tens
    Gpio_Init(GPIO_B, 9, GPIO_OUTPUT, GPIO_PUSH_PULL);   // Units
    Gpio_Init(GPIO_B, 10, GPIO_OUTPUT, GPIO_PUSH_PULL);  // Source

    // Initialize LED pin
    Gpio_Init(GPIO_A, 3, GPIO_OUTPUT, GPIO_PUSH_PULL);

    // Initialize button pins with pull-up resistors
    Gpio_Init(GPIO_B, 12, GPIO_INPUT, GPIO_PULL_UP);   // Increment button
    Gpio_Init(GPIO_B, 13, GPIO_INPUT, GPIO_PULL_DOWN);   // Decrement button
    Gpio_Init(GPIO_A, 4, GPIO_INPUT, GPIO_PULL_UP);    // LED control button

    // Configure EXTI for buttons
    Exti_Init(GPIO_B, 12, FALLING_EDGE);  // Increment on falling edge
    Exti_Init(GPIO_B, 13, RISING_EDGE);   // Decrement on rising edge
    Exti_Init(GPIO_A, 4, BOTH_EDGES);     // LED control on both edges

    // Enable EXTI interrupts
    Exti_Enable(12);
    Exti_Enable(13);
    Exti_Enable(4);

    // Enable NVIC interrupts
    NVIC_EnableIRQ(EXTI15_10_IRQn);  // For PB12, PB13
    NVIC_EnableIRQ(EXTI4_IRQn);      // For PA4

    // Initialize displays (turn off all)
    DisableAllDisplays();

    // Initialize LED (off)
    Gpio_WritePin(GPIO_A, 3, LOW);

    // Enable global interrupts
    __enable_irq();
}

void DisplayRefresh(void) {
    uint32 i;
    DisableAllDisplays();
    uint8 digitValue = GetDigitValue(g_currentDigit);
    SevenSegDisplay(digitValue);
    EnableDisplay(g_currentDigit);
    for (i = 0; i < 3000; i++) {} // ~1-2 ms delay, adjust as needed
    g_currentDigit = (g_currentDigit + 1) % 4;
}

void UpdateCounter(void) {
    // Counter update is handled in DisplayRefresh through GetDigitValue
    // This function can be used for additional counter-related operations
}

void UpdateLastSource(void) {
    // Source update is handled in DisplayRefresh through GetDigitValue
    // This function can be used for additional source-related operations
}

void DisableAllDisplays(void) {
    Gpio_WritePin(GPIO_B, 7, HIGH);   // Hundreds
    Gpio_WritePin(GPIO_B, 8, HIGH);   // Tens
    Gpio_WritePin(GPIO_B, 9, HIGH);   // Units
    Gpio_WritePin(GPIO_B, 10, HIGH);  // Source
}

void EnableDisplay(uint8 displayIndex) {
    switch (displayIndex) {
        case 0: // Hundreds
            Gpio_WritePin(GPIO_B, 7, LOW);
            break;
        case 1: // Tens
            Gpio_WritePin(GPIO_B, 8, LOW);
            break;
        case 2: // Units
            Gpio_WritePin(GPIO_B, 9, LOW);
            break;
        case 3: // Source
            Gpio_WritePin(GPIO_B, 10, LOW);
            break;
    }
}

uint8 GetDigitValue(uint8 digitIndex) {
    uint8 value = 0;
    uint16 localCounter;
    uint8 localSource;

    // Read shared data in critical section
    ENTER_CRITICAL();
    localCounter = g_counter;
    localSource = g_lastInterruptSource;
    EXIT_CRITICAL();

    switch (digitIndex) {
        case 0: // Hundreds digit
            value = (localCounter / 100) % 10;
            break;
        case 1: // Tens digit
            value = (localCounter / 10) % 10;
            break;
        case 2: // Units digit
            value = localCounter % 10;
            break;
        case 3: // Last interrupt source
            value = localSource;
            if (value > 9) value = 9;  // Clamp to single digit
            break;
    }

    return value;
}

void SevenSegDisplay(uint8 number) {
    if (number > 9) {
        return;
    }

    const uint8 numbers_array[10] = {
        0x3F, 0x06, 0x5B, 0x4F, 0x66,
        0x6D, 0x7D, 0x07, 0x7F, 0x67
    };

    for (uint8 i = 0; i < 7; i++) {
        Gpio_WritePin(sevenSegmentMap[i][0], sevenSegmentMap[i][1],
                      ((numbers_array[number] >> i) & 0x01));
    }
}


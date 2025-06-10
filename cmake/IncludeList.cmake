set(INCLUDE_LIST ${INCLUDE_LIST}
        ${PROJECT_PATH}/STM32-base/startup
        ${PROJECT_PATH}/STM32-base-STM32Cube/CMSIS/ARM/inc
        ${PROJECT_PATH}/STM32-base-STM32Cube/CMSIS/${SERIES_FOLDER}/inc
        ${PROJECT_PATH}/include
        ${PROJECT_PATH}/Lib
        ${PROJECT_PATH}/Rcc
        ${PROJECT_PATH}/Gpio
        ${PROJECT_PATH}/Keypad
        ${PROJECT_PATH}/Delay
        ${PROJECT_PATH}/7-segment
        ${PROJECT_PATH}/draft
        ${PROJECT_PATH}/Lib
        ${PROJECT_PATH}/EXTI

)

if (USE_HAL)
    set(INCLUDE_LIST ${INCLUDE_LIST} ${PROJECT_PATH}/STM32-base-STM32Cube/HAL/${SERIES_FOLDER}/inc)
endif ()
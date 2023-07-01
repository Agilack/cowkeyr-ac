/**
 * @file  startup.s
 * @brief Processor vector table and interrupts handlers (including reset)
 *
 * @author Saint-Genest Gwenael <gwen@cowlab.fr>
 * @copyright Agilack (c) 2023
 *
 * @page License
 * Cowkeyr-AC firmware is free software: you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * version 3 as published by the Free Software Foundation. You should have
 * received a copy of the GNU Lesser General Public License along with this
 * program, see LICENSE.md file for more details.
 * This program is distributed WITHOUT ANY WARRANTY.
 */
    .syntax unified
    .arch armv6-m

/* -- Stack and Head sections ---------------------------------------------- */
    .section .stack
    .align 2
    .equ    Stack_Size, 0x1FFC
    .globl    __StackTop
    .globl    __StackLimit
__StackLimit:
    .space    Stack_Size
    .size __StackLimit, . - __StackLimit
__StackTop:
    .size __StackTop, . - __StackTop

/* -- Vector Table --------------------------------------------------------- */

    .section .isr_vector
    .align 2
    .globl __isr_vector
__isr_vector:
    /* Cortex M33 Handlers */
    .long   _estack                     /* Top of Stack                       */
    .long   Reset_Handler               /* Reset Handler                      */
    .long   NMI_Handler                 /* NMI Handler                        */
    .long   HardFault_Handler           /* Hard Fault Handler                 */
    .long   MemManage_Handler           /* Memory Management                  */
    .long   BusFault_Handler            /* Pre-fetch or memory access fault   */
    .long   Usage_Fault                 /* Undefined instruction              */
    .long   SecureFault_Handler         /* Secure Fault                       */
    .long   0                           /* Reserved                           */
    .long   0                           /* Reserved                           */
    .long   0                           /* Reserved                           */
    .long   SVC_Handler                 /* SVCall Handler                     */
    .long   Debug_Handler               /* Debug Monitor Handler              */
    .long   0                           /* Reserved                           */
    .long   PendSV_Handler              /* PendSV Handler                     */
    .long   SysTick_Handler             /* SysTick Handler                    */
    /* STM32H5 Peripherals Handlers */
    .long   WWDG_Handler                /* Window Watchdog                    */
    .long   PVD_Handler                 /* Power Voltage Detector             */
    .long   RTC_Handler                 /* Real-Time Clock non-secure         */
    .long   RTC_S_Handler               /* Real-Time Clock secure             */
    .long   TAMP_Handler                /* Tamper global interrupt            */
    .long   RAMCFG_Handler              /* RAM configuration                  */
    .long   FLASH_Handler               /* Internal Flash non-secure          */
    .long   FLASH_S_Handler             /* Internal Flash secure              */
    .long   GTZC_Handler                /* Global TrustZone Controler         */
    .long   RCC_Handler                 /* Reset and Clocks non-secure        */
    .long   RCC_S_Handler               /* Reset and Clocks secure            */
    .long   EXTI0_Handler               /* External Interrupt line 0          */
    .long   EXTI1_Handler               /* External Interrupt line 1          */
    .long   EXTI2_Handler               /* External Interrupt line 2          */
    .long   EXTI3_Handler               /* External Interrupt line 3          */
    .long   EXTI4_Handler               /* External Interrupt line 4          */
    .long   EXTI5_Handler               /* External Interrupt line 5          */
    .long   EXTI6_Handler               /* External Interrupt line 6          */
    .long   EXTI7_Handler               /* External Interrupt line 7          */
    .long   EXTI8_Handler               /* External Interrupt line 8          */
    .long   EXTI9_Handler               /* External Interrupt line 9          */
    .long   EXTI10_Handler              /* External Interrupt line 10         */
    .long   EXTI11_Handler              /* External Interrupt line 11         */
    .long   EXTI12_Handler              /* External Interrupt line 12         */
    .long   EXTI13_Handler              /* External Interrupt line 13         */
    .long   EXTI14_Handler              /* External Interrupt line 14         */
    .long   EXTI15_Handler              /* External Interrupt line 15         */
    .long   GPDMA1_CH0_Handler          /* General Purpose DMA 1 channel 0    */
    .long   GPDMA1_CH1_Handler          /* General Purpose DMA 1 channel 1    */
    .long   GPDMA1_CH2_Handler          /* General Purpose DMA 1 channel 2    */
    .long   GPDMA1_CH3_Handler          /* General Purpose DMA 1 channel 3    */
    .long   GPDMA1_CH4_Handler          /* General Purpose DMA 1 channel 4    */
    .long   GPDMA1_CH5_Handler          /* General Purpose DMA 1 channel 5    */
    .long   GPDMA1_CH6_Handler          /* General Purpose DMA 1 channel 6    */
    .long   GPDMA1_CH7_Handler          /* General Purpose DMA 1 channel 7    */
    .long   IWDG_Handler                /* Independent Watchdog interrupt     */
    .long   SAES_Handler                /* Secure AES                         */
    .long   ADC1_Handler                /* Analog to Digital Converter 1      */
    .long   DAC1_Handler                /* Digital to Analog Converter 1      */
    .long   FDCAN1_IT0_Handler          /* Flexible Datarate CAN 1            */
    .long   FDCAN1_IT1_Handler          /* Flexible Datarate CAN 1            */
    .long   TIM1_Handler                /* Timer 1 : Break/Error              */
    .long   TIM1_UP_Handler             /* Timer 1 : Update                   */
    .long   TIM1_TRG_Handler            /* Timer 1 : Trigger/Commutation/Dir  */
    .long   TIM1_CC_Handler             /* Timer 1 : Capture and Compare      */
    .long   TIM2_Handler                /* Timer 2 (all events)               */
    .long   TIM3_Handler                /* Timer 3 (all events)               */
    .long   TIM4_Handler                /* Timer 4 (all events)               */
    .long   TIM5_Handler                /* Timer 5 (all events)               */
    .long   TIM6_Handler                /* Timer 6 (all events)               */
    .long   TIM7_Handler                /* Timer 7 (all events)               */
    .long   I2C1_EV_Handler             /* I2C controller 1 Events            */
    .long   I2C1_ER_Handler             /* I2C controller 1 Errors            */
    .long   I2C2_EV_Handler             /* I2C controller 2 Events            */
    .long   I2C2_ER_Handler             /* I2C controller 2 Errors            */
    .long   SPI1_Handler                /* SPI1 (all events)                  */
    .long   SPI2_Handler                /* SPI2 (all events)                  */
    .long   SPI3_Handler                /* SPI3 (all events)                  */
    .long   USART1_Handler              /* USART1 (all events)                */
    .long   USART2_Handler              /* USART2 (all events)                */
    .long   USART3_Handler              /* USART3 (all events)                */
    .long   UART4_Handler               /* UART4 (all events)                 */
    .long   UART5_Handler               /* UART5 (all events)                 */
    .long   LPUART1_Handler             /* Low Power UART1                    */
    .long   LPTIM1_Handler              /* Low Power Timer 1                  */
    .long   TIM8_Handler                /* Timer 8 : Break/Error              */
    .long   TIM8_UP_Handler             /* Timer 8 : Update                   */
    .long   TIM8_TRG_Handler            /* Timer 8 : Trigger/Commutation/Dir  */
    .long   TIM8_CC_Handler             /* Timer 8 : Capture and Compare      */
    .long   ADC2_Handler                /* Analog to Digital Converter 2      */
    .long   LPTIM2_Handler              /* Low Power Timer 2                  */
    .long   TIM15_Handler               /* Timer 15 (all events)              */
    .long   TIM16_Handler               /* Timer 16 (all events)              */
    .long   TIM17_Handler               /* Timer 17 (all events)              */
    .long   USB_FS_Handler              /* USB FS global interrupt            */

    .long   CRS_Handler                 /* Clock Recovery System              */
    .long   UCPD1_Handler               /* USB-C Power Delivery               */
    .long   FMC_Handler                 /* Flexible Memory COntroller         */
    .long   OCTOSPI1_Handler            /* Octo-SPI 1 interface               */
    .long   SDMMC1_Handler              /* SD/MMC 1 controller                */
    .long   I2C3_EV_Handler             /* I2C controller 3 Events            */
    .long   I2C3_ER_Handler             /* I2C controller 3 Errors            */
    .long   SPI4_Handler                /* SPI4 (all events)                  */
    .long   SPI5_Handler                /* SPI5 (all events)                  */
    .long   SPI6_Handler                /* SPI6 (all events)                  */
    .long   USART6_Handler              /* USART6 (all events)                */
    .long   USART10_Handler             /* USART10 (all events)               */
    .long   USART11_Handler             /* USART11 (all events)               */
    .long   SAI1_Handler                /* Serial Audio Interface 1           */
    .long   SAI2_Handler                /* Serial Audio Interface 2           */
    .long   GPDMA2_CH0_Handler          /* General Purpose DMA 2 channel 0    */
    .long   GPDMA2_CH1_Handler          /* General Purpose DMA 2 channel 1    */
    .long   GPDMA2_CH2_Handler          /* General Purpose DMA 2 channel 2    */
    .long   GPDMA2_CH3_Handler          /* General Purpose DMA 2 channel 3    */
    .long   GPDMA2_CH4_Handler          /* General Purpose DMA 2 channel 4    */
    .long   GPDMA2_CH5_Handler          /* General Purpose DMA 2 channel 5    */
    .long   GPDMA2_CH6_Handler          /* General Purpose DMA 2 channel 6    */
    .long   GPDMA2_CH7_Handler          /* General Purpose DMA 2 channel 7    */
    .long   UART7_Handler               /* UART7 (all events)                 */
    .long   UART8_Handler               /* UART8 (all events)                 */
    .long   UART9_Handler               /* UART9 (all events)                 */
    .long   UART12_Handler              /* UART12 (all events)                */
    .long   SDMMC2_Handler              /* SD/MMC 2 controller                */
    .long   FPU_Handler                 /* Floating Point Unit                */
    .long   ICACHE_Handler              /* Instruction cache global interrupt */
    .long   DCACHE_Handler              /* Data cache global interrupt        */
    .long   ETH_Handler                 /* Ethernet MAC                       */
    .long   ETH_WKUP_Handler            /* Ethernet Wakeup (with EXTI)        */
    .long   DCMI_PSSI_Handler           /* Digital Camera and Parallel Slave  */
    .long   FDCAN2_IT0_Handler          /* Flexible Datarate CAN 2            */
    .long   FDCAN2_IT1_Handler          /* Flexible Datarate CAN 2            */
    .long   CORDIC_Handler              /* Cordic coprocessor (math)          */
    .long   FMA_Handler                 /* Filter Math Accelerator            */
    .long   DTS_Handler                 /* Digital Temperature Sensor         */
    .long   RNG_Handler                 /* Random Number Generator            */
    .long   OTFDEC1_Handler             /* On-The-Fly Decyption Engine        */
    .long   AES_Handler                 /* AES coprocessor (not on all MCU)   */
    .long   HASH_Handler                /* HASH coprocessor                   */
    .long   PKA_Handler                 /* Public Key Accelerator             */
    .long   CEC_Handler                 /* HDMI-CEC global interrupt          */
    .long   TIM12_Handler               /* Timer 12 (all events)              */
    .long   TIM13_Handler               /* Timer 13 (all events)              */
    .long   TIM14_Handler               /* Timer 14 (all events)              */
    .long   I3C1_EV_Handler             /* I3C controller 1 Events            */
    .long   I3C1_ER_Handler             /* I3C controller 1 Errors            */
    .long   I2C4_EV_Handler             /* I2C controller 4 Events            */
    .long   I2C4_ER_Handler             /* I2C controller 4 Errors            */
    .long   LPTIM3_Handler              /* Low Power Timer 3                  */
    .long   LPTIM4_Handler              /* Low Power Timer 4                  */
    .long   LPTIM5_Handler              /* Low Power Timer 5                  */
    .long   LPTIM6_Handler              /* Low Power Timer 6                  */
    .size    __isr_vector, . - __isr_vector

    .section .fw_version
    .align 4
    .globl __fw_version
__fw_version:
    .long   0x00000001 /* Firmware version */
    .long   0x00000000 /* Rfu */
    .long   0x00000000 /* Serial Number (rfu) */
    .long   0x00000000 /* Serial Number (rfu) */

/**
 * @brief Reset handler called after a reset
 *
 */
    .text
    .thumb
    .thumb_func
    .align 2
    .globl    Reset_Handler
    .type    Reset_Handler, %function
Reset_Handler:
    /* Copy datas from flash to SRAM */
    ldr    r1, =_sidata
    ldr    r2, =__data_start__
    ldr    r3, =__data_end__
    subs    r3, r2
    ble    .copy_end
.copy_loop:
    subs    r3, #4
    ldr    r0, [r1, r3]
    str    r0, [r2, r3]
    bgt    .copy_loop
.copy_end:
    /* Call C code entry ("main" function) */
    bl  main

/**
 * @brief Default handler is an infinite loop for all unsupported events
 *
 */
.section .text.Default_Handler,"ax",%progbits
Default_Handler:
Infinite_Loop:
	b	Infinite_Loop
	.size	Default_Handler, .-Default_Handler

/*    Macro to define default handlers. Default handler
 *    will be weak symbol and just dead loops. They can be
 *    overwritten by other handlers */
    .macro    def_default_handler    handler_name
    .align 1
    .thumb_func
    .weak    \handler_name
    .thumb_set \handler_name,Default_Handler
    .endm

    /* Default handlers for Cortex M33 internal blocks */
    def_default_handler    NMI_Handler
    def_default_handler    HardFault_Handler
    def_default_handler    MemManage_Handler
    def_default_handler    BusFault_Handler
    def_default_handler    Usage_Fault
    def_default_handler    SecureFault_Handler
    def_default_handler    SVC_Handler
    def_default_handler    Debug_Handler
    def_default_handler    PendSV_Handler
    def_default_handler    SysTick_Handler
    /* Default handlers for STM32H5 peripherals */
    def_default_handler    WWDG_Handler
    def_default_handler    PVD_Handler
    def_default_handler    RTC_Handler
    def_default_handler    RTC_S_Handler
    def_default_handler    TAMP_Handler
    def_default_handler    RAMCFG_Handler
    def_default_handler    FLASH_Handler
    def_default_handler    FLASH_S_Handler
    def_default_handler    GTZC_Handler
    def_default_handler    RCC_Handler
    def_default_handler    RCC_S_Handler
    def_default_handler    EXTI0_Handler
    def_default_handler    EXTI1_Handler
    def_default_handler    EXTI2_Handler
    def_default_handler    EXTI3_Handler
    def_default_handler    EXTI4_Handler
    def_default_handler    EXTI5_Handler
    def_default_handler    EXTI6_Handler
    def_default_handler    EXTI7_Handler
    def_default_handler    EXTI8_Handler
    def_default_handler    EXTI9_Handler
    def_default_handler    EXTI10_Handler
    def_default_handler    EXTI11_Handler
    def_default_handler    EXTI12_Handler
    def_default_handler    EXTI13_Handler
    def_default_handler    EXTI14_Handler
    def_default_handler    EXTI15_Handler
    def_default_handler    GPDMA1_CH0_Handler
    def_default_handler    GPDMA1_CH1_Handler
    def_default_handler    GPDMA1_CH2_Handler
    def_default_handler    GPDMA1_CH3_Handler
    def_default_handler    GPDMA1_CH4_Handler
    def_default_handler    GPDMA1_CH5_Handler
    def_default_handler    GPDMA1_CH6_Handler
    def_default_handler    GPDMA1_CH7_Handler
    def_default_handler    IWDG_Handler
    def_default_handler    SAES_Handler
    def_default_handler    ADC1_Handler
    def_default_handler    DAC1_Handler
    def_default_handler    FDCAN1_IT0_Handler
    def_default_handler    FDCAN1_IT1_Handler
    def_default_handler    TIM1_Handler
    def_default_handler    TIM1_UP_Handler
    def_default_handler    TIM1_TRG_Handler
    def_default_handler    TIM1_CC_Handler
    def_default_handler    TIM2_Handler
    def_default_handler    TIM3_Handler
    def_default_handler    TIM4_Handler
    def_default_handler    TIM5_Handler
    def_default_handler    TIM6_Handler
    def_default_handler    TIM7_Handler
    def_default_handler    I2C1_EV_Handler
    def_default_handler    I2C1_ER_Handler
    def_default_handler    I2C2_EV_Handler
    def_default_handler    I2C2_ER_Handler
    def_default_handler    SPI1_Handler
    def_default_handler    SPI2_Handler
    def_default_handler    SPI3_Handler
    def_default_handler    USART1_Handler
    def_default_handler    USART2_Handler
    def_default_handler    USART3_Handler
    def_default_handler    UART4_Handler
    def_default_handler    UART5_Handler
    def_default_handler    LPUART1_Handler
    def_default_handler    LPTIM1_Handler
    def_default_handler    TIM8_Handler
    def_default_handler    TIM8_UP_Handler
    def_default_handler    TIM8_TRG_Handler
    def_default_handler    TIM8_CC_Handler
    def_default_handler    ADC2_Handler
    def_default_handler    LPTIM2_Handler
    def_default_handler    TIM15_Handler
    def_default_handler    TIM16_Handler
    def_default_handler    TIM17_Handler
    def_default_handler    USB_FS_Handler
    def_default_handler    CRS_Handler
    def_default_handler    UCPD1_Handler
    def_default_handler    FMC_Handler
    def_default_handler    OCTOSPI1_Handler
    def_default_handler    SDMMC1_Handler
    def_default_handler    I2C3_EV_Handler
    def_default_handler    I2C3_ER_Handler
    def_default_handler    SPI4_Handler
    def_default_handler    SPI5_Handler
    def_default_handler    SPI6_Handler
    def_default_handler    USART6_Handler
    def_default_handler    USART10_Handler
    def_default_handler    USART11_Handler
    def_default_handler    SAI1_Handler
    def_default_handler    SAI2_Handler
    def_default_handler    GPDMA2_CH0_Handler
    def_default_handler    GPDMA2_CH1_Handler
    def_default_handler    GPDMA2_CH2_Handler
    def_default_handler    GPDMA2_CH3_Handler
    def_default_handler    GPDMA2_CH4_Handler
    def_default_handler    GPDMA2_CH5_Handler
    def_default_handler    GPDMA2_CH6_Handler
    def_default_handler    GPDMA2_CH7_Handler
    def_default_handler    UART7_Handler
    def_default_handler    UART8_Handler
    def_default_handler    UART9_Handler
    def_default_handler    UART12_Handler
    def_default_handler    SDMMC2_Handler
    def_default_handler    FPU_Handler
    def_default_handler    ICACHE_Handler
    def_default_handler    DCACHE_Handler
    def_default_handler    ETH_Handler
    def_default_handler    ETH_WKUP_Handler
    def_default_handler    DCMI_PSSI_Handler
    def_default_handler    FDCAN2_IT0_Handler
    def_default_handler    FDCAN2_IT1_Handler
    def_default_handler    CORDIC_Handler
    def_default_handler    FMA_Handler
    def_default_handler    DTS_Handler
    def_default_handler    RNG_Handler
    def_default_handler    OTFDEC1_Handler
    def_default_handler    AES_Handler
    def_default_handler    HASH_Handler
    def_default_handler    PKA_Handler
    def_default_handler    CEC_Handler
    def_default_handler    TIM12_Handler
    def_default_handler    TIM13_Handler
    def_default_handler    TIM14_Handler
    def_default_handler    I3C1_EV_Handler
    def_default_handler    I3C1_ER_Handler
    def_default_handler    I2C4_EV_Handler
    def_default_handler    I2C4_ER_Handler
    def_default_handler    LPTIM3_Handler
    def_default_handler    LPTIM4_Handler
    def_default_handler    LPTIM5_Handler
    def_default_handler    LPTIM6_Handler
.end

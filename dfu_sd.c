/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2019 Ayke van Laethem
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

// This file contains interrupt handlers that forwards interrupts to
// either the SoftDevice or the application, depending on the interrupt.

#include "dfu.h"

__attribute__((used))
void handleSDInterrupt(uintptr_t offset) {
    // Calculate the pointer to the entry in the interrupt vector table.
    void (**ptr)() = (void(**)())((SD_CODE_BASE+offset));
    // Load the function pointer from the vector table.
    void (*handler)() = *ptr;
    // Call the interrupt.
    handler();
}

__attribute__((used))
void handleAppInterrupt(uintptr_t offset) {
    // Calculate the pointer to the entry in the interrupt vector table.
    void (**ptr)() = (void(**)())((APP_CODE_BASE+offset));
    // Load the function pointer from the vector table.
    void (*handler)() = *ptr;
    // Call the interrupt.
    handler();
}

// These macros define a single forwarding interrupt handler in the
// smallest amount of code possible, because this handler must be repeated
// for every interrupt.
//
// Each function takes up just 4 bytes with two instructions, for example:
//
//     movs r0, #64
//     b.n  handleSDInterrupt
//
// The 'offset' parameter here is the offset from SD_CODE_BASE or
// APP_CODE_BASE where the interrupt handler pointer lives.
#define DEFINE_SD_HANDLER(number, name) \
    __attribute__((naked)) \
    void name (void) { \
        __asm__ __volatile__("movs r0, %[offset]\nb.n handleSDInterrupt" : : [offset]"I" (number*4)); \
    }
#define DEFINE_APP_HANDLER(number, name) \
    __attribute__((naked)) \
    void name (void) { \
        __asm__ __volatile__("movs r0, %[offset]\nb.n handleAppInterrupt" : : [offset]"I" (number*4)); \
    }

// SD/app distinction based on SoftDevice Specification. When it is listed
// as accessible when the SoftDevice is enabled, the IRQ is forwarded to
// the application. Otherwise it is forwarded to the SoftDevice.
// This assumes that the application will always run with the SoftDevice
// enabled and will never try to handle one of the 'restricted' or
// 'blocked' interrupts.
DEFINE_SD_HANDLER(11, SVC_Handler)
DEFINE_SD_HANDLER(16, POWER_CLOCK_IRQHandler)
DEFINE_SD_HANDLER(17, RADIO_IRQHandler)
DEFINE_APP_HANDLER(18, UARTE0_UART0_IRQHandler)
DEFINE_APP_HANDLER(19, SPIM0_SPIS0_TWIM0_TWIS0_SPI0_TWI0_IRQHandler)
DEFINE_APP_HANDLER(20, SPIM1_SPIS1_TWIM1_TWIS1_SPI1_TWI1_IRQHandler)
DEFINE_APP_HANDLER(21, NFCT_IRQHandler)
DEFINE_APP_HANDLER(22, GPIOTE_IRQHandler)
DEFINE_APP_HANDLER(23, SAADC_IRQHandler)
DEFINE_SD_HANDLER(24, TIMER0_IRQHandler)
DEFINE_APP_HANDLER(25, TIMER1_IRQHandler)
DEFINE_APP_HANDLER(26, TIMER2_IRQHandler)
DEFINE_SD_HANDLER(27, RTC0_IRQHandler)
DEFINE_SD_HANDLER(28, TEMP_IRQHandler)
DEFINE_SD_HANDLER(29, RNG_IRQHandler)
DEFINE_SD_HANDLER(30, ECB_IRQHandler)
DEFINE_SD_HANDLER(31, CCM_AAR_IRQHandler)
DEFINE_APP_HANDLER(32, WDT_IRQHandler)
DEFINE_APP_HANDLER(33, RTC1_IRQHandler)
DEFINE_APP_HANDLER(34, QDEC_IRQHandler)
DEFINE_APP_HANDLER(35, COMP_LPCOMP_IRQHandler)
DEFINE_APP_HANDLER(36, SWI0_EGU0_IRQHandler)
DEFINE_SD_HANDLER(37, SWI1_EGU1_IRQHandler)
DEFINE_APP_HANDLER(38, SWI2_EGU2_IRQHandler)
DEFINE_APP_HANDLER(39, SWI3_EGU3_IRQHandler)
DEFINE_APP_HANDLER(40, SWI4_EGU4_IRQHandler)
DEFINE_SD_HANDLER(41, SWI5_EGU5_IRQHandler)
DEFINE_APP_HANDLER(42, TIMER3_IRQHandler)
DEFINE_APP_HANDLER(43, TIMER4_IRQHandler)
DEFINE_APP_HANDLER(44, PWM0_IRQHandler)
DEFINE_APP_HANDLER(45, PDM_IRQHandler)
DEFINE_SD_HANDLER(48, MWU_IRQHandler)
DEFINE_APP_HANDLER(49, PWM1_IRQHandler)
DEFINE_APP_HANDLER(50, PWM2_IRQHandler)
DEFINE_APP_HANDLER(51, SPIM2_SPIS2_SPI2_IRQHandler)
DEFINE_APP_HANDLER(52, RTC2_IRQHandler)
DEFINE_APP_HANDLER(53, I2S_IRQHandler)
DEFINE_APP_HANDLER(54, FPU_IRQHandler)

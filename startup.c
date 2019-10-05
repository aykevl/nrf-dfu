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

#include <stdint.h>

extern uint32_t _estack;
extern uint32_t _sidata;
extern uint32_t _sdata;
extern uint32_t _edata;
extern uint32_t _sbss;
extern uint32_t _ebss;

typedef void (*func)(void);

extern void  _start(void) __attribute__((noreturn));

// Note: Default_Handler lives in RAM because it can be used as SoftDevice
// assert handler, and modern SoftDevices don't allow a handler to live in
// the MBR region. Therefore it needs to live in a new section (so that
// the linker script can include it there).

#if DEBUG
void uart_write(char *s);

__attribute__((section(".Default_Handler")))
void Default_Handler(void) {
    uart_write("Default_Handler\r\n");
    while (1);
}

void HardFault_Handler(void) {
    uart_write("HardFault_Handler\r\n");
    while (1);
}
#else
__attribute__((section(".Default_Handler")))
void Default_Handler(void) {
    while (1) {
        // Save battery when accidentally entering an undefined handler.
        // Does not cost extra flash space, because functions are 4-byte
        // aligned and this extra instruction increases the size of this
        // function from 2 to 4 bytes.
        __asm__ __volatile__("wfi");
    }
}
void HardFault_Handler (void) __attribute__ ((weak, alias("Default_Handler")));
#endif

void Reset_Handler(void) {
#if 0
    // RAMON and RAMONB registers. These are the default values (after
    // reset, but are retained), so don't change them. Saves 20 bytes.
    uint32_t * ram_on_addr   = (uint32_t *)0x40000524;
    uint32_t * ram_on_b_addr = (uint32_t *)0x40000554;
    // RAM on in on-mode
    *ram_on_addr   = 3; // block 0 and 1
    *ram_on_b_addr = 3; // block 2 and 3
#endif

#if 0
    // RAM on in off-mode
    ram_on_addr   = 1 << 16;
    ram_on_b_addr = 1 << 17;
#endif

    // Initialize .data segment.
    uint32_t * p_src  = &_sidata;
    uint32_t * p_dest = &_sdata;

    while (p_dest < &_edata) {
      *p_dest++ = *p_src++;
    }

    // Initialize .bss segment.
    uint32_t * p_bss     = &_sbss;
    uint32_t * p_bss_end = &_ebss;
    while (p_bss < p_bss_end) {
        *p_bss++ = 0ul;
    }

    _start();
}

// Cortex-M4 interrupts.
void NMI_Handler              (void) __attribute__ ((weak, alias("HardFault_Handler")));
void MemoryManagement_Handler (void) __attribute__ ((weak, alias("HardFault_Handler")));
void BusFault_Handler         (void) __attribute__ ((weak, alias("HardFault_Handler")));
void UsageFault_Handler       (void) __attribute__ ((weak, alias("HardFault_Handler")));
void SVC_Handler              (void) __attribute__ ((weak, alias("Default_Handler")));
void DebugMon_Handler         (void) __attribute__ ((weak, alias("Default_Handler")));
void PendSV_Handler           (void) __attribute__ ((weak, alias("Default_Handler")));
void SysTick_Handler          (void) __attribute__ ((weak, alias("Default_Handler")));

// NRF52832 interrupts.
void POWER_CLOCK_IRQHandler                       (void) __attribute__((weak, alias("Default_Handler")));
void RADIO_IRQHandler                             (void) __attribute__((weak, alias("Default_Handler")));
void UARTE0_UART0_IRQHandler                      (void) __attribute__((weak, alias("Default_Handler")));
void SPIM0_SPIS0_TWIM0_TWIS0_SPI0_TWI0_IRQHandler (void) __attribute__((weak, alias("Default_Handler")));
void SPIM1_SPIS1_TWIM1_TWIS1_SPI1_TWI1_IRQHandler (void) __attribute__((weak, alias("Default_Handler")));
void NFCT_IRQHandler                              (void) __attribute__((weak, alias("Default_Handler")));
void GPIOTE_IRQHandler                            (void) __attribute__((weak, alias("Default_Handler")));
void SAADC_IRQHandler                             (void) __attribute__((weak, alias("Default_Handler")));
void TIMER0_IRQHandler                            (void) __attribute__((weak, alias("Default_Handler")));
void TIMER1_IRQHandler                            (void) __attribute__((weak, alias("Default_Handler")));
void TIMER2_IRQHandler                            (void) __attribute__((weak, alias("Default_Handler")));
void RTC0_IRQHandler                              (void) __attribute__((weak, alias("Default_Handler")));
void TEMP_IRQHandler                              (void) __attribute__((weak, alias("Default_Handler")));
void RNG_IRQHandler                               (void) __attribute__((weak, alias("Default_Handler")));
void ECB_IRQHandler                               (void) __attribute__((weak, alias("Default_Handler")));
void CCM_AAR_IRQHandler                           (void) __attribute__((weak, alias("Default_Handler")));
void WDT_IRQHandler                               (void) __attribute__((weak, alias("Default_Handler")));
void RTC1_IRQHandler                              (void) __attribute__((weak, alias("Default_Handler")));
void QDEC_IRQHandler                              (void) __attribute__((weak, alias("Default_Handler")));
void COMP_LPCOMP_IRQHandler                       (void) __attribute__((weak, alias("Default_Handler")));
void SWI0_EGU0_IRQHandler                         (void) __attribute__((weak, alias("Default_Handler")));
void SWI1_EGU1_IRQHandler                         (void) __attribute__((weak, alias("Default_Handler")));
void SWI2_EGU2_IRQHandler                         (void) __attribute__((weak, alias("Default_Handler")));
void SWI3_EGU3_IRQHandler                         (void) __attribute__((weak, alias("Default_Handler")));
void SWI4_EGU4_IRQHandler                         (void) __attribute__((weak, alias("Default_Handler")));
void SWI5_EGU5_IRQHandler                         (void) __attribute__((weak, alias("Default_Handler")));
void TIMER3_IRQHandler                            (void) __attribute__((weak, alias("Default_Handler")));
void TIMER4_IRQHandler                            (void) __attribute__((weak, alias("Default_Handler")));
void PWM0_IRQHandler                              (void) __attribute__((weak, alias("Default_Handler")));
void PDM_IRQHandler                               (void) __attribute__((weak, alias("Default_Handler")));
void MWU_IRQHandler                               (void) __attribute__((weak, alias("Default_Handler")));
void PWM1_IRQHandler                              (void) __attribute__((weak, alias("Default_Handler")));
void PWM2_IRQHandler                              (void) __attribute__((weak, alias("Default_Handler")));
void SPIM2_SPIS2_SPI2_IRQHandler                  (void) __attribute__((weak, alias("Default_Handler")));
void RTC2_IRQHandler                              (void) __attribute__((weak, alias("Default_Handler")));
void I2S_IRQHandler                               (void) __attribute__((weak, alias("Default_Handler")));
void FPU_IRQHandler                               (void) __attribute__((weak, alias("Default_Handler")));


const func __Vectors[] __attribute__ ((section(".isr_vector"),used)) = {
    (func)&_estack,
    Reset_Handler,
    NMI_Handler, // TODO: should this one be redirected to the SoftDevice?
    HardFault_Handler,
    // Dirty hack to save space: the following IRQs aren't used by the
    // bootloader so we can put anything in this space. It saves 152
    // bytes.
    // TODO: make sure only even values get written here (e.g. by
    // selecting readonly literals). This to ensure that even if an
    // interrupt gets called here, the CPU will fault (as function
    // pointers must always have the lowest bit set in Thumb mode).
#if defined(DFU_TYPE_mbr)
    MemoryManagement_Handler,
    BusFault_Handler,
    UsageFault_Handler,
    0,
    0,
    0,
    0,
    SVC_Handler,
    DebugMon_Handler,
    0,
    PendSV_Handler,
    SysTick_Handler,

    POWER_CLOCK_IRQHandler,
    RADIO_IRQHandler,
    UARTE0_UART0_IRQHandler,
    SPIM0_SPIS0_TWIM0_TWIS0_SPI0_TWI0_IRQHandler,
    SPIM1_SPIS1_TWIM1_TWIS1_SPI1_TWI1_IRQHandler,
    NFCT_IRQHandler,
    GPIOTE_IRQHandler,
    SAADC_IRQHandler,
    TIMER0_IRQHandler,
    TIMER1_IRQHandler,
    TIMER2_IRQHandler,
    RTC0_IRQHandler,
    TEMP_IRQHandler,
    RNG_IRQHandler,
    ECB_IRQHandler,
    CCM_AAR_IRQHandler,
    WDT_IRQHandler,
    RTC1_IRQHandler,
    QDEC_IRQHandler,
    COMP_LPCOMP_IRQHandler,
    SWI0_EGU0_IRQHandler,
    SWI1_EGU1_IRQHandler,
    SWI2_EGU2_IRQHandler,
    SWI3_EGU3_IRQHandler,
    SWI4_EGU4_IRQHandler,
    SWI5_EGU5_IRQHandler,
    TIMER3_IRQHandler,
    TIMER4_IRQHandler,
    PWM0_IRQHandler,
    PDM_IRQHandler,
    0,
    0,
    MWU_IRQHandler,
    PWM1_IRQHandler,
    PWM2_IRQHandler,
    SPIM2_SPIS2_SPI2_IRQHandler,
    RTC2_IRQHandler,
    I2S_IRQHandler,
    FPU_IRQHandler,
#endif
};

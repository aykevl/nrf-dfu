/* define output sections */
SECTIONS
{
    .bootloaderaddr :
    {
        KEEP(*(.bootloaderaddr))
    } >FLASH_BOOT

    /* The program code and other data goes into FLASH */
    .text :
    {
        _stext = .;
        . = ALIGN(4);
        KEEP(*(.isr_vector))
        *(.text)           /* .text sections (code) */
        *(.text*)          /* .text* sections (code) */
        *(.rodata)         /* .rodata sections (constants, strings, etc.) */
        *(.rodata*)        /* .rodata* sections (constants, strings, etc.) */
        . = ALIGN(4);
        _etext = .;        /* define a global symbol at end of code */
    } >FLASH_TEXT

    /* used by the startup to initialize data */
    _sidata = LOADADDR(.data);

    /* This is the initialized data section
    The program executes knowing that the data is in the RAM
    but the loader puts the initial values in the FLASH (inidata).
    It is one task of the startup to copy the initial values from FLASH to RAM. */
    .data :
    {
        . = ALIGN(4);
        _sdata = .;        /* create a global symbol at data start; used by startup code in order to initialise the .data section in RAM */
        *(.data)           /* .data sections */
        *(.data*)          /* .data* sections */
        /* These two are stored in .data to allow them to be used as SoftDevice
         * assert handlers. */
        *(.softdevice_assert_handler)
        *(.Default_Handler)

        . = ALIGN(4);
        _edata = .;        /* define a global symbol at data end; used by startup code in order to initialise the .data section in RAM */
    } >RAM AT>FLASH_TEXT

    /* Uninitialized data section */
    .bss :
    {
        . = ALIGN(4);
        _sbss = .;         /* define a global symbol at bss start; used by startup code */
        *(.bss)
        *(.bss*)
        *(COMMON)

        . = ALIGN(4);
        _ebss = .;         /* define a global symbol at bss end; used by startup code and GC */
    } >RAM

    .ARM.attributes 0 : { *(.ARM.attributes) }
}

MEMORY
{
    FLASH_TEXT (rw) : ORIGIN = 0x0007e000, LENGTH = 8K       /* .text */
    FLASH_BOOT (r)  : ORIGIN = 0x10001014, LENGTH = 4        /* 4 bytes, UICR.NRFFW[0] */
    RAM (xrw)       : ORIGIN = 0x20003800, LENGTH = 0x002000 /* 8 KiB */
    RAM_DATA (xrw)  : ORIGIN = 0x20002000, LENGTH = 0        /* .data, disabled */
}

/* top end of the stack */
_estack = ORIGIN(RAM) + LENGTH(RAM);

INCLUDE "common.ld"

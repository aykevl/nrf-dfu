
# Compile options.
# Note that you will have to run `make clean` before compiling with different options.
SD ?= s132_nrf52_6.1.1
PAGESIZE ?= 4096
DEBUG ?= 0
DFU_TYPE ?= mbr
BOARD ?= PCA10040

.PHONY: all
all: build/combined.hex

.PHONY: clean
clean:
	rm -rf build

.PHONY: flash
flash: build/combined.hex
	nrfjprog -f nrf52 --eraseall
	nrfjprog -f nrf52 --program $<
	nrfjprog -f nrf52 --reset

.PHONY: gdb
gdb:
	arm-none-eabi-gdb build/mbr.elf -ex "target remote :2331"

# Basic flags.
CFLAGS += -flto -Os -g -mthumb -mcpu=cortex-m4 -Wall -Werror -nostartfiles
LDFLAGS += -Wl,-T -Wl,nrf52_512k_s132_$(DFU_TYPE).ld -Wl,--gc-sections

# Extra include directories.
CFLAGS += -I.
CFLAGS += -Ilib/CMSIS/CMSIS/Core/Include
CFLAGS += -Ilib/nrfx
CFLAGS += -Ilib/nrfx/hal
CFLAGS += -Ilib/nrfx/mdk
CFLAGS += -Ilib/$(SD)/$(SD)_API/include
CFLAGS += -Ilib/$(SD)/$(SD)_API/include/nrf52

# Extra chip configuration flags.
CFLAGS += -DNRF52832_XXAA=1 # required by nrfx
CFLAGS += -DNRF52=1         # required by the DFU
CFLAGS += -DDFU_TYPE_$(DFU_TYPE)=1
CFLAGS += -DDEBUG=$(DEBUG)
CFLAGS += -D$(BOARD)

build/combined.hex: lib/$(SD)/$(SD)_softdevice.hex build/mbr.hex
	./mergehex.py --pagesize=$(PAGESIZE) $@ $^

build/mbr.hex: build/mbr.elf
	arm-none-eabi-objcopy -O ihex build/mbr.elf build/mbr.hex

build/mbr.elf: build/dfu.o build/dfu_sd.o build/dfu_uart.o build/dfu_ble.o build/startup.o
	arm-none-eabi-gcc $(CFLAGS) $(LDFLAGS) -o $@ $^
	arm-none-eabi-size $@

build/%.o: %.c *.h Makefile
	@mkdir -p build
	arm-none-eabi-gcc $(CFLAGS) -c -o $@ $<

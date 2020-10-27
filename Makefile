GPPPARAMS = -m64 -fno-use-cxa-atexit -nostdlib -fno-builtin -fno-rtti -fno-exceptions -fno-leading-underscore
LDPARAMS = -Ttext 0x7E00 --oformat binary

SRCDIR = src/
TEMPDIR = .temp/
OUTDIR = bin/

run: # Run the raw file
	nasm $(SRCDIR)Sector0/boot.asm -f bin -o $(OUTDIR)boot.bin
	nasm $(SRCDIR)Sector1/ExtendedProgram.asm -f elf64 -o $(TEMPDIR)ExtendedProgram.o
	
	g++ $(GPPPARAMS) -o $(TEMPDIR)Kernel.o -c $(SRCDIR)Kernel.cpp

	ld -o $(OUTDIR)Kernel.bin $(LDPARAMS) $(TEMPDIR)ExtendedProgram.o $(TEMPDIR)Kernel.o

	cat $(OUTDIR)boot.bin $(OUTDIR)Kernel.bin > $(OUTDIR)bootloader.bin

	qemu-system-x86_64 -drive file=$(OUTDIR)bootloader.bin,format=raw


.PHONEY: clean
clean:
	rm -r $(OUTDIR)
	mkdir $(OUTDIR)

	rm -r $(TEMPDIR)
	mkdir $(TEMPDIR)